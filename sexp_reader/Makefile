# Copyright (c) 2012 Li Gong. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.


GTEST_PATH = /your/gtest/path

TEST_LIB = $(GTEST_PATH)/libgtest.a -lpthread

CXXFLAGS += -Wall -I. -I$(GTEST_PATH)/include $(OPT)
CFLAGS = $(CXXFLAGS)
LIBS +=

SOURCES = $(shell find . -name '*test*.cc' -prune -o -name '*.c' -print | sort | tr "\n" " ")
TEST_CXX_SOURCES = $(shell find . -name '*test*.cc' | sort | tr "\n" " ")
TEST_C_SOURCES += symbol.c mem.c token_stream.c sexp.c reader.c

OBJECTS = $(SOURCES:.c=.o)
TEST_OBJECTS = $(TEST_CXX_SOURCES:.cc=.o) $(TEST_C_SOURCES:.c=.o) 

test: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o test $(TEST_LIB)

all: readsexp

readsexp: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o readsexp

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

run_test: test
	./test

clear:
	find . -name "*.o" -o -name test -o -name readsexp |xargs rm