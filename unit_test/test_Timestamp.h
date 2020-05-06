#pragma once

#include <iostream>

//#include <cstddef>
//#include <stdexcept>
//#include <cstring>
//#include <ostream>

#include <catch.hpp>
#include <chrono>
#include <ratio_io>

namespace _My_Test
{
// const int ci = 0;
// std::cout << type_name<decltype(ci)>() << '\n';
// std::cout << type_name<std::out_of_range>() << '\n';
#ifndef _MSC_VER
#if __cplusplus < 201103
#define CONSTEXPR11_TN
#define CONSTEXPR14_TN
#define NOEXCEPT_TN
#elif __cplusplus < 201402
#define CONSTEXPR11_TN constexpr
#define CONSTEXPR14_TN
#define NOEXCEPT_TN noexcept
#else
#define CONSTEXPR11_TN constexpr
#define CONSTEXPR14_TN constexpr
#define NOEXCEPT_TN noexcept
#endif
#else // _MSC_VER
#if _MSC_VER < 1900
#define CONSTEXPR11_TN
#define CONSTEXPR14_TN
#define NOEXCEPT_TN
#elif _MSC_VER < 2000
#define CONSTEXPR11_TN constexpr
#define CONSTEXPR14_TN constexpr
#define NOEXCEPT_TN noexcept
#else
#define CONSTEXPR11_TN constexpr
#define CONSTEXPR14_TN constexpr
#define NOEXCEPT_TN noexcept
#endif
#endif // _MSC_VER
    class static_string
    {
        const char* const p_;
        const std::size_t sz_;

    public:
        using const_iterator = const char*;

        template<std::size_t N>
        CONSTEXPR11_TN static_string(const char (&a)[N]) NOEXCEPT_TN : p_(a), sz_(N - 1)
        {
        }

        CONSTEXPR11_TN static_string(const char* p, std::size_t N) NOEXCEPT_TN : p_(p), sz_(N) {}

        CONSTEXPR11_TN const char* data() const NOEXCEPT_TN { return p_; }
        CONSTEXPR11_TN std::size_t size() const NOEXCEPT_TN { return sz_; }

        CONSTEXPR11_TN const_iterator begin() const NOEXCEPT_TN { return p_; }
        CONSTEXPR11_TN const_iterator end() const NOEXCEPT_TN { return p_ + sz_; }

        CONSTEXPR11_TN char operator[](std::size_t n) const
        {
            return n < sz_ ? p_[n] : throw std::out_of_range("static_string");
        }
    };

    inline std::ostream& operator<<(std::ostream& os, static_string const& s) { return os.write(s.data(), s.size()); }

    template<class T>
    CONSTEXPR14_TN static_string type_name() // https://stackoverflow.com/a/20170989/3353857
    {
#ifdef __clang__
        static_string p = __PRETTY_FUNCTION__;
        return static_string(p.data() + 31, p.size() - 31 - 1);
#elif defined(__GNUC__)
        static_string p = __PRETTY_FUNCTION__;
#if __cplusplus < 201402
        return static_string(p.data() + 36, p.size() - 36 - 1);
#else
        return static_string(p.data() + 46, p.size() - 46 - 1);
#endif
#elif defined(_MSC_VER)
        static_string p = __FUNCSIG__;
        return static_string(p.data() + 38, p.size() - 38 - 7);
#endif
    }
}

template<typename T = std::exception>
class ExceptionMatcher_StartWith : public Catch::MatcherBase<T>
{
    std::string_view m_expected;

public:
    explicit ExceptionMatcher_StartWith(const std::string& _expeted) : m_expected(_expeted) {}
    bool match(T const& se) const override
    {
        auto se_what = std::string_view{se.what()};
        if (se_what.empty() != m_expected.empty()) // one of them is empty, AND the other is not
        {
            return false;
        }
        else // whethere se.what() started with m_expected?
        {
            return se_what.find(m_expected) == 0;
        }
    }

    std::string describe() const override
    {
        std::ostringstream ss;
        // ss << "(exception) " << _MSC_VER << " ," << ::type_name<T>() << " e.what=[" << m_expected << ']';
        ss << "(exception), e.what=[" << m_expected << ']';
        return ss.str();
    }
};
