# Generated with AI assistance for the Z-Lander project.

# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
RAYLIB_DIR = external/raylib

# Include paths for header files
INCLUDES = -I$(INC_DIR) -I$(RAYLIB_DIR)/src

# Library paths and linker flags (includes Linux system dependencies for Raylib)
LDFLAGS = -L$(RAYLIB_DIR)/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Output binary name
TARGET = z-lander

# Automatically detect all .c source files in src/
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Default build target
all: $(BUILD_DIR) $(TARGET)

# Create build directory if it does not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile C source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
