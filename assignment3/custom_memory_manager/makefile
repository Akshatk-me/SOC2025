CFLAGS = -Wno-implicit-function-decleration -g 
CC = g++ 

all: final 


# make will compile main.o and mem_manage.o before producing final if main.o and mem_manage.o are not found.
# use @ to run the command put not print it to screen
# Make files don't take space as sperators. They need TAB.
final: main.o mem_manage.o
	@echo "Linking and producing the final application"
	$(CC) $(CFLAGS) main.o mem_manage.o -o final
	@chmod +x final

main.o: main.cpp
	@echo "Compiling the main file"
	$(CC) $(CFLAGS) -c $<

mem_manage.o: mem_manage.cpp
	@echo "Compiling the mem_manage file"
	$(CC) $(CFLAGS) -c $<

clean:
	@echo "Removing everything but the source file"
	rm main.o mem_manage.o final

#to create a library of somlib
