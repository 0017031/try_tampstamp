// open a file with boost.filemapping, and count it's number of lines

#include <string>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <algorithm>

namespace bip = boost::interprocess;


int main() {
// static_assert(0, "haha");
// static_assert(0);

    std::string filename = R"(C:\Users\baic\jp2.dom)";
    bip::file_mapping my_mapping1(filename.c_str(), bip::read_only);
    bip::mapped_region my_mapped_region1(my_mapping1, bip::read_only);

    auto const *const mapped_data = static_cast<char *>(my_mapped_region1.get_address());
    auto const mapped_size = my_mapped_region1.get_size();

    auto count_LF = std::count(mapped_data, mapped_data + mapped_size, '\n');
    std::cout << filename << " has " << count_LF << " lines.\n";

    auto count_Left_bracket = std::count(mapped_data, mapped_data + mapped_size, '[');
    std::cout << filename << " has " << count_Left_bracket << " '['.\n";

    auto count_right_bracket = std::count(mapped_data, mapped_data + mapped_size, ']');
    std::cout << filename << " has " << count_right_bracket << " ']'.\n";

    return 0;
}


