
CC = g++

GTEST = gtest

LIBS = -L ./$(GTEST)/
LIBS += -lgtest
LIBS += -lpthread

#CFLAGS = -fprofile-arcs -ftest-coverage
#CFLAGS = -fpermissive

CFLAGS =
INCLUDE = -I ./test/ \
	  -I ./ring_buffer/ \
	  -I $(GTEST)/ \
	  -I ./ 

TARGET_SRC  = $(wildcard ./ring_buffer/*.c)
TARGET_OBJ = $(patsubst %.c, %.o, $(TARGET_SRC))
COBJS = $(TARGET_OBJ)

TEST_SRC = $(wildcard ./test/*.cpp)
TEST_OBJ = $(patsubst %.cpp,%.o, $(TEST_SRC))
CPPOBJS = $(TEST_OBJ)

MAIN_SRC = $(wildcard ./*.cpp)
MAIN_OBJ =$(patsubst %.cpp,%.o, $(MAIN_SRC))
CPPOBJS += $(MAIN_OBJ) 

SRCS = $(TARGET_SRC) $(TEST_SRC) $(MAIN_SRC)
OBJS = $(TARGET_OBJ) $(TEST_OBJ) $(MAIN_OBJ)

TARGET = main

.PHONY: all clean test dump

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS) $(CFLAGS) $(INCLUDE)

$(COBJS): %.o:%.c
	$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $< 

$(CPPOBJS): %.o:%.cpp
	$(CC) $(INCLUDE) $(CFLAGS) -o $@ -c $<

test:
	./$(TARGET)

clean:
	@rm -rf $(TARGET) $(OBJS)
	@echo "clean success"

dump:
	@echo "CC:$(CC)"
	@echo "`$(CC) -v`"
	@echo "INC: $(INCLUDE)"
	@echo "LIB: $(LIBS)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "SRCS: $(SRCS)"
	@echo "OBJS: $(OBJS)"
	@echo "TARGET: $(TARGET)"


