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
    using MyDuration = std::chrono::duration<T, P>;

private:
    MyDuration m_nTimestamp; // Event timestamp in P (period), default to micro-seconds, represented in int64

public:
    // CTimestampBase() : m_nTimestamp(0){}; //???
    CTimestampBase() = default;
    CTimestampBase(T ts_integer) : m_nTimestamp(ts_integer) {}
    CTimestampBase(const CTimestampBase& src) { m_nTimestamp = src.m_nTimestamp; }
    ~CTimestampBase() {}

    constexpr T count() const { return m_nTimestamp.count(); }

    /** Cast operator.
     */
    //operator T() const { return m_nTimestamp.count(); } //??? dangrous!

    CTimestampBase& operator+=(const CTimestampBase& ts)
    {
        bool bOverflow = false;

        if (sizeof(T) == 4)
        {
            if (MyDuration{ULONG_MAX} - m_nTimestamp <= ts.m_nTimestamp) bOverflow = true;
        }
        else if (sizeof(T) == 8)
        {
            if (MyDuration{_I64_MAX} - m_nTimestamp <= ts.m_nTimestamp) bOverflow = true;
        }
        else
        {
            assert(0);
        }

        if (bOverflow)
        {
            std::stringstream strm;
            strm << "Timestamp overflow: " << __FILE__ << " (Line " << __LINE__ << ")";

            throw std::out_of_range(strm.str());
        }

        m_nTimestamp += ts.m_nTimestamp;
        return *this;
    }

    /** -= operator.
     * @return A reference to this timestamp object.
     */
    CTimestampBase& operator-=(const CTimestampBase ts)
    {
        if (ts.m_nTimestamp > m_nTimestamp)
            m_nTimestamp = 0;
        else
            m_nTimestamp -= ts.m_nTimestamp;

        return *this;
    }

    /** Convert the timestamp to a string.
     * @param ts_micro The timestamp in microseconds.
     * @return The timestamp formatted as a string.
     */
    static std::string ToString(T ts_integer) { return CTimestampBase{ts_integer}.ToString(); }

    /** Convert the timestamp to a string.
     * @return The timestamp formatted as a string.
     */
    std::string ToString() const
    {
        auto converted_to_seconds = std::chrono::duration<double, 1>{m_nTimestamp};

        std::stringstream retval;
        retval << std::fixed << std::setw(TIMESTAMP_STRING_LEN) << std::setprecision(TIMESTAMP_STRING_DECIMAL)
               << std::setfill('0') << converted_to_seconds.count();

        return retval.str();
    }

    /** Set the timestamp from a string.
     * @param sTimeString The timestamp formatted as a string in seconds.
     * @param bValidateFormat Validate that the format of the timestamp string is ######.######.
     * @return A reference to this timestamp object.
     */
    CTimestampBase& FromString(const std::string& sTimeString, bool bValidateFormat = false)
    {
        if (bValidateFormat)
        {
            // Assumed format: "######.######"
            const std::regex myRegexObj{R"(\d{6}\.\d{6})"};
            const bool bValid = std::regex_match(sTimeString, myRegexObj);
            ;

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
    double ToSeconds() const { return std::chrono::duration<double, 1>{m_nTimestamp}.count(); }

    /** Set the timestamp from a floating point numeric.
     * @param dSeconds The timestamp in seconds.
     * @return A reference to this timestamp object.
     */
    const CTimestampBase& FromSeconds(double dSeconds)
    {
        if (dSeconds < 0.0)
        {
            std::stringstream strm;
            strm << "Negative time not supported: " << __FILE__ << " (Line " << __LINE__ << ")";
            throw std::out_of_range(strm.str());
        }

        if (sizeof(T) == 4)
            constexpr auto dMaxSeconds = double(ULONG_MAX) / TIMESTAMP_TO_SECONDS_DIVISOR;
        else if (sizeof(T) == 8)
            constexpr auto dMaxSeconds = double(_I64_MAX) / TIMESTAMP_TO_SECONDS_DIVISOR;
        else
            assert(0);

        if (dSeconds >= dMaxSeconds)
        {
            std::stringstream strm;
            strm << "Timestamp overflow: " << __FILE__ << " (Line " << __LINE__ << ")";
            throw std::out_of_range(strm.str());
        }

        auto converted_to_seconds = std::chrono::duration<double, 1>{dSeconds};
        auto rounded_value = std::chrono::duration_cast<MyDuration>(converted_to_seconds);
        return CTimestampBase{rounded_value};

        return *this;
    }

    std::ostream& operator<<(std::ostream& os)
    {
        os << m_nTimestamp.count();
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
