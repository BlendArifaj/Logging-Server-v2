/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  msg_queue.h
 *
 * @brief Declare the help function
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Fri May 17, 15:54:12 2019 $
 *****************************************************************************/
#define 	_DEFAULT_SOURCE 	1
#define 	MSQ_LEN			30
#define 	MSQ_PATH 		"/tmp/message_queue"
#define 	MSQ_KEY 		(key_t)0x652484	/* message queue's identifier	*/
#define 	MSQ_FLAG 		(0666 | IPC_CREAT)

#include 	<sys/types.h>
#include 	<sys/ipc.h>
#include 	<sys/msg.h>
#include 	<sys/sem.h>
#include 	<sys/shm.h>
#include 	<string.h>
#include 	<errno.h>
#include 	<stdio.h>
#include 	<stdlib.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/sem.h>
#include	<errno.h>
#include 	<time.h>
#include 	<mqueue.h>
#include 	"nm_pipe.h"


typedef struct _msq_elm_t
{
	int c_pid;			/* client's process id 		*/
	int msq_len;			/* length in bytes of msg 	*/
	char msg [MSQ_LEN];		/* the message, pipe's name	*/
}msq_elm_t;				/* message_queue_element;	*/

typedef struct _msq_elm
{
	int msq_id;			/* message queue's id		*/
	msq_elm_t elm;			/* message queue's element	*/
}msq_elm;				/* message_queue 		*/




int init_msq_queue(msq_elm *const msq_obj);
int send_request_msq_queue(msq_elm *const msq_obj);
int get_requst_msq_queue(msq_elm *const msq_obj,nm_pipe *const nm_obj);
int wait_first_client(msq_elm *const msq_obj,nm_pipe *const nm_obj);
int get_number_message_queue(msq_elm *const msq_obj);
int next_phase(nm_pipe *const nm_obj,char *s);
key_t generate_key_t(const char *path, int id);




