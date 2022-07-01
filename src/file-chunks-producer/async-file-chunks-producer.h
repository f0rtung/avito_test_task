#pragma once

#include "file-chunk-producer-iface.h"
#include "../file-reader/file-reader-iface.h"

#include <atomic>
#include <thread>
#include <mutex>
#include <queue>

namespace freq {

class AsyncFileChunksProducer final : public FileChunksProducerIface
{
public:
    AsyncFileChunksProducer(FileReaderPtr file_reader, std::size_t chunk_size);
    ~AsyncFileChunksProducer() override;

public:
    Chunk chunk() override;

private:
    void asyncRead();

private:
    FileReaderPtr     file_reader_;
    const std::size_t chunk_size_;
    std::atomic_bool  finish_read_;
    std::size_t       next_chunk_index_;
    std::thread       reading_thread_;
    std::mutex        chunks_queue_guard_;
    std::queue<Chunk> chunks_queue_;
};

}