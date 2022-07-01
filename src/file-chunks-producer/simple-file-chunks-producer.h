#pragma once

#include "file-chunk-producer-iface.h"
#include "../file-reader/file-reader-iface.h"

#include <mutex>


namespace freq {

class SimpleFileChunksProducer final : public FileChunksProducerIface{
public:
    SimpleFileChunksProducer(FileReaderPtr file_reader, std::size_t chunk_size);
    ~SimpleFileChunksProducer() override = default;

public:
    Chunk chunk() override;

private:
    FileReaderPtr     file_reader_;
    const std::size_t chunk_size_;
    std::mutex        reader_guard_;
    std::size_t       next_chunk_index_;
};

}