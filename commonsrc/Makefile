CXX=g++
CFLAG=-c -g -D_USE_MUDUO_LIB__  -D_USE_MYSQL_ -Wno-deprecated
DEBUG=


INC = -I./include -I./deps -I./deps/hiredis -I./deps/mysql -I./deps/protobuff -I./deps/tinyxml -I./deps/jsoncpp/include
OBJ=./.obj
BIN=.
SRC=./src
LIB_DIR=-L./libs
LDFLAG=-lmuduo_base -lmuduo_net -lhiredis -lprotobuf -ltinyxml -ljson
LD=g++
AR = ar

TATGETLIB = ./libs/
TARGET = $(TATGETLIB)libQCool.a

TARGETOBJS= $(OBJ)/file_writer.o $(OBJ)/logger.o $(OBJ)/thread_info.o \
			$(OBJ)/tcp_module.o $(OBJ)/tcp_server.o $(OBJ)/tcp_client.o $(OBJ)/http_server.o $(OBJ)/lib_init_for_muduo.o \
			$(OBJ)/thread.o $(OBJ)/redis_client.o $(OBJ)/database_mysql.o $(OBJ)/db_cache.o\
			$(OBJ)/md5.o


all:createdir outtarget 

createdir:
	test -d $(TATGETLIB) || mkdir -p $(TATGETLIB)
	test -d $(OBJ) || mkdir -p $(OBJ)

clean:
	rm -f $(BIN)/$(TARGET)
	
	rm -f $(OBJ)/*
#----------------------------------------log file-------------------------------------------	
$(OBJ)/file_writer.o:$(SRC)/file_writer.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/file_writer.cpp -o $(OBJ)/file_writer.o
	
$(OBJ)/logger.o:$(SRC)/logger.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/logger.cpp -o $(OBJ)/logger.o

$(OBJ)/thread_info.o:$(SRC)/thread_info.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/thread_info.cpp -o $(OBJ)/thread_info.o	
	
#----------------------------------------net file-------------------------------------------	
$(OBJ)/tcp_module.o:$(SRC)/tcp_module.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/tcp_module.cpp -o $(OBJ)/tcp_module.o

$(OBJ)/tcp_server.o:$(SRC)/tcp_server.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/tcp_server.cpp -o $(OBJ)/tcp_server.o

$(OBJ)/tcp_client.o:$(SRC)/tcp_client.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/tcp_client.cpp -o $(OBJ)/tcp_client.o

$(OBJ)/http_server.o:$(SRC)/http_server.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/http_server.cpp -o $(OBJ)/http_server.o
	
$(OBJ)/lib_init_for_muduo.o:$(SRC)/lib_init_for_muduo.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/lib_init_for_muduo.cpp -o $(OBJ)/lib_init_for_muduo.o
	
#----------------------------------------base file-------------------------------------------
$(OBJ)/thread.o:$(SRC)/thread.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/thread.cpp -o $(OBJ)/thread.o
	
#----------------------------------------database file-------------------------------------------
$(OBJ)/redis_client.o:$(SRC)/redis_client.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/redis_client.cpp -o $(OBJ)/redis_client.o
	
$(OBJ)/database_mysql.o:$(SRC)/database_mysql.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/database_mysql.cpp -o $(OBJ)/database_mysql.o	
	
$(OBJ)/db_cache.o:$(SRC)/db_cache.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/db_cache.cpp -o $(OBJ)/db_cache.o
#----------------------------------------encrypt file-------------------------------------------
$(OBJ)/md5.o:$(SRC)/md5.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/md5.cpp -o $(OBJ)/md5.o
	
outtarget:$(TARGETOBJS)
	$(AR) rcs $(TARGET) $(TARGETOBJS) 
