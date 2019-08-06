/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  msg_queue.c
 *
 * @brief Implement the message queue
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Sat Jun 29, 08:10:54 2019 $
 *****************************************************************************/
#include 	"msg_queue.h"

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
int init_msq_queue(msq_elm *const msq_obj){
	if( (msq_obj->msq_id = msgget(MSQ_KEY, MSQ_FLAG)) == -1){

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
int send_request_msq_queue(msq_elm *const msq_obj){
	if(init_msq_queue(msq_obj)<0){
		perror("Failed here\n");
		return -1;
	}

	if(msgsnd(msq_obj->msq_id,msq_obj->elm.msg,msq_obj->elm.msq_len,0) == -1){

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
int get_requst_msq_queue(msq_elm *const msq_obj,nm_pipe *const nm_obj){

	if(init_msq_queue(msq_obj)<0){
		perror("Failed here\n");
		return -1;
	}
	/*for(;;){*/
		if((msq_obj->elm.msq_len = (int)msgrcv(msq_obj->msq_id, msq_obj->elm.msg, sizeof(msq_obj->elm.msg), 0, MSG_NOERROR)) == -1){
			perror("msgrcv failed");
			return -1;
		}else{	
			msq_obj->elm.msg[msq_obj->elm.msq_len] = '\0';	
			if(create_named_pipe(nm_obj,msq_obj->elm.msg) < 0){
				printf("Error while creating named pipe\n");
				return -1;
			}
			return 1;
		}
	/*}*/
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
int wait_first_client(msq_elm *const msq_obj,nm_pipe *const nm_obj){

	if(init_msq_queue(msq_obj)<0){
		perror("Failed here\n");
		return -1;
	}
	/*for(;;){*/
		if((msq_obj->elm.msq_len = msgrcv(msq_obj->msq_id, msq_obj->elm.msg, sizeof(msq_obj->elm.msg), 0, MSG_NOERROR)) == -1){
			perror("msgrcv failed");
			return -1;
		}else{	
			msq_obj->elm.msg[msq_obj->elm.msq_len] = '\0';	
			if(create_named_pipe(nm_obj,msq_obj->elm.msg) < 0){
				printf("Error while creating named pipe\n");
				return -1;
			}
			return 1;
		}
	/*}*/
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
int next_phase(nm_pipe *const nm_obj,char *s){
	nm_obj->elm.len = 30;
	if(create_named_pipe(nm_obj,s)<0){
		return -1;
	}
	
	if(nmp_recv(nm_obj)<0){
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
 *                      ele ment
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int get_number_message_queue(msq_elm *const msq_obj){
	struct msqid_ds *buf = (struct msqid_ds *)malloc(sizeof(struct msqid_ds));
	if(msgctl(msq_obj->msq_id,IPC_STAT,buf) < 0){
		perror("msgctl:");
		return -1;
	}
	int tmp = buf->msg_qnum;
	free(buf);
	return tmp;
}
/*
int enter(char *objname, int priority){
	int len, s_qid;
	struct q_entry s_entry;

	if((len=strlen(objname)) > MAXOBN){
		warn("name too long");
		return -1;
	}

	if(priority > MAXPRIOR || priority < 0){
		warn("invalid priority level");
		return -1;
	}

	if(( s_qid = init_queue()) == -1){
		return -1;
	}

	s_entry.mtype = (long)priority;
	strncpy(s_entry.mtext, objname, MAXOBN);
	long a = 0;
	if((a = msgsnd(s_qid,&s_entry, len,0))==-1){
		perror("msgsnd failed");
		return -1;
	}else{
		printf("Message send successfully: %ld \n ",a);
		return 0;
	}
}

int init_queue(void){
	int queue_id;	
	if( (queue_id = msgget(QKEY, IPC_CREAT | QPERM)) == -1){
		perror("msgget failed");
	}
	return queue_id;
}

void proc_handler(char *fifo){
	pid_t pid;
	nm_pipe_t *tmp =  (nm_pipe_t *)malloc(sizeof(nm_pipe_t));
	if(nmp_init(tmp,fifo) == -1){
		printf("Error");
	}

	while(1){
		key_t semkey = 0x200;
		switch(pid = fork()){
		case 0:
			if( nmp_recv(tmp) != -1){
			printf("\nCLINET WRITE\n");

			}

		case -1:
			exit(1);
			break;
		default:
			printf("\nSERVER READ\n");
			handlesem(semkey,1);
			break;
		}
	}

	
}
int serve(void){
	int mlen, r_qid;
	pid_t pid;
	struct q_entry r_entry;
	if((r_qid = init_queue()) == -1){
		printf("Failed to init queue\n %d",r_qid);
		return -1;
	}
	for(;;){
		if((mlen = msgrcv(r_qid, &r_entry, MAXOBN, (-1 * MAXPRIOR), MSG_NOERROR)) == -1){
			perror("msgrcv failed");
			exit(1);
			
		}else{
			r_entry.mtext[mlen] = '\0';
			switch(pid = fork()){
				case 0:
					printf("We are going to initilize pipe... \n");
					proc_handler(r_entry.mtext);
					break;
				case -1:
					break;
				default:
					break;
			}
		}
	}
}






key_t generate_key_t(const char *path, int id){
	return ftok(path,id);
}


int create_message_queue(message_queue  *message_queue_object){
	message_queue_object->message_queue_id = -1;
	if( (message_queue_object->message_queue_id = msgget(MSQ_KEY,IPC_CREAT| MSQ_PERM))==-1 ){
		return -1;
	}
	return 1;
}

int write_message_queue(message_queue *message_queue_object){
	
	if(msgsnd(message_queue_object->message_queue_id,&message_queue_object->elm,sizeof(message_queue_element),0) == -1){
		return -1;
	}
	return 1;
}

int read_message_queue(message_queue *message_queue_object){
	if(msgrcv(message_queue_object->message_queue_id,&message_queue_object->elm,sizeof(message_queue_element),0,MSG_NOERROR) == -1){
		return -1;
	}
	return 1;
}

int server_listen_message_queue(message_queue *message_queue_object){
	for(;;){
		if(read_message_queue(message_queue_object) != -1){
			named_pipe_phase(message_queue_object->elm.msg);
			printf("Message read: %s\n",message_queue_object->elm.msg);
		}
	}
	return 1;IPC_STAT
}

void named_pipe_phase(char *pipe_name){
	nm_pipe_t *tmp =  (nm_pipe_t *)malloc(sizeof(nm_pipe_t));
	if(nmp_init(tmp,pipe_name) == -1){
		printf("Error");
	}		
	pid_t pid;
	switch(pid=fork()){
		case 0:
			read_pipe(tmp);
			break;
		case -1:
			warn("Fork to tart server failed");
			break;
		default:
			printf("Server process pid is %d\n",pid);
	}
	exit(pid != -1? 0 : 1);

}

void read_pipe(nm_pipe_t *const nmp_obj){
	for(;;){
		if( nmp_recv(nmp_obj) != -1){
			printf("NM_PIPE :%s\n",nmp_obj->elm.msg); 
		}
	}
}


*/
