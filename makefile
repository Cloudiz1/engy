CXX      := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -std=c++20

SRC_DIR  := src
OBJ_DIR  := build
BIN_DIR  := bin

TARGET   := $(BIN_DIR)/out
SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

.PHONY: all clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
