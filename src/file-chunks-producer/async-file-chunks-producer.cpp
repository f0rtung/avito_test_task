#include "async-file-chunks-producer.h"
#include "exception.h"

namespace freq {

AsyncFileChunksProducer::AsyncFileChunksProducer(FileReaderPtr file_reader, std::size_t chunk_size)
: file_reader_{std::move(file_reader)}
, chunk_size_{chunk_size}
, finish_read_{false}
, next_chunk_index_{0}
, reading_thread_{&AsyncFileChunksProducer::asyncRead, this}
{}

AsyncFileChunksProducer::~AsyncFileChunksProducer()
{
    if (reading_thread_.joinable()) {
        reading_thread_.join();
    }
}

Chunk AsyncFileChunksProducer::chunk()
{
    std::unique_lock<std::mutex> lock{chunks_queue_guard_};
    while(chunks_queue_.empty()) {
        if (finish_read_) {
            throw NoMoreChunksException{};
        }
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        lock.lock();
    }
    Chunk chunk{std::move(chunks_queue_.front())};
    chunks_queue_.pop();
    return chunk;
}

void AsyncFileChunksProducer::asyncRead()
{
    while (!file_reader_->eof()) {
        Bytes buf(chunk_size_);
        const std::size_t read_bytes{file_reader_->read(buf.data(), buf.size())};
        if (read_bytes < buf.size()) { // last chunk
            buf.resize(read_bytes);
        }
        std::lock_guard<std::mutex> _{chunks_queue_guard_};
        chunks_queue_.emplace(next_chunk_index_++, std::move(buf));
    }
    finish_read_ = true;
}

}