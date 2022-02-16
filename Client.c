// Client source file
// Handles player creation and player requests

#include "Main.h"

void client()
{
	int new_socket;
	int new_connection;
	struct sockaddr_in server_address;
	struct sockaddr_in cli;

	new_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (new_socket == -1)
	{
		printf("Socket creation failed \n");
	}
	else
	{
		printf("SOcket created successfully \n");
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(PORT);

	if (connect(new_socket, (SA*)&server_address, sizeof(server_address)) != 0)
	{
		printf("Connection with server failed \n");
	}
	else
	{
		printf("Connected to server \n");
	}

	close(new_socket);
}