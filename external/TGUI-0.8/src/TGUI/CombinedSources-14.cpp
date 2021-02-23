/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2020 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/String.hpp>

#include <cctype> // tolower, toupper, isspace

#if TGUI_COMPILED_WITH_CPP_VER >= 17
    #include <charconv>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    const decltype(std::u32string::npos) String::npos = std::u32string::npos;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isWhitespace(char character)
    {
        if (character == ' ' || character == '\t' || character == '\r' || character == '\n')
            return true;
        else
            return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef TGUI_NEXT
    bool isWhitespace(uint32_t character)
#else
    bool isWhitespace(sf::Uint32 character)
#endif
    {
        if (character == U' ' || character == U'\t' || character == U'\r' || character == U'\n')
            return true;
        else
            return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int strToInt(const std::string& str, int defaultValue)
    {
#if defined(__cpp_lib_to_chars) && (__cpp_lib_to_chars >= 201611L)
        int value;
        const char* cstr = str.c_str();
        if (std::from_chars(&cstr[0], &cstr[str.length()], value).ec == std::errc{})
            return value;
        else
            return defaultValue;
#else
        try
        {
            return std::stoi(str);
        }
        catch (const std::exception&)
        {
            return defaultValue;
        }
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int strToUInt(const std::string& str, unsigned int defaultValue)
    {
#if defined(__cpp_lib_to_chars) && (__cpp_lib_to_chars >= 201611L)
        unsigned int value;
        const char* cstr = str.c_str();
        if (std::from_chars(&cstr[0], &cstr[str.length()], value).ec == std::errc{})
            return value;
        else
            return defaultValue;
#else
        try
        {
            return static_cast<unsigned int>(std::stoi(str));
        }
        catch (const std::exception&)
        {
            return defaultValue;
        }
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float strToFloat(const std::string& str, float defaultValue)
    {
#if defined(__cpp_lib_to_chars) && (__cpp_lib_to_chars >= 201611L)
        float value;
        const char* cstr = str.c_str();
        if (std::from_chars(&cstr[0], &cstr[str.length()], value).ec == std::errc{})
            return value;
        else
            return defaultValue;
#else
        // We can't use std::stof because it always depends on the global locale
        std::istringstream iss(str);
        iss.imbue(std::locale::classic());

        float result = 0;
        iss >> result;

        if (iss.fail())
            result = defaultValue;

        return result;
#endif
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string toLower(const std::string& oldString)
    {
        std::string newString;
        newString.reserve(oldString.length() + 1);
        for (const char& c : oldString)
            newString.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));

        return newString;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string toUpper(const std::string& oldString)
    {
        std::string newString;
        newString.reserve(oldString.length() + 1);
        for (const char& c : oldString)
            newString.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));

        return newString;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string trim(const std::string& str)
    {
        const auto firstIt = std::find_if(str.begin(), str.end(), [](unsigned char c) { return !std::isspace(c); });
        if (firstIt == str.end())
            return {};

        const auto lastIt = std::find_if(str.rbegin(), str.rend(), [](unsigned char c) { return !std::isspace(c); }).base();
        return std::string(firstIt, lastIt);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int String::toInt(int defaultValue) const
    {
        return strToInt(utf::convertUtf32toLatin1(m_string), defaultValue);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int String::toUInt(unsigned defaultValue) const
    {
        return strToUInt(utf::convertUtf32toLatin1(m_string), defaultValue);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float String::toFloat(float defaultValue) const
    {
        return strToFloat(utf::convertUtf32toLatin1(m_string), defaultValue);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String String::trim() const
    {
        const auto firstIt = std::find_if(m_string.begin(), m_string.end(), [](char32_t c) { return (c >= 256) || !std::isspace(static_cast<unsigned char>(c)); });
        if (firstIt == m_string.end())
            return {};

        const auto lastIt = std::find_if(m_string.rbegin(), m_string.rend(), [](char32_t c) { return (c >= 256) || !std::isspace(static_cast<unsigned char>(c)); }).base();
        return {firstIt, lastIt};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String String::toLower() const
    {
        String str;
        str.reserve(m_string.length() + 1);
        for (const char32_t& c : m_string)
        {
            if (c < 256)
                str.push_back(static_cast<char32_t>(std::tolower(static_cast<unsigned char>(c))));
            else
                str.push_back(c);
        }

        return str;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String String::toUpper() const
    {
        String str;
        str.reserve(m_string.length() + 1);
        for (const char32_t& c : m_string)
        {
            if (c < 256)
                str.push_back(static_cast<char32_t>(std::toupper(static_cast<unsigned char>(c))));
            else
                str.push_back(c);
        }

        return str;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef TGUI_NEXT // Code not working on GCC 4.9
    String& String::replace(const String& searchFor, const String& replaceWith)
    {
        std::size_t step = replaceWith.length();
        std::size_t len = searchFor.length();
        std::size_t pos = find(searchFor);

        // Replace each occurrence of search
        while (pos != npos)
        {
            replace(pos, len, replaceWith);
            pos = find(searchFor, pos + step);
        }

        return *this;
    }
#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String::String(const std::string& str) :
        m_string(utf::convertUtf8toUtf32(str.begin(), str.end()))
    {
    }

    String::String(const std::wstring& str) :
        m_string(utf::convertWidetoUtf32(str.begin(), str.end()))
    {
    }

    String::String(const std::u16string& str) :
        m_string(utf::convertUtf16toUtf32(str.begin(), str.end()))
    {
    }

    String::String(const std::u32string& str)
        : m_string{str}
    {
    }

    String::String(char ansiChar)
        : m_string(1, static_cast<char32_t>(ansiChar))
    {
    }

    String::String(wchar_t wideChar)
        : m_string(1, static_cast<char32_t>(wideChar))
    {
    }

    String::String(char16_t utfChar)
        : m_string(1, static_cast<char32_t>(utfChar))
    {
    }

    String::String(char32_t utfChar)
        : m_string(1, utfChar)
    {
    }

    String::String(const char* str) :
        m_string(utf::convertUtf8toUtf32(str, str + std::char_traits<char>::length(str)))
    {
    }

    String::String(const wchar_t* str) :
        m_string(utf::convertWidetoUtf32(str, str + std::char_traits<wchar_t>::length(str)))
    {
    }

    String::String(const char16_t* str) :
        m_string(utf::convertUtf16toUtf32(str, str + std::char_traits<char16_t>::length(str)))
    {
    }

    String::String(const char32_t* str)
        : m_string{str}
    {
    }

    String::String(std::size_t count, char ch)
        : m_string(count, static_cast<char32_t>(ch))
    {
    }

    String::String(std::size_t count, wchar_t ch)
        : m_string(count, static_cast<char32_t>(ch))
    {
    }

    String::String(std::size_t count, char16_t ch)
        : m_string(count, static_cast<char32_t>(ch))
    {
    }

    String::String(std::size_t count, char32_t ch)
        : m_string(count, ch)
    {
    }

    String::String(const std::string& str, std::size_t pos)
        : String{std::string(str, pos)}
    {
    }

    String::String(const std::wstring& str, std::size_t pos)
        : String{std::wstring(str, pos)}
    {
    }

    String::String(const std::u16string& str, std::size_t pos)
        : String{std::u16string(str, pos)}
    {
    }

    String::String(const std::u32string& str, std::size_t pos)
        : m_string(str, pos)
    {
    }

    String::String(const std::string& str, std::size_t pos, std::size_t count)
        : String{std::string(str, pos, count)}
    {
    }

    String::String(const std::wstring& str, std::size_t pos, std::size_t count)
        : String{std::wstring(str, pos, count)}
    {
    }

    String::String(const std::u16string& str, std::size_t pos, std::size_t count)
        : String{std::u16string(str, pos, count)}
    {
    }

    String::String(const std::u32string& str, std::size_t pos, std::size_t count)
        : m_string(str, pos, count)
    {
    }

    String::String(const char* str, std::size_t count)
        : String{std::string{str, count}}
    {
    }

    String::String(const wchar_t* str, std::size_t count)
        : String{std::wstring{str, count}}
    {
    }

    String::String(const char16_t* str, std::size_t count)
        : String{std::u16string{str, count}}
    {
    }

    String::String(const char32_t* str, std::size_t count)
        : m_string{str, count}
    {
    }

    String::String(std::initializer_list<char> chars)
        : String(std::string(chars.begin(), chars.end()))
    {
    }

    String::String(std::initializer_list<wchar_t> chars)
        : String(std::wstring(chars.begin(), chars.end()))
    {
    }

    String::String(std::initializer_list<char16_t> chars)
        : String(std::u16string(chars.begin(), chars.end()))
    {
    }

    String::String(std::initializer_list<char32_t> chars)
        : m_string(chars)
    {
    }

    String::String(std::string::const_iterator first, std::string::const_iterator last)
        : String{std::string(first, last)}
    {
    }

    String::String(std::wstring::const_iterator first, std::wstring::const_iterator last)
        : String{std::wstring(first, last)}
    {
    }

    String::String(std::u16string::const_iterator first, std::u16string::const_iterator last)
        : String{std::u16string(first, last)}
    {
    }

    String::String(std::u32string::const_iterator first, std::u32string::const_iterator last)
        : m_string(first, last)
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String::operator std::string() const
    {
        return toAnsiString();
    }

    String::operator std::wstring() const
    {
        return toWideString();
    }

    String::operator std::u16string() const
    {
        return toUtf16();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string String::toAnsiString() const
    {
        return utf::convertUtf32toStdStringUtf8(m_string);
    }

    std::wstring String::toWideString() const
    {
        return utf::convertUtf32toWide(m_string);
    }

    std::u16string String::toUtf16() const
    {
        return utf::convertUtf32toUtf16(m_string);
    }

    const std::u32string& String::toUtf32() const
    {
        return m_string;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String& String::assign(std::size_t count, char ch)
    {
        m_string.assign(count, static_cast<char32_t>(ch));
        return *this;
    }

    String& String::assign(std::size_t count, wchar_t ch)
    {
        m_string.assign(count, static_cast<char32_t>(ch));
        return *this;
    }

    String& String::assign(std::size_t count, char16_t ch)
    {
        m_string.assign(count, static_cast<char32_t>(ch));
        return *this;
    }

    String& String::assign(std::size_t count, char32_t ch)
    {
        m_string.assign(count, ch);
        return *this;
    }

    String& String::assign(const std::string& str)
    {
        return *this = str;
    }

    String& String::assign(const std::wstring& str)
    {
        return *this = str;
    }

    String& String::assign(const std::u16string& str)
    {
        return *this = str;
    }

    String& String::assign(const std::u32string& str)
    {
        m_string.assign(str);
        return *this;
    }

    String& String::assign(const String& str)
    {
        m_string.assign(str.m_string);
        return *this;
    }

    String& String::assign(const std::string& str, std::size_t pos, std::size_t count)
    {
        return *this = {str, pos, count};
    }

    String& String::assign(const std::wstring& str, std::size_t pos, std::size_t count)
    {
        return *this = {str, pos, count};
    }

    String& String::assign(const std::u16string& str, std::size_t pos, std::size_t count)
    {
        return *this = {str, pos, count};
    }

    String& String::assign(const std::u32string& str, std::size_t pos, std::size_t count)
    {
        m_string.assign(str, pos, count);
        return *this;
    }

    String& String::assign(const String& str, std::size_t pos, std::size_t count)
    {
        m_string.assign(str.m_string, pos, count);
        return *this;
    }

    String& String::assign(std::string&& str)
    {
        return *this = std::move(str);
    }

    String& String::assign(std::wstring&& str)
    {
        return *this = std::move(str);
    }

    String& String::assign(std::u16string&& str)
    {
        return *this = std::move(str);
    }

    String& String::assign(std::u32string&& str)
    {
        m_string.assign(std::move(str));
        return *this;
    }

    String& String::assign(String&& str)
    {
        m_string.assign(std::move(str.m_string));
        return *this;
    }

    String& String::assign(const char* str, std::size_t count)
    {
        return *this = {str, count};
    }

    String& String::assign(const wchar_t* str, std::size_t count)
    {
        return *this = {str, count};
    }

    String& String::assign(const char16_t* str, std::size_t count)
    {
        return *this = {str, count};
    }

    String& String::assign(const char32_t* str, std::size_t count)
    {
        m_string.assign(str, count);
        return *this;
    }

    String& String::assign(const char* str)
    {
        return *this = str;
    }

    String& String::assign(const wchar_t* str)
    {
        return *this = str;
    }

    String& String::assign(const char16_t* str)
    {
        return *this = str;
    }

    String& String::assign(const char32_t* str)
    {
        m_string.assign(str);
        return *this;
    }

    String& String::assign(std::initializer_list<char> chars)
    {
        return *this = chars;
    }

    String& String::assign(std::initializer_list<wchar_t> chars)
    {
        return *this = chars;
    }

    String& String::assign(std::initializer_list<char16_t> chars)
    {
        return *this = chars;
    }

    String& String::assign(std::initializer_list<char32_t> chars)
    {
        m_string.assign(chars);
        return *this;
    }

    String& String::assign(std::string::const_iterator first, std::string::const_iterator last)
    {
        return *this = {first, last};
    }

    String& String::assign(std::wstring::const_iterator first, std::wstring::const_iterator last)
    {
        return *this = {first, last};
    }

    String& String::assign(std::u16string::const_iterator first, std::u16string::const_iterator last)
    {
        return *this = {first, last};
    }

    String& String::assign(std::u32string::const_iterator first, std::u32string::const_iterator last)
    {
        m_string.assign(first, last);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String::reference       String::at(std::size_t pos)
    {
        return m_string.at(pos);
    }

    String::const_reference String::at(std::size_t pos) const
    {
        return m_string.at(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String::const_reference String::operator [](std::size_t index) const
    {
        return m_string[index];
    }

    String::reference String::operator [](std::size_t index)
    {
        return m_string[index];
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String::reference String::front()
    {
        return m_string.front();
    }

    String::const_reference String::front() const
    {
        return m_string.front();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String::reference String::back()
    {
        return m_string.back();
    }

    String::const_reference String::back() const
    {
        return m_string.back();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const char32_t* String::data() const noexcept
    {
        return m_string.data();
    }

    const char32_t* String::c_str() const noexcept
    {
        return m_string.c_str();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String::iterator String::begin() noexcept
    {
        return m_string.begin();
    }

    String::const_iterator String::begin() const noexcept
    {
        return m_string.begin();
    }

    String::const_iterator String::cbegin() const noexcept
    {
        return m_string.cbegin();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String::iterator String::end() noexcept
    {
        return m_string.end();
    }

    String::const_iterator String::end() const noexcept
    {
        return m_string.end();
    }

    String::const_iterator String::cend() const noexcept
    {
        return m_string.cend();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String::reverse_iterator String::rbegin() noexcept
    {
        return m_string.rbegin();
    }

    String::const_reverse_iterator String::rbegin() const noexcept
    {
        return m_string.rbegin();
    }

    String::const_reverse_iterator String::crbegin() const noexcept
    {
        return m_string.crbegin();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String::reverse_iterator String::rend() noexcept
    {
        return m_string.rend();
    }

    String::const_reverse_iterator String::rend() const noexcept
    {
        return m_string.rend();
    }

    String::const_reverse_iterator String::crend() const noexcept
    {
        return m_string.crend();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void String::clear() noexcept
    {
        m_string.clear();
    }

    bool String::empty() const noexcept
    {
        return m_string.empty();
    }

    std::size_t String::size() const noexcept
    {
        return m_string.size();
    }

    std::size_t String::length() const noexcept
    {
        return m_string.length();
    }

    std::size_t String::max_size() const noexcept
    {
        return m_string.max_size();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void String::reserve(std::size_t newCap)
    {
        m_string.reserve(newCap);
    }

    std::size_t String::capacity() const noexcept
    {
        return m_string.capacity();
    }

    void String::shrink_to_fit()
    {
        m_string.shrink_to_fit();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef TGUI_NEXT // Code not working on GCC 4.9
    String& String::insert(std::size_t index, std::size_t count, char ch)
    {
        m_string.insert(index, count, static_cast<char32_t>(ch));
        return *this;
    }

    String& String::insert(std::size_t index, std::size_t count, wchar_t ch)
    {
        m_string.insert(index, count, static_cast<char32_t>(ch));
        return *this;
    }

    String& String::insert(std::size_t index, std::size_t count, char16_t ch)
    {
        m_string.insert(index, count, static_cast<char32_t>(ch));
        return *this;
    }

    String& String::insert(std::size_t index, std::size_t count, char32_t ch)
    {
        m_string.insert(index, count, ch);
        return *this;
    }

    String& String::insert(std::size_t index, const std::string& str)
    {
        m_string.insert(index, String{str}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const std::wstring& str)
    {
        m_string.insert(index, String{str}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const std::u16string& str)
    {
        m_string.insert(index, String{str}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const std::u32string& str)
    {
        m_string.insert(index, str);
        return *this;
    }

    String& String::insert(std::size_t index, const String& str)
    {
        m_string.insert(index, str.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const std::string& str, std::size_t pos, std::size_t count)
    {
        m_string.insert(index, String{str, pos, count}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const std::wstring& str, std::size_t pos, std::size_t count)
    {
        m_string.insert(index, String{str, pos, count}.m_string);
        return *this;
    }
    String& String::insert(std::size_t index, const std::u16string& str, std::size_t pos, std::size_t count)
    {
        m_string.insert(index, String{str, pos, count}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const std::u32string& str, std::size_t pos, std::size_t count)
    {
        m_string.insert(index, str, pos, count);
        return *this;
    }

    String& String::insert(std::size_t index, const String& str, std::size_t pos, std::size_t count)
    {
        m_string.insert(index, str.m_string, pos, count);
        return *this;
    }

    String& String::insert(std::size_t index, const char* str, std::size_t count)
    {
        m_string.insert(index, String{str, count}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const wchar_t* str, std::size_t count)
    {
        m_string.insert(index, String{str, count}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const char16_t* str, std::size_t count)
    {
        m_string.insert(index, String{str, count}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const char32_t* str, std::size_t count)
    {
        m_string.insert(index, str, count);
        return *this;
    }

    String& String::insert(std::size_t index, const char* str)
    {
        m_string.insert(index, String{str}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const wchar_t* str)
    {
        m_string.insert(index, String{str}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const char16_t* str)
    {
        m_string.insert(index, String{str}.m_string);
        return *this;
    }

    String& String::insert(std::size_t index, const char32_t* str)
    {
        m_string.insert(index, str);
        return *this;
    }

    String::iterator String::insert(const_iterator pos, char ch)
    {
        return m_string.insert(pos, static_cast<char32_t>(ch));
    }

    String::iterator String::insert(const_iterator pos, wchar_t ch)
    {
        return m_string.insert(pos, static_cast<char32_t>(ch));
    }

    String::iterator String::insert(const_iterator pos, char16_t ch)
    {
        return m_string.insert(pos, static_cast<char32_t>(ch));
    }

    String::iterator String::insert(const_iterator pos, char32_t ch)
    {
        return m_string.insert(pos, ch);
    }

    String::iterator String::insert(const_iterator pos, std::size_t count, char ch)
    {
        return m_string.insert(pos, count, static_cast<char32_t>(ch));
    }

    String::iterator String::insert(const_iterator pos, std::size_t count, wchar_t ch)
    {
        return m_string.insert(pos, count, static_cast<char32_t>(ch));
    }

    String::iterator String::insert(const_iterator pos, std::size_t count, char16_t ch)
    {
        return m_string.insert(pos, count, static_cast<char32_t>(ch));
    }

    String::iterator String::insert(const_iterator pos, std::size_t count, char32_t ch)
    {
        return m_string.insert(pos, count, ch);
    }

    String::iterator String::insert(const_iterator pos, std::initializer_list<char> chars)
    {
        const auto tmpStr = String{chars};
        return m_string.insert(pos, tmpStr.begin(), tmpStr.end());
    }

    String::iterator String::insert(const_iterator pos, std::initializer_list<wchar_t> chars)
    {
        const auto tmpStr = String{chars};
        return m_string.insert(pos, tmpStr.begin(), tmpStr.end());
    }

    String::iterator String::insert(const_iterator pos, std::initializer_list<char16_t> chars)
    {
        const auto tmpStr = String{chars};
        return m_string.insert(pos, tmpStr.begin(), tmpStr.end());
    }

    String::iterator String::insert(const_iterator pos, std::initializer_list<char32_t> chars)
    {
        // This function was missing in libstdc++ with GCC 8.2 on Arch Linux. It is present now with GCC 9.1.
        //return m_string.insert(pos, chars);

        const auto tmpStr = String{chars};
        return m_string.insert(pos, tmpStr.begin(), tmpStr.end());
    }

    String::iterator String::insert(const_iterator pos, std::string::const_iterator first, std::string::const_iterator last)
    {
        const auto tmpStr = String{first, last};
        return m_string.insert(pos, tmpStr.begin(), tmpStr.end());
    }

    String::iterator String::insert(const_iterator pos, std::wstring::const_iterator first, std::wstring::const_iterator last)
    {
        const auto tmpStr = String{first, last};
        return m_string.insert(pos, tmpStr.begin(), tmpStr.end());
    }

    String::iterator String::insert(const_iterator pos, std::u16string::const_iterator first, std::u16string::const_iterator last)
    {
        const auto tmpStr = String{first, last};
        return m_string.insert(pos, tmpStr.begin(), tmpStr.end());
    }

    String::iterator String::insert(const_iterator pos, std::u32string::const_iterator first, std::u32string::const_iterator last)
    {
        return m_string.insert(pos, first, last);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String& String::erase(std::size_t index, std::size_t count)
    {
        m_string.erase(index, count);
        return *this;
    }

    String::iterator String::erase(const_iterator position)
    {
        return m_string.erase(position);
    }

    String::iterator String::erase(const_iterator first, const_iterator last)
    {
        return m_string.erase(first, last);
    }
#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void String::push_back(char ch)
    {
        m_string.push_back(static_cast<char32_t>(ch));
    }

    void String::push_back(wchar_t ch)
    {
        m_string.push_back(static_cast<char32_t>(ch));
    }

    void String::push_back(char16_t ch)
    {
        m_string.push_back(static_cast<char32_t>(ch));
    }

    void String::push_back(char32_t ch)
    {
        m_string.push_back(ch);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void String::pop_back()
    {
        m_string.pop_back();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String& String::append(std::size_t count, char ch)
    {
        m_string.append(count, static_cast<char32_t>(ch));
        return *this;
    }

    String& String::append(std::size_t count, wchar_t ch)
    {
        m_string.append(count, static_cast<char32_t>(ch));
        return *this;
    }

    String& String::append(std::size_t count, char16_t ch)
    {
        m_string.append(count, static_cast<char32_t>(ch));
        return *this;
    }

    String& String::append(std::size_t count, char32_t ch)
    {
        m_string.append(count, ch);
        return *this;
    }

    String& String::append(const std::string& str)
    {
        return append(String{str});
    }

    String& String::append(const std::wstring& str)
    {
        return append(String{str});
    }

    String& String::append(const std::u16string& str)
    {
        return append(String{str});
    }

    String& String::append(const std::u32string& str)
    {
        m_string.append(str);
        return *this;
    }

    String& String::append(const String& str)
    {
        m_string.append(str.m_string);
        return *this;
    }

    String& String::append(const std::string& str, std::size_t pos, std::size_t count)
    {
        return append(String{str, pos, count});
    }

    String& String::append(const std::wstring& str, std::size_t pos, std::size_t count)
    {
        return append(String{str, pos, count});
    }

    String& String::append(const std::u16string& str, std::size_t pos, std::size_t count)
    {
        return append(String{str, pos, count});
    }

    String& String::append(const std::u32string& str, std::size_t pos, std::size_t count)
    {
        m_string.append(str, pos, count);
        return *this;
    }

    String& String::append(const String& str, std::size_t pos, std::size_t count)
    {
        m_string.append(str.m_string, pos, count);
        return *this;
    }

    String& String::append(const char* str, std::size_t count)
    {
        return append(String{str, count});
    }

    String& String::append(const wchar_t* str, std::size_t count)
    {
        return append(String{str, count});
    }

    String& String::append(const char16_t* str, std::size_t count)
    {
        return append(String{str, count});
    }

    String& String::append(const char32_t* str, std::size_t count)
    {
        m_string.append(str, count);
        return *this;
    }

    String& String::append(const char* str)
    {
        return append(String{str});
    }

    String& String::append(const wchar_t* str)
    {
        return append(String{str});
    }

    String& String::append(const char16_t* str)
    {
        return append(String{str});
    }

    String& String::append(const char32_t* str)
    {
        m_string.append(str);
        return *this;
    }

    String& String::append(std::initializer_list<char> chars)
    {
        return append(String{chars});
    }

    String& String::append(std::initializer_list<wchar_t> chars)
    {
        return append(String{chars});
    }

    String& String::append(std::initializer_list<char16_t> chars)
    {
        return append(String{chars});
    }

    String& String::append(std::initializer_list<char32_t> chars)
    {
        m_string.append(chars);
        return *this;
    }

    String& String::append(std::string::const_iterator first, std::string::const_iterator last)
    {
        return append(String{first, last});
    }

    String& String::append(std::wstring::const_iterator first, std::wstring::const_iterator last)
    {
        return append(String{first, last});
    }

    String& String::append(std::u16string::const_iterator first, std::u16string::const_iterator last)
    {
        return append(String{first, last});
    }

    String& String::append(std::u32string::const_iterator first, std::u32string::const_iterator last)
    {
        m_string.append(first, last);
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String& String::operator+=(const String& str)
    {
        m_string += str.m_string;
        return *this;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int String::compare(const std::string& str) const noexcept
    {
        return m_string.compare(String{str}.m_string);
    }

    int String::compare(const std::wstring& str) const noexcept
    {
        return m_string.compare(String{str}.m_string);
    }

    int String::compare(const std::u16string& str) const noexcept
    {
        return m_string.compare(String{str}.m_string);
    }

    int String::compare(const std::u32string& str) const noexcept
    {
        return m_string.compare(str);
    }

    int String::compare(const String& str) const noexcept
    {
        return m_string.compare(str.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const std::string& str) const
    {
        return m_string.compare(pos1, count1, String{str}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const std::wstring& str) const
    {
        return m_string.compare(pos1, count1, String{str}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const std::u16string& str) const
    {
        return m_string.compare(pos1, count1, String{str}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const std::u32string& str) const
    {
        return m_string.compare(pos1, count1, str);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const String& str) const
    {
        return m_string.compare(pos1, count1, str.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const std::string& str, std::size_t pos2, std::size_t count2) const
    {
        return m_string.compare(pos1, count1, String{str, pos2, count2}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const std::wstring& str, std::size_t pos2, std::size_t count2) const
    {
        return m_string.compare(pos1, count1, String{str, pos2, count2}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const std::u16string& str, std::size_t pos2, std::size_t count2) const
    {
        return m_string.compare(pos1, count1, String{str, pos2, count2}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const std::u32string& str, std::size_t pos2, std::size_t count2) const
    {
        return m_string.compare(pos1, count1, str, pos2, count2);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const String& str, std::size_t pos2, std::size_t count2) const
    {
        return m_string.compare(pos1, count1, str.m_string, pos2, count2);
    }

    int String::compare(const char* s) const
    {
        return m_string.compare(String{s}.m_string);
    }

    int String::compare(const wchar_t* s) const
    {
        return m_string.compare(String{s}.m_string);
    }

    int String::compare(const char16_t* s) const
    {
        return m_string.compare(String{s}.m_string);
    }

    int String::compare(const char32_t* s) const
    {
        return m_string.compare(s);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const char* s) const
    {
        return m_string.compare(pos1, count1, String{s}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const wchar_t* s) const
    {
        return m_string.compare(pos1, count1, String{s}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const char16_t* s) const
    {
        return m_string.compare(pos1, count1, String{s}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const char32_t* s) const
    {
        return m_string.compare(pos1, count1, s);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const char* s, std::size_t count2) const
    {
        return m_string.compare(pos1, count1, String{s, count2}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const wchar_t* s, std::size_t count2) const
    {
        return m_string.compare(pos1, count1, String{s, count2}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const char16_t* s, std::size_t count2) const
    {
        return m_string.compare(pos1, count1, String{s, count2}.m_string);
    }

    int String::compare(std::size_t pos1, std::size_t count1, const char32_t* s, std::size_t count2) const
    {
        return m_string.compare(pos1, count1, s, count2);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef TGUI_NEXT // Code not working on GCC 4.9
    String& String::replace(std::size_t pos, std::size_t count, const std::string& str)
    {
        m_string.replace(pos, count, String{str}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const std::wstring& str)
    {
        m_string.replace(pos, count, String{str}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const std::u16string& str)
    {
        m_string.replace(pos, count, String{str}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const std::u32string& str)
    {
        m_string.replace(pos, count, str);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const String& str)
    {
        m_string.replace(pos, count, str.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const std::string& str)
    {
        m_string.replace(first, last, String{str}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const std::wstring& str)
    {
        m_string.replace(first, last, String{str}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const std::u16string& str)
    {
        m_string.replace(first, last, String{str}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const std::u32string& str)
    {
        m_string.replace(first, last, str);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const String& str)
    {
        m_string.replace(first, last, str.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const std::string& str, std::size_t pos2, std::size_t count2)
    {
        m_string.replace(pos, count, String{str, pos2, count2}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const std::wstring& str, std::size_t pos2, std::size_t count2)
    {
        m_string.replace(pos, count, String{str, pos2, count2}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const std::u16string& str, std::size_t pos2, std::size_t count2)
    {
        m_string.replace(pos, count, String{str, pos2, count2}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const std::u32string& str, std::size_t pos2, std::size_t count2)
    {
        m_string.replace(pos, count, str, pos2, count2);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const String& str, std::size_t pos2, std::size_t count2)
    {
        m_string.replace(pos, count, str.m_string, pos2, count2);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::string::const_iterator first2, std::string::const_iterator last2)
    {
        m_string.replace(first, last, String{first2, last2}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::wstring::const_iterator first2, std::wstring::const_iterator last2)
    {
        m_string.replace(first, last, String{first2, last2}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::u16string::const_iterator first2, std::u16string::const_iterator last2)
    {
        m_string.replace(first, last, String{first2, last2}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::u32string::const_iterator first2, std::u32string::const_iterator last2)
    {
        m_string.replace(first, last, first2, last2);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const char* cstr, std::size_t count2)
    {
        m_string.replace(pos, count, String{cstr, count2}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const wchar_t* cstr, std::size_t count2)
    {
        m_string.replace(pos, count, String{cstr, count2}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const char16_t* cstr, std::size_t count2)
    {
        m_string.replace(pos, count, String{cstr, count2}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const char32_t* cstr, std::size_t count2)
    {
        m_string.replace(pos, count, cstr, count2);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const char* cstr, std::size_t count2)
    {
        m_string.replace(first, last, String{cstr, count2}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const wchar_t* cstr, std::size_t count2)
    {
        m_string.replace(first, last, String{cstr, count2}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const char16_t* cstr, std::size_t count2)
    {
        m_string.replace(first, last, String{cstr, count2}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const char32_t* cstr, std::size_t count2)
    {
        m_string.replace(first, last, cstr, count2);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const char* cstr)
    {
        m_string.replace(pos, count, String{cstr}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const wchar_t* cstr)
    {
        m_string.replace(pos, count, String{cstr}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const char16_t* cstr)
    {
        m_string.replace(pos, count, String{cstr}.m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, const char32_t* cstr)
    {
        m_string.replace(pos, count, cstr);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const char* cstr)
    {
        m_string.replace(first, last, String{cstr}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const wchar_t* cstr)
    {
        m_string.replace(first, last, String{cstr}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const char16_t* cstr)
    {
        m_string.replace(first, last, String{cstr}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, const char32_t* cstr)
    {
        m_string.replace(first, last, cstr);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, std::size_t count2, char ch)
    {
        m_string.replace(pos, count, String(count2, ch).m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, std::size_t count2, wchar_t ch)
    {
        m_string.replace(pos, count, String(count2, ch).m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, std::size_t count2, char16_t ch)
    {
        m_string.replace(pos, count, String(count2, ch).m_string);
        return *this;
    }

    String& String::replace(std::size_t pos, std::size_t count, std::size_t count2, char32_t ch)
    {
        m_string.replace(pos, count, count2, ch);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::size_t count2, char ch)
    {
        m_string.replace(first, last, String(count2, ch).m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::size_t count2, wchar_t ch)
    {
        m_string.replace(first, last, String(count2, ch).m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::size_t count2, char16_t ch)
    {
        m_string.replace(first, last, String(count2, ch).m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::size_t count2, char32_t ch)
    {
        m_string.replace(first, last, count2, ch);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::initializer_list<char> chars)
    {
        m_string.replace(first, last, String{chars}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::initializer_list<wchar_t> chars)
    {
        m_string.replace(first, last, String{chars}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::initializer_list<char16_t> chars)
    {
        m_string.replace(first, last, String{chars}.m_string);
        return *this;
    }

    String& String::replace(const_iterator first, const_iterator last, std::initializer_list<char32_t> chars)
    {
        m_string.replace(first, last, chars);
        return *this;
    }
#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    String String::substr(std::size_t pos, std::size_t count) const
    {
        return {m_string.substr(pos, count)};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t String::copy(char32_t* dest, std::size_t count, std::size_t pos) const
    {
        return m_string.copy(dest, count, pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void String::resize(std::size_t count)
    {
        m_string.resize(count);
    }

    void String::resize(std::size_t count, char ch)
    {
        m_string.resize(count, static_cast<char32_t>(ch));
    }

    void String::resize(std::size_t count, wchar_t ch)
    {
        m_string.resize(count, static_cast<char32_t>(ch));
    }

    void String::resize(std::size_t count, char16_t ch)
    {
        m_string.resize(count, static_cast<char32_t>(ch));
    }

    void String::resize(std::size_t count, char32_t ch)
    {
        m_string.resize(count, ch);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void String::swap(String& other)
    {
        m_string.swap(other.m_string);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t String::find(const std::string& str, std::size_t pos) const noexcept
    {
        return m_string.find(String{str}.m_string, pos);
    }

    std::size_t String::find(const std::wstring& str, std::size_t pos) const noexcept
    {
        return m_string.find(String{str}.m_string, pos);
    }

    std::size_t String::find(const std::u16string& str, std::size_t pos) const noexcept
    {
        return m_string.find(String{str}.m_string, pos);
    }

    std::size_t String::find(const std::u32string& str, std::size_t pos) const noexcept
    {
        return m_string.find(str, pos);
    }

    std::size_t String::find(const String& str, std::size_t pos) const noexcept
    {
        return m_string.find(str.m_string, pos);
    }

    std::size_t String::find(const char* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find(String{s, count}.m_string, pos);
    }

    std::size_t String::find(const wchar_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find(String{s, count}.m_string, pos);
    }

    std::size_t String::find(const char16_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find(String{s, count}.m_string, pos);
    }

    std::size_t String::find(const char32_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find(s, pos, count);
    }

    std::size_t String::find(const char* s, std::size_t pos) const
    {
        return m_string.find(String{s}.m_string, pos);
    }

    std::size_t String::find(const wchar_t* s, std::size_t pos) const
    {
        return m_string.find(String{s}.m_string, pos);
    }

    std::size_t String::find(const char16_t* s, std::size_t pos) const
    {
        return m_string.find(String{s}.m_string, pos);
    }

    std::size_t String::find(const char32_t* s, std::size_t pos) const
    {
        return m_string.find(s, pos);
    }

    std::size_t String::find(char ch, std::size_t pos) const noexcept
    {
        return m_string.find(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find(wchar_t ch, std::size_t pos) const noexcept
    {
        return m_string.find(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find(char16_t ch, std::size_t pos) const noexcept
    {
        return m_string.find(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find(char32_t ch, std::size_t pos) const noexcept
    {
        return m_string.find(ch, pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t String::find_first_of(const std::string& str, std::size_t pos) const noexcept
    {
        return m_string.find_first_of(String{str}.m_string, pos);
    }

    std::size_t String::find_first_of(const std::wstring& str, std::size_t pos) const noexcept
    {
        return m_string.find_first_of(String{str}.m_string, pos);
    }

    std::size_t String::find_first_of(const std::u16string& str, std::size_t pos) const noexcept
    {
        return m_string.find_first_of(String{str}.m_string, pos);
    }

    std::size_t String::find_first_of(const std::u32string& str, std::size_t pos) const noexcept
    {
        return m_string.find_first_of(str, pos);
    }

    std::size_t String::find_first_of(const String& str, std::size_t pos) const noexcept
    {
        return m_string.find_first_of(str.m_string, pos);
    }

    std::size_t String::find_first_of(const char* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_first_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_first_of(const wchar_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_first_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_first_of(const char16_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_first_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_first_of(const char32_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_first_of(s, pos, count);
    }

    std::size_t String::find_first_of(const char* s, std::size_t pos) const
    {
        return m_string.find_first_of(String{s}.m_string, pos);
    }

    std::size_t String::find_first_of(const wchar_t* s, std::size_t pos) const
    {
        return m_string.find_first_of(String{s}.m_string, pos);
    }

    std::size_t String::find_first_of(const char16_t* s, std::size_t pos) const
    {
        return m_string.find_first_of(String{s}.m_string, pos);
    }

    std::size_t String::find_first_of(const char32_t* s, std::size_t pos) const
    {
        return m_string.find_first_of(s, pos);
    }

    std::size_t String::find_first_of(char ch, std::size_t pos) const noexcept
    {
        return m_string.find_first_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_first_of(wchar_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_first_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_first_of(char16_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_first_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_first_of(char32_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_first_of(ch, pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t String::find_first_not_of(const std::string& str, std::size_t pos) const noexcept
    {
        return m_string.find_first_not_of(String{str}.m_string, pos);
    }

    std::size_t String::find_first_not_of(const std::wstring& str, std::size_t pos) const noexcept
    {
        return m_string.find_first_not_of(String{str}.m_string, pos);
    }

    std::size_t String::find_first_not_of(const std::u16string& str, std::size_t pos) const noexcept
    {
        return m_string.find_first_not_of(String{str}.m_string, pos);
    }

    std::size_t String::find_first_not_of(const std::u32string& str, std::size_t pos) const noexcept
    {
        return m_string.find_first_not_of(str, pos);
    }

    std::size_t String::find_first_not_of(const String& str, std::size_t pos) const noexcept
    {
        return m_string.find_first_not_of(str.m_string, pos);
    }

    std::size_t String::find_first_not_of(const char* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_first_not_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_first_not_of(const wchar_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_first_not_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_first_not_of(const char16_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_first_not_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_first_not_of(const char32_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_first_not_of(s, pos, count);
    }

    std::size_t String::find_first_not_of(const char* s, std::size_t pos) const
    {
        return m_string.find_first_not_of(String{s}.m_string, pos);
    }

    std::size_t String::find_first_not_of(const wchar_t* s, std::size_t pos) const
    {
        return m_string.find_first_not_of(String{s}.m_string, pos);
    }

    std::size_t String::find_first_not_of(const char16_t* s, std::size_t pos) const
    {
        return m_string.find_first_not_of(String{s}.m_string, pos);
    }

    std::size_t String::find_first_not_of(const char32_t* s, std::size_t pos) const
    {
        return m_string.find_first_not_of(s, pos);
    }

    std::size_t String::find_first_not_of(char ch, std::size_t pos) const noexcept
    {
        return m_string.find_first_not_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_first_not_of(wchar_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_first_not_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_first_not_of(char16_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_first_not_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_first_not_of(char32_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_first_not_of(ch, pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t String::rfind(const std::string& str, std::size_t pos) const noexcept
    {
        return m_string.rfind(String{str}.m_string, pos);
    }

    std::size_t String::rfind(const std::wstring& str, std::size_t pos) const noexcept
    {
        return m_string.rfind(String{str}.m_string, pos);
    }

    std::size_t String::rfind(const std::u16string& str, std::size_t pos) const noexcept
    {
        return m_string.rfind(String{str}.m_string, pos);
    }

    std::size_t String::rfind(const std::u32string& str, std::size_t pos) const noexcept
    {
        return m_string.rfind(str, pos);
    }

    std::size_t String::rfind(const String& str, std::size_t pos) const noexcept
    {
        return m_string.rfind(str.m_string, pos);
    }

    std::size_t String::rfind(const char* s, std::size_t pos, std::size_t count) const
    {
        return m_string.rfind(String{s, count}.m_string, pos);
    }

    std::size_t String::rfind(const wchar_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.rfind(String{s, count}.m_string, pos);
    }

    std::size_t String::rfind(const char16_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.rfind(String{s, count}.m_string, pos);
    }

    std::size_t String::rfind(const char32_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.rfind(s, pos, count);
    }

    std::size_t String::rfind(const char* s, std::size_t pos) const
    {
        return m_string.rfind(String{s}.m_string, pos);
    }

    std::size_t String::rfind(const wchar_t* s, std::size_t pos) const
    {
        return m_string.rfind(String{s}.m_string, pos);
    }

    std::size_t String::rfind(const char16_t* s, std::size_t pos) const
    {
        return m_string.rfind(String{s}.m_string, pos);
    }

    std::size_t String::rfind(const char32_t* s, std::size_t pos) const
    {
        return m_string.rfind(s, pos);
    }

    std::size_t String::rfind(char ch, std::size_t pos) const noexcept
    {
        return m_string.rfind(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::rfind(wchar_t ch, std::size_t pos) const noexcept
    {
        return m_string.rfind(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::rfind(char16_t ch, std::size_t pos) const noexcept
    {
        return m_string.rfind(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::rfind(char32_t ch, std::size_t pos) const noexcept
    {
        return m_string.rfind(ch, pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t String::find_last_of(const std::string& str, std::size_t pos) const noexcept
    {
        return m_string.find_last_of(String{str}.m_string, pos);
    }

    std::size_t String::find_last_of(const std::wstring& str, std::size_t pos) const noexcept
    {
        return m_string.find_last_of(String{str}.m_string, pos);
    }

    std::size_t String::find_last_of(const std::u16string& str, std::size_t pos) const noexcept
    {
        return m_string.find_last_of(String{str}.m_string, pos);
    }

    std::size_t String::find_last_of(const std::u32string& str, std::size_t pos) const noexcept
    {
        return m_string.find_last_of(str, pos);
    }

    std::size_t String::find_last_of(const String& str, std::size_t pos) const noexcept
    {
        return m_string.find_last_of(str.m_string, pos);
    }

    std::size_t String::find_last_of(const char* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_last_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_last_of(const wchar_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_last_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_last_of(const char16_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_last_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_last_of(const char32_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_last_of(s, pos, count);
    }

    std::size_t String::find_last_of(const char* s, std::size_t pos) const
    {
        return m_string.find_last_of(String{s}.m_string, pos);
    }

    std::size_t String::find_last_of(const wchar_t* s, std::size_t pos) const
    {
        return m_string.find_last_of(String{s}.m_string, pos);
    }

    std::size_t String::find_last_of(const char16_t* s, std::size_t pos) const
    {
        return m_string.find_last_of(String{s}.m_string, pos);
    }

    std::size_t String::find_last_of(const char32_t* s, std::size_t pos) const
    {
        return m_string.find_last_of(s, pos);
    }

    std::size_t String::find_last_of(char ch, std::size_t pos) const noexcept
    {
        return m_string.find_last_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_last_of(wchar_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_last_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_last_of(char16_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_last_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_last_of(char32_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_last_of(ch, pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t String::find_last_not_of(const std::string& str, std::size_t pos) const noexcept
    {
        return m_string.find_last_not_of(String{str}.m_string, pos);
    }

    std::size_t String::find_last_not_of(const std::wstring& str, std::size_t pos) const noexcept
    {
        return m_string.find_last_not_of(String{str}.m_string, pos);
    }

    std::size_t String::find_last_not_of(const std::u16string& str, std::size_t pos) const noexcept
    {
        return m_string.find_last_not_of(String{str}.m_string, pos);
    }

    std::size_t String::find_last_not_of(const std::u32string& str, std::size_t pos) const noexcept
    {
        return m_string.find_last_not_of(str, pos);
    }

    std::size_t String::find_last_not_of(const String& str, std::size_t pos) const noexcept
    {
        return m_string.find_last_not_of(str.m_string, pos);
    }

    std::size_t String::find_last_not_of(const char* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_last_not_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_last_not_of(const wchar_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_last_not_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_last_not_of(const char16_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_last_not_of(String{s, count}.m_string, pos);
    }

    std::size_t String::find_last_not_of(const char32_t* s, std::size_t pos, std::size_t count) const
    {
        return m_string.find_last_not_of(s, pos, count);
    }

    std::size_t String::find_last_not_of(const char* s, std::size_t pos) const
    {
        return m_string.find_last_not_of(String{s}.m_string, pos);
    }

    std::size_t String::find_last_not_of(const wchar_t* s, std::size_t pos) const
    {
        return m_string.find_last_not_of(String{s}.m_string, pos);
    }

    std::size_t String::find_last_not_of(const char16_t* s, std::size_t pos) const
    {
        return m_string.find_last_not_of(String{s}.m_string, pos);
    }

    std::size_t String::find_last_not_of(const char32_t* s, std::size_t pos) const
    {
        return m_string.find_last_not_of(s, pos);
    }

    std::size_t String::find_last_not_of(char ch, std::size_t pos) const noexcept
    {
        return m_string.find_last_not_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_last_not_of(wchar_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_last_not_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_last_not_of(char16_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_last_not_of(static_cast<char32_t>(ch), pos);
    }

    std::size_t String::find_last_not_of(char32_t ch, std::size_t pos) const noexcept
    {
        return m_string.find_last_not_of(ch, pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os, const String& str)
    {
        os << std::string(str);
        return os;
    }

    std::basic_ostream<wchar_t>& operator<<(std::basic_ostream<wchar_t>& os, const String& str)
    {
        os << std::wstring(str);
        return os;
    }

// Code didn't compile with AppleClang 9.1 (Xcode 9.4). Android build also failed, so libc++ issue?
/*
    std::basic_ostream<char16_t>& operator<<(std::basic_ostream<char16_t>& os, const String& str)
    {
        os << std::u16string(str);
        return os;
    }

    std::basic_ostream<char32_t>& operator<<(std::basic_ostream<char32_t>& os, const String& str)
    {
        os << std::u32string(str);
        return os;
    }
*/

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2020 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Renderers/ListBoxRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_OUTLINE(ListBoxRenderer, Borders)
    TGUI_RENDERER_PROPERTY_OUTLINE(ListBoxRenderer, Padding)

    TGUI_RENDERER_PROPERTY_COLOR(ListBoxRenderer, BackgroundColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(ListBoxRenderer, BackgroundColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(ListBoxRenderer, SelectedBackgroundColor, Color(0, 110, 255))
    TGUI_RENDERER_PROPERTY_COLOR(ListBoxRenderer, SelectedBackgroundColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(ListBoxRenderer, TextColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(ListBoxRenderer, TextColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(ListBoxRenderer, SelectedTextColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(ListBoxRenderer, SelectedTextColorHover, {})
    TGUI_RENDERER_PROPERTY_COLOR(ListBoxRenderer, BorderColor, Color::Black)

    TGUI_RENDERER_PROPERTY_TEXTURE(ListBoxRenderer, TextureBackground)

    TGUI_RENDERER_PROPERTY_TEXT_STYLE(ListBoxRenderer, TextStyle, sf::Text::Regular)
    TGUI_RENDERER_PROPERTY_TEXT_STYLE(ListBoxRenderer, SelectedTextStyle, {})

    TGUI_RENDERER_PROPERTY_RENDERER(ListBoxRenderer, Scrollbar, "scrollbar")
    TGUI_RENDERER_PROPERTY_NUMBER(ListBoxRenderer, ScrollbarWidth, 0)
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2020 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Vector2f.hpp>
#include <TGUI/Clipping.hpp>
#include <TGUI/SignalImpl.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    namespace
    {
        float clamp(float value, float lower, float upper)
        {
            if (value < lower)
                return lower;

            if (value > upper)
                return upper;

            return value;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindow::ChildWindow(const sf::String& title, unsigned int titleButtons)
    {
        m_type = "ChildWindow";
        m_isolatedFocus = true;
        m_titleText.setFont(m_fontCached);

        setTitleTextSize(getGlobalTextSize());
        m_titleBarHeightCached = m_titleText.getSize().y * 1.25f;

        m_renderer = aurora::makeCopied<ChildWindowRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setTitle(title);
        setTitleButtons(titleButtons);
        setSize({400, 300});

        m_maximizeButton->connect("pressed", [this]{ onMaximize.emit(this); });
        m_minimizeButton->connect("pressed", [this]{ onMinimize.emit(this); });
        m_closeButton->connect("pressed", [this]{ close(); });
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindow::Ptr ChildWindow::create(const sf::String& title, unsigned int titleButtons)
    {
        return std::make_shared<ChildWindow>(title, titleButtons);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindow::Ptr ChildWindow::copy(ChildWindow::ConstPtr childWindow)
    {
        if (childWindow)
            return std::static_pointer_cast<ChildWindow>(childWindow->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindowRenderer* ChildWindow::getSharedRenderer()
    {
        return aurora::downcast<ChildWindowRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const ChildWindowRenderer* ChildWindow::getSharedRenderer() const
    {
        return aurora::downcast<const ChildWindowRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindowRenderer* ChildWindow::getRenderer()
    {
        return aurora::downcast<ChildWindowRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const ChildWindowRenderer* ChildWindow::getRenderer() const
    {
        return aurora::downcast<const ChildWindowRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setPosition(const Layout2d& position)
    {
        float x = position.getValue().x;
        float y = position.getValue().y;

        if (m_keepInParent && m_parent && (m_parent->getSize().x > 0) && (m_parent->getSize().y > 0)
         && ((y < 0) || (y > m_parent->getSize().y - getFullSize().y) || (x < 0) || (x > m_parent->getSize().x - getFullSize().x)))
        {
            if (y < 0)
                y = 0;
            else if (y > m_parent->getSize().y - getFullSize().y)
                y = std::max(0.f, m_parent->getSize().y - getFullSize().y);

            if (x < 0)
                x = 0;
            else if (x > m_parent->getSize().x - getFullSize().x)
                x = std::max(0.f, m_parent->getSize().x - getFullSize().x);

            Container::setPosition({x, y});
        }
        else
            Container::setPosition(position);

        // Calculate the distance from the right side that the buttons will need
        float buttonOffsetX = 0;
        for (const auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
        {
            if (button->isVisible())
                buttonOffsetX += (buttonOffsetX > 0 ? m_paddingBetweenButtonsCached : 0) + button->getSize().x;
        }

        if (buttonOffsetX > 0)
            buttonOffsetX += m_distanceToSideCached;

        if (m_titleAlignment == TitleAlignment::Left)
        {
            m_titleText.setPosition(m_distanceToSideCached,
                                    (m_titleBarHeightCached - m_titleText.getSize().y) / 2.0f);
        }
        else if (m_titleAlignment == TitleAlignment::Center)
        {
            m_titleText.setPosition(m_distanceToSideCached + ((getSize().x - (2 * m_distanceToSideCached) - buttonOffsetX - m_titleText.getSize().x) / 2.0f),
                                    (m_titleBarHeightCached - m_titleText.getSize().y) / 2.0f);
        }
        else // if (m_titleAlignment == TitleAlignment::Right)
        {
            m_titleText.setPosition(getSize().x - m_distanceToSideCached - buttonOffsetX - m_titleText.getSize().x,
                                    (m_titleBarHeightCached - m_titleText.getSize().y) / 2.0f);
        }

        buttonOffsetX = m_distanceToSideCached;
        for (auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
        {
            if (button->isVisible())
            {
                button->setPosition(m_bordersCached.getLeft() + getSize().x - buttonOffsetX - button->getSize().x,
                                    m_bordersCached.getTop() + (m_titleBarHeightCached - button->getSize().y) / 2.f);

                buttonOffsetX += button->getSize().x + m_paddingBetweenButtonsCached;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setSize(const Layout2d& size)
    {
        Container::setSize(size);

        m_bordersCached.updateParentSize(getSize());

        m_spriteTitleBar.setSize({getSize().x, m_titleBarHeightCached});
        m_spriteBackground.setSize(getSize());

        // Reposition the images and text
        setPosition(m_position);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f ChildWindow::getFullSize() const
    {
        return {getSize().x + m_bordersCached.getLeft() + m_bordersCached.getRight(),
                getSize().y + m_bordersCached.getTop() + m_bordersCached.getBottom() + m_titleBarHeightCached + m_borderBelowTitleBarCached};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setMaximumSize(Vector2f size)
    {
        if ((size.x < getSize().x) || (size.y < getSize().y))
        {
            // The window is currently larger than the new maximum size, lets downsize
            setSize(std::min(size.x, getSize().x), std::min(size.y, getSize().y));
        }

        m_maximumSize = size;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f ChildWindow::getMaximumSize() const
    {
        return m_maximumSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setMinimumSize(Vector2f size)
    {
        if ((size.x > getSize().x) || (size.y > getSize().y))
        {
            // The window is currently smaller than the new minimum size, lets upsize
            setSize(std::max(size.x, getSize().x), std::max(size.y, getSize().y));
        }

        m_minimumSize = size;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f ChildWindow::getMinimumSize() const
    {
        return m_minimumSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setTitle(const sf::String& title)
    {
        m_titleText.setString(title);

        // Reposition the images and text
        setPosition(m_position);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const sf::String& ChildWindow::getTitle() const
    {
        return m_titleText.getString();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setTitleTextSize(unsigned int size)
    {
        m_titleTextSize = size;

        if (m_titleTextSize)
            m_titleText.setCharacterSize(m_titleTextSize);
        else
            m_titleText.setCharacterSize(Text::findBestTextSize(m_fontCached, m_titleBarHeightCached * 0.8f));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int ChildWindow::getTitleTextSize() const
    {
        return m_titleTextSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setTitleAlignment(TitleAlignment alignment)
    {
        m_titleAlignment = alignment;

        // Reposition the images and text
        setPosition(m_position);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChildWindow::TitleAlignment ChildWindow::getTitleAlignment() const
    {
        return m_titleAlignment;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setTitleButtons(unsigned int buttons)
    {
        m_titleButtons = buttons;

        if (m_titleButtons & TitleButton::Close)
        {
            m_closeButton->setVisible(true);
            m_closeButton->setRenderer(getSharedRenderer()->getCloseButton());
            m_closeButton->setInheritedOpacity(m_opacityCached);

            if (m_showTextOnTitleButtonsCached)
                m_closeButton->setText(L"\u2715");
            else
                m_closeButton->setText("");
        }
        else
            m_closeButton->setVisible(false);

        if (m_titleButtons & TitleButton::Maximize)
        {
            m_maximizeButton->setVisible(true);
            m_maximizeButton->setRenderer(getSharedRenderer()->getMaximizeButton());
            m_maximizeButton->setInheritedOpacity(m_opacityCached);

            if (m_showTextOnTitleButtonsCached)
                m_maximizeButton->setText(L"\u2610");
            else
                m_maximizeButton->setText("");
        }
        else
            m_maximizeButton->setVisible(false);

        if (m_titleButtons & TitleButton::Minimize)
        {
            m_minimizeButton->setVisible(true);
            m_minimizeButton->setRenderer(getSharedRenderer()->getMinimizeButton());
            m_minimizeButton->setInheritedOpacity(m_opacityCached);

            if (m_showTextOnTitleButtonsCached)
                m_minimizeButton->setText(L"\u2043");
            else
                m_minimizeButton->setText("");
        }
        else
            m_minimizeButton->setVisible(false);

        updateTitleBarHeight();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int ChildWindow::getTitleButtons() const
    {
        return m_titleButtons;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::close()
    {
        if (!onClose.emit(this))
            destroy();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::destroy()
    {
        if (m_parent)
            m_parent->remove(shared_from_this());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setResizable(bool resizable)
    {
        m_resizable = resizable;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChildWindow::isResizable() const
    {
        return m_resizable;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setPositionLocked(bool positionLocked)
    {
        m_positionLocked = positionLocked;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChildWindow::isPositionLocked() const
    {
        return m_positionLocked;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setKeepInParent(bool enabled)
    {
        m_keepInParent = enabled;

        if (enabled)
            setPosition(m_position);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChildWindow::isKeptInParent() const
    {
        return m_keepInParent;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f ChildWindow::getChildWidgetsOffset() const
    {
        return {m_bordersCached.getLeft(), m_bordersCached.getTop() + m_titleBarHeightCached + m_borderBelowTitleBarCached};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::setParent(Container* parent)
    {
        Container::setParent(parent);
        if (m_keepInParent)
            setPosition(m_position);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChildWindow::mouseOnWidget(Vector2f pos) const
    {
        FloatRect region{getPosition(), getFullSize()};

        // Expand the region if the child window is resizable (to make the borders easier to click on)
        if (m_resizable)
        {
            region.left -= std::max(0.f, m_minimumResizableBorderWidthCached - m_bordersCached.getLeft());
            region.top -= std::max(0.f, m_minimumResizableBorderWidthCached - m_bordersCached.getTop());
            region.width += std::max(0.f, m_minimumResizableBorderWidthCached - m_bordersCached.getLeft())
                            + std::max(0.f, m_minimumResizableBorderWidthCached - m_bordersCached.getRight());
            region.height += std::max(0.f, m_minimumResizableBorderWidthCached - m_bordersCached.getTop())
                             + std::max(0.f, m_minimumResizableBorderWidthCached - m_bordersCached.getBottom());
        }

        if (region.contains(pos))
        {
            // If the mouse enters the border or title bar then then none of the widgets can still be under the mouse
            if (m_widgetBelowMouse && !FloatRect{getPosition() + getChildWidgetsOffset(), getSize()}.contains(pos))
                m_widgetBelowMouse->mouseNoLongerOnWidget();

            return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::leftMousePressed(Vector2f pos)
    {
        pos -= getPosition();

        m_mouseDown = true;

        // Move the child window to the front
        moveToFront();

        onMousePress.emit(this);

        if (FloatRect{getChildWidgetsOffset(), getSize()}.contains(pos))
        {
            // Propagate the event to the child widgets
            Container::leftMousePressed(pos + getPosition());
        }
        else if (!FloatRect{m_bordersCached.getLeft(), m_bordersCached.getTop(), getSize().x, getSize().y + m_titleBarHeightCached + m_borderBelowTitleBarCached}.contains(pos))
        {
            if (!m_focused)
                setFocused(true);

            // Mouse is on top of the borders
            if (m_resizable)
            {
                // Check on which border the mouse is standing
                m_resizeDirection = ResizeNone;
                if (pos.x < m_bordersCached.getLeft())
                    m_resizeDirection |= ResizeLeft;
                if (pos.y < m_bordersCached.getTop())
                    m_resizeDirection |= ResizeTop;
                if (pos.x >= getFullSize().x - m_bordersCached.getRight())
                    m_resizeDirection |= ResizeRight;
                if (pos.y >= getFullSize().y - m_bordersCached.getBottom())
                    m_resizeDirection |= ResizeBottom;
            }

            m_draggingPosition = pos;
        }
        else if (FloatRect{m_bordersCached.getLeft(), m_bordersCached.getTop(), getSize().x, m_titleBarHeightCached}.contains(pos))
        {
            if (!m_focused)
                setFocused(true);

            // Send the mouse press event to the title buttons
            for (auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
            {
                if (button->isVisible() && button->mouseOnWidget(pos))
                {
                    button->leftMousePressed(pos);
                    return;
                }
            }

            // The mouse went down on the title bar
            if (!m_positionLocked)
            {
                m_mouseDownOnTitleBar = true;
                m_draggingPosition = pos;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::leftMouseReleased(Vector2f pos)
    {
        pos -= getPosition();

        m_mouseDownOnTitleBar = false;
        m_resizeDirection = ResizeNone;

        if (FloatRect{getChildWidgetsOffset(), getSize()}.contains(pos))
        {
            // Propagate the event to the child widgets
            Container::leftMouseReleased(pos + getPosition());
        }
        else
        {
            // Tell the widgets that the mouse was released
            for (auto& widget : m_widgets)
                widget->leftMouseButtonNoLongerDown();

            // Check if the mouse is on top of the title bar
            if (FloatRect{m_bordersCached.getLeft(), m_bordersCached.getTop(), getSize().x, m_titleBarHeightCached}.contains(pos))
            {
                // Send the mouse release event to the title buttons
                for (auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
                {
                    if (button->isVisible() && button->mouseOnWidget(pos))
                    {
                        button->leftMouseReleased(pos);
                        break;
                    }
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::rightMousePressed(Vector2f pos)
    {
        if (FloatRect{getChildWidgetsOffset(), getSize()}.contains(pos - getPosition()))
            Container::rightMousePressed(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::rightMouseReleased(Vector2f pos)
    {
        pos -= getPosition();

        if (FloatRect{getChildWidgetsOffset(), getSize()}.contains(pos))
        {
            // Propagate the event to the child widgets
            Container::rightMouseReleased(pos + getPosition());
        }
        else
        {
            // Tell the widgets that the mouse was released
            for (auto& widget : m_widgets)
                widget->rightMouseButtonNoLongerDown();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::mouseMoved(Vector2f pos)
    {
        pos -= getPosition();

        // Check if you are dragging the child window
        if (m_mouseDown && m_mouseDownOnTitleBar)
        {
            // Move the child window, but don't allow the dragging position to leave the screen
            Vector2f newPosition;
            if (getPosition().x + pos.x <= 0)
                newPosition.x = -m_draggingPosition.x + 1;
            else if (m_parent && getPosition().x + pos.x >= m_parent->getSize().x)
                newPosition.x = m_parent->getSize().x - m_draggingPosition.x - 1;
            else
                newPosition.x = getPosition().x + (pos.x - m_draggingPosition.x);

            if (getPosition().y + pos.y <= 0)
                newPosition.y = -m_draggingPosition.y + 1;
            else if (m_parent && getPosition().y + pos.y >= m_parent->getSize().y)
                newPosition.y = m_parent->getSize().y - m_draggingPosition.y - 1;
            else
                newPosition.y = getPosition().y + (pos.y - m_draggingPosition.y);

            setPosition(newPosition);
        }

        // Check if you are resizing the window
        else if (m_mouseDown && m_resizeDirection != ResizeNone)
        {
            float minimumWidth = 0;
            for (const auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
            {
                if (button->isVisible())
                    minimumWidth += (minimumWidth > 0 ? m_paddingBetweenButtonsCached : 0) + button->getSize().x;
            }
            minimumWidth += 2 * m_distanceToSideCached;

            if ((m_resizeDirection & ResizeLeft) != 0)
            {
                const float diff = clamp(m_draggingPosition.x - pos.x, std::max(minimumWidth, m_minimumSize.x) - getSize().x, m_maximumSize.x - getSize().x);
                setPosition(getPosition().x - diff, getPosition().y);
                setSize(getSize().x + diff, getSize().y);
            }
            else if ((m_resizeDirection & ResizeRight) != 0)
            {
                const float diff = clamp(pos.x - m_draggingPosition.x, std::max(minimumWidth, m_minimumSize.x) - getSize().x, m_maximumSize.x - getSize().x);
                setSize(getSize().x + diff, getSize().y);
                m_draggingPosition.x += diff;
            }

            if ((m_resizeDirection & ResizeTop) != 0)
            {
                const float diff = clamp(m_draggingPosition.y - pos.y, m_minimumSize.y - getSize().y, m_maximumSize.y - getSize().y);
                setPosition(getPosition().x, getPosition().y - diff);
                setSize(getSize().x, getSize().y + diff);
            }
            else if ((m_resizeDirection & ResizeBottom) != 0)
            {
                const float diff = clamp(pos.y - m_draggingPosition.y, m_minimumSize.y - getSize().y, m_maximumSize.y - getSize().y);
                setSize(getSize().x, getSize().y + diff);
                m_draggingPosition.y += diff;
            }
        }
        else // Not dragging child window
        {
            if (FloatRect{getChildWidgetsOffset(), getSize()}.contains(pos))
            {
                // Propagate the event to the child widgets
                Container::mouseMoved(pos + getPosition());
            }
            else
            {
                if (!m_mouseHover)
                    mouseEnteredWidget();

                // Check if the mouse is on top of the title bar
                if (FloatRect{m_bordersCached.getLeft(), m_bordersCached.getTop(), getSize().x, m_titleBarHeightCached}.contains(pos))
                {
                    // Send the hover event to the buttons inside the title bar
                    for (auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
                    {
                        if (button->isVisible())
                        {
                            if (button->mouseOnWidget(pos))
                                button->mouseMoved(pos);
                            else
                                button->mouseNoLongerOnWidget();
                        }
                    }
                }
                else // When the mouse is not on the title bar, the mouse can't be on the buttons inside it
                {
                    for (auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
                    {
                        if (button->isVisible())
                            button->mouseNoLongerOnWidget();
                    }
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::keyPressed(const sf::Event::KeyEvent& event)
    {
        if (event.code == sf::Keyboard::Escape)
            onEscapeKeyPressed.emit(this);

        Container::keyPressed(event);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::mouseNoLongerOnWidget()
    {
        Container::mouseNoLongerOnWidget();

        for (const auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
        {
            if (button->isVisible())
                button->mouseNoLongerOnWidget();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::leftMouseButtonNoLongerDown()
    {
        Container::leftMouseButtonNoLongerDown();

        for (const auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
        {
            if (button->isVisible())
                button->leftMouseButtonNoLongerDown();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::updateTitleBarHeight()
    {
        m_spriteTitleBar.setSize({getSize().x, m_titleBarHeightCached});

        // Set the size of the buttons in the title bar
        for (auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
        {
            if (button->isVisible())
            {
                if (m_spriteTitleBar.isSet() && (button->getSharedRenderer()->getTexture().getData() != nullptr))
                {
                    button->setSize(button->getSharedRenderer()->getTexture().getImageSize().x * (m_titleBarHeightCached / m_spriteTitleBar.getTexture().getImageSize().y),
                                    button->getSharedRenderer()->getTexture().getImageSize().y * (m_titleBarHeightCached / m_spriteTitleBar.getTexture().getImageSize().y));
                }
                else
                    button->setSize({m_titleBarHeightCached * 0.8f, m_titleBarHeightCached * 0.8f});

                const Borders& buttonBorders = button->getSharedRenderer()->getBorders();
                button->setTextSize(Text::findBestTextSize(m_titleText.getFont(), (button->getSize().y - buttonBorders.getTop() - buttonBorders.getBottom()) * 0.8f));
            }
        }

        if (m_titleTextSize == 0)
            m_titleText.setCharacterSize(Text::findBestTextSize(m_fontCached, m_titleBarHeightCached * 0.8f));

        // Reposition the images and text
        setPosition(m_position);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Signal& ChildWindow::getSignal(std::string signalName)
    {
        if (signalName == toLower(onMousePress.getName()))
            return onMousePress;
        else if (signalName == toLower(onClose.getName()))
            return onClose;
        else if (signalName == toLower(onMinimize.getName()))
            return onMinimize;
        else if (signalName == toLower(onMaximize.getName()))
            return onMaximize;
        else if (signalName == toLower(onEscapeKeyPressed.getName()))
            return onEscapeKeyPressed;
        else
            return Container::getSignal(std::move(signalName));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::rendererChanged(const std::string& property)
    {
        if (property == "borders")
        {
            m_bordersCached = getSharedRenderer()->getBorders();
            setSize(m_size);
        }
        else if (property == "titlecolor")
        {
            m_titleText.setColor(getSharedRenderer()->getTitleColor());
        }
        else if (property == "texturetitlebar")
        {
            m_spriteTitleBar.setTexture(getSharedRenderer()->getTextureTitleBar());

            // If the title bar height is determined by the texture then update it (note that getTitleBarHeight has a non-trivial implementation)
            m_titleBarHeightCached = getSharedRenderer()->getTitleBarHeight();
            if (m_titleBarHeightCached == m_spriteTitleBar.getTexture().getImageSize().y)
                updateTitleBarHeight();
        }
        else if (property == "texturebackground")
        {
            m_spriteBackground.setTexture(getSharedRenderer()->getTextureBackground());
        }
        else if (property == "titlebarheight")
        {
            m_titleBarHeightCached = getSharedRenderer()->getTitleBarHeight();
            updateTitleBarHeight();
        }
        else if (property == "borderbelowtitlebar")
        {
            m_borderBelowTitleBarCached = getSharedRenderer()->getBorderBelowTitleBar();
        }
        else if (property == "distancetoside")
        {
            m_distanceToSideCached = getSharedRenderer()->getDistanceToSide();
            setPosition(m_position);
        }
        else if (property == "paddingbetweenbuttons")
        {
            m_paddingBetweenButtonsCached = getSharedRenderer()->getPaddingBetweenButtons();
            setPosition(m_position);
        }
        else if (property == "minimumresizableborderwidth")
        {
            m_minimumResizableBorderWidthCached = getSharedRenderer()->getMinimumResizableBorderWidth();
        }
        else if (property == "showtextontitlebuttons")
        {
            m_showTextOnTitleButtonsCached = getSharedRenderer()->getShowTextOnTitleButtons();
            setTitleButtons(m_titleButtons);
        }
        else if (property == "closebutton")
        {
            if (m_closeButton->isVisible())
            {
                m_closeButton->setRenderer(getSharedRenderer()->getCloseButton());
                m_closeButton->setInheritedOpacity(m_opacityCached);
            }

            updateTitleBarHeight();
        }
        else if (property == "maximizebutton")
        {
            if (m_maximizeButton->isVisible())
            {
                m_maximizeButton->setRenderer(getSharedRenderer()->getMaximizeButton());
                m_maximizeButton->setInheritedOpacity(m_opacityCached);
            }

            updateTitleBarHeight();
        }
        else if (property == "minimizebutton")
        {
            if (m_minimizeButton->isVisible())
            {
                m_minimizeButton->setRenderer(getSharedRenderer()->getMinimizeButton());
                m_minimizeButton->setInheritedOpacity(m_opacityCached);
            }

            updateTitleBarHeight();
        }
        else if (property == "backgroundcolor")
        {
            m_backgroundColorCached = getSharedRenderer()->getBackgroundColor();
        }
        else if (property == "titlebarcolor")
        {
            m_titleBarColorCached = getSharedRenderer()->getTitleBarColor();
        }
        else if (property == "bordercolor")
        {
            m_borderColorCached = getSharedRenderer()->getBorderColor();
        }
        else if (property == "bordercolorfocused")
        {
            m_borderColorFocusedCached = getSharedRenderer()->getBorderColorFocused();
        }
        else if ((property == "opacity") || (property == "opacitydisabled"))
        {
            Container::rendererChanged(property);

            for (auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
            {
                if (button->isVisible())
                    button->setInheritedOpacity(m_opacityCached);
            }

            m_titleText.setOpacity(m_opacityCached);
            m_spriteTitleBar.setOpacity(m_opacityCached);
            m_spriteBackground.setOpacity(m_opacityCached);
        }
        else if (property == "font")
        {
            Container::rendererChanged(property);

            for (auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
            {
                if (button->isVisible())
                    button->setInheritedFont(m_fontCached);
            }

            m_titleText.setFont(m_fontCached);
            if (m_titleTextSize == 0)
                m_titleText.setCharacterSize(Text::findBestTextSize(m_fontCached, getSharedRenderer()->getTitleBarHeight() * 0.8f));

            setPosition(m_position);
        }
        else
            Container::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> ChildWindow::save(SavingRenderersMap& renderers) const
    {
        auto node = Container::save(renderers);

        if (m_titleAlignment == TitleAlignment::Left)
            node->propertyValuePairs["TitleAlignment"] = std::make_unique<DataIO::ValueNode>("Left");
        else if (m_titleAlignment == TitleAlignment::Center)
            node->propertyValuePairs["TitleAlignment"] = std::make_unique<DataIO::ValueNode>("Center");
        else if (m_titleAlignment == TitleAlignment::Right)
            node->propertyValuePairs["TitleAlignment"] = std::make_unique<DataIO::ValueNode>("Right");

        if (getTitle().getSize() > 0)
            node->propertyValuePairs["Title"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(getTitle()));

        if (m_keepInParent)
            node->propertyValuePairs["KeepInParent"] = std::make_unique<DataIO::ValueNode>("true");

        if (m_resizable)
            node->propertyValuePairs["Resizable"] = std::make_unique<DataIO::ValueNode>("true");
        if (m_positionLocked)
            node->propertyValuePairs["PositionLocked"] = std::make_unique<DataIO::ValueNode>("true");

        if (m_minimumSize != Vector2f{})
            node->propertyValuePairs["MinimumSize"] = std::make_unique<DataIO::ValueNode>("(" + to_string(m_minimumSize.x) + ", " + to_string(m_minimumSize.y) + ")");
        if (m_maximumSize != Vector2f{std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()})
            node->propertyValuePairs["MaximumSize"] = std::make_unique<DataIO::ValueNode>("(" + to_string(m_maximumSize.x) + ", " + to_string(m_maximumSize.y) + ")");

        std::string serializedTitleButtons;
        if (m_titleButtons & TitleButton::Minimize)
            serializedTitleButtons += " | Minimize";
        if (m_titleButtons & TitleButton::Maximize)
            serializedTitleButtons += " | Maximize";
        if (m_titleButtons & TitleButton::Close)
            serializedTitleButtons += " | Close";

        if (!serializedTitleButtons.empty())
            serializedTitleButtons.erase(0, 3);
        else
            serializedTitleButtons = "None";

        node->propertyValuePairs["TitleButtons"] = std::make_unique<DataIO::ValueNode>(serializedTitleButtons);

        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Container::load(node, renderers);

        if (node->propertyValuePairs["titlealignment"])
        {
            if (toLower(node->propertyValuePairs["titlealignment"]->value) == "left")
                setTitleAlignment(TitleAlignment::Left);
            else if (toLower(node->propertyValuePairs["titlealignment"]->value) == "center")
                setTitleAlignment(TitleAlignment::Center);
            else if (toLower(node->propertyValuePairs["titlealignment"]->value) == "right")
                setTitleAlignment(TitleAlignment::Right);
            else
                throw Exception{"Failed to parse TitleAlignment property. Only the values Left, Center and Right are correct."};
        }

        if (node->propertyValuePairs["titlebuttons"])
        {
            int decodedTitleButtons = TitleButton::None;
            std::vector<std::string> titleButtons = Deserializer::split(node->propertyValuePairs["titlebuttons"]->value, '|');
            for (const auto& elem : titleButtons)
            {
                std::string requestedTitleButton = toLower(trim(elem));
                if (requestedTitleButton == "close")
                    decodedTitleButtons |= TitleButton::Close;
                else if (requestedTitleButton == "maximize")
                    decodedTitleButtons |= TitleButton::Maximize;
                else if (requestedTitleButton == "minimize")
                    decodedTitleButtons |= TitleButton::Minimize;
            }

            setTitleButtons(decodedTitleButtons);
        }

        if (node->propertyValuePairs["title"])
            setTitle(Deserializer::deserialize(ObjectConverter::Type::String, node->propertyValuePairs["title"]->value).getString());

        if (node->propertyValuePairs["keepinparent"])
            setKeepInParent(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["keepinparent"]->value).getBool());

        if (node->propertyValuePairs["resizable"])
            setResizable(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["resizable"]->value).getBool());
        if (node->propertyValuePairs["positionlocked"])
            setPositionLocked(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["positionlocked"]->value).getBool());

        if (node->propertyValuePairs["minimumsize"])
            setMinimumSize(Vector2f{node->propertyValuePairs["minimumsize"]->value});

        if (node->propertyValuePairs["maximumsize"])
            setMaximumSize(Vector2f{node->propertyValuePairs["maximumsize"]->value});
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindow::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());

        // Draw the borders
        if (m_bordersCached != Borders{0})
        {
            if (m_focused && m_borderColorFocusedCached.isSet())
                drawBorders(target, states, m_bordersCached, getFullSize(), m_borderColorFocusedCached);
            else
                drawBorders(target, states, m_bordersCached, getFullSize(), m_borderColorCached);

            states.transform.translate({m_bordersCached.getLeft(), m_bordersCached.getTop()});
        }

        // Draw the title bar
        if (m_spriteTitleBar.isSet())
            m_spriteTitleBar.draw(target, states);
        else
            drawRectangleShape(target, states, {getSize().x, m_titleBarHeightCached}, m_titleBarColorCached);

        // Draw the text in the title bar (after setting the clipping area)
        {
            float buttonOffsetX = 0;
            for (const auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
            {
                if (button->isVisible())
                    buttonOffsetX += (buttonOffsetX > 0 ? m_paddingBetweenButtonsCached : 0) + button->getSize().x;
            }

            if (buttonOffsetX > 0)
                buttonOffsetX += m_distanceToSideCached;

            const float clippingLeft = m_distanceToSideCached;
            const float clippingRight = getSize().x - m_distanceToSideCached - buttonOffsetX;

            const Clipping clipping{target, states, {clippingLeft, 0}, {clippingRight - clippingLeft, m_titleBarHeightCached}};

            m_titleText.draw(target, states);
        }

        // Draw the buttons
        states.transform.translate({-m_bordersCached.getLeft(), -m_bordersCached.getTop()});
        for (auto& button : {m_closeButton.get(), m_maximizeButton.get(), m_minimizeButton.get()})
        {
            if (button->isVisible())
                button->draw(target, states);
        }

        states.transform.translate({m_bordersCached.getLeft(), m_bordersCached.getTop() + m_titleBarHeightCached});

        // Draw the border below the title bar
        if (m_borderBelowTitleBarCached > 0)
        {
            if (m_focused && m_borderColorFocusedCached.isSet())
                drawRectangleShape(target, states, {getSize().x, m_borderBelowTitleBarCached}, m_borderColorFocusedCached);
            else
                drawRectangleShape(target, states, {getSize().x, m_borderBelowTitleBarCached}, m_borderColorCached);

            states.transform.translate({0, m_borderBelowTitleBarCached});
        }

        // Draw the background
        if (m_spriteBackground.isSet())
            m_spriteBackground.draw(target, states);
        else if (m_backgroundColorCached != Color::Transparent)
            drawRectangleShape(target, states, getSize(), m_backgroundColorCached);

        // Draw the widgets in the child window
        const Clipping clipping{target, states, {}, {getSize()}};
        drawWidgetContainer(&target, states);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2020 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/Tabs.hpp>
#include <TGUI/Clipping.hpp>

#if TGUI_COMPILED_WITH_CPP_VER >= 17
    #include <optional>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Tabs::Tabs()
    {
        m_type = "Tabs";
        m_distanceToSideCached = Text::getLineHeight(m_fontCached, getGlobalTextSize()) * 0.4f;

        m_renderer = aurora::makeCopied<TabsRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setTextSize(getGlobalTextSize());
        setTabHeight(Text::getLineHeight(m_fontCached, m_textSize) * 1.25f + m_bordersCached.getTop() + m_bordersCached.getBottom());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Tabs::Ptr Tabs::create()
    {
        return std::make_shared<Tabs>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Tabs::Ptr Tabs::copy(Tabs::ConstPtr tabs)
    {
        if (tabs)
            return std::static_pointer_cast<Tabs>(tabs->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TabsRenderer* Tabs::getSharedRenderer()
    {
        return aurora::downcast<TabsRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const TabsRenderer* Tabs::getSharedRenderer() const
    {
        return aurora::downcast<const TabsRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TabsRenderer* Tabs::getRenderer()
    {
        return aurora::downcast<TabsRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const TabsRenderer* Tabs::getRenderer() const
    {
        return aurora::downcast<const TabsRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        // Tabs is no longer auto-sizing
        m_autoSize = false;
        recalculateTabsWidth();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setEnabled(bool enabled)
    {
        Widget::setEnabled(enabled);
        updateTextColors();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setAutoSize(bool autoSize)
    {
        if (m_autoSize != autoSize)
        {
            m_autoSize = autoSize;
            recalculateTabsWidth();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::getAutoSize() const
    {
        return m_autoSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t Tabs::add(const sf::String& text, bool selectTab)
    {
        // Use the insert function to put the tab in the right place
        insert(m_tabs.size(), text, selectTab);

        // Return the index of the new tab
        return m_tabs.size()-1;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::insert(std::size_t index, const sf::String& text, bool selectTab)
    {
        // If the index is too high then just insert at the end
        if (index > m_tabs.size())
            index = m_tabs.size();

        // Create the new tab
        Tab newTab;
        newTab.visible = true;
        newTab.enabled = true;
        newTab.width = 0;
        newTab.text.setFont(m_fontCached);
        newTab.text.setColor(m_textColorCached);
        newTab.text.setOpacity(m_opacityCached);
        newTab.text.setCharacterSize(getTextSize());
        newTab.text.setString(text);

        m_tabs.insert(m_tabs.begin() + index, std::move(newTab));
        recalculateTabsWidth();

        // New hovered tab depends on several factors, we keep it simple and just remove the hover state
        m_hoveringTab = -1;

        if (m_selectedTab >= static_cast<int>(index))
            m_selectedTab++;

        // If the tab has to be selected then do so
        if (selectTab)
            select(index);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String Tabs::getText(std::size_t index) const
    {
        if (index >= m_tabs.size())
            return "";
        else
            return m_tabs[index].text.getString();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::changeText(std::size_t index, const sf::String& text)
    {
        if (index >= m_tabs.size())
            return false;

        m_tabs[index].text.setString(text);
        recalculateTabsWidth();
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::select(const sf::String& text)
    {
        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (m_tabs[i].text.getString() == text)
                return select(i);
        }

        deselect();
        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::select(std::size_t index)
    {
        // Don't select a tab that is already selected
        if (m_selectedTab == static_cast<int>(index))
            return true;

        // If the index is too high or if the tab is invisible or disabled then we can't select it
        if ((index >= m_tabs.size()) || !m_enabled || !m_tabs[index].visible || !m_tabs[index].enabled)
        {
            deselect();
            return false;
        }

        if (m_selectedTab >= 0)
            m_tabs[m_selectedTab].text.setColor(m_textColorCached);

        // Select the tab
        m_selectedTab = static_cast<int>(index);
        m_tabs[m_selectedTab].text.setColor(m_selectedTextColorCached);

        // Send the callback
        onTabSelect.emit(this, m_tabs[index].text.getString());
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::deselect()
    {
        if (m_selectedTab >= 0)
        {
            m_tabs[m_selectedTab].text.setColor(m_textColorCached);
            m_selectedTab = -1;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::remove(const sf::String& text)
    {
        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (m_tabs[i].text.getString() == text)
                return remove(i);
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::remove(std::size_t index)
    {
        // The index can't be too high
        if (index > m_tabs.size() - 1)
            return false;

        // Remove the tab
        m_tabs.erase(m_tabs.begin() + index);

        // Check if the selected tab should be updated
        if (m_selectedTab == static_cast<int>(index))
            m_selectedTab = -1;
        else if (m_selectedTab > static_cast<int>(index))
            --m_selectedTab;

        // New hovered tab depends on several factors, we keep it simple and just remove the hover state
        m_hoveringTab = -1;
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::removeAll()
    {
        m_tabs.clear();
        m_selectedTab = -1;
        m_hoveringTab = -1;

        recalculateTabsWidth();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String Tabs::getSelected() const
    {
        if (m_selectedTab >= 0)
            return m_tabs[m_selectedTab].text.getString();
        else
            return "";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    int Tabs::getSelectedIndex() const
    {
        return m_selectedTab;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setTabVisible(std::size_t index, bool visible)
    {
        if (index >= m_tabs.size())
            return;

        m_tabs[index].visible = visible;
        recalculateTabsWidth();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::getTabVisible(std::size_t index) const
    {
        if (index >= m_tabs.size())
            return false;

        return m_tabs[index].visible;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setTabEnabled(std::size_t index, bool enabled)
    {
        if (index >= m_tabs.size())
            return;

        m_tabs[index].enabled = enabled;
        updateTextColors();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::getTabEnabled(std::size_t index) const
    {
        if (index >= m_tabs.size())
            return false;

        return m_tabs[index].enabled;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setTextSize(unsigned int size)
    {
        if ((size == 0) || (m_requestedTextSize != size))
        {
            m_requestedTextSize = size;

            if (size == 0)
                m_textSize = Text::findBestTextSize(m_fontCached, (getSize().y - m_bordersCached.getTop() - m_bordersCached.getBottom()) * 0.8f);
            else
                m_textSize = size;

            for (auto& tab : m_tabs)
                tab.text.setCharacterSize(m_textSize);

            recalculateTabsWidth();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setTabHeight(float height)
    {
        Widget::setSize({getSizeLayout().x, height});
        m_bordersCached.updateParentSize(getSize());

        // Recalculate the size when the text is auto sizing
        if (m_requestedTextSize == 0)
            setTextSize(0);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setMaximumTabWidth(float maximumWidth)
    {
        m_maximumTabWidth = maximumWidth;

        recalculateTabsWidth();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Tabs::getMaximumTabWidth() const
    {
        return m_maximumTabWidth;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::setMinimumTabWidth(float minimumWidth)
    {
        m_minimumTabWidth = minimumWidth;

        recalculateTabsWidth();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Tabs::getMinimumTabWidth() const
    {
        return m_minimumTabWidth;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t Tabs::getTabsCount() const
    {
        return m_tabs.size();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Tabs::mouseOnWidget(Vector2f pos) const
    {
        return FloatRect{getPosition().x, getPosition().y, getSize().x, getSize().y}.contains(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::leftMousePressed(Vector2f pos)
    {
        pos -= getPosition();

        float width = m_bordersCached.getLeft() / 2.f;
        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (!m_tabs[i].visible)
                continue;

            // Append the width of the tab
            width += (m_bordersCached.getLeft() / 2.f) + m_tabs[i].width + (m_bordersCached.getRight() / 2.0f);

            // If the mouse went down on this tab then select it
            if (pos.x < width)
            {
                select(i);
                break;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::mouseMoved(Vector2f pos)
    {
        Widget::mouseMoved(pos);

        pos -= getPosition();
        m_hoveringTab = -1;
        float width = m_bordersCached.getLeft() / 2.f;

        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (!m_tabs[i].visible)
                continue;

            // Append the width of the tab
            width += (m_bordersCached.getLeft() / 2.f) + m_tabs[i].width + (m_bordersCached.getRight() / 2.0f);

            // If the mouse is on top of this tab then remember it
            if (pos.x < width)
            {
                if (m_tabs[i].enabled)
                    m_hoveringTab = i;

                break;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::mouseNoLongerOnWidget()
    {
        Widget::mouseNoLongerOnWidget();

        m_hoveringTab = -1;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::recalculateTabsWidth()
    {
        unsigned int visibleTabs = 0;
        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (m_tabs[i].visible)
                visibleTabs++;
        }

        if (m_autoSize)
        {
            if (m_tabs.empty())
                Widget::setSize({m_bordersCached.getLeft() + m_bordersCached.getRight(), getSizeLayout().y});
            else
            {
                // First calculate the width of the tabs as if there aren't any borders
                float totalWidth = 0;
                for (unsigned int i = 0; i < m_tabs.size(); ++i)
                {
                    if (!m_tabs[i].visible)
                        continue;

                    m_tabs[i].width = m_tabs[i].text.getSize().x + std::max(m_minimumTabWidth, 2 * m_distanceToSideCached);
                    if ((m_maximumTabWidth > 0) && (m_maximumTabWidth < m_tabs[i].width))
                        m_tabs[i].width = m_maximumTabWidth;

                    totalWidth += m_tabs[i].width;
                }

                // Now add the borders to the tabs
                totalWidth += (visibleTabs + 1) * ((m_bordersCached.getLeft() + m_bordersCached.getRight()) / 2.f);

                Widget::setSize({totalWidth, getSizeLayout().y});
            }
        }
        else // A size was provided
        {
            const float tabWidth = (getSize().x - ((visibleTabs + 1) * ((m_bordersCached.getLeft() + m_bordersCached.getRight()) / 2.f))) / visibleTabs;
            for (unsigned int i = 0; i < m_tabs.size(); ++i)
                m_tabs[i].width = tabWidth;
        }

        m_bordersCached.updateParentSize(getSize());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Signal& Tabs::getSignal(std::string signalName)
    {
        if (signalName == toLower(onTabSelect.getName()))
            return onTabSelect;
        else
            return Widget::getSignal(std::move(signalName));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::rendererChanged(const std::string& property)
    {
        if (property == "borders")
        {
            m_bordersCached = getSharedRenderer()->getBorders();
            recalculateTabsWidth();
        }
        else if (property == "textcolor")
        {
            m_textColorCached = getSharedRenderer()->getTextColor();
            updateTextColors();
        }
        else if (property == "textcolorhover")
        {
            m_textColorHoverCached = getSharedRenderer()->getTextColorHover();
            updateTextColors();
        }
        else if (property == "textcolordisabled")
        {
            m_textColorDisabledCached = getSharedRenderer()->getTextColorDisabled();
            updateTextColors();
        }
        else if (property == "selectedtextcolor")
        {
            m_selectedTextColorCached = getSharedRenderer()->getSelectedTextColor();
            updateTextColors();
        }
        else if (property == "selectedtextcolorhover")
        {
            m_selectedTextColorHoverCached = getSharedRenderer()->getSelectedTextColorHover();
            updateTextColors();
        }
        else if (property == "texturetab")
        {
            m_spriteTab.setTexture(getSharedRenderer()->getTextureTab());
        }
        else if (property == "texturetabhover")
        {
            m_spriteTabHover.setTexture(getSharedRenderer()->getTextureTabHover());
        }
        else if (property == "textureselectedtab")
        {
            m_spriteSelectedTab.setTexture(getSharedRenderer()->getTextureSelectedTab());
        }
        else if (property == "textureselectedtabhover")
        {
            m_spriteSelectedTabHover.setTexture(getSharedRenderer()->getTextureSelectedTabHover());
        }
        else if (property == "texturedisabledtab")
        {
            m_spriteDisabledTab.setTexture(getSharedRenderer()->getTextureDisabledTab());
        }
        else if (property == "distancetoside")
        {
            m_distanceToSideCached = getSharedRenderer()->getDistanceToSide();
            recalculateTabsWidth();
        }
        else if (property == "backgroundcolor")
        {
            m_backgroundColorCached = getSharedRenderer()->getBackgroundColor();
        }
        else if (property == "backgroundcolorhover")
        {
            m_backgroundColorHoverCached = getSharedRenderer()->getBackgroundColorHover();
        }
        else if (property == "backgroundcolordisabled")
        {
            m_backgroundColorDisabledCached = getSharedRenderer()->getBackgroundColorDisabled();
        }
        else if (property == "selectedbackgroundcolor")
        {
            m_selectedBackgroundColorCached = getSharedRenderer()->getSelectedBackgroundColor();
        }
        else if (property == "selectedbackgroundcolorhover")
        {
            m_selectedBackgroundColorHoverCached = getSharedRenderer()->getSelectedBackgroundColorHover();
        }
        else if (property == "bordercolor")
        {
            m_borderColorCached = getSharedRenderer()->getBorderColor();
        }
        else if ((property == "opacity") || (property == "opacitydisabled"))
        {
            Widget::rendererChanged(property);

            m_spriteTab.setOpacity(m_opacityCached);
            m_spriteTabHover.setOpacity(m_opacityCached);
            m_spriteSelectedTab.setOpacity(m_opacityCached);
            m_spriteSelectedTabHover.setOpacity(m_opacityCached);
            m_spriteDisabledTab.setOpacity(m_opacityCached);

            for (auto& tab : m_tabs)
                tab.text.setOpacity(m_opacityCached);
        }
        else if (property == "font")
        {
            Widget::rendererChanged(property);

            for (auto& tab : m_tabs)
                tab.text.setFont(m_fontCached);

            // Recalculate the size when the text is auto sizing
            if (m_requestedTextSize == 0)
                setTextSize(0);
            else
                recalculateTabsWidth();
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> Tabs::save(SavingRenderersMap& renderers) const
    {
        auto node = Widget::save(renderers);

        if (m_tabs.size() > 0)
        {
            bool allTabsVisible = true;
            bool allTabsEnabled = true;
            std::string tabList = "[" + Serializer::serialize(getText(0));
            std::string tabVisibleList = "[" + Serializer::serialize(getTabVisible(0));
            std::string tabEnabledList = "[" + Serializer::serialize(getTabEnabled(0));
            for (std::size_t i = 1; i < m_tabs.size(); ++i)
            {
                tabList += ", " + Serializer::serialize(getText(i));
                tabVisibleList += ", " + Serializer::serialize(getTabVisible(i));
                tabEnabledList += ", " + Serializer::serialize(getTabEnabled(i));

                if (!getTabVisible(i))
                    allTabsVisible = false;
                if (!getTabEnabled(i))
                    allTabsEnabled = false;
            }

            tabList += "]";
            tabVisibleList += "]";
            tabEnabledList += "]";

            node->propertyValuePairs["Tabs"] = std::make_unique<DataIO::ValueNode>(tabList);
            if (!allTabsVisible)
                node->propertyValuePairs["TabsVisible"] = std::make_unique<DataIO::ValueNode>(tabVisibleList);
            if (!allTabsEnabled)
                node->propertyValuePairs["TabsEnabled"] = std::make_unique<DataIO::ValueNode>(tabEnabledList);
        }

        if (getSelectedIndex() >= 0)
            node->propertyValuePairs["Selected"] = std::make_unique<DataIO::ValueNode>(to_string(getSelectedIndex()));

        if (m_maximumTabWidth > 0)
            node->propertyValuePairs["MaximumTabWidth"] = std::make_unique<DataIO::ValueNode>(to_string(m_maximumTabWidth));

        if (m_autoSize)
        {
            node->propertyValuePairs.erase("Size");
            node->propertyValuePairs["TabHeight"] = std::make_unique<DataIO::ValueNode>(to_string(getSize().y));
        }

        node->propertyValuePairs["TextSize"] = std::make_unique<DataIO::ValueNode>(to_string(m_textSize));
        node->propertyValuePairs["AutoSize"] = std::make_unique<DataIO::ValueNode>(to_string(m_autoSize));

        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Widget::load(node, renderers);

        if (node->propertyValuePairs["tabs"])
        {
            if (!node->propertyValuePairs["tabs"]->listNode)
                throw Exception{"Failed to parse 'Tabs' property, expected a list as value"};

            for (const auto& tabText : node->propertyValuePairs["tabs"]->valueList)
                add(Deserializer::deserialize(ObjectConverter::Type::String, tabText).getString(), false);
        }

        if (node->propertyValuePairs["tabsvisible"])
        {
            if (!node->propertyValuePairs["tabsvisible"]->listNode)
                throw Exception{"Failed to parse 'TabsVisible' property, expected a list as value"};

            const auto& values = node->propertyValuePairs["tabsvisible"]->valueList;
            for (unsigned int i = 0; i < values.size(); ++i)
                setTabVisible(i, Deserializer::deserialize(ObjectConverter::Type::Bool, values[i]).getBool());
        }

        if (node->propertyValuePairs["tabsenabled"])
        {
            if (!node->propertyValuePairs["tabsenabled"]->listNode)
                throw Exception{"Failed to parse 'TabsEnabled' property, expected a list as value"};

            const auto& values = node->propertyValuePairs["tabsenabled"]->valueList;
            for (unsigned int i = 0; i < values.size(); ++i)
                setTabEnabled(i, Deserializer::deserialize(ObjectConverter::Type::Bool, values[i]).getBool());
        }

        if (node->propertyValuePairs["maximumtabwidth"])
            setMaximumTabWidth(strToFloat(node->propertyValuePairs["maximumtabwidth"]->value));
        if (node->propertyValuePairs["textsize"])
            setTextSize(strToInt(node->propertyValuePairs["textsize"]->value));
        if (node->propertyValuePairs["tabheight"])
            setTabHeight(strToFloat(node->propertyValuePairs["tabheight"]->value));
        if (node->propertyValuePairs["selected"])
            select(strToInt(node->propertyValuePairs["selected"]->value));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::updateTextColors()
    {
        for (auto& tab : m_tabs)
        {
            if ((!m_enabled || !tab.enabled) && m_textColorDisabledCached.isSet())
                tab.text.setColor(m_textColorDisabledCached);
            else
                tab.text.setColor(m_textColorCached);
        }

        if (m_enabled)
        {
            if (m_selectedTab >= 0)
            {
                if ((m_selectedTab == m_hoveringTab) && m_selectedTextColorHoverCached.isSet())
                    m_tabs[m_selectedTab].text.setColor(m_selectedTextColorHoverCached);
                else if (m_selectedTextColorCached.isSet())
                    m_tabs[m_selectedTab].text.setColor(m_selectedTextColorCached);
            }

            if ((m_hoveringTab >= 0) && (m_selectedTab != m_hoveringTab))
            {
                if (m_textColorHoverCached.isSet())
                    m_tabs[m_hoveringTab].text.setColor(m_textColorHoverCached);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Tabs::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());
        states.transform.translate({});

        // Draw the borders around the tabs
        if (m_bordersCached != Borders{0})
        {
            drawBorders(target, states, m_bordersCached, getSize(), m_borderColorCached);
            states.transform.translate({m_bordersCached.getLeft(), m_bordersCached.getTop()});
        }

        const float usableHeight = getSize().y - m_bordersCached.getTop() - m_bordersCached.getBottom();
        for (unsigned int i = 0; i < m_tabs.size(); ++i)
        {
            if (!m_tabs[i].visible)
                continue;

            sf::RenderStates textStates = states;

            // Draw the background of the tab
            if (m_spriteTab.isSet() && m_spriteSelectedTab.isSet())
            {
                Sprite spriteTab;
                if ((!m_enabled || !m_tabs[i].enabled) && m_spriteDisabledTab.isSet())
                    spriteTab = m_spriteDisabledTab;
                else if (m_selectedTab == static_cast<int>(i))
                {
                    if ((m_hoveringTab == static_cast<int>(i)) && m_spriteSelectedTabHover.isSet())
                        spriteTab = m_spriteSelectedTabHover;
                    else
                        spriteTab = m_spriteSelectedTab;
                }
                else if ((m_hoveringTab == static_cast<int>(i)) && m_spriteTabHover.isSet())
                    spriteTab = m_spriteTabHover;
                else
                    spriteTab = m_spriteTab;

                spriteTab.setSize({m_tabs[i].width, usableHeight});
                spriteTab.draw(target, states);
            }
            else // No texture was loaded
            {
                if ((!m_enabled || !m_tabs[i].enabled) && m_backgroundColorDisabledCached.isSet())
                    drawRectangleShape(target, states, {m_tabs[i].width, usableHeight}, m_backgroundColorDisabledCached);
                else if (m_selectedTab == static_cast<int>(i))
                {
                    if ((m_hoveringTab == static_cast<int>(i)) && m_selectedBackgroundColorHoverCached.isSet())
                        drawRectangleShape(target, states, {m_tabs[i].width, usableHeight}, m_selectedBackgroundColorHoverCached);
                    else
                        drawRectangleShape(target, states, {m_tabs[i].width, usableHeight}, m_selectedBackgroundColorCached);
                }
                else if ((m_hoveringTab == static_cast<int>(i)) && m_backgroundColorHoverCached.isSet())
                    drawRectangleShape(target, states, {m_tabs[i].width, usableHeight}, m_backgroundColorHoverCached);
                else
                    drawRectangleShape(target, states, {m_tabs[i].width, usableHeight}, m_backgroundColorCached);
            }

            // Draw the borders between the tabs
            states.transform.translate({m_tabs[i].width, 0});
            if ((m_bordersCached != Borders{0}) && (i < m_tabs.size() - 1))
            {
                drawRectangleShape(target, states, {(m_bordersCached.getLeft() + m_bordersCached.getRight()) / 2.f, usableHeight}, m_borderColorCached);
                states.transform.translate({(m_bordersCached.getLeft() + m_bordersCached.getRight()) / 2.f, 0});
            }

            // Apply clipping if required for the text in this tab
            const float usableWidth = m_tabs[i].width - (2 * m_distanceToSideCached);
#if TGUI_COMPILED_WITH_CPP_VER >= 17
            std::optional<Clipping> clipping;
            if (m_tabs[i].text.getSize().x > usableWidth)
                clipping.emplace(target, textStates, Vector2f{m_distanceToSideCached, 0}, Vector2f{usableWidth, usableHeight});
#else
            std::unique_ptr<Clipping> clipping;
            if (m_tabs[i].text.getSize().x > usableWidth)
                clipping = std::make_unique<Clipping>(target, textStates, Vector2f{m_distanceToSideCached, 0}, Vector2f{usableWidth, usableHeight});
#endif

            // Draw the text
            textStates.transform.translate({m_distanceToSideCached + ((usableWidth - m_tabs[i].text.getSize().x) / 2.f), ((usableHeight - m_tabs[i].text.getSize().y) / 2.f)});
            m_tabs[i].text.draw(target, textStates);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
