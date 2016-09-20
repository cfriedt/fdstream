TAG := fdstream

CC := clang
CXX := clang++
LD := $(CXX)

LIB := lib$(TAG).a
LIBTEST := lib$(TAG)-test.a

TOPDIR := $(shell pwd)

SRCDIR := src
TESTDIR := test

C_SRC := $(shell find $(SRCDIR) -name '*.c')
C_OBJ := $(C_SRC:.c=.o)
$(info C_SRC := '$(C_SRC)' C_OBJ := '$(C_OBJ)')

CPP_SRC := $(shell find $(SRCDIR) -name '*.cpp')
CPP_OBJ := $(CPP_SRC:.cpp=.o)
$(info CPP_SRC := '$(CPP_SRC)' CPP_OBJ := '$(CPP_OBJ)')

OBJ := $(C_OBJ) $(CPP_OBJ)
$(info LIB := '$(LIB)' OBJ := '$(OBJ)')

LIBTEST_C_SRC := $(shell find $(TESTDIR) -name '*.c')
LIBTEST_C_OBJ := $(LIBTEST_C_SRC:.c=.o)
$(info LIBTEST_C_SRC := '$(LIBTEST_C_SRC)' LIBTEST_C_OBJ := '$(LIBTEST_C_OBJ)')

LIBTEST_CPP_SRC := $(shell find $(TESTDIR) -name '*.cpp')
LIBTEST_CPP_OBJ := $(LIBTEST_CPP_SRC:.cpp=.o)
$(info LIBTEST_CPP_SRC := '$(LIBTEST_CPP_SRC)' LIBTEST_CPP_OBJ := '$(LIBTEST_CPP_OBJ)')

LIBTEST_OBJ := $(LIBTEST_C_OBJ) $(LIBTEST_CPP_OBJ)
$(info LIBTEST := '$(LIBTEST)' LIBTEST_OBJ := '$(LIBTEST_OBJ)')

TEST_SRC := $(shell find $(TESTDIR) -name '*.cc')
TEST_EXE := $(TEST_SRC:.cc=)
$(info TEST_SRC := '$(TEST_SRC)' TEST_EXE := '$(TEST_EXE)')

$(info CFLAGS := '$(CFLAGS)')
$(info CXXFLAGS := '$(CXXFLAGS)')

CFLAGS :=
CFLAGS += -ferror-limit=1
CFLAGS += -Wall
CFLAGS += -O0 -g
CFLAGS += -I$(TOPDIR)/include

CXXFLAGS :=
CXXFLAGS += $(CFLAGS)

CFLAGS   += -std=c99
CXXFLAGS += -std=c++11

$(info CFLAGS := '$(CFLAGS)')
$(info CXXFLAGS := '$(CXXFLAGS)')

LDFLAGS :=
LDFLAGS += -O0

TEST_LDLIBS :=
TEST_LDLIBS += -lgtest -lgtest_main 

.PHONY: all clean check

all: check $(LIB) $(LIBTEST)

clean:
	rm -Rf $(TESTEXE) $(LIB) $(LIBTEST) $(OBJ) $(LIBTEST_OBJ) $(shell find * -name '*.dSYM')

%: %.cc $(HDR) $(LIB) $(LIBTEST) Makefile
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIB) $(LIBTEST) $(LDLIBS)

%.o: %.c $(HDR) Makefile
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.cpp $(HDR) Makefile
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(LIB): $(OBJ)
	libtool -static -o $@ $^

$(LIBTEST): $(LIBTEST_OBJ)
	libtool -static -o $@ $^

check: $(TESTEXE)
	for i in $^; do \
		./$$i || true; \
	done
