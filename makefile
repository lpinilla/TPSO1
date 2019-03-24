#MakeFile para compilar o debugear los archivos

#arhivos que tienen un main
TARGETS = slave queueTest
#arhivos que no tienen un main
SOURCES = queue

CROSS_TOOL =
CC_C = $(CROSS_TOOL) gcc

CFLAGS = -Wall -Werror -g

all: clean $(TARGETS)

$(TARGETS):
	$(CC_C) $(CFLAGS) $(SOURCES:=.c) $@.c -o $@

clean:
	rm -f $(TARGETS) $(TARGETS:=.o)
	rm -f $(SOURCES) $(SOURCES:=.o)

debug: all #primero limpiar y compilar todo
	#checkeos cppcheck
	cppcheck $(TARGETS:=.c)
	cppcheck $(SOURCES:=.c)
	#checkeos de los binarios con valgrind
	$(foreach f, $(TARGETS:=), valgrind ./$f)

#test: correr todas las suites de test
	
.PHONY: all, debug

	