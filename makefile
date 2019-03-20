#MakeFile para correr los archivos

#arhivos que tienen un main
TARGETS = slave
#arhivos que no tienen un main
SOURCES = queue

CROSS_TOOL =
CC_C = $(CROSS_TOOL) gcc

CFLAGS = -Wall -Werror -g

all: clean $(TARGETS)

$(TARGETS):
	$(CC_C) $(CFLAGS) $(SOURCES:=.c) $@.c -o $@

clean:
	rm -f $(TARGETS)
	rm -f $(SOURCES)

debug: all
	cppcheck $(TARGETS:=.c)
	cppcheck $(SOURCES:=.c)
	
.PHONY: all, debug

	