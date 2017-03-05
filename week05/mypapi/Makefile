PAPIINC = /home/win/Desktop/papi-5.5.1/src/
PAPILIB = /home/win/Desktop/papi-5.5.1/src/libpapi.a
CC = gcc
CFLAGS += -I$(PAPIINC)
OS = $(shell uname)

TARGETS_NTHD =  high_level2 

TARGETS_PTHREAD =  

ifeq ($(OS), SunOS)
   LDFLAGS =  $(PAPILIB) -lcpc
   LDFLAGS_PTHREAD =  $(PAPILIB) -lpthread -lcpc
   TARGETS = $(TARGETS_NTHD) $(TARGETS_PTHREAD) 
else 
 ifeq ($(OS), AIX)
   CC = xlc
   LDFLAGS =  $(PAPILIB) -lpmapi
   LDFLAGS_PTHREAD =  $(PAPILIB) -lpthread -lpmapi
   TARGETS = $(TARGETS_NTHD) $(TARGETS_PTHREAD) 
 else
    ifeq ($(OS), OSF1)
        LDFLAGS =  $(PAPILIB) -lrt
        LDFLAGS_PTHREAD =  $(PAPILIB) -lpthread -lrt
        TARGETS = $(TARGETS_NTHD)  
    else
        ifeq ($(OS), Linux)
             TARGETS = $(TARGETS_NTHD) $(TARGETS_PTHREAD) 
        else
             TARGETS = $(TARGETS_NTHD)  
        endif
        LDFLAGS =  $(PAPILIB) 
        LDFLAGS_PTHREAD =  $(PAPILIB) -lpthread
    endif
 endif
endif

all: $(TARGETS) 

$(TARGETS_NTHD):  %:%.o 
	$(CC) -o $@ $(CFLAGS) $^  $(LDFLAGS)

$(TARGETS_PTHREAD):  %:%.o 
	$(CC) -o $@ $(CFLAGS) $^  $(LDFLAGS_PTHREAD)
clean:
	$(RM) *.o $(TARGETS) 

