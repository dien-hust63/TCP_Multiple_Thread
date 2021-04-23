#include "stdafx.h"
#include "stdio.h"
#include "string.h"
#include <winsock2.h>
#include <WS2tcpip.h>
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#define MAX_CLIENT 3
#pragma comment (lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
	// Initiate Winsock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Winsock 2 is not supported.\n");
		return 0;
	}

	//Construct socket
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client == INVALID_SOCKET) {
		printf("Error: %d Cannot create client socket.", WSAGetLastError());
		return 0;
	}

	//Set time-out for receiving
	int tv = 10000;
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(int));

	//Specify server address
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);

	//Request to connect server
	if (connect(client, (sockaddr *)&serverAddr, sizeof(serverAddr))) {
		printf("Error %d Cannot connect to server.", WSAGetLastError());
		return 0;
	}
	printf("Connect server.\n");

	//Communicate with server
	char  buff[BUFF_SIZE];
	int ret, messageLen;
	while (1) {
		//Send message
		printf("Send to server: ");
		gets_s(buff, BUFF_SIZE);
		messageLen = strlen(buff);
		if (messageLen == 0) break;

		ret = send(client, buff, messageLen, 0);
		if (ret = SOCKET_ERROR) {
			printf("Error %d: Cannot send data.", WSAGetLastError());
			break;
		}

		//Receive echo message
		ret = recv(client, buff, BUFF_SIZE, 0);
		if (ret == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT) {
				printf("Time out.");
			}
			else printf("Error %d: Cannot receive data.", WSAGetLastError());
		}
		else if (strlen(buff) > 0) {
			buff[ret] = 0;
			printf("%s", buff);
		}
	}
	//Close socket
	closesocket(client);
	//Terminate winsock
	WSACleanup();


	return 0;
}




