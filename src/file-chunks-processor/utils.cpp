#include "utils.h"

#include <algorithm>

namespace freq {

bool isCharacter(Byte b)
{
    // omg, we can do lower case right here O_o: -32
    return (b >= 'a' && b <= 'z') ||
           (b >= 'A' && b <= 'Z');
}

template<typename It>
std::string makeLowerCaseString(It begin, It end)
{
    std::string result;
    result.reserve(std::distance(begin, end));
    std::transform(begin, end, std::back_inserter(result), [](unsigned char c){ return std::tolower(c); });
    return result;
}

HeadTailPart parseChunk(const Bytes& chunk, FreqDictByWord& freq_dict_by_word)
{
    HeadTailPart head_tail_part;

    auto next_word_begin{std::find_if(chunk.cbegin(), chunk.cend(), isCharacter)};

    if (chunk.cbegin() == next_word_begin) { // mb part of word from prev chunk
        auto next_word_end{std::find_if_not(next_word_begin, chunk.cend(), isCharacter)};
        head_tail_part.head = makeLowerCaseString(next_word_begin, next_word_end);
        next_word_begin = std::find_if(next_word_end, chunk.cend(), isCharacter);
    }

    while (chunk.cend() != next_word_begin) {
        auto next_word_end{std::find_if_not(next_word_begin, chunk.cend(), isCharacter)};
        if (chunk.cend() == next_word_end) {
            head_tail_part.tail = makeLowerCaseString(next_word_begin, next_word_end);
            next_word_begin = next_word_end;
        } else {
            freq_dict_by_word[makeLowerCaseString(next_word_begin, next_word_end)]++;
            next_word_begin = std::find_if(next_word_end, chunk.cend(), isCharacter);
        }
    }

    return head_tail_part;
}

void mergeUnsortedIndexedHeadTailPartsToResult(IndexedHeadTailParts& parts, FreqDictByWord& freq_dict_by_word)
{
    if (parts.empty()) {
        return;
    }

    std::sort(parts.begin(), parts.end(), [](const auto& lhs, const auto& rhs) { return lhs.index < rhs.index; });

    IndexedHeadTailPart& curr_part{parts[0]};

    if (!curr_part.head_tail_part.head.empty()) {
        freq_dict_by_word[curr_part.head_tail_part.head] += 1;
    }

    for (std::size_t idx{1}; idx < parts.size(); ++idx) {

        IndexedHeadTailPart& next_part{parts[idx]};

        if (curr_part.index == next_part.index - 1) {
            std::string word{curr_part.head_tail_part.tail + next_part.head_tail_part.head};
            if (!word.empty()) {
                freq_dict_by_word[std::move(word)] += 1;
            }
        } else {
            if (!curr_part.head_tail_part.tail.empty()) {
                freq_dict_by_word[curr_part.head_tail_part.tail] += 1;
            }
            if (!next_part.head_tail_part.head.empty()) {
                freq_dict_by_word[next_part.head_tail_part.head] += 1;
            }
        }
        curr_part = next_part;
    }

    if (!curr_part.head_tail_part.tail.empty()) {
        freq_dict_by_word[curr_part.head_tail_part.tail] += 1;
    }
}

FreqDictByCount convertDictByWordToByCount(const FreqDictByWord& freq_dict_by_word)
{
    FreqDictByCount freq_dict_by_count;
    for (auto& [word, count] : freq_dict_by_word) {
        freq_dict_by_count[count].emplace_back(word);
    }
    for (auto& [_, words] : freq_dict_by_count) {
        std::sort(words.begin(), words.end());
    }
    return freq_dict_by_count;
}

}