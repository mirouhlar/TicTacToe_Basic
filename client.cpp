#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "Ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>
#include <memory.h>

SOCKET createClientSocket(const char* address, int port)
{
	//inicializacia kniznice WinSock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		printf("Error in WSA startup\n");
		return INVALID_SOCKET;
	}

	//vytvorenie socketu
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("Error creating socket\n");
		WSACleanup();
		return INVALID_SOCKET;
	}

	//nastavenie IP adresy a portu servera
	sockaddr_in socketAddr;
	memset(&socketAddr, 0, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = inet_addr(address);
	socketAddr.sin_port = htons(port);

	//pripojenie na server
	if (connect(clientSocket, (sockaddr*)&socketAddr, sizeof(socketAddr)) == SOCKET_ERROR)
	{
		printf("Error connecting to server\n");
		WSACleanup();
		return INVALID_SOCKET;
	}

	return clientSocket;
}

int main()
{


	int port;
	printf("Enter port: ");
	scanf("%d", &port);
	char address[16];
	printf("Enter server address: ");
	scanf("%s", &address);
	char name[20];
	printf("Enter your name: ");
	scanf("%s", &name);

	getchar();

	//vytvorenie klientskeho socketu
	SOCKET clientSocket = createClientSocket(address, port);
	if (clientSocket == INVALID_SOCKET)
	{
		return -1;
	}

	printf("TCP socket created\n");
	send(clientSocket, name, strlen(name) + 1, 0);

	char sendBuff[256], recvBuff[256];

	memset(recvBuff, 0, 256);
	//prijatie dat zo servera
	if (recv(clientSocket, recvBuff, 256, 0) <= 0)
	{
		printf("Error receiving data\n");
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}
	printf("%s\n", recvBuff);
	if (recv(clientSocket, recvBuff, 256, 0) <= 0)
	{
		printf("Error receiving data\n");
		closesocket(clientSocket);
		WSACleanup();
		return -1;
	}
	printf("%s\n", recvBuff);
	while (1) {
		
		scanf("%s", sendBuff);

		send(clientSocket, sendBuff, strlen(sendBuff) + 1, 0);

		if (recv(clientSocket, recvBuff, 256, 0) <= 0)
		{
			printf("Error receiving data\n");
			closesocket(clientSocket);
			WSACleanup();
			return -1;
		}
		if ((!strcmp(recvBuff, "VYHRAL HRAC 1\n"))){
			printf("KONIEC HRY, VYHRAL HRAC 1\n");
			break;
		}
		else if ((!strcmp(recvBuff, "VYHRAL HRAC 2\n"))) {
			printf("KONIEC HRY, VYHRAL HRAC 2\n");
			break;
		}

		printf("%s\n", recvBuff);

		
	}
	//ukoncenie spojenia so serverom
	shutdown(clientSocket, SD_SEND);
	closesocket(clientSocket);
	//ukoncenie pouzivania kniznice WinSock
	WSACleanup();

	return 0;
}
