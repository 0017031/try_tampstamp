#include <algorithm>
#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <string>
//#include <utility>

#include <gsl/gsl>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
// https://www.boost.org/doc/libs/1_67_0/doc/html/interprocess/sharedmemorybetweenprocesses.html#interprocess.sharedmemorybetweenprocesses.mapped_file
#include <boost/tokenizer.hpp>
#include <boost/endian/conversion.hpp>

using BYTE = char;


using boost::escaped_list_separator;

class isf_parsed_into_buffer {
public:
  std::vector<BYTE> buffer;
  gsl::span<BYTE> header_span;
  gsl::span<BYTE> data_span;
  std::map<std::string, std::string> tags_map;
  bool MY_TRACE{false};

public:
  explicit isf_parsed_into_buffer(const std::string &isf_file_name) {
	  auto my_file = std::ifstream{isf_file_name, std::ios::binary | std::ios::in};
	  buffer = std::vector<BYTE>{std::istreambuf_iterator<BYTE>{my_file}, std::istreambuf_iterator<BYTE>()};

	  if (MY_TRACE) {
		  std::cout << "\nmapped size:\t" << buffer.size() << '\n'
		            << "mapped_address:\t0x" << std::hex << reinterpret_cast<std::ptrdiff_t>(buffer.data()) << '\n'
		            << std::dec << "dump:\n"
		            << buffer.data();
	  }

	  _parse_buffer_to_locate_spans(header_span, data_span);
	  tags_map = _parse_header_into_std_map();

	  if (MY_TRACE) {
		  std::cout << "\n\ndata size:\t" << data_span.size_bytes() << '\n'
		            << "data_address:\t0x" << std::hex << reinterpret_cast<size_t>(data_span.data()) << '\n'
		            << std::dec;
	  }

  };

private:
  void _parse_buffer_to_locate_spans(gsl::span<BYTE> &header_span, gsl::span<BYTE> &data_span) {
	  constexpr auto CURVE_TEXT = ":CURVE #";
	  const auto _CURVE_start =
		  std::search(buffer.cbegin(), buffer.cend(), &CURVE_TEXT[0], &CURVE_TEXT[0] + std::strlen(CURVE_TEXT));
	  //":CURVE_TEXT " is the last tag of the header, followed by '#XYYY' with X being the number of bytes of YYY.
	  // YYY is the length of the data-stream following in bytes. for example ":CURVE #820000000"
	  const auto itr_X = _CURVE_start + std::strlen(CURVE_TEXT);
	  const auto _X_value = std::stoi(std::string(itr_X, itr_X + 1));
	  const auto itr_YYY = itr_X + 1;
	  const auto _YYY_value = std::stoi(std::string(itr_YYY, itr_YYY + _X_value));

	  auto itr_data = itr_YYY + _X_value;
	  auto header_length = itr_data - buffer.cbegin();
	  auto data_length = _YYY_value;
	  assert((itr_data - buffer.cbegin()) + data_length == buffer.size()); //470 + 20000000 == 20000470

	  const auto full_span = gsl::make_span(buffer);
	  header_span = full_span.first(header_length);
	  data_span = full_span.last(data_length);
  }

  std::map<std::string, std::string> _parse_header_into_std_map() {
	  //const std::string buffer = R"(:WFMPRE:NR_PT 10000000;:WFMPRE:BYT_NR 2;BIT_NR 16;ENCDG BINARY;BN_FMT RI;BYT_OR MSB;WFID "Ch1, DC coupling, 200.0mV/div, 20.00s/div, 10000000 points, Pk Detect mode";NR_PT 10000000;PT_FMT ENV;XUNIT "s";XINCR 20.0000E-6;XZERO -100.0000;PT_OFF 0;YUNIT "V";YMULT 31.2500E-6;YOFF 18.8160E+3;YZERO 0.0E+0;VSCALE 200.0000E-3;HSCALE 20.0000;VPOS 2.9400;VOFFSET 0.0E+0;HDELAY 0.0E+0;DOMAIN TIME;WFMTYPE ANALOG;CENTERFREQUENCY 0.0E+0;SPAN 0.0E+0;REFLEVEL 0.0E+0;:CURVE #820000000)";

	  std::map<std::string, std::string> my_map;

	  const auto tag_separator = boost::escaped_list_separator<char>{'\\', ';', 0};
	  const auto pair_separator = boost::escaped_list_separator<char>{'\\', ' ', '"'};

	  const auto header_string = std::string(header_span.cbegin(), header_span.cend());

	  boost::tokenizer<escaped_list_separator<char>> buffer_to_tags(header_string, tag_separator);
	  for (auto &i_tag : buffer_to_tags) {
		  boost::tokenizer<escaped_list_separator<char>> tag_to_pair(i_tag, pair_separator);
		  auto s0 = tag_to_pair.begin();
		  my_map.emplace(*s0, *next(s0));
	  }

	  if (MY_TRACE) {
		  for (auto &i:my_map) {
			  std::cout << '(' << i.first << ')' << ' ' << '[' << i.second << ']' << '\n';
		  }
	  }
	  return my_map;

  }
};

