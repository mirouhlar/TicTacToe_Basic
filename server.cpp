#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "Ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>
#include <memory.h>

typedef struct Sockets
{
	SOCKET client;
	const char* name;
};

struct Sockets array[10];
static bool terminator = false;
int len = -1;

SOCKET createListenSocket(int port)
{
	//inicializacia kniznice WinSock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		printf("Error in WSA startup\n");
		return INVALID_SOCKET;
	}

	//vytvorenie socketu pre prijimanie spojeni
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		printf("Error creating socket\n");
		WSACleanup();
		return INVALID_SOCKET;
	}

	//nastavenie portu servera
	sockaddr_in socketAddr;
	memset(&socketAddr, 0, sizeof(socketAddr));
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socketAddr.sin_port = htons(port);

	//priradenie portu pre socket
	if (bind(listenSocket, (sockaddr*)&socketAddr, sizeof(socketAddr)) == SOCKET_ERROR)
	{
		printf("Error binding socket\n");
		closesocket(listenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	//nastavenie socketu do rezimu prijimania spojeni
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("Error listening on socket\n");
		closesocket(listenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	return listenSocket;
}

SOCKET acceptClient(SOCKET listenSocket)
{
	//cakanie na klienta a nasledne jeho pripojenie
	SOCKET clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("Error accepting client\n");
		return INVALID_SOCKET;
	}

	return clientSocket;
}


DWORD _stdcall function(void* arg) {

	SOCKET clientSocket = *(SOCKET*)arg;
	char name[20];
	int myIdx = len;
	recv(clientSocket, name, 256, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		closesocket(clientSocket);
		return -1;
	}
	printf("%s connected\n", name);
	array[myIdx].client = clientSocket;
	array[myIdx].name = name;

	char recvBuff[256], sendBuffer[256], msg[5000];
	int result;
	if (myIdx == 1) {
		sprintf(msg, "%s", "WELCOME TO THE GAME\nPole je oznacene cislami 1 az 9 takto :\n1|2|3\n-----\n4|5|6\n-----\n7|8|9\n");
		send(array[0].client, msg, strlen(msg) + 1, 0);
		send(array[1].client, msg, strlen(msg) + 1, 0);
	
	
	int vyhra = 0;
	int value = 0;
	int hrpole[9] = {0,0,0,0,0,0,0,0,0};

	
	while (vyhra != 1) {
		printf("Na rade je hrac 1\n");
		int hrac1 = -1;
		int hrac2 = -1;
		sprintf(msg, "%s", "Zadaj poziciu: ");
		send(array[0].client, msg, strlen(msg) + 1, 0);
		while (hrac1 != 0) {
		
			result = recv(array[0].client, recvBuff, 256, 0);
			if (result == SOCKET_ERROR)
			{
				array[myIdx].name = NULL;
				array[myIdx].client = INVALID_SOCKET;
				closesocket(array[0].client);
				return -1;
			}
			if (result > 0)
			{

				sscanf(recvBuff, "%d", &value);
			//	printf("%s %d\n", recvBuff, value);
				if ((hrpole[value - 1] == 0) && (value != 0)) {
					hrpole[value - 1] = 1;
					hrac1 = 0;
				}
				else {
					hrac1 = -1;
					sprintf(msg, "%s", "OBSADENE POLICKO!\nZadaj poziciu: ");
					send(array[0].client, msg, strlen(msg) + 1, 0);
				}
				printf("%d %d %d\n%d %d %d\n%d %d %d\n", hrpole[0], hrpole[1], hrpole[2], hrpole[3], hrpole[4], hrpole[5], hrpole[6], hrpole[7], hrpole[8]);
			}
		}
		

		if (((hrpole[0] == 1) && (hrpole[3] == 1) && (hrpole[6] == 1))|| ((hrpole[1] == 1) && (hrpole[4] == 1) && (hrpole[7] == 1))||
			((hrpole[2] == 1) && (hrpole[5] == 1) && (hrpole[8] == 1)) || ((hrpole[0] == 1) && (hrpole[1] == 1) && (hrpole[2] == 1)) ||
			((hrpole[3] == 1) && (hrpole[4] == 1) && (hrpole[5] == 1)) || ((hrpole[6] == 1) && (hrpole[7] == 1) && (hrpole[8] == 1)) ||
			((hrpole[0] == 1) && (hrpole[4] == 1) && (hrpole[8] == 1)) || ((hrpole[2] == 1) && (hrpole[4] == 1) && (hrpole[6] == 1)) 
			)
		{
			sprintf(msg, "%s", "VYHRAL HRAC 1\n");
			printf("VYHRAL HRAC 1\n");
			send(array[0].client, msg, strlen(msg) + 1, 0);
			send(array[1].client, msg, strlen(msg) + 1, 0);
			vyhra = 1;
			shutdown(array[0].client, SD_SEND);
			closesocket(array[0].client);
			shutdown(array[1].client, SD_SEND);
			closesocket(array[1].client);
		}

		printf("Na rade je hrac 2\n");
		char polo[256];
		for (int i = 0; i < 9; i++) {
			polo[i] = hrpole[i] + '0';
			polo[i + 1] = '\0';
		}
		//printf("%s\n",polo);

		sprintf(msg, "%s", "Zadaj poziciu: ");
		send(array[1].client, msg, strlen(msg) + 1, 0);
		while (hrac2 != 0) {
			
			result = recv(array[1].client, recvBuff, 256, 0);
			if (result == SOCKET_ERROR)
			{
				array[myIdx].name = NULL;
				array[myIdx].client = INVALID_SOCKET;
				closesocket(array[1].client);
				return -1;
			}
			if (result > 0)
			{
				sscanf(recvBuff, "%d", &value);
				//printf("%s %d\n", recvBuff, value);
				if ((hrpole[value - 1] == 0) && (value != 0)) {
					hrpole[value - 1] = 2;
					hrac2 = 0;
				}
				else {
					hrac2 = -1;
					sprintf(msg, "%s", "OBSADENE POLICKO!\nZadaj poziciu: ");
					send(array[1].client, msg, strlen(msg) + 1, 0);
				}
				printf("%d %d %d\n%d %d %d\n%d %d %d\n", hrpole[0], hrpole[1], hrpole[2], hrpole[3], hrpole[4], hrpole[5], hrpole[6], hrpole[7], hrpole[8]);
			}
		}
		

		if (((hrpole[0] == 2) && (hrpole[3] == 2) && (hrpole[6] == 2)) || ((hrpole[1] == 2) && (hrpole[4] == 2) && (hrpole[7] == 2)) ||
			((hrpole[2] == 2) && (hrpole[5] == 2) && (hrpole[8] == 2)) || ((hrpole[0] == 2) && (hrpole[1] == 2) && (hrpole[2] == 2)) ||
			((hrpole[3] == 2) && (hrpole[4] == 2) && (hrpole[5] == 2)) || ((hrpole[6] == 2) && (hrpole[7] == 2) && (hrpole[8] == 2)) ||
			((hrpole[0] == 2) && (hrpole[4] == 2) && (hrpole[8] == 2)) || ((hrpole[2] == 2) && (hrpole[4] == 2) && (hrpole[6] == 2))
			) {
			sprintf(msg, "%s", "VYHRAL HRAC 2\n");
			printf("VYHRAL HRAC 2\n");
			send(array[0].client, msg, strlen(msg) + 1, 0);
			send(array[1].client, msg, strlen(msg) + 1, 0);
			vyhra = 1;
			shutdown(array[0].client, SD_SEND);
			closesocket(array[0].client);
			shutdown(array[1].client, SD_SEND);
			closesocket(array[1].client);
		}

		for (int i = 0; i < 9; i++) {
			polo[i] = hrpole[i] + '0';
			polo[i + 1] = '\0';
		}
		//printf("%s\n", polo);
	}
	}
	

}




int main()
{
	int port;
	printf("Enter TCP port: ");
	scanf("%d", &port);

	//vytvorenie servera
	SOCKET listenSocket = createListenSocket(port);
	if (listenSocket == INVALID_SOCKET)
	{
		return -1;
	}

	printf("TCP socket created\n");
	SOCKET clientSocket;
	do
	{
		char name[20];
		clientSocket = acceptClient(listenSocket);
		if (clientSocket != INVALID_SOCKET)
		{
			DWORD threadId;
			HANDLE threadHandle = CreateThread(NULL, 0, function, &clientSocket, 0, &threadId);
			len++;
		}
	} while (clientSocket != INVALID_SOCKET);
	terminator = true;
	WSACleanup();
	

	return 0;
}

