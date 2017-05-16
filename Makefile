# toolchain prefix
TOOLCHAIN_PREFIX = x86_64-hermit-

# compile flags 
OPT_FLAGS  = -g -O2 -fno-bulitin #-O # -O2
REENT_FLAGS = -DREENTRANT_SYSCALLS_PROVIDED -D__DYNAMIC_REENT__ -DSIGNAL_PROVIDED
MALLOCR_FLAGS = -DINTERNAL_NEWLIB 
WARN_FLAGS = -Wall 

# toolchains 
CC = $(TOOLCHAIN_PREFIX)gcc

RM        = rm -f
AR        = $(TOOLCHAIN_PREFIX)ar
RANLIB    = $(TOOLCHAIN_PREFIX)ranlib

# targets
LIB_MALLOC = libhbwmalloc.a
MALLOC_OBJ = wrapper.o mallocr.o mlock.o


all: $(LIB_MALLOC) 

wrapper.o: wrapper.c 
	$(CC) -c $(REENT_FLAGS) $(OPT_FLAGS) $(WARN_FLAGS) $<

mallocr.o: mallocr.c 
	$(CC) -c $(REENT_FLAGS) $(OPT_FLAGS) $(MALLOCR_FLAGS) $(WARN_FLAGS) $<

mlock.o : mlock.c
	$(CC) -c $(REENT_FLAGS) $(OPT_FLAGS) $(WARN_FLAGS) $<

$(LIB_MALLOC): $(MALLOC_OBJ)
	$(AR) cru $@ $(MALLOC_OBJ)
	$(RANLIB) $@

clean:
	$(RM) $(LIB_MALLOC) *.o 
