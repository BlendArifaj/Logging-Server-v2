/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  msg_queue.c
 *
 * @brief Implement the message queue
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Sat Jun 29, 08:10:54 2019 $
 *****************************************************************************/
#include 	"socket.h"

/***************************************************************************//** 
 * @brief Init socket
 *
 * Create socket for communication
 *
 * @param[in]: arr, size
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int init_socket(socket_t *const sock_obj){
	sock_obj->socket_fd = create_socket();
	if(sock_obj->socket_fd == -1){
		return -1;
	}
	return 1;
}

/***************************************************************************//** 
 * @brief Create socket
 *
 * Create socket for communication
 *
 * @param[in]: arr, size
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int create_socket(){
    struct sockaddr_in socket_address; 
    socket_address.sin_family = AF_INET; 
    socket_address.sin_addr.s_addr = INADDR_ANY; 
    socket_address.sin_port = htons(SOCK_PORT); 
    int server_fd; 
    int opt = 1; 
   
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    if (bind(server_fd, (struct sockaddr *)&socket_address,sizeof(socket_address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
  
	return server_fd;

}


/***************************************************************************//** 
 * @brief Send Message on socket
 *
 * Send Message on socket
 *
 * @param[in]: arr, size
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int send_message_socket(socket_t *const sock_obj){
    	if( send(sock_obj->socket_fd, sock_obj->msg, strlen(sock_obj->msg), 0) <= 0){
		printf("Message failed\n");
		return -1;
	}
    	return 1;
}

/***************************************************************************//** 
 * @brief handle_request_socket
 *
 * Handle with socket req
 *
 * @param[in]: socket
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int handle_request_socket(socket_t *const sock_obj,nm_pipe *const nm_obj){
    	int addrlen = sizeof(struct sockaddr_in); 
    	char nm_pipe_name[30] = {0};
	int new_socket_handle;
	int read_socket;
    	struct sockaddr_in socket_address; 
    	socket_address.sin_family = AF_INET; 
    	socket_address.sin_addr.s_addr = INADDR_ANY; 
    	socket_address.sin_port = htons(SOCK_PORT); 
	if ((new_socket_handle = accept(sock_obj->socket_fd, (struct sockaddr *)&socket_address,(socklen_t*)&addrlen))<0){ 
        	perror("accept"); 
        	return -1;
    	} 
    	read_socket = read( new_socket_handle , nm_pipe_name, 30); 
	if(read_socket == -1){
		perror("read");
		return -1;
	}
	if(create_named_pipe(nm_obj,nm_pipe_name) < 0){
		return -1;
	}
	return 1;
}


/***************************************************************************//** 
 * @brief Create Socket
 * 
 * Create socket
 *
 * @param[in]: i
 * @param[in,out]: dest
 *
 * @retval -1 : on error
 * @retval  sockfd : on success
 ******************************************************************************/
int client_connect_socket(socket_t *const sock_obj){ 
    struct sockaddr_in server_address; 
    if ((sock_obj->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(SOCK_PORT); 
    
    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0){ 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock_obj->socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){ 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    return 1;
}

