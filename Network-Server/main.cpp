#include <stdio.h>
#include <WinSock2.h> //used for open sockets on operative system.


int main()
{
	//version 2.2,
	WSADATA wsa_data; //data struct
	WSAStartup(MAKEWORD(2, 2), &wsa_data); //initialize winsock lib.

	printf("Enter port: ");
	unsigned short port;
	scanf_s("%hu", &port); //%hu = reads half unsigned (unsigned short).

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//bind up listen socket
	sockaddr_in bind_addr;
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_addr.s_addr = INADDR_ANY; //binds to any adress.
	bind_addr.sin_port = htons(port); //swap endian

	//binds socket to listen to port.
	int bind_result = bind(listen_sock, (sockaddr*)&bind_addr, sizeof(bind_addr));



	if (bind_result)
	{
		printf("Bind failed: %d", WSAGetLastError());
		return 1;
	}

	//sets listen sock to listen mode, and to allow int amount of connections.
	listen(listen_sock, 5);

	//blocking function. Blocks until a connection is established. Then returns socket.
	SOCKET client = accept(listen_sock, NULL, NULL);

	char read_buffer[1024];
	int read_len = recv(client, read_buffer, 1024, 0);
	read_buffer[read_len] = 0;

	printf("Client MSG: %s", read_buffer);

	const char* response = "CONNECTED TO SERVER!";
	send(client, response, strlen(response), 0);

	return 0;
}