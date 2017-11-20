#include "deviceTCP.h"


uint8_t numConnections = 0u;
pthread_t threads[MAX_NUM_CONNECTIONS];
int acceptSocketsDesc[MAX_NUM_CONNECTIONS];
 
 
int main(int argc , char *argv[])
{
    int socketDesc, acceptSocketDesc;
    struct sockaddr_in client;
    numConnections = 0u;

    socketDesc = createSocket();
     
    /* Bind */
    bindSocket(socketDesc, TCP_COMMUNICATION_PORT);
     
    /* Listen to port */
    listen(socketDesc , 3);
     
    /* Accept incoming connection */
    #ifdef DEBUG_LOG
    puts("Waiting for incoming connections...");
    #endif

    while( (acceptSocketDesc = acceptConnect(socketDesc, &client)) && (numConnections < MAX_NUM_CONNECTIONS))
    {
        createThread(acceptSocketDesc);
        numConnections++;
    }
     
    if (acceptSocketDesc < 0)
    {
        perror("Accept failed");
        return(RES_COMMON_ERROR);
    }
     
    return(RES_SUCCESS);
}
 
/*
 * @brief The function to handle connection for each client
 */
void *connectHandler(void *socket_desc)
{
    /* Get the socket descriptor */
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[MAX_SIZE_OF_TCP_MSG];
    
    #ifdef GREETINGS_FOR_CLIENT
    /* Send a message to the client */
    message = "It's Mulhousen RTSA. Send your SCPI commands to control the device\n";
    write(sock , message , strlen(message));
    #endif
     
    /* Receive a message from client */
    while( (read_size = recv(sock , client_message , MAX_SIZE_OF_TCP_MSG , 0)) > 0 )
    {
        client_message[read_size] = 0;
        printf("Received a message from client: %s\n", client_message);
        puts("--------------------------------\n");
        
        /* TODO: Handle SCPI command */
        
        #ifdef ECHO_MODE
        /* Send the message back to client */
        write(sock , client_message , read_size);
        #endif
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
     
    return 0;
}

/* 
 * @brief Create TCP socket
 *
 * @return The socket file descrptor (non-negative integer)
 *         or (-1) if socket can not be created
 */
int32_t createSocket()
{
    int32_t sockfd;

    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if ((-1) == sockfd)
    {
        puts("ERROR: Could not create TCP/IP socket");
    }
    
    return(sockfd);
}

/* 
 * @brief Bind the socket from any IP address
 *
 * @return Result of bind
 *          = 0 if bind is successful
 */
int16_t bindSocket(int32_t sockfd, uint16_t connectPort)
{
    struct sockaddr_in server;

    /* Prepare the sockaddr_in structure */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( connectPort );
     
    /* Bind */
    if( bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("ERROR: Bind failed");
        return(RES_COMMON_ERROR);
    }
    
    #ifdef DEBUG_LOG
    puts("Bind done");
    #endif
    
    return(RES_SUCCESS);
}

uint16_t acceptConnect(int32_t sockfd, struct sockaddr_in* client)
{
    uint16_t accSockfd; /* accepted socket file descriptor */
    uint16_t addrlen = sizeof(struct sockaddr_in);
    
    accSockfd = accept(sockfd, (struct sockaddr *)client, (socklen_t*)&addrlen);
    
    #ifdef DEBUG_LOG
    puts("Connection accepted");
    #endif
  
    return(accSockfd);
}

int16_t createThread(int socketfd)
{
    acceptSocketsDesc[numConnections] = socketfd;
         
    /* Assign handler function with socketfd as an argument */
    if( pthread_create( &threads[numConnections], NULL, connectHandler, (void*)(&acceptSocketsDesc[numConnections]) ) < 0)
    {
        perror("could not create thread");
        return(RES_COMMON_ERROR);
    }
         
    #ifdef DEBUG_LOG
    puts("Handler assigned\n");
    #endif
    
    //pthread_exit(NULL);
    //pthread_join(thread, NULL);
    
    return(RES_SUCCESS);
}
