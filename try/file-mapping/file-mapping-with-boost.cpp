// open a file with boost.filemapping, and count it's number of lines

#include <cstddef>
#include <string>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

namespace bip = boost::interprocess;




int main (void)
{
// static_assert(0, "haha");
// static_assert(0);

		std::string filename = /*...*/;
		bip::file_mapping mapping(filename.c_str(), bip::read_only);
		bip::mapped_region mapped_rgn(mapping, bip::read_only);
		char const* const mmaped_data = static_cast<char*>(mapped_rgn.get_address());
                std::size_t const mmap_size = mapped_rgn.get_size();
                return 0;
}


// C:\vs141\VC\Tools\MSVC\14.11.25503\atlmfc\include\atlfile.h
// W:\heron\src\Heron_000_INT\CommonTools\CommonLogFile\CommonEventLibrary\CEL\src\FileMapping.h
// CreateFileMapping
