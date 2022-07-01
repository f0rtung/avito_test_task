#include "file-reader/binary-file-reader.h"
#include "file-chunks-producer/simple-file-chunks-producer.h"
#include "file-chunks-producer/async-file-chunks-producer.h"
#include "file-chunks-processor/multi-thread-file-chunk-processor.h"
#include "file-writer/result-file-writer.h"


#include <exception>
#include <chrono>
#include <iostream>
#include <thread>

constexpr std::size_t ONE_MB_CHUNK_SIZE{1024 * 1024};
constexpr std::size_t THREADS_COUNT{4};


std::uint64_t getCurrentTimestampMs()
{
    using namespace std::chrono;
    return time_point_cast<milliseconds, system_clock>(system_clock::now()).time_since_epoch().count();
}

int main(int argc, char **argv)
{
    try {
        if (argc < 3) {
            throw std::invalid_argument{"Not enough arguments: freq infile outfile"};
        }
        const std::string in_file{argv[1]};
        const std::string out_file{argv[2]};

//        {
//            // some tests
//            for (std::size_t chunk_size{100 * 1024}; chunk_size < 26 * 1024 * 1024; chunk_size *= 2) {
//                for (std::size_t threads_count{2}; threads_count < 32; threads_count *= 2) {
//                    const auto start_ms{getCurrentTimestampMs()};
//                    freq::FileReaderPtr file_reader{std::make_unique<freq::BinaryFileReader>(in_file)};
//                    const std::size_t chunks_count{file_reader->size() / chunk_size + 1};
//                    freq::FileChunksProducerPtr file_chunk_producer{
//                        std::make_unique<freq::AsyncFileChunksProducer>(std::move(file_reader), chunk_size)
//                    };
//
//                    freq::FileChunkProcessorPtr file_chunk_processor{std::make_unique<freq::MultiThreadFileChunkProcessor>(
//                        std::move(file_chunk_producer),
//                        chunks_count,
//                        threads_count
//                    )};
//
//                    const auto result{file_chunk_processor->process()};
//
//                    freq::ResultFileWriter result_file_writer{out_file};
//                    result_file_writer.write(result);
//                    const auto diff_ms{getCurrentTimestampMs() - start_ms};
//
//                    std::cout << "Chunk size (b): " << chunk_size <<", "
//                                 "threads count: " << threads_count << ", "
//                                 "duration ms: " << diff_ms << std::endl;
//                }
//            }
//        }

        freq::FileReaderPtr file_reader{std::make_unique<freq::BinaryFileReader>(in_file)};
        const std::size_t chunks_count{file_reader->size() / ONE_MB_CHUNK_SIZE + 1};
        freq::FileChunksProducerPtr file_chunk_producer{
            std::make_unique<freq::SimpleFileChunksProducer>(std::move(file_reader), ONE_MB_CHUNK_SIZE)
        };

        std::size_t hc{std::thread::hardware_concurrency()};
        const std::size_t threads_count{hc > 0 ? hc : THREADS_COUNT};

        freq::FileChunkProcessorPtr file_chunk_processor{
            std::make_unique<freq::MultiThreadFileChunkProcessor>(
                std::move(file_chunk_producer),
                chunks_count,
                threads_count
            )
        };

        const freq::FreqDictByCount result{file_chunk_processor->process()};

        freq::ResultFileWriter result_file_writer{out_file};
        result_file_writer.write(result);

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}