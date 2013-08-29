
CC = gcc

INCLUDE_DIRS =  -I . -I /segfs/tango/transport/zmq3.2.2/ubuntu1204/include \
				-I /home/taurel/bin/czmq/ubuntu1204/include

OBJS_DIR    = obj

LIB_DIRS    = -L /segfs/tango/transport/zmq3.2.2/ubuntu1204/lib \
			  -L /home/taurel/bin/czmq/ubuntu1204/lib

CXXFLAGS =  -g $(INCLUDE_DIRS)

LFLAGS =  -g $(LIB_DIRS)  	\
				-l czmq -lzmq

%.o: %.c 
	$(CC) $(CXXFLAGS) -c $< -o $(OBJS_DIR)/$*.o


#-----------------------------------------
#	 Make Entry
#-----------------------------------------
all: clnt svr

svr:	svr.o
	$(CC) $(OBJS_DIR)/svr.o -o svr $(LFLAGS)

clnt:	clnt.o
	$(CC) $(OBJS_DIR)/clnt.o -o clnt $(LFLAGS)


clean:
	rm -f $(OBJS_DIR)/*.o  \
	core

make_obj_dir:
	@mkdir -p obj

	
