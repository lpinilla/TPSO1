#MakeFile para compilar o debugear los archivos

#arhivos que tienen un main
TARGETS = slave
#arhivos que no tienen un main
SOURCES = queue
#testing
TASTEFUL = tasteful
TESTS = queueTest test1
TEST_SOURCES = queue Testing/testing_suite

CROSS_TOOL =
CC_C = $(CROSS_TOOL) gcc

CFLAGS = -Wall -Werror -g

all: clean $(TARGETS)

$(TARGETS):
	$(CC_C) $(CFLAGS) $(SOURCES:=.c) $@.c -o $@

clean:
	rm -f $(TARGETS) $(TARGETS:=.o)
	rm -f $(SOURCES) $(SOURCES:=.o)
	rm -f $(TESTS) $(TESTS:=.o)
	rm -f $(TEST_SOURCES) $(TEST_SOURCES:=.o)

debug: all #primero limpiar y compilar todo
	#checkeos cppcheck
	cppcheck $(TARGETS:=.c)
	cppcheck $(SOURCES:=.c)
	#checkeos de los binarios con valgrind
	$(foreach f, $(TARGETS:=), valgrind ./$f)

test: all $(TESTS) #correr todas las suites de test
	$(CC_C) $(CFLAGS) Testing/$(TASTEFUL).c -o Tests/$(TASTEFUL)
	cd Tests && ./$(TASTEFUL)
	
$(TESTS):
	$(CC_C) $(CFLAGS) $(TEST_SOURCES:=.c) Tests/$@.c -o Tests/$@
	
.PHONY: all, debug

	