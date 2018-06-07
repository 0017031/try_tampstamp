// open a file with boost.filemapping, and count it's number of lines

//#include <algorithm>
#include <experimental/filesystem>
#include <iostream>
#include <string>

#include <gsl/gsl>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/tokenizer.hpp>
// https://www.boost.org/doc/libs/1_67_0/doc/html/interprocess/sharedmemorybetweenprocesses.html#interprocess.sharedmemorybetweenprocesses.mapped_file


using BYTE = char;

using boost::interprocess::file_mapping;
using boost::interprocess::mapped_region;
using boost::interprocess::read_only;

class my_map {

public:
    const std::string file_name;
    file_mapping my_mapping1;
    mapped_region my_mapped_region1;
    gsl::span<BYTE> isf_span;
    gsl::span<BYTE> header_span;
    gsl::span<BYTE> data_span;

public:
    my_map() = delete;
    //~my_map() = default;
    //my_map(const my_map &) = delete;
    //my_map &operator=(const my_map &) = delete;
    //my_map(my_map &&) = default;
    //my_map &operator=(my_map &&) = default;

    explicit my_map(const std::string &f);
};

my_map::my_map(const std::string &f)
        : file_name(f), my_mapping1(f.c_str(), read_only), my_mapped_region1(my_mapping1, read_only),
          isf_span(reinterpret_cast<BYTE *>(my_mapped_region1.get_address()), my_mapped_region1.get_size()) {
    /* find data_stream, store info in data_span */
    const auto CURVE_TEXT = std::string{":CURVE "};
    const auto curve_tag_start_itr =
            std::search(isf_span.cbegin(), isf_span.cend(), std::cbegin(CURVE_TEXT), std::cend(CURVE_TEXT));
    //":CURVE_TEXT " is the last tag of the header, followed by
    //'#XYYY' with X being the number of bytes of YYY.
    // YYY is the length of the data-stream following in bytes.
    // for example ":CURVE #820000000"
    const auto curve_X_position_itr = curve_tag_start_itr + CURVE_TEXT.length() + 1; //"#" before x
    const auto curveX_value = std::stoi(std::string(curve_X_position_itr, curve_X_position_itr + 1));
    const auto curve_YYY_position_itr = curve_X_position_itr + 1;
    const auto curve_YYY_value =
            std::stoi(std::string(curve_YYY_position_itr, curve_YYY_position_itr + curveX_value));

    const auto data_stream_start_itr = curve_YYY_position_itr + curveX_value;
    const auto data_stream_length = curve_YYY_value;
    assert((data_stream_start_itr - isf_span.cbegin()) + data_stream_length == isf_span.size_bytes());
    // 470 + 20000000 == 20000470
    header_span = isf_span.subspan(0,data_stream_start_itr - isf_span.cbegin());
    data_span = isf_span.subspan(data_stream_start_itr - isf_span.cbegin());
}


my_map try_read_file_with_mmap(int argc, char **argv) {
    std::string filename;
    if (argc < 2) {
        filename = std::string{R"(w:\PulseAnalyzer_GUI\src\tek0009CH1.isf)"};
    } else {
        filename = std::string{argv[1]};
    }

    my_map mm{filename};

    std::cout << "\nmapped size:\t" << mm.isf_span.size_bytes() << '\n'
              << "mapped_address:\t0x" << std::hex << reinterpret_cast<size_t>(mm.isf_span.data()) << '\n'
              << std::dec << "dump:\n"
              << mm.isf_span.data();

    std::cout << "\n\ndata size:\t" << mm.data_span.size_bytes() << '\n'
              << "data_address:\t0x" << std::hex << reinterpret_cast<size_t>(mm.data_span.data()) << '\n'
              << std::dec;

    return mm;
}

//#define UNREFERENCED_PARAMETER(x) (x)



int parse_header(const std::string &buffer) {

    using boost::tokenizer;
    using boost::escaped_list_separator;

    std::map<std::string, std::string> my_tags_map;
    //const std::string buffer = R"(:WFMPRE:NR_PT 10000000;:WFMPRE:BYT_NR 2;BIT_NR 16;ENCDG BINARY;BN_FMT RI;BYT_OR MSB;WFID "Ch1, DC coupling, 200.0mV/div, 20.00s/div, 10000000 points, Pk Detect mode";NR_PT 10000000;PT_FMT ENV;XUNIT "s";XINCR 20.0000E-6;XZERO -100.0000;PT_OFF 0;YUNIT "V";YMULT 31.2500E-6;YOFF 18.8160E+3;YZERO 0.0E+0;VSCALE 200.0000E-3;HSCALE 20.0000;VPOS 2.9400;VOFFSET 0.0E+0;HDELAY 0.0E+0;DOMAIN TIME;WFMTYPE ANALOG;CENTERFREQUENCY 0.0E+0;SPAN 0.0E+0;REFLEVEL 0.0E+0;:CURVE #820000000)";

    const auto tag_split_f = escaped_list_separator<char>{'\\', ';', 0};
    const auto pair_split_f = escaped_list_separator<char>{'\\', ' ', '"'};

    tokenizer<escaped_list_separator<char>> buffer_to_tags(buffer, tag_split_f);
    for (auto &i_tag : buffer_to_tags) {
        tokenizer<escaped_list_separator<char>> tag_to_pair(i_tag, pair_split_f);
        auto s0 = tag_to_pair.begin();
        my_tags_map.emplace(*s0, *std::next(s0));
    }

    for (auto &t : my_tags_map) {
        std::cout << '(' << t.first << ')' << " [" << t.second << ']' << std::endl;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    auto mm = try_read_file_with_mmap(argc, argv);
    auto buffer = std::string(mm.header_span.cbegin(),mm.header_span.cend());
    return parse_header(buffer);
}

#if 0
std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

#include <boost/algorithm/string.hpp>
void bbb() {
    std::string text = "Let me split this into words";
    std::vector<std::string> results;

    boost::split(results, text, [](char c) { return c == ' '; });
}
#endif