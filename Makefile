CC=gcc
CC_DEFINES_GIT_SHA1=$(shell git rev-parse --verify --short=10 HEAD)
CC_DEFINES_GIT_BRANCH=$(shell git branch 2>/dev/null | grep -E '^\* ' | sed -e 's/^\* //')
CC_DEFINES_GIT_UNCLEAN=$(shell git status -s 2>/dev/null | wc -l)
CC_FLAGS=-shared -Wall -c -DGIT_SHA1="\"$(CC_DEFINES_GIT_SHA1)\"" -DGIT_BRANCH="\"$(CC_DEFINES_GIT_BRANCH)\"" -DGIT_UNCLEAN=$(CC_DEFINES_GIT_UNCLEAN)
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
