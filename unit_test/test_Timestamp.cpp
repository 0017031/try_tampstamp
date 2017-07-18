#include "stdafx.h"
using namespace std;
using namespace std::chrono;
using namespace date;

TEST_CASE("1: Constuctor", "[CTimestamp]")
{
    SECTION("compute max")
    {
        const auto max_duratian_micro_second = microseconds{numeric_limits<microseconds::rep>::max()};
        const auto max_duration_day = duration<double, days::period>{max_duratian_micro_second};
        const auto max_duration_month = duration<double, months::period>{max_duratian_micro_second};
        const auto max_duration_year = duration<double, years::period>{max_duratian_micro_second};

        std::cout << "max:\n"
                  << std::fixed << std::setw(TIMESTAMP_STRING_LEN) << std::setprecision(TIMESTAMP_STRING_DECIMAL)
                  << std::setfill('0') << max_duratian_micro_second.count() << " mus\n"
                  << max_duration_day.count() << " day\n"
                  << max_duration_month.count() << " month\n"
                  << max_duration_year.count() << " year\n";
    }

    SECTION("constructing from T")
    {
        CTimestamp t0{42};
        REQUIRE(t0.count() == 42);
    }

    // For each SECTION the TEST_CASE is executed from the start
    SECTION("copy constructor")
    {
        const auto t1 = CTimestamp{4242};
        const auto t2 = t1;
        REQUIRE(t2.count() == 4242);
    }
}

TEST_CASE("2: this->count", "[CTimestamp]")
{
    auto d = duration<int64_t, std::micro>{123'456'123'456i64};
    auto t3 = CTimestamp{123'456'123'456};
    REQUIRE(t3.count() == d.count());
}

#if 0

int main(void)
{
    // Test constructor

    // CTimestampBase(T ts_integer) : m_nTimestamp(ts_integer) {}
    // CTimestampBase(const CTimestampBase& src) { m_nTimestamp = src.m_nTimestamp; }

    // CTimestamp t2;
    auto t2 = CTimestamp{4242};
    auto t3 = t2 + 1;
    auto t4 = CTimestamp{t2};

	auto t5 = static_cast<int64_t>(t2);

    cout << "t2=" << t2.count() << '\n';
    cout << "t3=" << t3 << '\n';
    cout << "t4=" << t4.count() << '\n';
	cout << "t5=" << t5 << '\n';


    return 0;
}
#endif