#include "Timestamp.h"
#include "stdafx.h"
using namespace std;
using namespace chrono;
using namespace date;

using CTimestampTEST = CTimestamp;

constexpr auto my_numeric_limits_max = CTimestampTEST::_numeric_limits_max;
constexpr auto my_numeric_limits_min = -CTimestampTEST::_numeric_limits_max - 1;
// constexpr auto my_numeric_limits_max = std::numeric_limits<CTimestampTEST::rep>::max();
// constexpr auto my_numeric_limits_min = std::numeric_limits<CTimestampTEST::rep>::min();

constexpr auto my_min = std::numeric_limits<CTimestampTEST::rep>::min();
constexpr auto my_max = std::numeric_limits<CTimestampTEST::rep>::max();

TEST_CASE("1: Constuctor", "[CTimestampTEST]")
{
    SECTION("1.1 constructing from _Rep") // For each SECTION the TEST_CASE is executed from the start
    {
        REQUIRE(CTimestampTEST{my_numeric_limits_max}.Count() == my_numeric_limits_max);
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

        cout << "max:\n"
             << fixed << setw(TIMESTAMP_STRING_LEN) << setprecision(TIMESTAMP_STRING_DECIMAL) << setfill('0')
             << max_duratian_micro_second.count() << " mus\n"
             << max_duration_day.count() << " day\n"
             << max_duration_month.count() << " month\n"
             << max_duration_year.count() << " year\n";

        const auto min_duratian_micro_second = microseconds{my_numeric_limits_min};
        const auto min_duration_day = duration<double, days::period>{min_duratian_micro_second};
        const auto min_duration_month = duration<double, months::period>{min_duratian_micro_second};
        const auto min_duration_year = duration<double, years::period>{min_duratian_micro_second};

        cout << "min:\n"
             << fixed << setw(TIMESTAMP_STRING_LEN) << setprecision(TIMESTAMP_STRING_DECIMAL) << setfill('0')
             << min_duratian_micro_second.count() << " mus\n"
             << min_duration_day.count() << " day\n"
             << min_duration_month.count() << " month\n"
             << min_duration_year.count() << " year\n";

        // using t = chrono::system_clock::period;
        CHECK(my_min == -my_max - 1);
#ifndef _MSC_FULL_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverflow"
#else
#pragma warning(disable : 4307) //  warning C4307: '-': integral constant overflow
#endif
        CHECK(my_min - 1 == my_max);
#ifndef _MSC_FULL_VER
#pragma GCC diagnostic pop
#endif
    }
}

TEST_CASE("2: this->Count", "[CTimestampTEST]")
{
    auto d = duration<CTimestampTEST::rep, ratio<1, TIMESTAMP_TO_SECONDS_DIVISOR>>{123'456'123'456LL};
    auto t3 = CTimestampTEST{123'456'123'456};
    REQUIRE(t3.Count() == d.count());
}

TEST_CASE("3: operator+=", "[CTimestampTEST]")
{
    auto near_max = CTimestampTEST{my_numeric_limits_max - 42};
    auto my42ms = CTimestampTEST{42};
    auto my43ms = CTimestampTEST{43};

    SECTION("3.1 near max, no throw")
    {
        CHECK_NOTHROW(near_max += my42ms);
        REQUIRE(near_max.Count() == CTimestampTEST{my_numeric_limits_max}.Count());
    }

    SECTION("3.2 overflow, throw")
    {
        auto tmax = CTimestampTEST{my_numeric_limits_max};
        CHECK_THROWS(tmax += CTimestampTEST{1});
        REQUIRE_THROWS(near_max += my43ms);
    }
}

TEST_CASE("4: operator-=", "[CTimestampTEST]")
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

TEST_CASE("5: ToString", "[CTimestampTEST]")
{
    REQUIRE(CTimestampTEST{0}.ToString() == string("000000.000000"));
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

// todo: test FromString()
TEST_CASE("6: FromString()", "[CTimestampTEST]")
{
	auto t1 = CTimestampTEST{};
	t1.FromString("123456.123456", false);
	REQUIRE(t1.Count() == 123456123456);
}