isf_parsed_into_buffer f0_read_file_with_buffer(int argc, char **argv) {
	std::string filename;
	if (argc < 2) {
		filename = std::string{
			R"(C:\Users\baic\OneDrive - Boston Scientific\work_one_drive\PulseAnalyzer_GUI\src\tek0016CH1.isf)"};
	} else {
		filename = std::string{argv[1]};
	}

	auto parsed_isf = isf_parsed_into_buffer{filename};

	size_t sample_len = std::stoul(parsed_isf.tags_map.at("NR_PT"));
	auto endian = parsed_isf.tags_map.at("BYT_OR");

	auto data_span_int16 = gsl::span<int16_t>(reinterpret_cast<int16_t *> (&parsed_isf.data_span[0]), sample_len);

	auto valid_data_begin_itr = std::find_if_not(data_span_int16.begin(), data_span_int16.end(),
	                                             [&endian](auto &i) {
	                                               int16_t invalid_data = -32768;
	                                               if (endian == "MSB") {
		                                               invalid_data = 128;
	                                               }
	                                               return i == invalid_data;
	                                             });

	auto valid_data_span = data_span_int16.last(valid_data_begin_itr-data_span_int16.begin());

	if (endian == "MSB") {
		std::transform(valid_data_span.begin(), valid_data_span.end(),
		               valid_data_span.begin(),
		               [](auto &i) { return boost::endian::big_to_native(i); });
	}


	return parsed_isf;
}

int main(int argc, char *argv[]) {
	f0_read_file_with_buffer(argc, argv);

	//f2_convert_sample_inplace();

	return 0;
}

#if 1
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

int parse_header2(const std::string &buffer) {
	//const std::string buffer = R"(:WFMPRE:NR_PT 10000000;:WFMPRE:BYT_NR 2;BIT_NR 16;ENCDG BINARY;BN_FMT RI;BYT_OR MSB;WFID "Ch1, DC coupling, 200.0mV/div, 20.00s/div, 10000000 points, Pk Detect mode";NR_PT 10000000;PT_FMT ENV;XUNIT "s";XINCR 20.0000E-6;XZERO -100.0000;PT_OFF 0;YUNIT "V";YMULT 31.2500E-6;YOFF 18.8160E+3;YZERO 0.0E+0;VSCALE 200.0000E-3;HSCALE 20.0000;VPOS 2.9400;VOFFSET 0.0E+0;HDELAY 0.0E+0;DOMAIN TIME;WFMTYPE ANALOG;CENTERFREQUENCY 0.0E+0;SPAN 0.0E+0;REFLEVEL 0.0E+0;:CURVE #820000000)";

	using boost::escaped_list_separator;

	const auto separator1 = escaped_list_separator<char>{"", "; ", "\""};
	boost::tokenizer<escaped_list_separator<char>> tok(buffer, separator1);

	std::map<std::string, std::string> tags_map;
	auto iter = std::cbegin(tok);
	while (iter != std::cend(tok)) {
		auto s1 = *iter;
		iter = std::next(iter);
		auto s2 = *iter;
		iter = std::next(iter);
		//tags_map[s1] = s2;
		tags_map.emplace(s1, s2);
	}

	for (auto &i:tags_map) {
		std::cout << '(' << i.first << ')' << ' ' << '[' << i.second << ']' << '\n';
	}

	return 0;
}

