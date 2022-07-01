#include "exception.h"

namespace freq {

const char* NoMoreChunksException::what() const noexcept
{
    return "No more chunks";
}

}