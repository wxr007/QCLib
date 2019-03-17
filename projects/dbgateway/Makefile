CXX=g++
CFLAG=-c -g -D_USE_MUDUO_LIB__ -D_USE_MYSQL_ -Wno-deprecated
DEBUG=

PROTOINC=-I../proto
INC =  -I../../commonsrc/include -I../../commonsrc/deps/hiredis/ $(PROTOINC) -I../../commonsrc/deps/
OBJ=./.obj
BIN=.
QCOOLSRC=../../commonsrc/src
PROTOSRC=../proto
SRC=.
LIB_DIR=-L../../commonsrc/libs/ -L/usr/lib64/mysql
LDFLAG=-lQCool -lmuduo_net -lmuduo_base -Wl,-Bstatic  -lhiredis -ltinyxml -Wl,-Bdynamic  -lmysqlclient_r -ldl -lrt -lpthread -Wl,--as-needed
LD=g++

TARGET= dbgw

TARGETOBJS=$(OBJ)/db_gateway.o $(OBJ)/table_object_map.o \
           $(OBJ)/db_gw_main.o 

all:createdir MakeTarget

createdir:
	test -d $(OBJ) || mkdir -p $(OBJ)

clean:
	rm -f $(BIN)/$(TARGET)
	rm -f $(OBJ)/*

	
$(OBJ)/db_gateway.o:$(SRC)/db_gateway.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/db_gateway.cpp -o $(OBJ)/db_gateway.o
	
$(OBJ)/table_object_map.o:$(SRC)/table_object_map.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/table_object_map.cpp -o $(OBJ)/table_object_map.o
	
$(OBJ)/db_gw_main.o:$(SRC)/db_gw_main.cpp
	$(CXX) $(CFLAG) $(INC) $(SRC)/db_gw_main.cpp -o $(OBJ)/db_gw_main.o

#----------------------------------------------------------------	
$(OBJ)/tinyxml.o:$(XMLSRC)/tinyxml.cpp
	$(CXX) $(CFLAG) $(INC) $(XMLSRC)/tinyxml.cpp -o $(OBJ)/tinyxml.o	
	
$(OBJ)/tinystr.o:$(XMLSRC)/tinystr.cpp
	$(CXX) $(CFLAG) $(INC) $(XMLSRC)/tinystr.cpp -o $(OBJ)/tinystr.o	

$(OBJ)/tinyxmlerror.o:$(XMLSRC)/tinyxmlerror.cpp
	$(CXX) $(CFLAG) $(INC) $(XMLSRC)/tinyxmlerror.cpp -o $(OBJ)/tinyxmlerror.o	
	
$(OBJ)/tinyxmlparser.o:$(XMLSRC)/tinyxmlparser.cpp
	$(CXX) $(CFLAG) $(INC) $(XMLSRC)/tinyxmlparser.cpp -o $(OBJ)/tinyxmlparser.o	
#-------------LINK------------------------------

MakeTarget:$(TARGETOBJS)
	$(LD) $(TARGETOBJS) -o $(TARGET) $(LIB_DIR) $(LDFLAG)  
