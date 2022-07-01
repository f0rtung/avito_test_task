#include "../src/file-chunks-processor/utils.h"

#include <string>
#include <gtest/gtest.h>

struct MergePartsParameters {
    const std::string          name;
    freq::IndexedHeadTailParts parts;
    const freq::FreqDictByWord expected_process_result;
};

std::ostream& operator<<(std::ostream& ostream, const MergePartsParameters& params)
{
    ostream << params.name;
    return ostream;
}

class MergePartsParameterizedTestFixture :public ::testing::TestWithParam<MergePartsParameters> {
};

TEST_P(MergePartsParameterizedTestFixture, MergePartsTest)
{
    auto params{GetParam()};
    freq::FreqDictByWord process_result;
    freq::mergeUnsortedIndexedHeadTailPartsToResult(params.parts, process_result);
    EXPECT_EQ(process_result, params.expected_process_result);
}

INSTANTIATE_TEST_SUITE_P(
    MergePartsTest,
    MergePartsParameterizedTestFixture,
    ::testing::Values(
        MergePartsParameters {
            "empty parts",
            {},
            {}
        },
        MergePartsParameters {
            "one parts with tail",
            {freq::IndexedHeadTailPart{10, freq::HeadTailPart{"", "qwe"}}},
            {{"qwe", 1}}
        },
        MergePartsParameters {
            "one parts with head",
            {freq::IndexedHeadTailPart{10, freq::HeadTailPart{"qwe", ""}}},
            {{"qwe", 1}}
        },
        MergePartsParameters {
            "one parts with head and tail",
            {freq::IndexedHeadTailPart{10, freq::HeadTailPart{"qwe", "yy"}}},
            {{"qwe", 1}, {"yy", 1}}
        },
        MergePartsParameters {
            "one parts with the same head and tail",
            {freq::IndexedHeadTailPart{10, freq::HeadTailPart{"qwe", "qwe"}}},
            {{"qwe", 2}}
        },
        MergePartsParameters {
            "two not connected parts h--t equal",
            {
                freq::IndexedHeadTailPart{11, freq::HeadTailPart{"", "qwe"}},
                freq::IndexedHeadTailPart{9, freq::HeadTailPart{"qwe", ""}}
            },
            {{"qwe", 2}}
        },
        MergePartsParameters {
            "two not connected parts h--t not equal",
            {
                freq::IndexedHeadTailPart{11, freq::HeadTailPart{"", "qwe"}},
                freq::IndexedHeadTailPart{9, freq::HeadTailPart{"qws", ""}}
            },
            {{"qwe", 1}, {"qws", 1}}
        },
        MergePartsParameters {
            "two not connected parts ht-t",
            {
                freq::IndexedHeadTailPart{11, freq::HeadTailPart{"", "qwe"}},
                freq::IndexedHeadTailPart{9, freq::HeadTailPart{"qwe", "sds"}}
            },
            {{"qwe", 2}, {"sds", 1}}
        },
        MergePartsParameters {
            "two not connected parts htht",
            {
                freq::IndexedHeadTailPart{11, freq::HeadTailPart{"sds", "qwe"}},
                freq::IndexedHeadTailPart{9, freq::HeadTailPart{"qwe", "sds"}}
            },
            {{"qwe", 2}, {"sds", 2}}
        },
        MergePartsParameters {
            "two not connected parts htht all diff",
            {
                freq::IndexedHeadTailPart{11, freq::HeadTailPart{"sdsw", "qwez"}},
                freq::IndexedHeadTailPart{9, freq::HeadTailPart{"qweq", "sdsd"}}
            },
            {{"qweq", 1}, {"sdsd", 1}, {"sdsw", 1}, {"qwez", 1}}
        },
        MergePartsParameters {
            "two not connected parts -tht",
            {
                freq::IndexedHeadTailPart{11, freq::HeadTailPart{"sds", "qwe"}},
                freq::IndexedHeadTailPart{9, freq::HeadTailPart{"", "sds"}}
            },
            {{"qwe", 1}, {"sds", 2}}
        },
        MergePartsParameters {
            "two not connected parts -t-t",
            {
                freq::IndexedHeadTailPart{11, freq::HeadTailPart{"", "qwe"}},
                freq::IndexedHeadTailPart{9, freq::HeadTailPart{"", "sds"}}
            },
            {{"qwe", 1}, {"sds", 1}}
        },
        MergePartsParameters {
            "two not connected parts -th-",
            {
                freq::IndexedHeadTailPart{11, freq::HeadTailPart{"qwe", ""}},
                freq::IndexedHeadTailPart{9, freq::HeadTailPart{"", "sds"}}
            },
            {{"qwe", 1}, {"sds", 1}}
        },
        MergePartsParameters {
            "two connected parts -tht",
            {
                freq::IndexedHeadTailPart{10, freq::HeadTailPart{"a", "qwe"}},
                freq::IndexedHeadTailPart{9, freq::HeadTailPart{"", "w"}}
            },
            {{"qwe", 1}, {"wa", 1}}
        },
        MergePartsParameters {
            "two connected parts htht",
            {
                freq::IndexedHeadTailPart{10, freq::HeadTailPart{"a", "qwe"}},
                freq::IndexedHeadTailPart{9, freq::HeadTailPart{"wa", "w"}}
            },
            {{"qwe", 1}, {"wa", 2}}
        }
    )
);