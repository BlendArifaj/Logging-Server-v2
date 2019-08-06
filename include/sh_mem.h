/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  shm_mem.h
 *
 * @brief Shared memory
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Fri May 17, 15:54:12 2019 $
 *****************************************************************************/
#define _DEFAULT_SOURCE 1

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define		SHM_KEY			(key_t) 0x10

/* Define the shared memory message length in bytes */
#define		SHM_MSG_LEN		(int) 512

/* Define shared memory flags */
#define		SHM_FLAGS		(0666 | IPC_CREAT | IPC_EXCL)
/* Define shared memory states */
#define		SHM_EMPTY		(char) 0x01
#define		SHM_FULL		(char) 0x02

/* Define mode to open file */
#define 	OPEN_FILE_MODE 		"r+"
#define		CREATE_FILE_MODE 	"w"

/* Define shared memory structure */
typedef struct _shm_elm_t{
	char state; 		/* State of shared memory element	*/
	char len;		/* length of shared memory element 	*/
	char msg[SHM_MSG_LEN];	/* shared memory message		*/
} shm_elm_t;

/*Define shared memory element, that will handle shmat and also shared memory structure */
typedef struct _shm_elm{
	int		shm_id;	/* shared memory Id 			*/
	char*		shm_att;
	char*		file_name;
	shm_elm_t	elm;	/* element of shared memory structure	*/
}shm_elm;


int create_shared_memory	(shm_elm *const shm_obj);
int destroy_shared_memory	(shm_elm *const shm_obj);
int attach_shared_memory	(shm_elm *const shm_obj);
int detach_shared_memory	(shm_elm *const shm_obj);
int read_shared_memory		(shm_elm *const shm_obj);
int write_shared_memory		(shm_elm *const shm_obj);
int append_text_in_file		(shm_elm *const shm_obj);
int create_file			(const char * file_name);
int str_len 			(const char *str);
int str_write 			(FILE *fp, const char *str);
int str_read 			(FILE *fp, /*@null@*/ char **dst);



