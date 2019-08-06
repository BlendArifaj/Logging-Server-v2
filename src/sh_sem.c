/*****************************************************************************
 * Copyright (C) 2019 BSc. Blend Arifaj. All rights reserved
 * Str. Agim Ramadani, Prishtine, Republic of Kosovo. blendarifaj@hotmail.com
 *
 * @file  sh_sem.c
 *
 * @brief Implement the server form
 * 
 * @author (IS) BSc. Blend Arifaj (blendarifaj@hotmail.com)
 * @date   $Date: Sat Jun 29, 08:10:54 2019 $
 *****************************************************************************/
#include "sh_sem.h"


/***************************************************************************//** 
 * @brief Public member - Create semaphore
 *
 * This functions creates a semahpore
 *
 * @param[in,out] sem_obj - the object that describes semaphore
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int create_semaphore(sem_type *const sem_obj){
	
	if((sem_obj->sem_id = semget(SEM_KEY,3,SEM_FLAG)) == -1){
		if(errno == EEXIST)
			sem_obj->sem_id = semget(SEM_KEY,3,0);
	}else{
		sem_obj->arg.val = 1;
		sem_obj->status = semctl(sem_obj->sem_id,0,SETVAL,sem_obj->arg);
	}

	if(sem_obj->sem_id==-1 || sem_obj->status ==-1){
		perror("Initsem failed");
		return -1;
	}

	return 1;
}

/***************************************************************************//** 
 * @brief Public member - Start read
 *
 * This functions bllock reading semaphore
 *
 * @param[in] - semid - semaphore id
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int start_read(int semid){
	struct sembuf p_buf = {0,-1,0};
	if(semop(semid, &p_buf, 1) == -1){
		perror("p(semdi) failed");
		return -1;
	}
	return 1;	
}

/***************************************************************************//** 
 * @brief Public member - Start write waiting
 *
 * This functions bllock waiting for writing
 *
 * @param[in] - semid - semaphore id
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int start_write_waiting(int semid){
	struct sembuf p_buf = {1,-1,0};
	if(semop(semid, &p_buf, 1) == -1){
		perror("p(semdi) failed");
		return -1;
	}
	return 1;	
}

/***************************************************************************//** 
 * @brief Public member - Start writing
 *
 * This functions bllock  writing
 *
 * @param[in] - semid - semaphore id
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int start_writing(int semid){
	struct sembuf p_buf = {2,-1,0};
	if(semop(semid, &p_buf, 1) == -1){
		perror("p(semdi) failed");
		return -1;
	}
	return 1;	
}

/***************************************************************************//** 
 * @brief Public member - Finish read
 *
 * This functions release semaphore for read
 *
 * @param[in] - semid - semaphore id
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int finish_read(int semid){
	struct sembuf v_buf = {0,1,0};
	if(semop(semid, &v_buf, 1) == -1){
		perror("v(semid) failed");
		return -1;
	}
	return 1;
}

/***************************************************************************//** 
 * @brief Public member - Finish write waiting
 *
 * This functions release semaphore for write waiting
 *
 * @param[in] - semid - semaphore id
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int finish_write_waiting(int semid){
	struct sembuf v_buf = {1,1,0};
	if(semop(semid, &v_buf, 1) == -1){
		perror("p(semdi) failed");
		return -1;
	}
	return 1;
}

/***************************************************************************//** 
 * @brief Public member - Finish writing
 *
 * This functions release semaphore for writing
 *
 * @param[in] - semid - semaphore id
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int finish_writing(int semid){
	struct sembuf v_buf = {2,1,0};
	if(semop(semid, &v_buf, 1) == -1){
		perror("p(semdi) failed");
		return -1;
	}
	return 1;
}

/***************************************************************************//** 
 * @brief Public member - sem wait
 *
 * Wait for semaphore
 *
 * @param[in] semdid
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int sem_wait(int semid){
	struct sembuf p_buf = {0,-1,SEM_UNDO};

	if(semop(semid, &p_buf, 1) == -1){
		perror("p(semdi) failed");
		exit(1);
	}
	return 0;
}

/***************************************************************************//** 
 * @brief sem signal
 *
 * Signal for sem
 *
 * @param[in] - semdid
 *
 * @retval -1 : on error
 * @retval  0 : on success
 ******************************************************************************/
int sem_signal(int semid){
	struct sembuf v_buf = {0,1,SEM_UNDO};

	if(semop(semid, &v_buf, 1) == -1){
		perror("v(semid) failed");
		exit(1);
	}

	return 0;
}



/***************************************************************************//** 
 * @brief Handlesems
 *
 * In this function other proces will try to go in critical section
 *
 * @param[in,out] sem_obj, shm_obj
 *
 * @retval: none, void function
 ******************************************************************************/
void handlesems(sem_type *sem_obj, shm_elm *shm_obj){
	pid_t pid = getpid();
	switch(pid = fork()){
		case -1: 
			perror("Fork failed");
			break;
		case 0:
			start_read(sem_obj->sem_id);
			start_writing(sem_obj->sem_id);
			finish_read(sem_obj->sem_id);	
			record(shm_obj);
			start_read(sem_obj->sem_id);
			finish_writing(sem_obj->sem_id);
			finish_read(sem_obj->sem_id);	
			break;
		default:
			start_write_waiting(sem_obj->sem_id);
			start_writing(sem_obj->sem_id);
			conn_handler_write(shm_obj);
			finish_writing(sem_obj->sem_id);
			finish_write_waiting(sem_obj->sem_id);
			break;
	}
}
/*
void handlesems(sem_type *sem_obj,shm_elm *shm_obj){
	pid_t pid = getpid();
	if(create_semaphore(sem_obj) < 0){
		exit(1);
	}
	printf("\nprocess %d before critical section\n",pid);
	sem_wait(sem_obj->sem_id );
	printf("process %d in critical section\n",pid);
	switch(fork()){
		case -1: printf("Failed\n"); break;
		case 0: 
			if(read_shared_memory(shm_obj) < 0){
				printf("Process %d failed to read on shared memory",pid);
			}else{
				printf("Process %d read on shared memory\n",pid);
				shm_obj->elm.state = SHM_EMPTY;
			}
			break;
		default:
			if(write_shared_memory(shm_obj) < 0){
				printf("Process %d failed to write on shared memory",pid);
			}else{
				printf("Process %d write on shared memory\n",pid);
				shm_obj->elm.state = SHM_FULL;
			}
			wait(NULL);
			break;
	}
	
	sem_signal(sem_obj->sem_id );
	printf("Proccess %d exiting\n",pid);

}*/

/***************************************************************************//** 
 * @brief Conn handler writer
 *
 * This process will write in shared memory;
 *
 * @param[in,out] shm_obj
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int conn_handler_write(shm_elm *shm_obj){
	if(write_shared_memory(shm_obj) < 0){
		printf("Process conn_handler failed to write on shared memory");
		return -1;
	}else{
		printf("Process conn_handler write on shared memory\n");
		shm_obj->elm.state = SHM_FULL;
		return 1;
	}
}

/***************************************************************************//** 
 * @brief Record
 *
 * This process will read in shared memory;
 *
 * @param[in,out] shm_obj
 *
 * @retval -1 : on error
 * @retval  1 : on success
 ******************************************************************************/
int record(shm_elm *shm_obj){
	if(read_shared_memory(shm_obj) < 0){
		printf("Process record failed to read on shared memory");
		return -1;
	}else{
		printf("Process record read on shared memory\n");
		shm_obj->elm.state = SHM_FULL;
		return 1;
	}
}



















