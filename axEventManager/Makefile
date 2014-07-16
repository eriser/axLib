CC = clang++ -std=c++11
LIBS = -pthread
LIB_NAME = libaxEventManager.a
OBJECT_DIR = obj
SRC_DIR = src
HEADER_DIR = include

FILE_LIST = axMsg axObject axEventManager axTimer 

OBJECTS = $(addsuffix .o, $(FILE_LIST))
OBJECTS_LIST = $(addprefix $(OBJECT_DIR)/, $(OBJECTS))

CPP = $(addsuffix .cpp, $(FILE_LIST))
CPP_LIST = $(addprefix $(SRC_DIR)/, $(CPP))

HEADER = $(addsuffix .h, $(FILE_LIST))
HEADER_LIST = $(addprefix $(HEADER_DIR)/, $(HEADER))

default: $(FILE_LIST)
	ar rcs $(LIB_NAME) $(OBJECTS_LIST)
	#$(CC) $(LIBS) $(OBJECTS_LIST) -o $(PROGRAM_NAME)

$(FILE_LIST): $(CPP_LIST) $(HEADER_LIST)
	$(CC) -c $(SRC_DIR)/$@.cpp -o $(OBJECT_DIR)/$@.o -I $(HEADER_DIR)

clean: 
	rm -f $(OBJECTS_LIST)
	rm -f $(LIB_NAME)


