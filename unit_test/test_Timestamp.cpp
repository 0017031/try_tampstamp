#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "stdafx.h"
using namespace std;

TEST_CASE("CTimestamp_constuctor", "[CTimestamp]")
{
    CTimestamp t2{42};
	auto t4 = CTimestamp{ t2 };

	REQUIRE(t2.count() == 42);
	REQUIRE(t4.count() == 42);

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