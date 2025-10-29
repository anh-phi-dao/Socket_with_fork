SRC:=src/%.c #usage of wildcard
INC:=include
BUILD:=src/%.o
LIST_SRC:=$(wildcard src/*.c)
OBJECT:=$(LIST_SRC:%.c=%.o) 
FILE:=create_file/create_binary_file
CC:=gcc
TEMP_FLAG:=
CFLAG:=-c -Wall  -g $(TEMP_FLAG)
LFLAG:=-g -pthread $(TEMP_FLAG)
INC_FLAG:= -I$(INC)/


#build all target
.PHONY: all
all: main create_binary_file
	rm src/*.o
	@echo "You can run the program"

#build relocatable object file
$(BUILD): $(SRC) 
	$(CC) $(CFLAG) $^ -o $@ $(INC_FLAG)

#build executable file
main: $(OBJECT)
	$(CC) $(LFLAG)  $^ -o $@  
	@echo "$@ program has been compiled sucessfully"

create_binary_file: $(FILE).o
	$(CC) $(LFLAG)  $^ -o $@ 
	rm $^

$(FILE).o : $(FILE).c
	$(CC) $(CFLAG) $^ -o $@ 

#debug command
.PHONY: debug
debug: main
	@gdb main

#execute the program
.PHONY: excecute
excecute: main
	@./main

#clean the program and unessesary file
.PHONY: clean
clean:
	- rm main 
	- rm create_binary_file
