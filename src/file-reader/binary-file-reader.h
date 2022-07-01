#pragma once

#include "file-reader-iface.h"

#include <fstream>
#include <string>

namespace freq {

class BinaryFileReader final : public FileReaderIface {
public:
    explicit BinaryFileReader(const std::string& file_path);

public:
    ~BinaryFileReader() override = default;
    std::size_t read(Byte* buf, std::size_t size) override;
    std::size_t size() const noexcept override;
    bool eof() const override;

private:
    std::ifstream bf_stream_;
    std::size_t   bf_size_;
};

}