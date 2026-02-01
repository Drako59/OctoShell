// ping.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>
#include "CommandFuncs.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")




BOOL ResolveDNSv4() {

}

int main(int argc, char** argv)
{

	
	 
	char* test = "hello";
	if (argc != 2)
	{
		printf("ping: Enter valid number of parameters\n");
		return 1;
	}

	/*CommandParsed command;
	CommandParser_init(&command,argc,argv);*/

	int PackagesAmount = 4;

	/*char* startInde;
	char* strParametrs = ParameterAsString(command.param, command.param_num);

	if (strchr(strParametrs, 'a') || strchr(strParametrs, 'A')) {

	}*/

	const char* domain = argv[1];
	struct addrinfo infoDNS;
	ZeroMemory(&infoDNS, sizeof(infoDNS));

	infoDNS.ai_family = AF_INET;
	infoDNS.ai_socktype = SOCK_DGRAM;
	WSADATA WSA;
	DWORD startUpRet = WSAStartup(MAKEWORD(2, 2), &WSA);
	if (startUpRet) {
		printf("ping: wsa start up failed, ERROR: %d\n", startUpRet);
		return 1;
	}
	struct addrinfo* resultDNS;
	int ret = getaddrinfo(domain, NULL, &infoDNS, &resultDNS);
	if (ret) {
		printf("ping: Error occured while getting the IP, ERROR: %d\n", ret);
		return 1;
	}
	struct sockaddr_in* addr = (struct sockaddr_int*)resultDNS->ai_addr;
	
	

	char addrStr[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &(addr->sin_addr), addrStr, sizeof(addrStr));
	printf("-----ping %s------\n",addrStr);


	HANDLE hIcmpFile = IcmpCreateFile();

	if (hIcmpFile == NULL)
	{
		printf("ping: There was a problem in opening the ICMP file.");
		return 1;
	}
	
	IP_OPTION_INFORMATION options;
	ZeroMemory(&options, sizeof(IP_OPTION_INFORMATION));
	options.Ttl = 64;
	char* replayBuffer[1024];
	PICMP_ECHO_REPLY reply;
	int success_counter = 0;
	ULONG sum_time = 0;
	for (int i = 0; i < 4; i++) {
		IcmpSendEcho(hIcmpFile, addr->sin_addr.S_un.S_addr, test, sizeof(test) * strlen(test), &options, replayBuffer, sizeof(replayBuffer), 1000);

		reply = (PICMP_ECHO_REPLY)replayBuffer;

		if (reply->Status == IP_REQ_TIMED_OUT) {
			printf("Ping request reached his timeout......\n");
		}
		else if (reply->Status == IP_TTL_EXPIRED_TRANSIT) {
			printf("ping reached his hops limit and got thorwed away.\n");

		}
		else if (reply->Status == IP_SUCCESS) {
			success_counter++;
			sum_time += reply->RoundTripTime;
			char addrReplayStr[INET6_ADDRSTRLEN];
			IN_ADDR replayAddr;
			replayAddr.S_un.S_addr = reply->Address;
			inet_ntop(AF_INET, &replayAddr, addrReplayStr, sizeof(addrReplayStr));

			printf("REPLAY ADDR: %s RTT: %lums TTL: %u\n", addrReplayStr, reply->RoundTripTime,options.Ttl);
		}
		else {
			printf("ping failed :(\n");
		}
		Sleep(1000);

	}
	
	if (success_counter > 1) {
		printf("avg ping RTT: %.2fms\n", (double)sum_time / success_counter);
	}

	/*
	* ---8.8.8.8 ping statistics---
			5 packets transmitted, 5 received, 0% packet loss, time 4004ms
			rtt min/avg/max/mdev = 12.345/12.456/12.567/0.123 ms
	*/

	//For demosntrating a real ping
	 
	//FreeCcommandParser(&command);
	WSACleanup();
	freeaddrinfo(resultDNS);
	IcmpCloseHandle(hIcmpFile);
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
