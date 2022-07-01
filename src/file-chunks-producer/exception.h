#pragma once


#include <exception>

namespace freq {

class NoMoreChunksException final : public std::exception
{
public:
    [[nodiscard]] const char* what() const noexcept override;
};

}