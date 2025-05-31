#ifndef __CONST_H__
#define __CONST_H__

#define PROJECT_NAME "gmock_gen_tester"
#define PROJECT_VERSION "00.00.01"
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <iomanip>

#define RETURN_OK (0U)

#define UINT32_UINT8 Common32to8
typedef enum
{
    SYS_STATUS_OK,
    SYS_STATUS_FAIL = 1,
    SYS_STATUS_ENOENT = 2,
    SYS_STATUS_EIO = 5,
    SYS_STATUS_EACCES = 13,
    SYS_STATUS_ENODEV = 19,
    SYS_STATUS_EINVAL = 22,
    SYS_STATUS_ENOSPC = 28,
    SYS_STATUS_ENOSYS = 38,
    SYS_STATUS_ENOTCONN = 107,
} Status_t;

static inline void Common32to8(uint8_t *dest,
                               uint32_t src)
{
    int8_t i = sizeof(uint32_t) - 1U;
    dest[i--] = (uint8_t) (src);
    for (; i >= 0; --i)
    {
        dest[i] = (uint8_t) (src >>= 8U);
    }
}

static inline std::string Common8VtoHexStr(const std::vector<uint8_t>& v)
{
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    for (const auto& element : v)
    {
        ss << "\\x" << std::setw(2) << static_cast<unsigned>(element);
    }
    return ss.str();
}

// Unit Testing Special Defines
#ifdef TEST
#define STATIC
#define STATIC_INLINE
#define sleep(ms)
#define THREAD_INIT(name, func, obj, ...) (*obj.*func)(__VA_ARGS__)
#define THREAD_JOIN(name)
#define THREAD_DEINIT(name)
#define THREAD_LOOP(condition)
#define SAFE_EXIT(code)
#else
#define STATIC static
#define STATIC_INLINE static inline
#define sleep(ms)\
    usleep(1000*ms)
#define THREAD_INIT(name, ...) name = std::thread(__VA_ARGS__)
#define THREAD_JOIN(name) name.join()
#define THREAD_DEINIT(name) name.~thread()
#define THREAD_LOOP(condition) while(condition)
#define SAFE_EXIT(code) std::exit(code)
#endif

#endif // __CONST_H__