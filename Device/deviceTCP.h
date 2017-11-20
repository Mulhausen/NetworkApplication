#ifndef __DEVICETCP_H
#define __DEVICETCP_H

#include <stdio.h>
#include <string.h>      /* for strlen */
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>   /* for inet_addr */
#include <unistd.h>      /* write */
#include <pthread.h>     /* for threading , link with lpthread */
#include "globals.h"

#include "SCPI/scpi-def.h"


#define DEBUG_LOG
/* #define ECHO_MODE */
/* #define GREETINGS_FOR_CLIENT */

#define MAX_SIZE_OF_TCP_MSG         (2000u)
#define TCP_COMMUNICATION_PORT      (8020u)

#define MAX_NUM_CONNECTIONS         (5)


void *connectHandler(void *);
int32_t createSocket();
int16_t bindSocket(int32_t sockfd, uint16_t connectPort);
uint16_t acceptConnect(int32_t sockfd, struct sockaddr_in* client);
int16_t createThread(int new_socket);


#endif //__DEVICETCP_H
