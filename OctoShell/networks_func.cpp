#include "includes.h"

BOOL ping(int argc, wchar_t** argv);

BOOL trace(int argc, wchar_t** argv[]);

BOOL arp_search(int argc, wchar_t** argv[]);

BOOL nslookup(int argc, wchar_t** argv[]);

BOOL IPconfig(int argc, wchar_t** argv[]);




//
//typedef struct {
//	char* dest;
//	char* src;
//	char* checksum;
//	char* size;
//	char* type;
//	char* identifier;
//	char* data; 
//} PACKET_HEADER;
//
//unsigned int checksum(PACKET_HEADER);
//
//
//
//
//BOOL  my_ping(int argc, char** argv[]) {
//	
//	if (argc > 1)
//		printf("Ivalid argument number");
//	SOCKET my_socket = INVALID_SOCKET;
//
//
//	my_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_ICMP);
//
//
//
//}

