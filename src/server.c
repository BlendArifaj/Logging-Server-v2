/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  server.c
 *
 * @brief Implement the server form
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Sat Jun 29, 08:10:04 2019 $
 *****************************************************************************/
#include "server.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#define SIZE sizeof(struct sockaddr_in)
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
static int finish = 1;
int main(int argc, char **argv){
	/* check if is the right call*/
	if(is_right_call(argc,argv) == -1){
		return -1;
	}
	/*Create semaphore and initialize it*/
	sem_type *sem_obj = (sem_type *)malloc(sizeof(sem_type));
	if(create_semaphore(sem_obj) < 0){
		exit(1);
	}
	finish_read(sem_obj->sem_id);
	finish_write_waiting(sem_obj->sem_id);
	finish_writing(sem_obj->sem_id);

	/*Create shared memory and initialize it*/
	shm_elm  *shm_obj = (shm_elm  *)malloc(sizeof(shm_elm ));
	shm_obj->elm.state = SHM_EMPTY;
	shm_obj->file_name= argv[2];
	if(create_shared_memory(shm_obj)<0){
		printf("Failed to create shared memory \n");
		return -1;
	}
	/*Create dynamic array where we are going to store our named pipes*/
	int index = 0;
     	int size = 0;
     	int capacity = INITIAL_CAPACITY;
        int* arr = malloc(INITIAL_CAPACITY * sizeof(int));
	int i;
	fd_set fds;
	fd_set socket_handler;
	int maxfd;
	
	/*Create message queue*/
	/*
	msq_elm *msq_obj = (msq_elm *)malloc(sizeof(msq_elm));
	if(init_msq_queue(msq_obj)<0){
		printf("Error while creating message queue\n");
	}
	*/
	socket_t *sock_obj = (socket_t *)malloc(sizeof(socket_t));
	if(init_socket(sock_obj) == -1){
		printf("Can't create socket\n");
		free(sock_obj);
		return -1;
	}
	printf("Socket u krijua\n");
	FD_ZERO(&socket_handler);
	FD_SET(sock_obj->socket_fd,&socket_handler);
	/*Create timeval, time to check for any new message in named pipe*/
	struct timeval tv;
	tv.tv_sec = 1;
       	tv.tv_usec = 0;

	/*terminate all programs in case of press CTRL+C*/
	terminate_program();

	/*Listen if there is any new request on message queue or in named pipes*/
	for(;;){
		if(finish==0){
			terminate_all_ipcs();
			free(shm_obj);
			free(sem_obj);
			close(sock_obj->socket_fd);
			free(sock_obj);
			/*free(msq_obj);*/
			free(arr);
			return 1;
		}
		FD_ZERO(&fds);
		for(i = 0; i<size;i++){
			FD_SET(arr[i],&fds);
		}
		/*If there is any new message in message queue*/
		/*
		if(get_number_message_queue(msq_obj)>0){
			nm_pipe *nm_obj = (nm_pipe *)malloc(sizeof(nm_pipe));
			if(get_requst_msq_queue(msq_obj,nm_obj)>-1){
				push(arr, &index, nm_obj->nmp_id, &size, &capacity);
				FD_SET(nm_obj->nmp_id,&fds);
			}
			free(nm_obj);
		}
		*/
		FD_ZERO(&socket_handler);
		FD_SET(sock_obj->socket_fd,&socket_handler);
		select(4,&socket_handler,NULL,NULL, &tv);
		if(FD_ISSET(sock_obj->socket_fd, &socket_handler)){
			nm_pipe *nm_obj = (nm_pipe *)malloc(sizeof(nm_pipe));
			if(handle_request_socket(sock_obj,nm_obj)>-1){
				push(arr, &index, nm_obj->nmp_id, &size, &capacity);
				FD_SET(nm_obj->nmp_id,&fds);
			}
			free(nm_obj);
		}
		FD_ZERO(&fds);
		for(i = 0; i<size;i++){
			FD_SET(arr[i],&fds);
		}
		maxfd = return_max_value(arr,size);
		/*if there is any new message on named pipe*/
		select(maxfd+1,&fds,NULL,NULL,&tv);

		/*handle that requst*/
		for(i = 0; i<size;i++){
			if(FD_ISSET(arr[i],&fds)){
				nm_pipe *nm_obj = (nm_pipe *)malloc(sizeof(nm_pipe));
				nm_obj->nmp_id = arr[i];
				if(nmp_recv(nm_obj) < 0){
					printf("Error\n");
					return -1;
				}
				strcpy(shm_obj->elm.msg,nm_obj->elm.msg);

				handlesems(sem_obj,shm_obj);
				free(nm_obj);
			}
		}

	}

	return 1;

}


