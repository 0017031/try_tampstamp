// Event timestamp in microseconds

#pragma once

#include <assert.h>
#include <chrono>
#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>

constexpr unsigned long TIMESTAMP_TO_SECONDS_DIVISOR = 1'000'000; // micro-second based
constexpr int TIMESTAMP_STRING_LEN = 13;                          //"######.######", i.e. regex: \d{6}\.\d{6}
constexpr int TIMESTAMP_STRING_DECIMAL = 6;

template<typename T = std::int64_t, typename P = std::ratio<1, TIMESTAMP_TO_SECONDS_DIVISOR>>
class CTimestampBase
{
    using _Duration = std::chrono::duration<T, P>;

private:
    _Duration _MyTimestamp; // Event timestamp in P (period), default to micro-seconds, represented in int64

public:
    // CTimestampBase() : _MyTimestamp(0){}; //???
    constexpr CTimestampBase() = default;
    constexpr CTimestampBase(const CTimestampBase& src) = default;
    constexpr CTimestampBase(T ts_integer) : _MyTimestamp(ts_integer) {}
    ~CTimestampBase() = default;

    constexpr T count() const { return _MyTimestamp.count(); }

    /** Cast operator.
     */
    // operator T() const { return _MyTimestamp.count(); } //??? dangrous!

	constexpr CTimestampBase& operator+=(const CTimestampBase& ts)
    {
        auto max_duration = _Duration{std::numeric_limits<T>::max()};
        bool bOverflow = (max_duration - _MyTimestamp) <= ts._MyTimestamp ? true : false;
        /*if (sizeof(T) == 4)
        {
            if (_Duration{ULONG_MAX} - _MyTimestamp <= ts._MyTimestamp) bOverflow = true;
        }
        else if (sizeof(T) == 8)
        {
            if (_Duration{_I64_MAX} - _MyTimestamp <= ts._MyTimestamp) bOverflow = true;
        }
        else
        {
            assert(0);
        }*/
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
	constexpr  CTimestampBase& operator-=(const CTimestampBase ts)
    {
        if (ts._MyTimestamp > _MyTimestamp)
            _MyTimestamp = 0;
        else
            _MyTimestamp -= ts._MyTimestamp;

        return *this;
    }

    /** Convert the timestamp to a string.
     * @param ts_micro The timestamp in microseconds.
     * @return The timestamp formatted as a string.
     */
	constexpr  static std::string ToString(T ts_integer) { return CTimestampBase{ts_integer}.ToString(); }

    /** Convert the timestamp to a string.
     * @return The timestamp formatted as a string.
     */
	constexpr  std::string ToString() const
    {
        double converted_to_seconds = this->ToSeconds();
        std::stringstream retval;
        retval << std::fixed << std::setw(TIMESTAMP_STRING_LEN) << std::setprecision(TIMESTAMP_STRING_DECIMAL)
               << std::setfill('0') << converted_to_seconds;
        return retval.str();
    }

    /** Set the timestamp from a string.
     * @param sTimeString The timestamp formatted as a string in seconds.
     * @param bValidateFormat Validate that the format of the timestamp string is ######.######.
     * @return A reference to this timestamp object.
     */
	constexpr CTimestampBase& FromString(const std::string& sTimeString, bool bValidateFormat = false)
    {
        if (bValidateFormat)
        { // Assumed format: "######.######"
            const std::regex myRegexObj{R"(\d{6}\.\d{6})"};
            const bool bValid = std::regex_match(sTimeString, myRegexObj);
            if (!bValid)
            {
                std::stringstream strm;
                strm << "Badly formatted timestamp: " << sTimeString << std::endl;
                strm << __FILE__ << " (Line " << __LINE__ << ")";

                throw std::invalid_argument(strm.str());
            }
        }

        FromSeconds(std::stod(sTimeString));
        return *this;
    }

    /** Convert the timestamp to a floating point value.
     * @return The timestamp as a floating point numeric.
     */
	constexpr  double ToSeconds() const { return std::chrono::duration<double, 1>{_MyTimestamp}.count(); }

    /** Set the timestamp from a floating point numeric.
     * @param dSeconds The timestamp in seconds.
     * @return A reference to this timestamp object.
     */
	constexpr  CTimestampBase& FromSeconds(double dSeconds)
    {
        if (dSeconds < 0.0)
        {
            std::stringstream strm;
            strm << "Negative time not supported: " << __FILE__ << " (Line " << __LINE__ << ")";
            throw std::out_of_range(strm.str());
        }

        double dMaxSeconds = std::chrono::duration<double, 1>{std::numeric_limits<T>::max()}.count();
        if (dSeconds >= dMaxSeconds)
        {
            std::stringstream strm;
            strm << "Timestamp overflow: " << __FILE__ << " (Line " << __LINE__ << ")";
            throw std::out_of_range(strm.str());
        }

        auto duration_s_double = std::chrono::duration<double, 1>{dSeconds};
        _MyTimestamp = std::chrono::duration_cast<_Duration>(
            duration_s_double); // std::chrono::duration_cast(), convert with trunctaion toward zero
        return *this;
    }

    constexpr std::ostream& operator<<(std::ostream& os)
    {
        os << _MyTimestamp.count();
        return os;
    };
};

/** The timestamp typedef used in the current version of the CEL.
 *
 * Note:
 * By defining CTimestamp as CTimestampBase<__int64>, the maximum
 * timestamp is greater than 1150 days.
 */

using CTimestamp = CTimestampBase<int64_t, std::micro>;

/* "<<" stream redirection for CTimestamp */
