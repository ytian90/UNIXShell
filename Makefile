# Test Program for Project 1 CSC716
# Author: YU TIAN

# the compiler
CC = clang++

# compiler flags:
CFLAGS = -std=c++11 -stdlib=libc++ -Weverything

# the build target executable
TARGET = p1_YUTIAN_CSC716

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
