#include "chunk.h"

namespace freq {

Chunk::Chunk(std::size_t index, Bytes buf)
: index_{index}
, buf_{std::move(buf)}
{}

const Bytes& Chunk::buf() const noexcept
{
    return buf_;
}

std::size_t Chunk::index() const noexcept
{
    return index_;
}

}