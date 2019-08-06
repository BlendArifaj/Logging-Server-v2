/**
 * Copyright (C) 2003-2019 Dipl.Ing. Dr. techn. Idriz Smaili. All rights reserved
 * Siebenbuergerstrasse 16-26/26/17, A--1220 Wien, Austria. smaili.idriz@gmail.com
 *
 * @file  nm_pipe.c
 *
 * @brief Implements the named pipe functionality
 * 
 * @author (IS) Dr.techn. Dipl.-Ing. Idriz Smaili (smaili.idriz@gmail.com)
 * @date   $Date: Thu, Jun 27, 2019 2:07:09 PM$
 */


#include "nm_pipe.h"

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
int create_named_pipe  (nm_pipe *const nmp_obj, const char *nmp_name)
{
  nmp_obj->nmp_id = -1;
  
  /* create the fifo if it was not already created */
  if (mkfifo (nmp_name, NMP_PERM) == -1)
    {
      if (errno != EEXIST)
        {
          return -1;
        }
    }

  /* open the fifo */
  if ((nmp_obj->nmp_id = open (nmp_name, O_RDWR)) <0)
    {
      return -1;
    }

  return 1;
}


/***************************************************************************//** 
 * @brief Public member - closes a named pipe
 *
 * This functions closes a named pipe
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @retval 
 ******************************************************************************/
void destroy_named_pipe (nm_pipe *const nmp_obj)
{
  if (nmp_obj->nmp_id != -1)
    {
      (void) close (nmp_obj->nmp_id);
    }
}

/***************************************************************************//** 
 * @brief Public member - sends a message through the named pipe
 *
 * This functions sends a message through the named pipe.
 *
 * @param[in,out] nmp_obj - a object describing the name pipe including its 
 *                      element
 *
 * @param[in]     msg - pointer to the data needs to be written 
 * @param[in]     len - number of bytes needs to be written 
 *
 * @retval -1 : on error
 * @retval number of sent bytes : on success
 ******************************************************************************/
int nmp_send  (nm_pipe *const nmp_obj)
{
  register int res = -1;

  res = write (nmp_obj->nmp_id, &nmp_obj->elm, nmp_obj->elm.len + sizeof (int));

  if (res == -1)
    {
      perror ("msq_send:: send message failed");
    }

  printf ("\n nmp_send: len= %d, msg= '%s'", nmp_obj->elm.len,
          &nmp_obj->elm.msg [0]);
  
  return res;
}

/***************************************************************************//** 
 * @brief Public member - receives a message through the message queue
 *
 * This functions receives a message through the message queue.
 *
 * @param[in,out] msq_obj - a object describing the msg queue including its 
 *                      element
 * @retval -1 : on error
 * @retval number of sent bytes : on success
 ******************************************************************************/
int nmp_recv  (nm_pipe *const nmp_obj)
{
  int res = -1;
  int len = 0;

  res = read (nmp_obj->nmp_id, &len, sizeof (int));
  if (res <0)
    {
      perror ("msq_recv:: receive message failed");
    }
  
  res = read (nmp_obj->nmp_id, &nmp_obj->elm.msg, len);
  if (res <0)
    {
      perror ("msq_recv:: receive message failed");
    }
	
  nmp_obj->elm.len = len; 
  return res;
}

void get_message(char *message){

}
