# =====================
#  Compiler Configuration
# =====================
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g \
         -fsanitize=address -fno-omit-frame-pointer \
         -Iinclude \
         $(shell pkg-config --cflags sdl2 SDL2_image SDL2_mixer)
LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_image SDL2_mixer) \
          -fsanitize=address

# =====================
#  Project Structure
# =====================
SRC_DIR = src
BUILD_DIR = build
SOURCES = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDS = $(OBJECTS:.o=.d)

# =====================
#  Build Targets
# =====================
TARGET = game
.PHONY: all clean debug release

all: $(BUILD_DIR) debug

# Create build directory first
$(BUILD_DIR):
	@mkdir -p $@

debug: CFLAGS += -DDEBUG -O0
debug: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

release: CFLAGS += -O3 -flto
release: LDFLAGS += -flto
release: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

# Link executable
$(BUILD_DIR)/$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPENDS)