# Generated with AI assistance for the Z-Lander project.

# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -Wstrict-prototypes -std=c99 -g

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
RAYLIB_DIR = external/raylib
RAYLIB_LIB = $(RAYLIB_DIR)/src/libraylib.a

# Include paths for header files
INCLUDES = -I$(INC_DIR) -I$(RAYLIB_DIR)/src

# Library paths and linker flags (includes Linux system dependencies for Raylib)
LDFLAGS = -L$(RAYLIB_DIR)/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Output binary name
TARGET = z-lander

# Automatically detect all .c source files in src/ and .h header files in include/
SRCS = $(wildcard $(SRC_DIR)/*.c)
HDRS = $(wildcard $(INC_DIR)/*.h)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Default build target
all: $(BUILD_DIR) $(RAYLIB_LIB) $(TARGET)

# Create build directory if it does not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clone Raylib if the directory is missing
$(RAYLIB_DIR):
	mkdir -p external
	git clone https://github.com/raysan5/raylib.git $(RAYLIB_DIR)

# Build the Raylib static library if it does not exist
$(RAYLIB_LIB): | $(RAYLIB_DIR)
	cd $(RAYLIB_DIR)/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP

# Link object files into the final executable
$(TARGET): $(OBJS) $(RAYLIB_LIB)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile C source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS) | $(BUILD_DIR) $(RAYLIB_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean project build artifacts (leaves Raylib intact)
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Clean everything including external Raylib clone
clean-all: clean
	rm -rf $(RAYLIB_DIR)

.PHONY: all clean clean-all
