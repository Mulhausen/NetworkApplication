#ifndef __CLIENTTCP_H
#define __CLIENTTCP_H

#include <stdio.h>
#include <string.h>    /* for strlen, strchr */
#include <sys/socket.h>
#include <arpa/inet.h> /* for inet_addr */
#include "globals.h"

#define DEBUG_LOG 

#define TCP_COMMUNICATION_PORT      (8020u)
#define MAX_SIZE_OF_TCP_MSG         (2048u)
 
#define DEFAULT_DEVICE_INET_ADDR            "127.0.0.1"

int32_t createSocket();
int16_t connectToDevice(int32_t sockfd, const char *ipAddr, uint16_t connectPort);
int16_t sendMsgToDevice(int32_t sockfd, char* msg);
int16_t recvMsgFromDevice(int32_t sockfd, char* msgIn, uint32_t maxSizeMsg);

char *fgetsSafe(char *inpstr,int inpsiz);
void getMsgFromKeyboard(char* msg);


#endif //__CLIENTTCP_H
