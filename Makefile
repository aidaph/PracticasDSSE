# CROSS COMPILING SETUP
CROSS_COMPILER =/home/aida/Documents/Master/DSSE/arm-2009q1/bin/
CC        = $(CROSS_COMPILER)arm-none-linux-gnueabi-gcc
LD        = $(CROSS_COMPILER)arm-none-linux-gnueabi-ld

CFLAGS  =  -I../include/ #COMPLETE WITH FLAGS FOR COMPILER
LDFLAGS =  ../lib/ev3c-cross.a -lpthread #COMPLETE WITH FLAGS FOR LINKER

# DEFAULT TESTS TO COMPILE 
all: bin/practica2_all_button_threads 

# RULES
bin/practica2_all_button_threads: practica2_all_button_threads.c
	mkdir -p $(@D)
	$(CC) -o $@ $< $(LDFLAGS) $(CFLAGS)

.PHONY: all clean

clean:
	rm -rf obj
	rm -f *~

#COMPLETE WITH COMMAND FOR REMOTE COPY	
copy:

