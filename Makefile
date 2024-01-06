include .conan/conandeps.mk

CXX ?= g++
CXXFLAGS = -std=c++20 -fPIC -O3 -pedantic -Werror -Wall -Wextra $(shell pkg-config --cflags python3) \
		   -Iinclude/ -Isrc/ -I$(CONAN_INCLUDE_DIRS)
CXXFLAGS_TEST = -std=c++20 -fPIC -g -O0 $(shell pkg-config --cflags python3) -Iinclude/ -Isrc/ -I$(CONAN_INCLUDE_DIRS)
LDFLAGS = -shared -fPIC $(shell python3-config --libs --embed) -Ltarget/ -Wl,-rpath target
LDFLAGS_TEST = $(shell python3-config --libs --embed) -Ltarget -l$(CONAN_LIBS_GTEST_LIBGTEST) \
 			   -l$(CONAN_LIBS_GTEST_GTEST_MAIN)

SOURCES = $(shell find src/ -name '*.cc')
OBJECTS = $(patsubst src/%.cc,target/src/%.o,$(SOURCES))
SOURCE_DIRS = $(shell find src -type d)
TEST_DIRS = $(shell find test -type d)

target:
	mkdir target/ && cd target && mkdir -p $(SOURCE_DIRS) $(TEST_DIRS)

target/src/%.o: src/%.cc | target
	$(CXX) $(CXXFLAGS_TEST) -MMD -MP -c $< -o $@

libmamba.so: $(OBJECTS)
	$(CXX) $(LDFLAGS) -lmamba $^ -o target/$@

libmamba-debug.so: $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o target/$@


TEST_SOURCES = $(shell find test/ -name '*.cc')
TEST_OBJECTS = $(patsubst test/%.cc,target/test/%.o,$(TEST_SOURCES))

target/test/%.o: test/%.cc | target
	$(CXX) $(CXXFLAGS_TEST) -MMD -MP -c $< -o $@

tests: $(TEST_OBJECTS) | libmamba-debug.so
	$(CXX) $(LDFLAGS_TEST) -g -lmamba-debug $^ -o target/$@

all: libmamba.so tests

clean:
	rm -rf target/

.PHONY: clean