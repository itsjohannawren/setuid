CC=gcc
CC_FLAGS=-shared -Wall -c
LD=ld
LD_FLAGS=-Bshared
LD_LIBS=
RM=rm -f
CP=cp

SRC_FILES=setuid.c
OBJ_FILES=$(SRC_FILES:.c=.o)

all: setuid

clean:
	@echo "RM  setuid"
	@$(RM) setuid
	@echo "RM  *.o"
	@$(RM) *.o

.c.o:
	@echo "CC  $<"
	@$(CC) $(CC_FLAGS) -o $@ $< 2>&1 | sed -e 's/^/    /'

setuid: $(OBJ_FILES)
	@echo "LD  $@"
	@$(CC) $(LD_FLAGS) $(LD_LIBS) -o $@ $(OBJ_FILES) 2>&1 | sed -e 's/^/    /'
