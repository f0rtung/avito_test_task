#include "binary-file-reader.h"

namespace freq {

constexpr std::ifstream::openmode bin_file_open_mod{
    std::ifstream::in |
    std::ifstream::binary |
    std::ifstream::ate
};

BinaryFileReader::BinaryFileReader(const std::string& file_path)
{
    bf_stream_.open(file_path, bin_file_open_mod);
    if (!bf_stream_) {
        throw std::runtime_error{"Can not open file \"" + file_path + "\"" };
    }
    bf_size_ = bf_stream_.tellg();
    bf_stream_.seekg(0);
}

std::size_t BinaryFileReader::read(Byte* buf, std::size_t size)
{
    bf_stream_.read(buf, size);
    return bf_stream_.gcount();
}

std::size_t BinaryFileReader::size() const noexcept
{
    return bf_size_;
}

bool BinaryFileReader::eof() const
{
    return bf_stream_.eof();
}

}