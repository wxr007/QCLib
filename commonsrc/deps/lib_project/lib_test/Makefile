CXX=g++
CFLAG=-c -g -D_USE_MUDUO_LIB__
DEBUG=

INC =  -I/root/muduo-master/ -I/root/redis-3.0/deps/hiredis
OBJ=./.obj
BIN=.
SRC=.
LIB_DIR=-L/root/build/release/lib -L/root/redis-3.0/deps/hiredis -L/usr/local/lib
LDFLAG=  -lmuduo_net -lmuduo_base -lhiredis -lpthread
LD=g++

TARGET= muduo_hiredis_client

TARGETOBJS=$(OBJ)/hiredis_with_muduo_main.o
           

all:createdir MakeTarget

createdir:
	test -d $(OBJ) || mkdir -p $(OBJ)

clean:
	rm -f $(BIN)/$(TARGET)
	rm -f $(OBJ)/*

	
$(OBJ)/hiredis_with_muduo_main.o:$(SRC)/hiredis_with_muduo_main.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/hiredis_with_muduo_main.cpp -o $(OBJ)/hiredis_with_muduo_main.o
	
#-------------LINK------------------------------

MakeTarget:$(TARGETOBJS)
	$(LD) $(TARGETOBJS) -o $(TARGET) $(LIB_DIR) $(LDFLAG)  
