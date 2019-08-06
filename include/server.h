/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  server.h
 *
 * @brief Declare the help function
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Fri May 17, 15:54:12 2019 $
 *****************************************************************************/
#define 	_DEFAULT_SOURCE 	1	
#define		INITIAL_CAPACITY 	2
#define 	_POSIX_SOURCE
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<pthread.h>
#include 	<mqueue.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<unistd.h>
#include	<signal.h>
#include 	"sh_sem.h"
#include	"socket.h"
/*int  conn_handler	(nm_pipe *const nm_obj,shm_elm *const shm_obj);
int  main_process	(msq_elm *const msq_obj,nm_pipe *const nm_obj,shm_elm *const shm_obj);*/
void push		(int *arr, int *index, int value, int *size, int *capacity);
int  return_max_value	(int* arr, int size);
void server_help();
void server_error_call();
int is_right_call(int argc, char **argv);

int file_exist(char *filename);

void terminate_program();
void signal_handler_int(int signo);
void terminate_all_ipcs();

void terminate_message_queue();
void terminate_shared_memory();
void terminate_shared_semaphore();

