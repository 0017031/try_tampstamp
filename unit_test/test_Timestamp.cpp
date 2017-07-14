// test_Timestamp.cpp
#include <iostream>
#include "stdafx.h"

using namespace std;

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
