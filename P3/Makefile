# CROSS COMPILING SETUP
CROSS_COMPILER=/home/aida/Documentos/Master/DSSE/arm-2014.05/bin/
CC        = $(CROSS_COMPILER)arm-none-linux-gnueabi-gcc
LD        = $(CROSS_COMPILER)arm-none-linux-gnueabi-ld

CFLAGS  =  -I ../include/  #COMPLETE WITH FLAGS FOR COMPILER
LDFLAGS =  ../lib/ev3c-cross.a ../lib/ev3c-addons.a  ../lib/libespeak.a ../lib/libasound.so ../lib/libsonic.so  -lpthread -lrt -lm #COMPLETE WITH FLAGS FOR LINKER

# DEFAULT TESTS TO COMPILE 
all: bin/practica3_threads_plan

# RULES
bin/practica3_threads_plan: practica3_threads_plan.c
	mkdir -p $(@D)
	$(CC) -o $@ $< $(LDFLAGS) $(CFLAGS)

.PHONY: all clean

clean:
	rm -rf obj
	rm -f *~

#COMPLETE WITH COMMAND FOR REMOTE COPY	
copy:

