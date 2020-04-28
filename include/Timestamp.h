// Event timestamp in microseconds

#pragma once

//#include <assert.h>
#include <chrono>
#include <cstdint>
//#include <exception>
#include <iomanip>
//#include <iostream>
//#include <map>
#include <regex>
#include <sstream>
#include <string>

constexpr unsigned long TIMESTAMP_TO_SECONDS_DIVISOR = 1'000'000; // micro-second based

// string is always 6.6, no matter what the above divisor is.
constexpr int TIMESTAMP_STRING_DECIMAL = 6; // "123456.654321" microseconds, "######.######",  i.e. regex: \d{6}\.\d{6}
constexpr int TIMESTAMP_STRING_LEN = TIMESTAMP_STRING_DECIMAL + 1 + TIMESTAMP_STRING_DECIMAL; // 13 characters

template<typename _Rep = int64_t, typename _Period = std::ratio<1, TIMESTAMP_TO_SECONDS_DIVISOR>>
class CTimestampBase
{
public:
    using _Duration = std::chrono::duration<_Rep, _Period>;
    using rep = _Rep;
    using period = _Period;
    constexpr static auto _numeric_limits_max = std::numeric_limits<_Rep>::max();

private:
    _Duration _MyTimestamp; // Event timestamp in _Period, default to micro-seconds, represented in int64_t

public: // todo: dicision on negtive value?
    // CTimestampBase() : _MyTimestamp(0){}; //??? todo: default construct as un-initialized or 0?
    constexpr CTimestampBase() = default;
    constexpr CTimestampBase(const CTimestampBase& src) = default;
    explicit constexpr CTimestampBase(_Rep ts_integer) : _MyTimestamp(ts_integer) {}
    ~CTimestampBase() = default;

    constexpr _Rep Count() const { return _MyTimestamp.count(); }

    /** Cast operator.  */
    // operator _Rep() const { return _MyTimestamp.count(); } //??? dangrous!
    // todo: decision on casting to _Rep?

    constexpr CTimestampBase& operator+=(const CTimestampBase& ts)
    {
        bool bOverflow = (_Duration{_numeric_limits_max} - _MyTimestamp) < ts._MyTimestamp ? true : false;
        if (bOverflow)
        {
            std::stringstream strm;
            strm << "Timestamp overflow: " << __FILE__ << " (Line " << __LINE__ << ")";
            throw std::out_of_range(strm.str());
        }

        _MyTimestamp += ts._MyTimestamp;
        return *this;
    }

    /** -= operator.
     * @return A reference to this timestamp object.
     */
    constexpr CTimestampBase& operator-=(const CTimestampBase ts)
    {
        if (ts._MyTimestamp > _MyTimestamp)
            _MyTimestamp = _Duration{0}; // todo: who needs this? when small minus big?
        else
            _MyTimestamp -= ts._MyTimestamp;
        return *this;
    }

    /** Convert the timestamp to a string.
     * @param ts The timestamp in ratio<1, TIMESTAMP_TO_SECONDS_DIVISOR>.
     * @return The timestamp formatted as a string.
     * @note //when exceed 6.6 bits, keep as it is.*/
    constexpr static std::string ToString(_Rep const ts)
    { // "123456.654321" microseconds, "######.######",  i.e. regex: \d{6}\.\d{6}
        if (ts > 0)
        {
            auto _seconds = std::chrono::seconds{ts / TIMESTAMP_TO_SECONDS_DIVISOR};
            // std::chrono::microseconds _fraction = _Duration{ts % TIMESTAMP_TO_SECONDS_DIVISOR};
            std::chrono::microseconds _fraction = _Duration{ts} - _seconds;

            std::stringstream retval;
            retval << std::dec << std::setw(TIMESTAMP_STRING_DECIMAL) << std::setfill('0') << _seconds.count() << "."
                   << std::setw(TIMESTAMP_STRING_DECIMAL) << std::setfill('0') << _fraction.count();
            return retval.str();
        }
        else if (ts == 0)
        {
            return std::string("000000.000000");
        }
        else // ts <= 0
        {
            /* double lose pricision near min() double seconds = CTimestampBase{ts}.ToSeconds();*/
            // std::stringstream retval;
            // retval << std::fixed << std::internal << std::setw(TIMESTAMP_STRING_LEN + 1)
            //       << std::setprecision(TIMESTAMP_STRING_DECIMAL) << std::setfill('0') << seconds;
            // return retval.str();

            auto _seconds = std::chrono::seconds{-(ts / TIMESTAMP_TO_SECONDS_DIVISOR)};
            // std::chrono::microseconds _fraction = _Duration{-1 * (ts % TIMESTAMP_TO_SECONDS_DIVISOR)};
            std::chrono::microseconds _fraction = -(_Duration{ts} + _seconds);

            std::stringstream retval;
            retval << "-" << std::dec << std::setw(TIMESTAMP_STRING_DECIMAL) << std::setfill('0') << _seconds.count()
                   << "." << std::setw(TIMESTAMP_STRING_DECIMAL) << std::setfill('0') << _fraction.count();
            return retval.str();
        }
    }

    /** Convert the timestamp to a string.
     * @return The timestamp formatted as a string.
     * @note //when exceed 6.6 bits, keep as it is.     */
    constexpr std::string ToString() const
    { // "123456.654321" microseconds, "######.######",  i.e. regex: \d{6}\.\d{6}
        return ToString(_MyTimestamp.count());
    }

    /** Set the timestamp from a string.
     * @param sTimeString The timestamp formatted as a string in seconds.
     * @param bValidateFormat Validate that the format of the timestamp string is ######.######.
     * @return A reference to this timestamp object.
     */
    constexpr CTimestampBase& FromString(const std::string& sTimeString, bool const bValidateFormat = false)
    { // "123456.654321" microseconds, "######.######",  i.e. regex: \d{6}\.\d{6}
        if (bValidateFormat)
        { // Assumed format: "######.######"
            const std::regex myRegexObj{R"(\d{6}\.\d{6})"};
            const auto bValid = regex_match(sTimeString, myRegexObj);
            if (!bValid)
            {
                std::stringstream strm;
                strm << "Badly formatted timestamp: " << sTimeString << std::endl;
                strm << __FILE__ << " (Line " << __LINE__ << ")";

                throw std::invalid_argument(strm.str());
            }
        }

        FromSeconds(stod(sTimeString));
        return *this;
    }

    /** Convert the timestamp to a floating point value.
     * @return The timestamp as a floating point numeric.
     */
    constexpr double ToSeconds() const { return std::chrono::duration<double>{_MyTimestamp}.count(); }

    /** Set the timestamp from a floating point numeric.
     * @param dSeconds The timestamp in seconds.
     * @return A reference to this timestamp object.
     */
    constexpr CTimestampBase& FromSeconds(double const dSeconds)
    {
        if (dSeconds < 0.0)
        {
            std::stringstream strm;
            strm << "Negative time not supported: " << __FILE__ << " (Line " << __LINE__ << ")";
            throw std::out_of_range(strm.str());
        }

        auto dMaxSeconds = std::chrono::duration<double>{_numeric_limits_max}.count();
        if (dSeconds > dMaxSeconds)
        {
            std::stringstream strm;
            strm << "Timestamp overflow: " << __FILE__ << " (Line " << __LINE__ << ")";
            throw std::out_of_range(strm.str());
        }

        auto duration_s_double = std::chrono::duration<double>{dSeconds};
        _MyTimestamp = std::chrono::duration_cast<_Duration>(
            duration_s_double); // std::chrono::duration_cast(), convert with trunctaion toward zero
        return *this;
    }
};

template<typename _Rep = int64_t, typename _Period = std::ratio<1, TIMESTAMP_TO_SECONDS_DIVISOR>>
constexpr std::ostream& operator<<(std::ostream& os, const CTimestampBase<_Rep, _Period> &t)
{
    return os << t.ToString();
}

using CTimestamp = CTimestampBase<int64_t, std::micro>;
