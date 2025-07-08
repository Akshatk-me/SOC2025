CFLAGS =  -g  -Wall -Wextra
CC = g++ 

SRC_BASIC = main.cpp mem_manage.cpp mem_manage_basic.cpp
SRC_BITWISE = main.cpp mem_manage.cpp mem_manage_bitwise.cpp

all: basic bitwise

# make will compile main.o and mem_manage.o before producing final if main.o and mem_manage.o are not found.
# use @ to run the command put not print it to screen
# Make files don't take space as sperators. They need TAB.
basic: $(SRC_BASIC)
	@echo "Linking and producing the final application"
 	# \$^ are the prereq, \$@ is the target
	$(CC) -DUSE_BASIC $(CFLAGS) $^ -o $@ 
	@chmod +x basic 

bitwise: $(SRC_BITWISE)
	@echo "Linking and producing the final application"
	 # \$^ are the prereq, \$@ is the target
	$(CC) -DUSE_BITWISE $(CFLAGS) $^ -o $@ 
	@chmod +x bitwise 


clean:
	@echo "Removing everything but the source file"
	rm basic bitwise

#to create a library of somlib
