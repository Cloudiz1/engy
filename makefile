CXX      := g++
BASE_FLAGS := -Wall -Wextra -pedantic -std=c++20 -MMD -MP

SRC_DIR  := src
OBJ_BASE := build
BIN_DIR  := bin

# Default build type
BUILD_TYPE ?= debug

# Configure flags, object directories, and target names dynamically
ifeq ($(BUILD_TYPE),prod)
    CXXFLAGS    := $(BASE_FLAGS) -O3 -DNDEBUG
    OBJ_DIR     := $(OBJ_BASE)/prod
    TARGET_NAME := out_prod
else
    CXXFLAGS    := $(BASE_FLAGS) -g -O0 -DDEBUG
    OBJ_DIR     := $(OBJ_BASE)/debug
    TARGET_NAME := out_debug
endif

TARGET   := $(BIN_DIR)/$(TARGET_NAME)
SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS     := $(OBJS:.o=.d)

# Shortcut targets
.PHONY: debug prod
debug:
	@$(MAKE) BUILD_TYPE=debug all

prod:
	@$(MAKE) BUILD_TYPE=prod all

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

-include $(DEPS)

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

.PHONY: all clean
clean:
	rm -rf $(OBJ_BASE) $(BIN_DIR)
