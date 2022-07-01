#include "simple-file-chunks-producer.h"
#include "exception.h"

namespace freq {

SimpleFileChunksProducer::SimpleFileChunksProducer(FileReaderPtr file_reader, std::size_t chunk_size)
: file_reader_{std::move(file_reader)}
, chunk_size_{chunk_size}
, reader_guard_{}
, next_chunk_index_{0}
{}

Chunk SimpleFileChunksProducer::chunk()
{
    Bytes buf(chunk_size_); // It could be useless memory allocation, but not under mutex

    std::unique_lock<std::mutex> lock{reader_guard_};

    if (file_reader_->eof()) {
        throw NoMoreChunksException{};
    }
    const std::size_t read_bytes{file_reader_->read(buf.data(), buf.size())};
    const std::size_t next_chunk_index = next_chunk_index_++;

    lock.unlock();

    if (read_bytes < buf.size()) { // last chunk
        buf.resize( read_bytes );
    }

    return Chunk{next_chunk_index, std::move(buf)};
}

}