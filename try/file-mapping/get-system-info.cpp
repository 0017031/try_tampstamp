#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "user32.lib")

int main() {
  SYSTEM_INFO siSysInfo;

  // Copy the hardware information to the SYSTEM_INFO structure.

  GetNativeSystemInfo(&siSysInfo);
  // GetSystemInfo(&siSysInfo);

  // Display the contents of the SYSTEM_INFO structure.

  printf("Hardware information: \n");
  printf("  OEM ID: %lu\n", siSysInfo.dwOemId);
  printf("  Number of processors: %lu\n", siSysInfo.dwNumberOfProcessors);
  printf("  Page size: %lu\n", siSysInfo.dwPageSize);
  printf("  Processor type: %lu\n", siSysInfo.dwProcessorType);
  printf("  Minimum application address: 0x%p\n",
         siSysInfo.lpMinimumApplicationAddress);
  printf("  Maximum application address: 0x%p\n",
         siSysInfo.lpMaximumApplicationAddress);
  printf("  Active processor mask: %lu\n", siSysInfo.dwActiveProcessorMask);
  
  return 0;
}
