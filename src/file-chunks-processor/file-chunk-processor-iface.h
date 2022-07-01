#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace freq {

using Words = std::vector<std::string>;
using FreqDictByCount = std::map<std::size_t, Words, std::greater<>>;

class FileChunkProcessorIface {
public:
    virtual ~FileChunkProcessorIface() = default;
    virtual FreqDictByCount process() = 0;
};

using FileChunkProcessorPtr = std::unique_ptr<FileChunkProcessorIface>;

}