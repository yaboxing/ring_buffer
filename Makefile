CC = g++

gtest = gtest

LIBS = -L ./$(gtest)/
LIBS += -lgtest
LIBS += -lpthread

#CFLAGS = -fprofile-arcs -ftest-coverage
CFLAGS = -fpermissive

INCLUDE = -I ./test/ \
	 -I ./ring_buffer/ \
	 -I ./gtest \
	 -I ./ 

TARGET_SRC  = $(wildcard ./ring_buffer/*.c)
TARGET_OBJ = $(patsubst %.c, %.o, $(TARGET_SRC))

TEST_SRC = $(wildcard ./test/*.cpp)
TEST_OBJ = $(patsubst %.cpp,%.o, $(TEST_SRC))

MAIN_SRC = $(wildcard ./$(gtest)/main/*.cpp)
MAIN_OBJ =$(patsubst %.cpp,%.o, $(MAIN_SRC))

TARGET = main

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(TARGET_OBJ) $(TEST_OBJ) $(MAIN_OBJ)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS) $(INCLUDE)

.c.o:$(TARGET_SRC)
	$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $< 

.cpp.o:$(TEST_SRC) $(MAIN_OBJ)
	$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $<

SUBDIR = $(shell ls ./ -R | grep /)
SUBDIRS = $(subst :,/,$(SUBDIR))
SOURCE = $(foreach dir, $(SUBDIRS),$(wildcard $(dir)*.o))

test:
	./$(TARGET)

clean:
	@rm -rf *~ $(SOURCE) $(TARGET)
	@find . -name "*.gc[dn][ao]" | xargs rm -rf
	@echo "clean success"
