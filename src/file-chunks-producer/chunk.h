#pragma once

#include "../common/types.h"

namespace freq {

class Chunk {
public:
    Chunk(std::size_t index, Bytes buf);

public:
    [[nodiscard]] const Bytes& buf() const noexcept;
    [[nodiscard]] std::size_t index() const noexcept;

private:
    std::size_t index_;
    Bytes       buf_;
};

}