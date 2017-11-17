#include "clientTCP.h"


int main(int argc , char *argv[])
{
    int32_t socketDesc;

    char msgToSend[MAX_SIZE_OF_TCP_MSG] = {0};
    char msgReply[MAX_SIZE_OF_TCP_MSG] = {0};
     
    socketDesc = createSocket();
         
    if( connectToDevice(socketDesc, DEVICE_INET_ADDR, TCP_COMMUNICATION_PORT) )
    {
        printf("Client can't be connected to the device. The program will be closed\n");
        return(-1);
    }

    /* 
     * TODO while(1)
     * waiting for data for send and listening for input network 
     * messages from the device
     */

    /* Communicating with the device with commands entered from the keyboard */
    while(1)
    {
        printf("Enter command : ");
        fgetsSafe(msgToSend, MAX_SIZE_OF_TCP_MSG);

        /* Send user command */
        sendMsgToDevice(socketDesc, msgToSend); 
         
        /* Waiting for reply from the device */
        recvMsgFromDevice(socketDesc, msgReply, MAX_SIZE_OF_TCP_MSG);
        
        printf("Reply from the device: %s \n", msgReply); 
        puts("------------------------------\n");
    }
    
    return 0;
}


/*
 * @brief This function serves for using fgets multiple times to write string
 * from stdin stream into the same char pointer
 *
 * It gets a string from stdin, search for the end of line ('\n') and replace 
 * it with 0 to mark the end of the read string
 *
 * @params inpstr - destination pointer where stream have to be stored,
 * @params inpsiz - allocated size of inpstr buffer, a readable buffer should 
 *                  have a smaller size, otherwise error will be return
 */
char *fgetsSafe(char *inpstr, int inpsiz) 
{
    char *seachr;

    if ( NULL == inpstr ) 
    {
        return(NULL);
    }
    
    if ( NULL == fgets(inpstr,inpsiz,stdin) ) 
    {
        return(NULL);
    }
    
    seachr = strchr(inpstr,'\n');
    if ( NULL != seachr ) 
    {
        *seachr = 0;
    }

    return(inpstr);
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
 * @brief Connect to the device with a specified IP address
 *
 * @return Result of connection
 *          = 0 if connection is successful
 */
int16_t connectToDevice(int32_t sockfd, const char *ipAddr, uint16_t connectPort)
{
    struct sockaddr_in server;

    server.sin_addr.s_addr = inet_addr( ipAddr );
    server.sin_family = AF_INET;
    server.sin_port = htons( connectPort );
 
    /* Connect to device */
    if (connect(sockfd , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("ERROR: Connection to device error");
        return(RES_COMMON_ERROR);
    }
    
    #ifdef DEBUG_LOG
    puts("Connected\n");
    #endif
    
    return(RES_SUCCESS);
}

/* 
 * Send a message to the device 
 */
int16_t sendMsgToDevice(int32_t sockfd, char* msg)
{
    if( send(sockfd, msg, strlen(msg), 0) < 0)
    {
        puts("ERROR: Send failed");
        return(RES_COMMON_ERROR);
    }
    
    return(RES_SUCCESS);
}


/* 
 * Receive a reply from the device 
 */
int16_t recvMsgFromDevice(int32_t sockfd, char* msgIn, uint32_t maxSizeMsg)
{
    int16_t read_size;

    if( (read_size = recv(sockfd, msgIn, maxSizeMsg , 0)) < 0)
    {
        puts("ERROR: Reply receiving failed");
        return(RES_COMMON_ERROR);
    }
    
    msgIn[read_size] = 0;
    
    return(RES_SUCCESS);
}
