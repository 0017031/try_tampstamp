
http://stackoverflow.com/questions/10839747/istream-vs-memory-mapping-a-file

boost:  
#include <boost/interprocess/file_mapping.hpp>  
#include <boost/interprocess/mapped_region.hpp>  

windows:  







AWE:Address Windowing Extensions (32bit)   
https://msdn.microsoft.com/en-us/library/windows/desktop/aa366527(v=vs.85).aspx  
AWE solves this problem by allowing applications to directly address huge amounts of memory while continuing to use 32-bit pointers. AWE allows applications to have data caches larger than 4GB (where sufficient physical memory is present). AWE uses physical nonpaged memory and window views of various portions of this physical memory within a 32-bit virtual address space.

Virtual Address Space  
https://msdn.microsoft.com/en-us/library/windows/desktop/aa366912(v=vs.85).aspx  
Virtual Address Space 64bit   
https://msdn.microsoft.com/en-us/library/windows/desktop/aa384271(v=vs.85).aspx  
  
Memory Limits for Windows and Windows Server Releases  
https://msdn.microsoft.com/en-us/library/windows/desktop/aa366778(v=vs.85).aspx  


At last, if your algorithm permits it, 
you might csplit the files in smaller pieces and process each of them in parallel processes.



unix that would be mmap on windows CreateFileMapping/MapViewIntoFile
