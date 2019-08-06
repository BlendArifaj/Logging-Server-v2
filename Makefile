####################################################################
# Makefile                                                         #
####################################################################

.SUFFIXES: all				
.PHONY: clean all dox splint

####################################################################
# Path Definitions                                                 #
####################################################################
OUT_DIR     = ./output
OBJ_DIR     = $(OUT_DIR)/build
EXE_DIR     = $(OUT_DIR)/exe
DOX_DIR     = $(OUT_DIR)/docu
SRC_DIR     = ./src
INCLUDE_DIR = ./include

####################################################################
# Flag Definitions                                                 #
####################################################################
FLAGS = -Wall -D_XOPEN_SOURCE -D_SVID_SOURCE -c -ansi -g
# SPLINTFLAGS = +standard -skipposixheaders -mustfreeonly -likelybool -temptrans -usedef

SPLINTFLAGS = +posixlib +standard -mustfreeonly -likelybool \
			  -temptrans -nullstate -mustdefine -compdef -compdestroy \
			  -dependenttrans -noeffect
## -warnposix -preproc 
INCLUDEPATHS =-I$(INCLUDE_DIR)



####################################################################
# Create build and ouput directory								   #
####################################################################
$(OBJ_DIR):
	mkdir -p $(OUT_DIR)
	mkdir -p $(OBJ_DIR)
	@echo "Created build directory."

$(EXE_DIR):
	mkdir -p $(EXE_DIR)
	@echo "Created executable directory."

####################################################################
# Build instructions			 								   #
####################################################################
all: $(OBJ_DIR) $(EXE_DIR) $(EXE_DIR)/client $(EXE_DIR)/server 

$(EXE_DIR)/client: $(OBJ_DIR)/client.o $(OBJ_DIR)/msg_queue.o $(OBJ_DIR)/nm_pipe.o $(OBJ_DIR)/socket.o 
	gcc $(OBJ_DIR)/client.o $(OBJ_DIR)/msg_queue.o $(OBJ_DIR)/nm_pipe.o $(OBJ_DIR)/socket.o -o $(EXE_DIR)/client

$(EXE_DIR)/server: $(OBJ_DIR)/server.o $(OBJ_DIR)/sh_sem.o $(OBJ_DIR)/sh_mem.o $(OBJ_DIR)/msg_queue.o $(OBJ_DIR)/nm_pipe.o $(OBJ_DIR)/socket.o 
	gcc $(OBJ_DIR)/server.o $(OBJ_DIR)/sh_sem.o $(OBJ_DIR)/sh_mem.o $(OBJ_DIR)/msg_queue.o $(OBJ_DIR)/nm_pipe.o $(OBJ_DIR)/socket.o -o $(EXE_DIR)/server

$(OBJ_DIR)/client.o: $(SRC_DIR)/client.c 
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/client.o $(SRC_DIR)/client.c 

$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c $(SRC_DIR)/sh_mem.c 
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/server.o $(SRC_DIR)/server.c 

$(OBJ_DIR)/sh_sem.o: $(SRC_DIR)/sh_sem.c 
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/sh_sem.o $(SRC_DIR)/sh_sem.c

$(OBJ_DIR)/sh_mem.o: $(SRC_DIR)/sh_mem.c 
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/sh_mem.o $(SRC_DIR)/sh_mem.c

$(OBJ_DIR)/msg_queue.o: $(SRC_DIR)/msg_queue.c 
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/msg_queue.o $(SRC_DIR)/msg_queue.c

$(OBJ_DIR)/nm_pipe.o: $(SRC_DIR)/nm_pipe.c
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/nm_pipe.o $(SRC_DIR)/nm_pipe.c

$(OBJ_DIR)/socket.o: $(SRC_DIR)/socket.c
	gcc $(FLAGS) $(INCLUDEPATHS) -o $(OBJ_DIR)/socket.o $(SRC_DIR)/socket.c

dox:
	mkdir -p $(DOX_DIR)
	@echo "Created doxygen directory."

	doxygen ./doc/exc.dox

splint:
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/server.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/sh_sem.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/f_ser.c
	splint $(SPLINTFLAGS) $(INCLUDEPATHS) $(SRC_DIR)/opt.c

clean:
	rm -rf $(OBJ_DIR) $(DOX_DIR) $(EXE_DIR) $(OUT_DIR) 
