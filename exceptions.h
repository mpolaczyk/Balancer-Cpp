#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <cerrno>
#include <system_error>

static inline std::system_error posix_error()
{
    return std::system_error(errno, std::system_category());
}

static inline std::system_error posix_error(const char * what)
{
    return std::system_error(errno, std::system_category(), what);
}

#endif // EXCEPTIONS_H
