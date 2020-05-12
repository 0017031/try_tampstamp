
#include "Timestamp.h"
#include <chrono>

int main()
{
    CTimestamp t1(0);
    CTimestamp t2 {};
    // CTimestampBase<> t3 =42; //compile-error if constructor declared explicit
    // t1 = 4.2;

    CTimestamp t3(4.2);

    return 0;
}