#pragma once

#include "../common/types.h"

#include <cstddef>
#include <memory>

namespace freq {

class FileReaderIface {
public:
    virtual ~FileReaderIface() = default;
    virtual std::size_t read(Byte* buf, std::size_t size) = 0;
    [[nodiscard]] virtual std::size_t size() const noexcept = 0;
    [[nodiscard]] virtual bool eof() const = 0;
};

using FileReaderPtr = std::unique_ptr<FileReaderIface>;

}