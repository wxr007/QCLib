CXX=g++
CFLAG=-c -g -D_USE_MUDUO_LIB__
DEBUG=

PROTOINC=-I../proto -I../../commonsrc/deps/protobuff
PROTOSRC=../proto
INC =  -I../../commonsrc/include -I../../commonsrc/deps/ $(PROTOINC)  
OBJ=./.obj
BIN=.
SRC=.
LIB_DIR=-L../../commonsrc/libs/
LDFLAG=-lQCool -lmuduo_base -lmuduo_net -lmuduo_http -lpthread -Wl,-Bstatic -lprotobuf -Wl,-Bdynamic -Wl,--as-needed
LD=g++

TARGET= gate_server

TARGETOBJS=$(OBJ)/gate_manager.o $(OBJ)/gate_main.o\
		   $(OBJ)/login.pb.o $(OBJ)/server.pb.o
           

all:createdir MakeTarget

createdir:
	test -d $(OBJ) || mkdir -p $(OBJ)

clean:
	rm -f $(BIN)/$(TARGET)
	rm -f $(OBJ)/*

	
$(OBJ)/gate_main.o:$(SRC)/gate_main.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/gate_main.cpp -o $(OBJ)/gate_main.o
	
$(OBJ)/gate_manager.o:$(SRC)/gate_manager.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/gate_manager.cpp -o $(OBJ)/gate_manager.o
		
#----------------------proto--------------------------
$(OBJ)/login.pb.o:$(PROTOSRC)/login.pb.cc
	$(CXX) $(CFLAG) $(INC) $(PROTOSRC)/login.pb.cc -o $(OBJ)/login.pb.o
	
$(OBJ)/server.pb.o:$(PROTOSRC)/server.pb.cc
	$(CXX) $(CFLAG) $(INC) $(PROTOSRC)/server.pb.cc -o $(OBJ)/server.pb.o
	
#-------------LINK------------------------------

MakeTarget:$(TARGETOBJS)
	$(LD) $(TARGETOBJS) -o $(TARGET) $(LIB_DIR) $(LDFLAG)  
