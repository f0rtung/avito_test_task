#include "../src/file-chunks-processor/utils.h"

#include <string>
#include <gtest/gtest.h>

namespace freq {

bool operator==(const HeadTailPart& lhs, const HeadTailPart& rhs)
{
    return lhs.head == rhs.head &&
           lhs.tail == rhs.tail;
}

}

struct ParseChunkParameters {
    const std::string          name;
    const std::string          content;
    const freq::FreqDictByWord expected_process_result;
    const freq::HeadTailPart   expected_head_tail_parts;
};

std::ostream& operator<<(std::ostream& ostream, const ParseChunkParameters& params)
{
    ostream << params.name;
    return ostream;
}

class ParseChunkParameterizedTestFixture :public ::testing::TestWithParam<ParseChunkParameters> {
};

TEST_P(ParseChunkParameterizedTestFixture, ParseChunkTest)
{
    const auto params{GetParam()};
    freq::FreqDictByWord process_result;
    auto head_tail_parts{freq::parseChunk(freq::Bytes{params.content.cbegin(), params.content.cend()},
                                          process_result)};
    EXPECT_EQ(process_result, params.expected_process_result);
    EXPECT_EQ(head_tail_parts, params.expected_head_tail_parts);
}

INSTANTIATE_TEST_SUITE_P(
    ParseChunkTest,
    ParseChunkParameterizedTestFixture,
    ::testing::Values(
        ParseChunkParameters {
            "starts with word, ends with word, does not contain word",
            "abc 545 cba",
            {},
            freq::HeadTailPart{"abc", "cba"}
        },
        ParseChunkParameters {
            "starts with word, ends with word, contains word",
            "abc eee cba",
            {{"eee", 1}},
            freq::HeadTailPart{"abc", "cba"}
        },
        ParseChunkParameters {
            "starts with word, ends with word, contains several words",
            "abc eee  354  35  sdd   eee   cba",
            {{"eee", 2}, {"sdd", 1}},
            freq::HeadTailPart{"abc", "cba"}
        },
        ParseChunkParameters {
            "one big word",
            "abccba",
            {},
            freq::HeadTailPart{"abccba", ""}
        },
        ParseChunkParameters {
            "no word",
            " ` 234234 \n  ",
            {},
            freq::HeadTailPart{"", ""}
        }
        ,
        ParseChunkParameters {
            "empty content",
            "",
            {},
            freq::HeadTailPart{"", ""}
        },
        ParseChunkParameters {
            "starts with not word, ends with word, contains several words",
            "7   abc eee  354  35  sdd   eee   cba",
            {{"abc", 1}, {"eee", 2}, {"sdd", 1}},
            freq::HeadTailPart{"", "cba"}
        },
        ParseChunkParameters {
            "starts with not word, ends with not word, contains several words",
            "7   abc eee  354  35  sdd   eee   cba  434",
            {{"abc", 1}, {"eee", 2}, {"sdd", 1}, {"cba", 1}},
            freq::HeadTailPart{"", ""}
        },
        ParseChunkParameters {
            "starts with word, ends with not word, contains several words",
            "abc eee  354  35  sdd   eee   cba  434",
            {{"eee", 2}, {"sdd", 1}, {"cba", 1}},
            freq::HeadTailPart{"abc", ""}
        },
        ParseChunkParameters {
            "starts with word, ends with not word, contains several same words in diff cases",
            "ABc EEe  354  35  sdd   eee   cba   2323###  CbA 434 wEr",
            {{"eee", 2}, {"sdd", 1}, {"cba", 2}},
            freq::HeadTailPart{"abc", "wer"}
        }
    )
);