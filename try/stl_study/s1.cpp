// STL_study.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <vector>

using namespace std;

int main() {

    constexpr auto vec1 = array<int, 8>{1, 3, 222, 334, 21, 21, 3, 25};
    constexpr auto t_25 = 25;
    constexpr auto l_200 = 200;

    auto result1 = count(begin(vec1), end(vec1), t_25);
    auto result2 = count_if(begin(vec1), end(vec1), [&](auto &x) { return x > l_200; });

    for (auto &v : vec1) {
        cout << v << ", ";
    }
    cout << '\n';
    cout << "count target:" << t_25 << " ~ " << result1 << "times\n";
    cout << "count >200 : " << result2 << " times\n";

    // not yet
    // cout << "for_each_n\n";
    // for_each_n(begin(vec), 5,
    // [](auto &v) { cout << v << ","; });
    // cout << '\n';

    auto my_max_value = *max_element(begin(vec1), end(vec1));
    auto my_min_value = *min_element(begin(vec1), end(vec1));
    cout << "max value: " << my_max_value << '\n';
    cout << "min value: " << my_min_value << '\n';

    auto my_pair = minmax_element(begin(vec1), end(vec1));

    cout << "pair (min, max) " << *my_pair.first << ", " << *my_pair.second << '\n';

    constexpr auto v224 = 224;

    {
        auto p = std::find(std::begin(vec1), std::end(vec1), v224); //
        if (p != std::end(vec1)) {
            cout << "Found " << v224 << " (" << *p << ") at vec[" << p - begin(vec1) << "] !\n";
        }
    }

    {
        auto p = std::find_if(std::begin(vec1), std::end(vec1), [](auto &a) { return a == 1; });
        if (p != std::end(vec1)) {
            cout << "Found "
                 << "a==1: " << *p << " at vec[" << p - begin(vec1) << "] !\n";
        }
    }

    {
        auto p = std::find_if_not(std::begin(vec1), std::end(vec1), [](auto &a) { return a == 1; });
        if (p != std::end(vec1)) {
            cout << "Found "
                 << "NOT a==1: " << *p << " at vec[" << p - begin(vec1) << "] !\n";
        }
    }

    auto vec2 = /*std::vector<int>*/ {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
    auto pattern = std::array<int, 3>{1, 2, 3};

    {
        auto p = std::find_end(std::begin(vec2), std::end(vec2), std::begin(pattern), std::end(pattern));
        if (p != std::end(vec2)) {
            std::cout << "Found last subsequence at: " << p - begin(vec2) << "\n";
        }
    }

    {
        // auto[x, y, z] = pattern;
        struct S {
            int a;
            char b;
            double c;
        };
        // auto[a, b, c] = S{1, '2', 3.0};
    }

    return 0;
}
