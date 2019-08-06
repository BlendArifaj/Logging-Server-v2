/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  client.h
 *
 * @brief Declare the method and other needed library for client 
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Fri May 17, 15:54:12 2019 $
 *****************************************************************************/
#define 	_DEFAULT_SOURCE 	1

#include 	<sys/types.h>
#include 	<unistd.h>
#include 	<stdio.h>
#include 	<limits.h>
#include	"socket.h"


static char *itoa_simple_helper(char *dest, int i);
char *itoa_simple(char *dest, int i);
int str_read (FILE *fp, /*@null@*/ char **dst);
void client_help();
void client_error_call();
void file_not_exists();
int is_right_call(int argc, char **argv);

/* to do */
int file_exist(char *filename);

