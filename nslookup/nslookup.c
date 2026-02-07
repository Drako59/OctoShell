// nslookup.c : This file contains the 'main' function. Program execution begins and ends there.
//
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "CommandFuncs.h"
#include <WinDNS.h>

#pragma comment(lib, "Dnsapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

int main(int argc, char** argv)
{

	//DEBUG--------------------------------------
	/*argc = 3;

	argv = (char*)malloc(argc * sizeof(char));

	argv[1] = "www.google.com";
	argv[2] = "www.facebook.com";*/
	//-------------------------------------------

	CommandParsed command;
	CommandParser_init(&command, argc, argv, "p");
	
	char* strParameters = ParameterAsString(command.param,command.param_num);

	struct sockaddr_in* addr;
	WSADATA WSA;
	DWORD startUpRet = WSAStartup(MAKEWORD(2, 2), &WSA);
	if (startUpRet) {
		printf("nslookup: wsa start up failed, ERROR: %d\n", startUpRet);
		return 1;
	}


	WORD queryType = DNS_TYPE_A;
	if (strParameters != NULL) {
		if (strchr(strParameters, 'p')) {
			queryType = DNS_TYPE_PTR;
		}
	}

	wchar_t* domainNameW;
	char* domainName;
	char* UTF8buffer;
	DNS_RECORD* responseRecords = NULL;
	DNS_RECORD* pCur;
	for (int i = 0; i < command.argc; i++) {
		domainName = command.argv[i];
		domainNameW = utf8_to_utf16(domainName);
		if (domainName) {
			BOOL first = 0;
			//Loop thorugh responses and print the result
			DWORD status = DnsQuery_W(domainNameW, queryType, DNS_QUERY_STANDARD, NULL, &responseRecords, NULL);
			if (status == 0) {
				//HEADER

				for (pCur = responseRecords; pCur != NULL; pCur = pCur->pNext) {
					IN_ADDR infoDNS;

					switch (pCur->wType) {
					case(DNS_TYPE_A):
						if (!first++) printf("\nDNS RESPONSEs TYPE->A:\n");
						char ip[INET_ADDRSTRLEN];
						infoDNS.S_un.S_addr = pCur->Data.A.IpAddress;
						inet_ntop(AF_INET, &infoDNS, ip, sizeof(ip));
						UTF8buffer = utf16_to_utf8(pCur->pName);
						if (UTF8buffer == NULL) {
							printUtfCastError();
						}
						else {
							printf("\t%s-->%s, TTL: %d\n", UTF8buffer, ip, pCur->dwTtl);
							free(UTF8buffer);
						}
						break;
					case(DNS_TYPE_PTR):

						if (!first++) printf("\nDNS RESPONSEs TYPE->PTR:\n");
						UTF8buffer = utf16_to_utf8(pCur->Data.PTR.pNameHost);
						if (UTF8buffer == NULL) {
							printUtfCastError();
						}
						else {
							printf("\t%s-->%s, TTL: %d\n", argv[i], UTF8buffer, pCur->dwTtl);
							free(UTF8buffer);

						}
						free(UTF8buffer);
						break;
					}
					
				}

			}
			else {
				printf("nslookup: DNS QUERY FAILED.");
			}

			first = 0;

			DnsRecordListFree(responseRecords, DnsFreeRecordList);
			free(domainNameW);
		}
		else {
			printUtfCastError();
			
		}
		
	}













	

	free(strParameters);
	FreeCommandParser(&command);
	WSACleanup();
	//freeaddrinfo(resultDNS);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file




/*struct addrinfo infoDNS;
	struct addrinfo* resultDNS = NULL;
	const char* domain;*/






	//printf("\nDNS RESPONSE TYPE->A:\n");//HEADER
	//char* replayBuffer[1024];
	//for (int j = 0; j < command.argc; j++) {
	//	ZeroMemory(&infoDNS, sizeof(infoDNS));

	//	infoDNS.ai_family = AF_INET;
	//	infoDNS.ai_socktype = SOCK_DGRAM;



	//	domain = command.argv[j];

	//	int ret = getaddrinfo(domain, NULL, &infoDNS, &resultDNS);
	//	if (ret) {
	//		printf("nslookup: Error occured while getting the IP, ERROR: %d\n", ret);
	//		return 1;
	//	}
	//	addr = (struct sockaddr_int*)resultDNS->ai_addr;

	//	//printf("argv[0]->%s\n", command.argv[j]); //DEBUG-> REMOVE


	//	char addrStr[INET6_ADDRSTRLEN];
	//	inet_ntop(AF_INET, &(addr->sin_addr), addrStr, sizeof(addrStr));
	//	printf("%s-->%s\n", command.argv[j], addrStr);
	//}
	//printf("\n");