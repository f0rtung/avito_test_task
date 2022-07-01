#pragma once

#include "chunk.h"

#include <memory>

namespace freq {

class FileChunksProducerIface
{
public:
    virtual ~FileChunksProducerIface() = default;
    virtual Chunk chunk() = 0;
};

using FileChunksProducerPtr = std::unique_ptr<FileChunksProducerIface>;

}