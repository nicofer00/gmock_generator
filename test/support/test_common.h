#ifndef __TEST_COMMON_H__
#define __TEST_COMMON_H__

#include <vector>
#include <random>
#include <filesystem>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define GMOCK_TEST_ATTRIBUTES       \
using ::testing::AtLeast;           \
using ::testing::InSequence;        \
using ::testing::Return;            \
using ::testing::HasSubstr;         \
using ::testing::MatchesRegex;      \
using ::testing::_;                 \
using ::testing::SetArgPointee;     \
using ::testing::NotNull;           \
using ::testing::A;                 \
using ::testing::ContainerEq;       \
using ::testing::ElementsAreArray;  \
using ::testing::Pointee;           \
using ::testing::Eq;                \
using ::testing::Ge;                \
using ::testing::Le;                \
using ::testing::SetArgReferee;     \
using ::testing::SetArrayArgument;  \
using ::testing::WithArg;           \
using ::testing::Assign;            \
using ::testing::Invoke;            \
using ::testing::Throw;             \
using ::testing::DoAll;

#define LOCAL_FILE_PATH(name) std::filesystem::path(__FILE__).parent_path().string().append("/" #name)
#define SUPPORT_FILE_PATH(name) std::filesystem::path(__FILE__).parent_path().parent_path().parent_path().string().append("/support/" #name)
#define EMPTY_STRING std::string()

static inline std::vector<uint8_t> generateRandomBytes(size_t num_bytes)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::vector<uint8_t> bytes(num_bytes);
    for(size_t i = 0; i < num_bytes; ++i)
    {
        bytes[i] = static_cast<uint8_t>(dis(gen));
    }

    return bytes;
}

static inline void printVector(std::vector<uint8_t> myVector)
{
    for (uint8_t value : myVector)
    {
        std::cout << static_cast<int>(value) << " ";
    }
    std::cout << std::endl;
}

// Custom action to conditionally throw an exception or return a value
ACTION_P2(ThrowOrReturn, throws, value)
{
    if (throws)
    {
        throw std::runtime_error("EXPECT_EXCEPTION");
    }
    else
    {
        return value;
    }
}

#endif // __TEST_COMMON_H__