#pragma once

#include "../file-chunks-producer/file-chunk-producer-iface.h"
#include "file-chunk-processor-iface.h"
#include "utils.h"

#include <mutex>

namespace freq {

class MultiThreadFileChunkProcessor final : public FileChunkProcessorIface {
public:
    MultiThreadFileChunkProcessor(FileChunksProducerPtr file_chunk_producer,
                                  std::size_t           chunks_count,
                                  std::size_t           threads_count);
    ~MultiThreadFileChunkProcessor() override = default;

public:
    FreqDictByCount process() override;

private:
    struct PreliminaryResult {
        FreqDictByWord       freq_dict_by_word;
        IndexedHeadTailParts head_tail_parts;
    };

    PreliminaryResult processImpl();

private:
    FileChunksProducerPtr file_chunk_producer_;
    const std::size_t     chunks_count_;
    const std::size_t     threads_count_;
};

}