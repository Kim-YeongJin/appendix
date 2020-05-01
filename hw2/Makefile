CC=g++
CFLAGS= -Wall 
OBJS = BufFileTest.o
INCDIR=./include
LIBDIR=./lib
DIRS = buf fixed student coursereg var

.PHONY: all clean

all: BufFileTest
%.o: %.cpp
	$(CC) -c -I$(INCDIR) $(CFLAGS) -o $@ $<

BufFileTest: $(OBJS)
	@for d in $(DIRS); \
	do \
		$(MAKE) -C $$d; \
	done
	$(CC) -o BufFileTest $(OBJS) -L$(LIBDIR) -lmybuffer -lmyfixed -lmystudent -lmycoursereg -lmyvar
	export LD_LIBRARY_PATH=$(LIBDIR)

clean:
	@for d in $(DIRS); \
	do \
		$(MAKE) -C $$d clean; \
	done
	-rm -rf BufFileTest $(OBJS) 
