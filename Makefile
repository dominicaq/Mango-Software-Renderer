CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic
LDFLAGS = -lm
SRC_DIR = src
BUILD_DIR = build

# Define separate build directories for debug and release
DEBUG_BUILD_DIR = $(BUILD_DIR)/debug
RELEASE_BUILD_DIR = $(BUILD_DIR)/release

# Set the appropriate build directory based on the build type
ifdef DEBUG
  BUILD_DIR := $(DEBUG_BUILD_DIR)
  CFLAGS += -g -DDEBUG
else
  BUILD_DIR := $(RELEASE_BUILD_DIR)
  CFLAGS += -O3 -DNDEBUG
endif

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
EXECUTABLE = $(BUILD_DIR)/soft_render

all: $(BUILD_DIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXECUTABLE) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

gdb: $(EXECUTABLE)
	gdb ./$<

run: $(EXECUTABLE)
	./$<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf build
