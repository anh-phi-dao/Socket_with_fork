SRC:=src/%.c #usage of wildcard
INC:=include
BUILD:=src/%.o
LIST_SRC:=$(wildcard src/*.c)
OBJECT:=$(LIST_SRC:%.c=%.o)
SHARED_OBJECT:=liball_in_one.so 
SHARED_OBJECT_FLAG:=-lall_in_one
FILE:=create_file/create_binary_file
EXECUTE_SOURE:=$(wildcard add_environment_variable/*.c)
EXECUTE_OBJECT:=$(EXECUTE_SOURE:%.c=%.o)
CC:=gcc
CFLAG:=-c -Wall  -fPIC 
LFLAG:=-pthread -L./ $(SHARED_OBJECT_FLAG)
INC_FLAG:= -I$(INC)/


#build all target
.PHONY: all
all: main create_binary_file path
	@rm src/*.o
	@echo "\nBefore running main, please execute this command in terminal"
	@./path
	
$(SHARED_OBJECT) : $(OBJECT)
	$(CC) -shared $^ -o $@

#build relocatable object file
$(BUILD): $(SRC) 
	$(CC) $(CFLAG) $^ -o $@ $(INC_FLAG)

#build executable file
main:  src/main.o $(SHARED_OBJECT)
	$(CC) $(LFLAG)  $^ -o $@  
	@echo "$@ program has been compiled sucessfully"

create_binary_file: $(FILE).o
	$(CC) $(LFLAG)  $^ -o $@ 
	rm $^

$(FILE).o : $(FILE).c
	$(CC) $(CFLAG) $^ -o $@ 

$(EXECUTE_OBJECT):$(EXECUTE_SOURE)
	$(CC) -c $^ -o $@

path:$(EXECUTE_OBJECT)
	$(CC) $^ -o $@
	rm $(EXECUTE_OBJECT)

#debug command
.PHONY: debug
debug: main
	@gdb main

#execute the program
.PHONY: execute
execute: main
	./main

#clean the program and unessesary file
.PHONY: clean
clean:
	- rm main 
	- rm create_binary_file
	- rm path
