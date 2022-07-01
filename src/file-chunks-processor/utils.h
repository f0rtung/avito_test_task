#pragma once

#include "file-chunk-processor-iface.h"
#include "../common/types.h"

#include <unordered_map>

namespace freq {

struct HeadTailPart {
    std::string head;
    std::string tail;
};

struct IndexedHeadTailPart{
    std::size_t  index{};
    HeadTailPart head_tail_part;
};

using IndexedHeadTailParts = std::vector<IndexedHeadTailPart>;

using FreqDictByWord = std::unordered_map<std::string, std::size_t>;

HeadTailPart parseChunk(const Bytes& chunk, FreqDictByWord& freq_dict_by_word);

void mergeUnsortedIndexedHeadTailPartsToResult(IndexedHeadTailParts& parts, FreqDictByWord& freq_dict_by_word);

FreqDictByCount convertDictByWordToByCount(const FreqDictByWord& freq_dict_by_word);

}