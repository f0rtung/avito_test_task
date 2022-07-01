#pragma once

#include "../file-chunks-processor/file-chunk-processor-iface.h"

#include <fstream>

namespace freq {

class ResultFileWriter {
public:
    explicit ResultFileWriter(const std::string& file_path);
    void write(const FreqDictByCount& result);

private:
    std::ofstream tf_stream_;
};

}