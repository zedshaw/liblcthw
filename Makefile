CC=gcc
CFLAGS=-g -std=c99 -O0 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
LDFLAGS=$(OPTLIBS)
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/*_tests.c tests/leetcode/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))
TESTAREA=lcthw
TESTSUIT=all

TARGET=build/liblcthw.a

OS=$(shell lsb_release -si)
ifeq ($(OS),CentOS)
	LDLIBS=-llcthw -lbsd -L./build -lm
endif

# The Target Build
all: $(TARGET) tests

dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build cleanobj $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

build:
	@mkdir -p build
	@mkdir -p bin

cleanobj:
	@echo clean test object and bin
	@if [[ "$(TESTSUIT)" != "all" ]];then rm -rf src/$(TESTAREA)/$(TESTSUIT).o;fi
	@if [[ "$(TESTSUIT)" != "all" ]];then rm -rf tests/$(TESTAREA)/$(TESTSUIT)_tests;fi
	

# The Unit Tests
# eg: make TESTAREA=leetcode TESTSUIT=romantransfer
.PHONY: tests
tests: LDLIBS += $(TARGET)
tests: $(TESTS)
	sh ./tests/runtests.sh $(TESTAREA) $(TESTSUIT)

valgrind:
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)

# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log 
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true

