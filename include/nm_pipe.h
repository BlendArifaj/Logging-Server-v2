/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  help.h
 *
 * @brief Declare the help function
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Fri May 17, 15:54:12 2019 $
 *****************************************************************************/
#define 	_DEFAULT_SOURCE 		1
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define 	NAMED_PIPE_MSG_LEN 		512
#define 	SERVER_NAMED_PIPE_MODE 		O_RDWR
#define 	CLIENT_NAMED_PIPE_MODE 		O_WRONLY
#define 	NAMED_PIPE_PERM			0666
#define 	NMP_MSG_LEN       		512
#define 	NMP_PERM         		0666
#define 	NMP_ELM_LEN      		(int)(sizeof (nmp_elm_t))

typedef struct _nmp_elm_t
{
  int   len;
  char  msg [NMP_MSG_LEN];
} nmp_elm_t;


typedef struct _nm_pipe
{
  int       nmp_id;
  nmp_elm_t elm;
  
} nm_pipe;

int  create_named_pipe  (nm_pipe *const nmp_obj, const char *nmp_name);
void destroy_named_pipe (nm_pipe *const nmp_obj); 
int  nmp_send  		(nm_pipe *const nmp_obj);
int  nmp_recv  		(nm_pipe *const nmp_obj);
int  open_nm_pipe	(nm_pipe *const nm_obj);