/***************************************************************************//** 
 * @brief Public member - main process
 *
 * Handle main proccess, where request from message queue is handled and 
 * named pipe is created. Than is called another process.
 *
 * @param[in,out]: msq_obj, nm_obj, shm_obj
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
/*
int main_process(msq_elm *const msq_obj,nm_pipe *const nm_obj,shm_elm *const shm_obj){
	while(get_requst_msq_queue(msq_obj,nm_obj)>-1){
		if(conn_handler(nm_obj,shm_obj) < 0){
			return -1;
		}
	}
	return 1;
}*/

/***************************************************************************//** 
 * @brief Public member - Connection Handler
 *
 * Read from named pipe than write on shared memory.
 *
 * @param[in,out]: msq_obj, nm_obj, shm_obj
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int conn_handler(nm_pipe *const nm_obj,shm_elm *const shm_obj){
	if(nmp_recv(nm_obj) < 0){
		printf("Error\n");
		return -1;
	}
	sem_type *sem_obj = (sem_type *)malloc(sizeof(sem_type));
	printf("THE MESSAGE IS: %s\n",nm_obj->elm.msg);
	strcpy(shm_obj->elm.msg,nm_obj->elm.msg);

	handlesems(sem_obj,shm_obj);
	strcpy(nm_obj->elm.msg,"");
	close(nm_obj->nmp_id);
	free(sem_obj);
	
	return 1;
}


/***************************************************************************//** 
 * @brief Push function
 *
 * Read from named pipe than write on shared memory.
 *
 * @param[in,out]: msq_obj, nm_obj, shm_obj
 *
 * @retval: void
 ******************************************************************************/
void push(int *arr, int *index, int value, int *size, int *capacity){
	if(*size > *capacity){
		if(realloc(arr, sizeof(arr) * 2)==NULL){
			printf("Error while allocating memory\n");
			exit(1);
		}
    		*capacity = sizeof(arr) * 2;
    	}
     arr[*index] = value;
     *size = *size + 1;
     *index = *index + 1;
}

/***************************************************************************//** 
 * @brief Public member - Get maximum value from array
 *
 * Return max value from array
 *
 * @param[in]: arr, size
 *
 * @retval -1 : on error
 * @retval  max : on success
 ******************************************************************************/
int return_max_value(int* arr, int size){
	int i;	
	int tmp = arr[0];
	for(i = 0; i<size;i++){
		if(tmp < arr[i]){
			tmp = arr[i];		
		}
	}
	return tmp;
}

/***************************************************************************//** 
 * @brief Public member - Server helper
 *
 * Print the Synopsis for server
 *
 * @param: none
 *
 * @retval: void - no return value
 ******************************************************************************/
void server_help(){
	printf("Server - The Server tool\n\n");
	printf("SYNOPSIS\n");
	printf("\tserver [OPTIONS] ...\n");
	printf("\t-f arg, (mandatory) the file to record message\n");

}


/***************************************************************************//** 
 * @brief Server error call
 *
 * Print an error for server's user
 *
 * @param: none
 *
 * @retval: void, no return value;
 ******************************************************************************/
void server_error_call(){
	printf("\nError on calling Server program.");
	printf("\nFor any help write -h\n");
}


