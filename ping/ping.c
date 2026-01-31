// ping.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")


int main(int argc, char** argv)
{
	char* test = "hello";

	IN_ADDR addr;
	if (argc != 2)
	{
		printf("ping: Enter valid number of parameters");
	}
	if (inet_pton(AF_INET, argv[1], &addr) != 1) {
		printf("ping: couldn't intilize the ip.");
	}

	char addrStr[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr, addrStr, sizeof(addrStr));
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
	for (int i = 0; i < 4; i++) {
		IcmpSendEcho(hIcmpFile, addr.S_un.S_addr, test, sizeof(test) * strlen(test), &options, replayBuffer, sizeof(replayBuffer), 1000);

		reply = (PICMP_ECHO_REPLY)replayBuffer;

		if (reply->Status == IP_REQ_TIMED_OUT) {
			printf("Ping request reached his timeout......\n");
		}
		else if (reply->Status == IP_TTL_EXPIRED_TRANSIT) {
			printf("ping reached his hops limit and got thorwed away.");

		}
		else if (reply->Status == IP_SUCCESS) {
			char addrReplayStr[INET6_ADDRSTRLEN];
			IN_ADDR replayAddr;
			replayAddr.S_un.S_addr = reply->Address;
			inet_ntop(AF_INET, &replayAddr, addrReplayStr, sizeof(addrReplayStr));

			printf("REPLAY ADDR: %s RTT: %lu TTL: %u\n", addrReplayStr, reply->RoundTripTime,options.Ttl);
		}
		else {
			printf("ping failed :(");
		}
		Sleep(1000);

	}

	/*
	* ---8.8.8.8 ping statistics---
			5 packets transmitted, 5 received, 0% packet loss, time 4004ms
			rtt min/avg/max/mdev = 12.345/12.456/12.567/0.123 ms
	*/

	//For demosntrating a real ping

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