//#define UNREFERENCED_PARAMETER(x) (x)
class isf_parsed_with_mmap {
private:
  const std::string file_name;
  boost::interprocess::file_mapping my_mapping1;
  boost::interprocess::mapped_region my_mapped_region1;

public:
  gsl::span<BYTE> isf_span;
  gsl::span<BYTE> header_span;
  gsl::span<BYTE> data_span;

public:
  isf_parsed_with_mmap() = delete;
  //~isf_parsed_with_mmap() = default;
  //isf_parsed_with_mmap(const isf_parsed_with_mmap &) = delete;
  //isf_parsed_with_mmap &operator=(const isf_parsed_with_mmap &) = delete;
  //isf_parsed_with_mmap(isf_parsed_with_mmap &&) = default;
  //isf_parsed_with_mmap &operator=(isf_parsed_with_mmap &&) = default;

  explicit isf_parsed_with_mmap(const std::string &isf_file) :
	  file_name(isf_file),
	  my_mapping1(isf_file.c_str(), boost::interprocess::read_only),
	  my_mapped_region1(my_mapping1, boost::interprocess::read_only),
	  isf_span(reinterpret_cast<BYTE *>(my_mapped_region1.get_address()),
			   my_mapped_region1.get_size()) {
	  _find_span();
  }

private:
  void _find_span() {    /* find data_stream, store info in data_span */
	  const auto CURVE_TEXT = std::string{":CURVE "};
	  const auto curve_tag_start_itr =
		  search(isf_span.cbegin(), isf_span.cend(), cbegin(CURVE_TEXT), cend(CURVE_TEXT));
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
	  header_span = isf_span.subspan(0, data_stream_start_itr - isf_span.cbegin());
	  data_span = isf_span.subspan(data_stream_start_itr - isf_span.cbegin());
  }

};

isf_parsed_with_mmap (int argc, char **argv) {
	std::string filename;
	if (argc < 2) {
		filename = std::string{
			R"(C:\Users\baic\OneDrive - Boston Scientific\work_one_drive\PulseAnalyzer_GUI\src\tek0000CH1.isf)"};
	} else {
		filename = std::string{argv[1]};
	}

	isf_parsed_with_mmap mm{filename};

	std::cout << "\nmapped size:\t" << mm.isf_span.size_bytes() << '\n'
			  << "mapped_address:\t0x" << std::hex << reinterpret_cast<size_t>(mm.isf_span.data()) << '\n'
			  << std::dec << "dump:\n"
			  << mm.isf_span.data();

	std::cout << "\n\ndata size:\t" << mm.data_span.size_bytes() << '\n'
			  << "data_address:\t0x" << std::hex << reinterpret_cast<size_t>(mm.data_span.data()) << '\n'
			  << std::dec;

	return mm;
}
try_read_file_with_mmap

void f1_convert_sample() {
	constexpr size_t buffer_len = 4;
	constexpr size_t sample_len = buffer_len / 2;

	uint8_t input_buffer[] = {0xab, 0xcd, 0x12, 0x34};
	std::vector<int16_t> output_sample_vector;

	auto span_byte = gsl::make_span(input_buffer);
	gsl::span<int16_t, sample_len> span_words(reinterpret_cast<int16_t *> (&input_buffer[0]), sample_len);

	//auto converter_f = boost::endian::big_to_native<int16_t>;
	auto converter_f = boost::endian::little_to_native<int16_t>;

	output_sample_vector.reserve(sample_len);

	for (auto i : span_words) {
		output_sample_vector.push_back(converter_f(i));
	}

	for (auto i: output_sample_vector) {
		std::cout << "0x" << std::uppercase << std::hex << i << '\n';
	}
}

void f2_convert_sample_inplace() {
	constexpr size_t buffer_len = 4;
	constexpr size_t sample_len = buffer_len / 2;

	uint8_t input_buffer[] = {0x0a, 0x0b, 0x03, 0x04};
	std::vector<int16_t> output_sample_vector;

	//auto span_byte = gsl::make_span(input_buffer);
	auto span_words = gsl::span<int16_t, sample_len>(reinterpret_cast<int16_t *> (&input_buffer[0]), sample_len);

	//auto converter_f = boost::endian::little_to_native<int16_t>;
	auto converter_f = boost::endian::big_to_native<int16_t>;

	std::transform(span_words.begin(), span_words.end(),
				   span_words.begin(),
				   [&](auto &i) { return converter_f(i); });

	for (auto &i : span_words) {
		std::cout << "0x" << std::uppercase << std::hex << i << '\n';
	}
}


#endif
