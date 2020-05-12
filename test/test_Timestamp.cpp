#include "catch2_single_file.hpp"

//#include "pch.h"

#include "date.h"
#include "Timestamp.h"
#include "test_Timestamp.h"
 

using namespace std;
using namespace chrono;
using namespace date;
using namespace Catch;

using CTimestampTEST = CTimestamp;

constexpr auto my_numeric_limits_max = CTimestampTEST::numeric_limits_max;
constexpr auto my_numeric_limits_min = -CTimestampTEST::numeric_limits_max - 1;
// constexpr auto my_numeric_limits_max = std::numeric_limits<CTimestampTEST::rep>::max();
// constexpr auto my_numeric_limits_min = std::numeric_limits<CTimestampTEST::rep>::min();

constexpr auto my_min = numeric_limits<CTimestampTEST::Rep>::min();
constexpr auto my_max = numeric_limits<CTimestampTEST::Rep>::max();

TEST_CASE("1 Constuctor", "[CTimestampTEST]")
{
    SECTION("1.1 constructing from _Rep") // For each SECTION the TEST_CASE is executed from the start
    {
        REQUIRE(CTimestampTEST{my_numeric_limits_max/1'000'000}.Count() == my_numeric_limits_max);
        REQUIRE(CTimestampTEST{0}.Count() == 0);
        REQUIRE(CTimestampTEST{-my_numeric_limits_max - 1}.Count() == -my_numeric_limits_max - 1);
    }

    SECTION("1.2 copy constructor")
    {
        const auto t1 = CTimestampTEST{4242};
        const auto t2 = t1;
        const auto t3 = CTimestampTEST{t1};
        REQUIRE(t2.Count() == 4242);
        REQUIRE(t3.Count() == 4242);
    }

    SECTION("1.3 compute chrono-max")
    {
        const auto max_duratian_micro_second = microseconds{my_numeric_limits_max};
        const auto max_duration_day = duration<double, days::period>{max_duratian_micro_second};
        const auto max_duration_month = duration<double, months::period>{max_duratian_micro_second};
        const auto max_duration_year = duration<double, years::period>{max_duratian_micro_second};

        std::cout << "max:\n"
                  << fixed << setw(TIMESTAMP_STRING_LEN) << setprecision(TIMESTAMP_STRING_DECIMAL) << setfill('0')
                  << max_duratian_micro_second.count() << " mus\n"
                  << max_duration_day.count() << " day\n"
                  << max_duration_month.count() << " month\n"
                  << max_duration_year.count() << " year\n";

        const auto min_duratian_micro_second = microseconds{my_numeric_limits_min};
        const auto min_duration_day = duration<double, days::period>{min_duratian_micro_second};
        const auto min_duration_month = duration<double, months::period>{min_duratian_micro_second};
        const auto min_duration_year = duration<double, years::period>{min_duratian_micro_second};

        std::cout << "min:\n"
                  << fixed << setw(TIMESTAMP_STRING_LEN) << setprecision(TIMESTAMP_STRING_DECIMAL) << setfill('0')
                  << min_duratian_micro_second.count() << " mus\n"
                  << min_duration_day.count() << " day\n"
                  << min_duration_month.count() << " month\n"
                  << min_duration_year.count() << " year\n";

        // using t = chrono::system_clock::period;
        CHECK(my_min == -my_max - 1);
#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
#else
#pragma warning(disable : 4307) //  warning C4307: '-': integral constant overflow
#endif
        CHECK(my_min - 1 == my_max);
#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif
    }
}

