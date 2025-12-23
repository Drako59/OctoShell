#include "includes.h"



	BOOL grep(int argc, char** argv);


	BOOL FCopy(int argc , wchar_t** argv) {

		if (argc < 2) {


			printf("Ivalid number of arguments. ");
			return -1;
		}

		wchar_t*  file_dest = argv[0];
		wchar_t* file_src = argv[1];


		HANDLE hFile_dest = CreateFileW(file_dest, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE hFile_src = CreateFileW(file_src, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


		int size = GetFileSize(hFile_src, NULL);


		int start = 0; 
		int* NumOfBytesRead;
		wchar_t buffer[1024];





		SYSTEM_INFO sys_info;

		GetSystemInfo(&sys_info);

		int mem_buffer_size = sys_info.dwAllocationGranularity;


	}








