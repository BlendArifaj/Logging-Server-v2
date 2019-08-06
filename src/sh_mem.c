/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  sh_mem.c
 *
 * @brief Implement the server form
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Sat Jun 29, 08:10:54 2019 $
 *****************************************************************************/

#include "sh_mem.h"

/***************************************************************************//** 
 * @brief Public member - initializes a named pipe
 *
 * This functions initializes a named pipe
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int create_shared_memory(shm_elm *const shm_obj){
	if( (shm_obj->shm_id = shmget(SHM_KEY, sizeof(shm_obj->elm.msg), SHM_FLAGS)) == -1){
		if(errno == EEXIST)
			shm_obj->shm_id = shmget(SHM_KEY, sizeof(shm_obj->elm.msg), 0);
	}
	if(create_file(shm_obj->file_name) < 0){
		return -1;
	}
	return 1;
}

/***************************************************************************//** 
 * @brief Public member - initializes a named pipe
 *
 * This functions initializes a named pipe
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int write_shared_memory(shm_elm *const shm_obj){
	char *temp;
	int i;
	shm_obj->shm_att = shmat(shm_obj->shm_id, NULL, 0);
	if(shm_obj->shm_att == (char *)-1){
		return -1;
	}
	temp = shm_obj->shm_att;
	i = 0;
	while(shm_obj->elm.msg[i] != 0){
		*temp++ = shm_obj->elm.msg[i];
		i = i + 1;
	}
	*temp = '\0';
	
	shm_obj->elm.state = SHM_FULL;
	shmdt(shm_obj->shm_att);
	return 1;
}

/***************************************************************************//** 
 * @brief Public member - initializes a named pipe
 *
 * This functions initializes a named pipe
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int read_shared_memory(shm_elm *const shm_obj){
	shm_obj->shm_att = shmat(shm_obj->shm_id, NULL, 0);
	if(shm_obj->shm_att == (char *)-1){
		return -1;
	}
	strcpy(shm_obj->elm.msg,shm_obj->shm_att);
	shm_obj->elm.state = SHM_EMPTY;
	append_text_in_file(shm_obj);
	shmdt(shm_obj->shm_att);
	return 1;
}


/***************************************************************************//** 
 * @brief Public member - initializes a named pipe
 *
 * This functions initializes a named pipe
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int destroy_shared_memory(shm_elm *const shm_obj){
	if(shmctl(shm_obj->shm_id,IPC_RMID,NULL) == -1){
		return -1;
	}
	free(shm_obj);
	return 1;
}

/***************************************************************************//** 
 * @brief Public member - initializes a named pipe
 *
 * This functions initializes a named pipe
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int append_text_in_file(shm_elm *const shm_obj){
	/*open file*/
	FILE *file;
	if( (file = fopen(shm_obj->file_name,"a+")) == 0){
		return -1;
	}
	/*move cursore to the end of the file*/
	fseek(file,0,SEEK_END);

	/*write data in the file*/
	if(str_write(file,(char *)shm_obj->elm.msg) == 0){
		printf("Error while writing the number of students \n");
		return -1;	
	}

	/*close file*/
	fclose(file);

	return 1;
}

/***************************************************************************//** 
 * @brief Public member - initializes a named pipe
 *
 * This functions initializes a named pipe
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int create_file(const char * file_name){
	/*Try to open file*/
	FILE *file;
	if( (file = fopen(file_name,"a+")) == 0){
		return 0;
	}
	fclose(file);
	return 1;
}

/***************************************************************************//** 
 * @brief Public member - initializes a named pipe
 *
 * This functions initializes a named pipe
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int str_len (const char *str)
{
  register int count = 0;

  while (*str++ != '\0')
    {
      count++;
    }

  return count;
}

/***************************************************************************//** 
 * @brief Public member - initializes a named pipe
 *
 * This functions initializes a named pipe
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int str_write (FILE *fp, const char *str)
{
  int status = 0;
  int length = 0; 

  length = str_len (str);
  
  status = (int) fwrite ((const void *) str, (size_t) 1, 
                         (size_t) length, fp);

  if (status == 0)
    {
       printf ("\nError writing string '%s' to the file", str);
    }

  return status;
}

/***************************************************************************//** 
 * @brief Public member - initializes a named pipe
 *
 * This functions initializes a named pipe
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int str_read (FILE *fp, /*@null@*/ char **dst)
{
  int   s_len  = 512;
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
  
  if (status == 0)
    {
      printf ("\nError reading from file");
    }
  
  *dst = ptr;
  return status;
}