TEST_CASE("2 this.Count()", "[CTimestampTEST]")
{
    auto d = duration<CTimestampTEST::Rep, ratio<1, TIMESTAMP_TO_SECONDS_DIVISOR>>{123'456'123'456LL};
    auto t3 = CTimestampTEST{123'456'123'456};
    REQUIRE(t3.Count() == d.count());
}

TEST_CASE("3 operator+=", "[CTimestampTEST]")
{
    auto near_max = CTimestampTEST{my_numeric_limits_max - 42};
    auto my42ms = CTimestampTEST{42};
    auto my43ms = CTimestampTEST{43};
    auto tmax = CTimestampTEST{my_numeric_limits_max};

    SECTION("3.1 near max, no throw")
    {
        CHECK_NOTHROW(near_max += my42ms);
        REQUIRE(near_max.Count() == CTimestampTEST{my_numeric_limits_max}.Count());
    }

    SECTION("3.2 overflow, throw")
    {
        // CHECK_THROWS(tmax += CTimestampTEST{1});
        // CHECK_THROWS(near_max += my43ms);
        // CHECK_THROWS_WITH(tmax += CTimestampTEST{1}, StartsWith("Timestamp overflow"));
        // CHECK_THROWS_WITH(near_max += my43ms, StartsWith("Timestamp overflow"));

        // StartsWith("Timestamp overflow")
        REQUIRE_THROWS_MATCHES(
            tmax += CTimestampTEST{1}, out_of_range, ExceptionMatcher_StartWith<out_of_range>("Timestamp overflow"));
        REQUIRE_THROWS_MATCHES(
            near_max += my43ms, out_of_range, ExceptionMatcher_StartWith<out_of_range>("Timestamp overflow"));
    }
}

TEST_CASE("4 operator-=", "[CTimestampTEST]")
{
    auto t43 = CTimestampTEST{42 + 1};
    auto t42 = CTimestampTEST{42};

    SECTION("4.1 t42 -= t43")
    {
        CHECK_NOTHROW(t42 -= t43);
        REQUIRE(t42.Count() == 0);
    }

    SECTION("4.2 t43 -= t42")
    {
        CHECK_NOTHROW(t43 -= t42);
        REQUIRE(t43.Count() == 1);
    }
}

TEST_CASE("5 this.ToString()", "[CTimestampTEST]")
{
    REQUIRE(CTimestampTEST{0}.ToString() == "000000.000000");
    REQUIRE(CTimestampTEST{1'000'001}.ToString() == string("000001.000001"));
    REQUIRE(CTimestampTEST{900'000'900'000}.ToString() == string("900000.900000"));
    REQUIRE(CTimestampTEST{123'456'123'456}.ToString() == string("123456.123456"));
    REQUIRE(CTimestampTEST{999'999'999'999}.ToString() == string("999999.999999"));
    // when exceed 6+6 characters, keep as it is.
    REQUIRE(CTimestampTEST{999'999'999'999 + 1}.ToString() == string("1000000.000000"));
    REQUIRE(CTimestampTEST{1'234'567'123'456}.ToString() == string("1234567.123456"));
    // positive max
    CHECK(my_numeric_limits_max == my_max);
    REQUIRE(CTimestampTEST{my_numeric_limits_max}.ToString() ==
            to_string(my_max / TIMESTAMP_TO_SECONDS_DIVISOR) + "." + to_string(my_max % TIMESTAMP_TO_SECONDS_DIVISOR));
    REQUIRE(CTimestampTEST::ToString(my_max) == CTimestampTEST{my_numeric_limits_max}.ToString());
    // negtive value
    REQUIRE(CTimestampTEST{-1}.ToString() == string("-000000.000001"));
    REQUIRE(CTimestampTEST{-1'000'001}.ToString() == string("-000001.000001"));
    REQUIRE(CTimestampTEST{-900'000'900'000}.ToString() == string("-900000.900000"));
    REQUIRE(CTimestampTEST{-123'456'123'456}.ToString() == string("-123456.123456"));
    REQUIRE(CTimestampTEST{-999'999'999'999}.ToString() == string("-999999.999999"));
    REQUIRE(CTimestampTEST{-999'999'999'999 - 1}.ToString() == string("-1000000.000000"));
    // negitive min
    CHECK(my_numeric_limits_min == -my_max - 1);
    REQUIRE(CTimestampTEST{my_min}.ToString() == to_string(my_min / TIMESTAMP_TO_SECONDS_DIVISOR) + "." +
                                                     to_string(-(my_min % TIMESTAMP_TO_SECONDS_DIVISOR)));
    REQUIRE(CTimestampTEST::ToString(my_min) == CTimestampTEST{my_numeric_limits_min}.ToString());
}

TEST_CASE("6 CTimestampBase::ToString()", "[CTimestampTEST]") // constexpr static std::string ToString(_Rep ts)
{
    REQUIRE(CTimestampTEST::ToString(0) == "000000.000000");
    REQUIRE(CTimestampTEST::ToString(1'000'001) == string("000001.000001"));
    REQUIRE(CTimestampTEST::ToString(900'000'900'000) == string("900000.900000"));
    REQUIRE(CTimestampTEST::ToString(123'456'123'456) == string("123456.123456"));
    REQUIRE(CTimestampTEST::ToString(999'999'999'999) == string("999999.999999"));
    // when exceed 6+6 characters, keep as it is.
    REQUIRE(CTimestampTEST::ToString(999'999'999'999 + 1) == string("1000000.000000"));
    REQUIRE(CTimestampTEST::ToString(1'234'567'123'456) == string("1234567.123456"));
    // positive max
    CHECK(my_numeric_limits_max == my_max);
    REQUIRE(CTimestampTEST::ToString(my_numeric_limits_max) ==
            to_string(my_max / TIMESTAMP_TO_SECONDS_DIVISOR) + "." + to_string(my_max % TIMESTAMP_TO_SECONDS_DIVISOR));
    REQUIRE(CTimestampTEST::ToString(my_max) == CTimestampTEST{my_numeric_limits_max}.ToString());
    // negtive value
    REQUIRE(CTimestampTEST::ToString(-1) == string("-000000.000001"));
    REQUIRE(CTimestampTEST::ToString(-1'000'001) == string("-000001.000001"));
    REQUIRE(CTimestampTEST::ToString(-900'000'900'000) == string("-900000.900000"));
    REQUIRE(CTimestampTEST::ToString(-123'456'123'456) == string("-123456.123456"));
    REQUIRE(CTimestampTEST::ToString(-999'999'999'999) == string("-999999.999999"));
    REQUIRE(CTimestampTEST::ToString(-999'999'999'999 - 1) == string("-1000000.000000"));
    // negitive min
    CHECK(my_numeric_limits_min == -my_max - 1);
    REQUIRE(CTimestampTEST::ToString(my_min) == to_string(my_min / TIMESTAMP_TO_SECONDS_DIVISOR) + "." +
                                                    to_string(-(my_min % TIMESTAMP_TO_SECONDS_DIVISOR)));
    REQUIRE(CTimestampTEST::ToString(my_min) == CTimestampTEST{my_numeric_limits_min}.ToString());
}

TEST_CASE("7 this.FromString()", "[CTimestampTEST]")
{
    auto t1 = CTimestampTEST{};

    t1.FromString("000000.000000", false);
    REQUIRE(t1.Count() == 0);

    t1.FromString("123456.123456", false);
    REQUIRE(t1.Count() == 123456123456);

    t1.FromString("123456.12345", false);
    REQUIRE(t1.Count() == 123456123450);

    t1.FromString("123456.1234567890", false);
    REQUIRE(t1.Count() == 123456123456);

    REQUIRE_THROWS_MATCHES(t1.FromString("-123456.123456", false),
                           out_of_range,
                           ExceptionMatcher_StartWith<out_of_range>("Negative time not supported"));

    t1.FromString("-000000.000000", false);
    REQUIRE(t1.Count() == 0);

    // valid-format"123456.654321" microseconds, "######.######",  i.e. regex: \d{6}\.\d{6}
    t1.FromString("123456.123456", true);
    REQUIRE(t1.Count() == 123456123456);

    REQUIRE_THROWS_MATCHES(t1.FromString("123456.12345", true),
                           invalid_argument,
                           ExceptionMatcher_StartWith<invalid_argument>("Badly formatted timestamp"));

    REQUIRE_THROWS_MATCHES(t1.FromString("54321.123456", true),
                           invalid_argument,
                           ExceptionMatcher_StartWith<invalid_argument>("Badly formatted timestamp"));

    REQUIRE_THROWS_MATCHES(t1.FromString("123456.123456789", true),
                           invalid_argument,
                           ExceptionMatcher_StartWith<invalid_argument>("Badly formatted timestamp"));

    REQUIRE_THROWS_MATCHES(t1.FromString("123456123456", true),
                           invalid_argument,
                           ExceptionMatcher_StartWith<invalid_argument>("Badly formatted timestamp"));

    REQUIRE_THROWS_MATCHES(t1.FromString("-12345.123456", true),
                           invalid_argument,
                           ExceptionMatcher_StartWith<invalid_argument>("Badly formatted timestamp"));

    REQUIRE_THROWS_MATCHES(t1.FromString("123456.12345a", true),
                           invalid_argument,
                           ExceptionMatcher_StartWith<invalid_argument>("Badly formatted timestamp"));
}

TEST_CASE("8 this.ToSeconds()", "[CTimestampTEST]")
{
    REQUIRE(CTimestampTEST{123456123456}.ToSeconds() == 123456.123456);
    REQUIRE(CTimestampTEST{0}.ToSeconds() == 0.0);
    REQUIRE(CTimestampTEST{1}.ToSeconds() == 0.000001);
    REQUIRE(CTimestampTEST{1000000}.ToSeconds() == 1.0);
    REQUIRE(CTimestampTEST{1000001}.ToSeconds() == 1.000001);
    REQUIRE(CTimestampTEST{999999999999}.ToSeconds() == 999999.999999);
    REQUIRE(CTimestampTEST{999999999999 + 1}.ToSeconds() == 1000000.0);

    REQUIRE(CTimestampTEST{my_max}.ToSeconds() == my_max / 1000000 + 0.775807); //   9223372036854775807
    REQUIRE(CTimestampTEST{my_min}.ToSeconds() == my_min / 1000000 - 0.775808); // - 9223372036854775808
}

// todo: test FromSeconds()
TEST_CASE("9 this.FromSeconds()", "[CTimestampTEST]")
{
    auto t1 = CTimestampTEST{};

    t1.FromSeconds(000000.000000);
    REQUIRE(t1.Count() == 0);

    t1.FromSeconds(000000.000001);
    REQUIRE(t1.Count() == 1);

    t1.FromSeconds(1);
    REQUIRE(t1.Count() == 1'000'000);

    t1.FromSeconds(123456.123456);
    REQUIRE(t1.Count() == 123'456'123'456);

    t1.FromSeconds(123456.12345);
    REQUIRE(t1.Count() == 123'456'123'450);

    t1.FromSeconds(123456.1234567890);
    REQUIRE(t1.Count() == 123'456'123'456);

    t1.FromSeconds(-000000.000000);
    REQUIRE(t1.Count() == 0);

    REQUIRE_THROWS_MATCHES(t1.FromSeconds(-123456.123456),
                           out_of_range,
                           ExceptionMatcher_StartWith<out_of_range>("Negative time not supported"));
}

// constexpr std::ostream& operator<<(std::ostream& os)
TEST_CASE("10 ostream", "[CTimestampTEST]")
{
    ostringstream ostr{};
    ostr << CTimestampTEST{123456123456};
    REQUIRE(ostr.str() == "123456.123456");

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{0};
    REQUIRE(ostr.str() == "000000.000000");

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{1'000'001};
    REQUIRE(ostr.str() == string("000001.000001"));

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{900'000'900'000};
    REQUIRE(ostr.str() == string("900000.900000"));

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{999'999'999'999};
    REQUIRE(ostr.str() == string("999999.999999"));

    // when exceed 6+6 characters, keep as it is.
    ostringstream().swap(ostr);
    ostr << CTimestampTEST{999'999'999'999 + 1};
    REQUIRE(ostr.str() == string("1000000.000000"));

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{1'234'567'123'456};
    REQUIRE(ostr.str() == string("1234567.123456"));

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{12'345'671'234'567};
    REQUIRE(ostr.str() == string("12345671.234567"));

    // positive max
    ostringstream().swap(ostr);
    ostr << CTimestampTEST{my_numeric_limits_max};
    REQUIRE(ostr.str() ==
            to_string(my_max / TIMESTAMP_TO_SECONDS_DIVISOR) + "." + to_string(my_max % TIMESTAMP_TO_SECONDS_DIVISOR));

    REQUIRE(CTimestampTEST::ToString(my_max) == ostr.str());

    // negtive value
    ostringstream().swap(ostr);
    ostr << CTimestampTEST{-1};
    REQUIRE(ostr.str() == string("-000000.000001"));

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{-1'000'001};
    REQUIRE(ostr.str() == string("-000001.000001"));

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{-900'000'900'001};
    REQUIRE(ostr.str() == string("-900000.900001"));

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{-123'456'123'456};
    REQUIRE(ostr.str() == string("-123456.123456"));

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{-999'999'999'999};
    REQUIRE(ostr.str() == string("-999999.999999"));

    ostringstream().swap(ostr);
    ostr << CTimestampTEST{-999'999'999'999 - 1};
    REQUIRE(ostr.str() == string("-1000000.000000"));

    // negitive min
    ostringstream().swap(ostr);
    ostr << CTimestampTEST{my_min};
    REQUIRE(ostr.str() == to_string(my_min / TIMESTAMP_TO_SECONDS_DIVISOR) + "." +
                              to_string(-(my_min % TIMESTAMP_TO_SECONDS_DIVISOR)));
    REQUIRE(ostr.str() == CTimestampTEST{my_numeric_limits_min}.ToString());
}