#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

class CStringUtils
{
public:
    /// Trim right.
    static std::string TrimRight(const std::string& Source, const std::string& T = " ");

    /// Trim left.
    static std::string TrimLeft(const std::string& Source, const std::string& T = " ");

    /// Trim left and right.
    static std::string Trim(const std::string& Source, const std::string& T = " ");

private:
    CStringUtils(void);
    ~CStringUtils(void);
};

#endif // STRING_UTILS_H
