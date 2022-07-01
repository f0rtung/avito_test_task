#include "result-file-writer.h"

namespace freq {

ResultFileWriter::ResultFileWriter(const std::string &file_path)
{
    tf_stream_.open(file_path, std::ofstream::trunc);
    if (!tf_stream_) {
        throw std::runtime_error{"Can not open file \"" + file_path + "\""};
    }
}

void ResultFileWriter::write(const FreqDictByCount &result)
{
    for (const auto& [count, words] : result) {
        for (const std::string& word : words) {
            tf_stream_ << count << " " << word << "\n";
        }
    }
    tf_stream_.flush();
}

}