#include "multi-thread-file-chunk-processor.h"
#include "utils.h"
#include "../file-chunks-producer/exception.h"

#include <future>
#include <algorithm>

namespace freq {

MultiThreadFileChunkProcessor::MultiThreadFileChunkProcessor(FileChunksProducerPtr file_chunk_producer,
                                                             std::size_t           chunks_count,
                                                             std::size_t           threads_count)
: file_chunk_producer_{std::move(file_chunk_producer)}
, chunks_count_{chunks_count}
, threads_count_{threads_count}
{}

FreqDictByCount MultiThreadFileChunkProcessor::process()
{
    std::vector<std::future<PreliminaryResult>> futures;
    futures.reserve(threads_count_);
    for (std::size_t idx{0}; idx < threads_count_; ++idx) {
        futures.emplace_back(std::async(std::launch::async, [this](){ return processImpl();}));
    }

    FreqDictByWord freq_dict_by_word;
    IndexedHeadTailParts result_head_tail_parts;
    result_head_tail_parts.reserve(chunks_count_);

    for (std::future<PreliminaryResult>& f : futures) {
        const auto [freq_dict, head_tail_parts] = f.get();
        for (const auto& [word, count] : freq_dict) {
            freq_dict_by_word[word] += count;
        }
        // merge sort?
        std::move(head_tail_parts.begin(), head_tail_parts.end(), std::back_inserter(result_head_tail_parts));
    }

    mergeUnsortedIndexedHeadTailPartsToResult(result_head_tail_parts, freq_dict_by_word);

    return convertDictByWordToByCount(freq_dict_by_word);
}

MultiThreadFileChunkProcessor::PreliminaryResult MultiThreadFileChunkProcessor::processImpl()
{
    FreqDictByWord freq_dict_by_word;
    IndexedHeadTailParts head_tail_parts;
    head_tail_parts.reserve(chunks_count_ / threads_count_ + 1);
    try {
        while (true) {
            const Chunk chunk{file_chunk_producer_->chunk()};
            HeadTailPart head_tail_part{parseChunk(chunk.buf(), freq_dict_by_word)};
            if (!head_tail_part.head.empty() || !head_tail_part.tail.empty()) {
                head_tail_parts.emplace_back(IndexedHeadTailPart{chunk.index(), std::move(head_tail_part)});
            }
        }
    } catch (const NoMoreChunksException&) {
    }
    return {std::move(freq_dict_by_word), std::move(head_tail_parts)};
}

}