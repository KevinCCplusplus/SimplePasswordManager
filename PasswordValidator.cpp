// PasswordValidator.cpp

#include "PasswordValidator.h"
#include <regex>

bool ValidatePassword(const std::wstring& password)
{
    // Define regex patterns for password requirements
    std::wregex lengthRegex(L".{8,}"); // At least 8 characters
    std::wregex uppercaseRegex(L"[A-Z]"); // At least one uppercase letter
    std::wregex digitRegex(L"\\d"); // At least one digit
    std::wregex specialCharRegex(L"[^A-Za-z0-9]"); // At least one special character

    // Check if the password meets all requirements
    return std::regex_search(password, lengthRegex) &&
        std::regex_search(password, uppercaseRegex) &&
        std::regex_search(password, digitRegex) &&
        std::regex_search(password, specialCharRegex);
}
