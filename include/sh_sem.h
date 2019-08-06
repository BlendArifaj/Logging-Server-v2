/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  sh_sem.h
 *
 * @brief Declare the help function
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Fri May 17, 15:54:12 2019 $
 *****************************************************************************/
#define 	_DEFAULT_SOURCE 1
#define 	SEM_KEY		(key_t)0x52342
#define		SEM_FLAG	(0600|IPC_CREAT|IPC_EXCL)
#define		SEMPERM		0600
#define		TRUE		1
#define		FALSE		0
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/sem.h>
#include	<errno.h>
#include 	"sh_mem.h"
#include 	<sys/types.h>
#include 	<unistd.h>
#include 	<sys/wait.h>

typedef union _semun{
	int val;
	struct semid_ds *buf;
	ushort *array;
}semun;


typedef struct _sem_type{
	int sem_id;
	int status;
	semun arg;
}sem_type;

int create_semaphore(sem_type *const sem_obj);
int sem_wait(int sem_id);
int sem_signal(int sem_id);
void handlesems(sem_type *sem_obj,shm_elm *shm_obj);


int start_read(int semid);
int start_write_waiting(int semid);
int start_writing(int semid);

int finish_read(int semid);
int finish_write_waiting(int semid);
int finish_writing(int semid);

int conn_handler_write(shm_elm *shm_obj);
int record(shm_elm *shm_obj);
