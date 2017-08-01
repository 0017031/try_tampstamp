#pragma once

#include <iostream>

#include <string_view>
#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <cstdlib>
#include <memory>

#include <catch.hpp>

template<class T>
std::string type_name()
{
    using TR = typename std::remove_reference_t<T>;

#ifndef _MSC_VER
    std::unique_ptr<char, void (*)(void*)> own(abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
                                               std::free);
#else
    std::unique_ptr<char, void (*)(void*)> own(nullptr, std::free);

#endif

    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value) r += " const";
    if (std::is_volatile<TR>::value) r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}
//https://stackoverflow.com/a/20170989/3353857
// const int ci = 0;
// std::cout << type_name<decltype(ci)>() << '\n';
// std::cout << type_name<std::out_of_range>() << '\n';

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
        ss << "(exception), e.what=[" << m_expected << ']';
        return ss.str();
    }
};
