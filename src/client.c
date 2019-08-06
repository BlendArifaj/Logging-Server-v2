/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  client.c
 *
 * @brief Implement the client form
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Sat Jun 29, 08:09:12 2019 $
 *****************************************************************************/
#include "client.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 

int main(int argc, char **argv){
	/*if the call is right*/
	if(is_right_call(argc,argv) == -1){
		return -1;
	}
	FILE *file;
	if( (file = fopen(argv[2],"r+")) == NULL){
		printf("Error while opening file.\n Try another file.\n");
		return -1;
	}

	int wait_time;
	if(argc == 3){
		wait_time = 5;
	}else{
		wait_time =atoi(argv[4]);
		if(wait_time < 4){
			wait_time = 4;
		}
	}
	/* get process id*/
	pid_t pid = getpid();
	
	/* create name for named pipe*/
	char s[100];
	itoa_simple(s, pid);
	char nmp_name[50];
	strcpy(nmp_name,"/tmp/p");
	strcat(nmp_name,s);
	

	/*get message queue*/
	/*
	msq_elm *msq_obj = (msq_elm *)malloc(sizeof(msq_elm));
	strcpy(msq_obj->elm.msg,nmp_name);
	msq_obj->elm.msq_len = 12;
	msq_obj->elm.c_pid = pid;
	*/
	/*send request on message queue*/
	/*
	if(send_request_msq_queue(msq_obj) < 0){
		perror("Gabim:");

		return -1;
	}
	*/
	socket_t *sock_obj = (socket_t *)malloc(sizeof(socket_t));
	if( client_connect_socket(sock_obj) == -1){
		printf("Client can't connect to the server\n");
		free(sock_obj);
		return -1;
	}
	strcpy(sock_obj->msg,nmp_name);
	if(send_message_socket(sock_obj) == -1){
		printf("Request can't sent to the server\n");
		free(sock_obj);
		return -1;
	}

	/*create named pipe*/
	nm_pipe *nm_obj = (nm_pipe *)malloc(sizeof(nm_pipe));
	nm_obj->elm.len = 20;
	if(create_named_pipe(nm_obj,nmp_name) < 0){
		printf("Error while creating first pipe\n");
		return -1;
	}

	/*Send message on named pipe*/

	nm_obj->elm.len = 511;
	while(nm_obj->elm.len == 511){
		char *tmp;
		nm_obj->elm.len = str_read(file,&tmp);
		strcpy(nm_obj->elm.msg,tmp);
		if(nmp_send(nm_obj)<0){
			return -1;
		}
		free(tmp);
		sleep(wait_time);
	}
	/*free(msq_obj);*/
	free(sock_obj);
	free(nm_obj);

	fclose(file);

	return 1;

}

/***************************************************************************//** 
 * @brief Public member - itoa simple helper
 *
 * Helps function iota simple to get the number from string
 *
 * @param[in]: i
 * @param[in,out]: dest
 *
 * @retval -1 : on error
 * @retval  dest : on success
 ******************************************************************************/
static char *itoa_simple_helper(char *dest, int i) {
  if (i <= -10) {
    dest = itoa_simple_helper(dest, i/10);
  }
  *dest++ = '0' - i%10;
  return dest;
}

/***************************************************************************//** 
 * @brief Public member - itoa - get a number from string
 *
 * This functions can get a number from strig input
 *
 * @param[in]: i
 * @param[in,out]: dest
 *
 * @retval -1 : on error
 * @retval  dest : on success
 ******************************************************************************/
char *itoa_simple(char *dest, int i) {
  char *s = dest;
  if (i < 0) {
    *s++ = '-';
  } else {
    i = -i;
  }
  *itoa_simple_helper(s, i) = '\0';
  return dest;
}  

/***************************************************************************//** 
 * @brief String read
 *
 * This function read a text from file
 *
 * @param[in,out]: fp, dst
 *
 * @retval -1 : on error
 * @retval  status : on success
 ******************************************************************************/
int str_read (FILE *fp, /*@null@*/ char **dst)
{
  int   s_len  = 511;
  int   status = 0;
  char *ptr    = NULL;

  ptr = (char *) malloc ((size_t) s_len + 1);
  
  if (ptr == NULL)
    {
      printf ("\nError allocating memory");
      return -1;
    }
  
  status = fread ( (void *) ptr, (size_t) 1,
                   (size_t) s_len, fp);

  ptr [s_len] = '\0';
  
  *dst = ptr;

  return status;
}

/***************************************************************************//** 
 * @brief Client error call
 *
 * This functions print an error message for user
 *
 * @param: none
 *
 * @retval: none
 ******************************************************************************/
void client_error_call(){
	printf("\nError on calling Client program.");
	printf("\nFor any help write -h\n");
}


/***************************************************************************//** 
 * @brief client help
 *
 * This functions print Synopsis for client
 *
 * @param: none
 *
 * @retval: none
 ******************************************************************************/
void client_help(){
	printf("Client - The client tool\n\n");
	printf("SYNOPSIS\n");
	printf("\tclient [OPTIONS] ...\n");
	printf("\t-f arg, (mandatory) the file wich contains messages\n");
	printf("\t-t arg, (optinal) the number of seconds to wait\n");
}

/***************************************************************************//** 
 * @brief file not exist
 *
 * This functions print a message
 *
 *
 * @retval: none - void
 ******************************************************************************/
void file_not_exists(){
	printf("\tError while opening file. File not exists on system!\n");
}

/***************************************************************************//** 
 * @brief Check if client has made right call
 *
 * This functions initializes a named pipe
 *
 * @param[in,out]: fp
 * @param[in,out]: dst
 *
 * @retval: none - void function
 ******************************************************************************/
int is_right_call(int argc, char **argv){
	if(argc == 1){
		client_error_call();
		return -1;
	}

	if(strcmp(argv[1],"-h") == 0){
		client_help();
		return -1;
	}

	if(strcmp(argv[1],"-f") != 0){
		client_error_call();
		return -1;
	}else{
		if(argc == 2){
			client_error_call();
			return -1;
		}
	}
	if(argc == 5){
		if(strcmp(argv[3],"-t") != 0){
			client_error_call();
			return -1;
		}else{
			if(argc == 4){
				client_error_call();
				return -1;
			}
		}
	}
	


	return 1;
}
