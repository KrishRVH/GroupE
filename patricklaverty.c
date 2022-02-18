// Author: Patrick Laverty
// - Server communication
// - Based on code from geeksforgeeks.com

#include "Main.h"

void server() {
	int new_socket;
	int new_connection;
	int len;

	struct sockaddr_in server_address;
	struct sockaddr_in cli;

	// Creating Socket
	new_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (new_socket == -1) { printf("Socket creation failed\n"); }
	else { printf("Socket created.\n"); }

	bzero(&server_address, sizeof(server_address)); // Zero out the address space of the struct


	// Binding socket to the port
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // Convert int to ip byte order
	server_address.sin_port = htons(BINDING_PORT); // Converting ip port to byte order

	int binder = bind(new_socket, (SA*)&server_address, sizeof(server_address));
	if binder != 0) { printf("Socket bind failed\n"); }
	else { printf("Socket binded successfully\n"); }

	// Socket listener
	int listener = listen(new_socket, 5);
	if (listener != 0) { printf("Listen failed\n"); }
	else { printf("Server listening\n"); }

	// Testing connection
	len = sizeof(cli);
	new_connection = accept(new_socket, (SA*)&cli, &len);
	if (new_connection < 0) { printf("Server accept failed\n"); }
	else { printf("Server accepted the client\n"); }

	//close(new_socket);
}