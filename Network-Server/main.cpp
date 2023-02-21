#include <stdio.h>
#include <WinSock2.h> //used for open sockets on operative system.
#include <Windows.h>



DWORD client_loop(void* ptr)
{
	SOCKET client = reinterpret_cast<SOCKET>(ptr);
	while (true)
	{
		char buffer[1024];
		int recv_size = recv(client, buffer, 1024, 0);

		if (recv_size == -1)
		{
			printf("Client Disconnected!");
			return 0;
		}

		printf("%.*s\n", recv_size, buffer);
	}
	return 0;
}

int main()
{
	
	

	//version 2.2,
	WSADATA wsa_data; //data struct
	WSAStartup(MAKEWORD(2, 2), &wsa_data); //initialize winsock lib.

	//printf("Enter port: ");
	unsigned short port = 49;
	//scanf_s("%hu", &port); //%hu = reads half unsigned (unsigned short).

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

	
	

	while (true)
	{
		SOCKET client = accept(listen_sock, NULL, NULL); //blocking function. Blocks until a connection is established.
		CreateThread(
			nullptr,
			0,
			client_loop,
			(void*)client, //casts the value to a void ptr to avoid client value going out of scope before thread is created.
			0,
			nullptr
		);
	}
}