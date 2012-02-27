CFLAGS=-g -O2 -Wall -Wextra -Isrc -pthread -rdynamic -DNDEBUG $(OPTFLAGS) -D_FILE_OFFSET_BITS=64
LIBS=-ldl $(OPTLIBS)
PREFIX?=/usr/local

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,${SOURCES})
TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,${TEST_SRC})
MAKEOPTS=OPTFLAGS="${NOEXTCFLAGS} ${OPTFLAGS}" OPTLIBS="${OPTLIBS}" LIBS="${LIBS}" DESTDIR="${DESTDIR}" PREFIX="${PREFIX}"

all: build/liblcthw.a tests

dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS) -D_FILE_OFFSET_BITS=64
dev: all

${OBJECTS_NOEXT}: CFLAGS += ${NOEXTCFLAGS}

build/liblcthw.a: CFLAGS += -fPIC
build/liblcthw.a: build ${OBJECTS}
	ar rcs $@ ${OBJECTS}
	ranlib $@

build:
	@mkdir -p build
	@mkdir -p bin

clean:
	rm -rf build ${OBJECTS} ${TESTS}
	rm -f tests/tests.log 
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

.PHONY: tests
tests: ${TESTS}
	sh ./tests/runtests.sh

$(TESTS): %: %.c build/liblcthw.a
	$(CC) $(CFLAGS) -o $@ $< build/liblcthw.a $(LIBS)

check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)' $(filter-out src/bstr/bsafe.c,${SOURCES})

install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install lib/liblcthw.a $(DESTDIR)/$(PREFIX)/lib/

valgrind:
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" ${MAKE}

coverage: NOEXTCFLAGS += -fprofile-arcs -ftest-coverage
coverage: LIBS += -lgcov
coverage: LDFLAGS += -fprofile-arcs
coverage: clean all coverage_report

coverage_report:
	rm -rf tests/m2.zcov tests/coverage
	zcov-scan tests/m2.zcov
	zcov-genhtml --root $(CURDIR) tests/m2.zcov tests/coverage
	zcov-summarize tests/m2.zcov


