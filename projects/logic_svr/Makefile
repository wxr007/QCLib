CXX=g++
CFLAG=-c -g -D_USE_MUDUO_LIB__ -Wno-deprecated
DEBUG=

PROTOBUFINC=-I../../commonsrc/deps/protobuff
PROTOINC=-I../proto 
PROTOSRC=../proto
REDISINC= -I../../commonsrc/deps/hiredis
JSONINC=-I../../commonsrc/deps/jsoncpp/include
INC =  -I../../commonsrc/include -I../../commonsrc/deps/ $(REDISINC) $(PROTOINC) $(JSONINC)
OBJ=./.obj
BIN=.
SRC=.
LIB_DIR=-L../../commonsrc/libs/
LDFLAG=-lQCool -lmuduo_base -lmuduo_net -lmuduo_http -ljson -lpthread -Wl,-Bstatic -lhiredis -lprotobuf -Wl,-Bdynamic -Wl,--as-needed
LD=g++

TARGET= logic_svr

TARGETOBJS= $(OBJ)/logic_main.o $(OBJ)/data_cache.o $(OBJ)/logic_manager.o $(OBJ)/logic_module_manager.o  \
			$(OBJ)/logic_thread.o $(OBJ)/login_module.o $(OBJ)/chat_module.o $(OBJ)/map_module.o \
			$(OBJ)/tower_aoi.o $(OBJ)/server.pb.o $(OBJ)/login.pb.o $(OBJ)/chat.pb.o $(OBJ)/move.pb.o $(OBJ)/time.pb.o\
			$(OBJ)/game_debug.o 

all:createdir MakeTarget

createdir:
	test -d $(OBJ) || mkdir -p $(OBJ)

clean:
	rm -f $(BIN)/$(TARGET)
	rm -f $(OBJ)/*
	
$(OBJ)/logic_main.o:$(SRC)/logic_main.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/logic_main.cpp -o $(OBJ)/logic_main.o
	
$(OBJ)/data_cache.o:$(SRC)/data_cache.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/data_cache.cpp -o $(OBJ)/data_cache.o
	
$(OBJ)/logic_manager.o:$(SRC)/logic_manager.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/logic_manager.cpp -o $(OBJ)/logic_manager.o
		
$(OBJ)/logic_module_manager.o:$(SRC)/logic_module_manager.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/logic_module_manager.cpp -o $(OBJ)/logic_module_manager.o
	
$(OBJ)/login_module.o:$(SRC)/login_module.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/login_module.cpp -o $(OBJ)/login_module.o

$(OBJ)/chat_module.o:$(SRC)/chat_module.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/chat_module.cpp -o $(OBJ)/chat_module.o	
	
$(OBJ)/map_module.o:$(SRC)/map_module.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/map_module.cpp -o $(OBJ)/map_module.o	
	
$(OBJ)/tower_aoi.o:$(SRC)/tower_aoi.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/tower_aoi.cpp -o $(OBJ)/tower_aoi.o
	
$(OBJ)/logic_thread.o:$(SRC)/logic_thread.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/logic_thread.cpp -o $(OBJ)/logic_thread.o
#----------------------proto--------------------------
	
$(OBJ)/login.pb.o:$(PROTOSRC)/login.pb.cc
	$(CXX) $(CFLAG) $(INC) $(PROTOSRC)/login.pb.cc -o $(OBJ)/login.pb.o

$(OBJ)/chat.pb.o:$(PROTOSRC)/chat.pb.cc
	$(CXX) $(CFLAG) $(INC) $(PROTOSRC)/chat.pb.cc -o $(OBJ)/chat.pb.o	
	
$(OBJ)/move.pb.o:$(PROTOSRC)/move.pb.cc
	$(CXX) $(CFLAG) $(INC) $(PROTOSRC)/move.pb.cc -o $(OBJ)/move.pb.o
	
$(OBJ)/server.pb.o:$(PROTOSRC)/server.pb.cc
	$(CXX) $(CFLAG) $(INC) $(PROTOSRC)/server.pb.cc -o $(OBJ)/server.pb.o	
	
$(OBJ)/time.pb.o:$(PROTOSRC)/time.pb.cc
	$(CXX) $(CFLAG) $(INC) $(PROTOSRC)/time.pb.cc -o $(OBJ)/time.pb.o	
	
#----------------------debug--------------------------
$(OBJ)/game_debug.o:$(SRC)/game_debug.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/game_debug.cpp -o $(OBJ)/game_debug.o

#-------------LINK------------------------------

MakeTarget:$(TARGETOBJS)
	$(LD) $(TARGETOBJS) -o $(TARGET) $(LIB_DIR) $(LDFLAG)  