/***************************************************************************//** 
 * @brief Public member - Is right call
 *
 * Check if user has call server in the right way. If so than can continue execution
 * of the program, else the program will show an error and will finished
 *
 * @param[in]: argc, argv;
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int is_right_call(int argc, char **argv){
	if(argc == 1){
		server_error_call();
		return -1;
	}
	if( strcmp(argv[1],"-h") == 0){
		server_help();
		return -1;
	}
	if( strcmp(argv[1],"-f") != 0){
		server_error_call();
		return -1;
	}else{
		if(argc == 2){
			server_error_call();
			return -1;
		}
	}
	return 1;
}
/************************************************************************
 * @brief Terminate program using CTRL+C
 *
 * After the user press CTRL+C the program will print a text that has been terminated
 * 
 * @param[in,out] - none
 * @param[in] - singo
 *
 * @void program, so there will be no return value
 *************************************************************************/
void terminate_program(){
	struct sigaction signal_handler;
	void signal_handler_int(int);
	signal_handler.sa_handler = signal_handler_int;
	sigfillset(&(signal_handler.sa_mask));
	sigaction(SIGINT, &signal_handler, NULL);

}

/************************************************************************
 * @brief What to do when press CTRL+C
 *
 * This function tells the program what to do when the user press CTRL+C
 * 
 * @param[in,out] - none
 * @param[in] - signo
 *
 * @void program, so there will be no return value
 ************************************************************************/
void signal_handler_int(int signo){
	finish = 0;
	printf("Program has been successfully terminated\n");

}

/************************************************************************
 * @brief Terminate all IPCS
 *
 * In this function we are going to remove Message queue, shared memory 
 * and semaphores from our system.
 * 
 * @param: none
 *
 * @void program, so there will be no return value
 ************************************************************************/
void terminate_all_ipcs(){
	/*Terminate message queue*/
	terminate_message_queue();
	
	/*Terminate shared memory*/
	terminate_shared_memory();

	/*Terminate shared semaphore*/
	terminate_shared_semaphore();
	
}


/************************************************************************
 * @brief Terminate message queue
 *
 * In this function we are going to remove Message queue from our system.
 * 
 * @param: none
 *
 * @void program, so there will be no return value
 ************************************************************************/
void terminate_message_queue(){
	/*Terminate message queue*/
	/*
	msq_elm *msq_obj = (msq_elm *)malloc(sizeof(msq_elm));
	if(init_msq_queue(msq_obj)<0){
		printf("Error while creating message queue\n");
	}
	if(msgctl(msq_obj->msq_id,IPC_RMID,0) < -1){
		printf("Error while removing message queue \n");
	}
	free(msq_obj);*/
}

/************************************************************************
 * @brief Terminate shared memory
 *
 * In this function we are going to remove shared memory from our system.
 * 
 * @param: none
 *
 * @void program, so there will be no return value
 ************************************************************************/
void terminate_shared_memory(){
	/*Terminate message queue*/
	shm_elm  *shm_obj = (shm_elm  *)malloc(sizeof(shm_elm ));

	if(create_shared_memory(shm_obj)<0){
		printf("Failed to getting shared memory \n");
	}

	if(shmctl(shm_obj->shm_id,IPC_RMID,0) < -1){
		printf("Error while removing message queue \n");
	}
	free(shm_obj);
}


/************************************************************************
 * @brief Terminate shared semaphore
 *
 * In this function we are going to remove semaphores from our system.
 * 
 * @param: none
 *
 * @void program, so there will be no return value
 ************************************************************************/
void terminate_shared_semaphore(){
	/*Terminate message queue*/
	sem_type *sem_obj = (sem_type *)malloc(sizeof(sem_type));
	if(create_semaphore(sem_obj) < 0){
		printf("Error while getting semaphore\n");
	}
	if(semctl(sem_obj->sem_id,IPC_RMID,0) < -1){
		printf("Error while removing message queue \n");
	}
	free(sem_obj);
}
