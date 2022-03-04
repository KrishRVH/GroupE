//Author: Drew Nguyen
//client handling

#include "Main.h"

void minusPlayerScore(struct Player new_player, int num)
{
    new_player.score += num;
}

void addPlayerScore(struct Player new_player)
{
    size_t length = nnew;
    if (length == 3 || length == 4)
    {
        new_player.score += 1;
    }
    if (length == 5)
    {
        new_player.score += 2;
    }
    if (length == 6)
    {
        new_player.score += 3;
    }
    if (length == 7)
    {
        new_player.score += 4;
    }
    if (length >= 8)
    {
        new_player.score += 11;
    }
}

void addComputerScore(struct Computer new_computer)
{
    size_t length = nnew;
    if (length == 3 || length == 4)
    {
        new_computer.score += 1;
    }
    if (length == 5)
    {
        new_computer.score += 2;
    }
    if (length == 6)
    {
        new_computer.score += 3;
    }
    if (length == 7)
    {
        new_computer.score += 4;
    }
    if (length >= 8)
    {
        new_computer.score += 11;
    }
}

void clientt()
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

	//close(new_socket);
}

//function for communication between client and server
void /*varnameTBD*/(int new_socket)
{
    char buff[MAX];
    int n;
    
    //loop for chat
    for(){
        //bzero();
        //accept message from client and copy to buffer
        printf("Enter input: ");
        //print buffer that contains the contents
     
        n = 0;   
        //copy server message from the buffer
        while((buff[n++] = getchar()) != '\n')
        //send buffer to client
        //write(new_socket, buff, sizeof(buff));
        //bzero(buff, sizeof(buff));


        //in-game logic conditionals for exit
        if(){
            printf("");
            break;
        }

    }

}

//socket creation and verification from client side

void clientToServer()
{

    int new_socket;
    int new_connection;
    struct sockaddr_in serverAddress;
    struct sockaddr_in cli;

    new_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(new_socket == 1){
        printf("socket creation failed");
    }
    else{
        printf("Socket creation successful");

    }
    bzero();

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr();
    serverAddress.sin_port = htons(PORT);

    //client socket to server socket
    if(connect(new_socket, (SA*)&serverAddress, sizeof(serverAddress)) != 0){
        printf();

    }
    else{
        printf();
    }

    //function for client-server communication
    /*varnameTBD*/(new_socket);
    close(new_socket);
}

void recieveMsg(int new_socket, char * msg)
{
    memset(/*tbd*/);
    int n = read(new_socket, msg, 3);
    //server conditionals for  reading message from server socket
    error("");
}


//function for update from server
void receiveUpdate(int new_socket, char /*var name of game state*/)
{
    int player_id = receiveInt(new_socket);
    int /*Player state of board*/(new_socket);
    //update game state
}

//function for receiving player struct
void recievePlayerMsg(){


}


//recieve message for game instruction
void recvGameMsg(){
    
}




//function for error message
void error(){
    //unfin
    exit(0);
}


//function for writing from client to server
void sendToServer(int newsocket, int msg){
    //update state
    int n = write(new_socket, &msg, sizeof(int));
    if(n < 0){
        error()
    }
    printf(/*print statement to server*/)

}




