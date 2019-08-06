/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  socket.h
 *
 * @brief Declare the help function
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Fri Jun 26, 18:54:10 2019 $
 *****************************************************************************/
#define 	_DEFAULT_SOURCE 	1	
#define		SOCK_PORT		8080
#define		MSQ_LEN			30
#define 	SIZE 			sizeof(struct sockaddr_in)
#include <netinet/in.h> 
#include <string.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include "nm_pipe.h"

typedef struct _socket_t{
	int socket_fd;
	int msq_len;
	char msg [MSQ_LEN];
}socket_t;

int create_socket();
int client_connect_socket(socket_t *const sock_obj);
int init_socket(socket_t *const sock_obj);
int send_message_socket(socket_t *const sock_obj);
int accept_request_socket(socket_t *const sock_obj);
int handle_request_socket(socket_t *const sock_obj,nm_pipe *const nm_obj);


