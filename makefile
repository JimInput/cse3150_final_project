# === Compiler and Flags ===
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS :=
TEST_LIBS := -lgtest_main -lgtest -pthread

# Auto header dependency generation
DEPFLAGS := -MMD -MP

# === Directories ===
SRC_DIR := src
INC_DIR := include
TEST_DIR := tests
BUILD_DIR := build

# === Output Binaries ===
PROGRAM_OUT := $(BUILD_DIR)/program.out
TEST_OUT := $(BUILD_DIR)/test.out

# === Source Discovery ===
SRC_FILES  := $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES := $(wildcard $(TEST_DIR)/*.cpp)

# Entry point for the program; override with:
#   make MAIN_SRC=src/app_main.cpp
MAIN_SRC ?= $(SRC_DIR)/main.cpp

# === Objects ===
# Map sources -> objects (flat layout in build/)
OBJ_ALL      := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TEST_OBJ     := $(TEST_FILES:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/test_%.o)

# Object corresponding to MAIN_SRC and objects excluding it
MAIN_OBJ     := $(BUILD_DIR)/$(notdir $(MAIN_SRC:.cpp=.o))
OBJ_NO_MAIN  := $(filter-out $(MAIN_OBJ),$(OBJ_ALL))

# Dependencies (.d files emitted by -MMD)
DEPS := $(OBJ_ALL:.o=.d) $(TEST_OBJ:.o=.d)

# === Default Target ===
all: $(PROGRAM_OUT) $(TEST_OUT)

# === Link Binaries ===
# Program links ALL src (incl. main)
$(PROGRAM_OUT): $(OBJ_ALL) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Tests link: all src EXCEPT main + all test objs, with gtest_main
$(TEST_OUT): $(OBJ_NO_MAIN) $(TEST_OBJ) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(TEST_LIBS) $(LDFLAGS)

# === Compile Rules ===
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

# === Build Dir ===
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# === Convenience Targets ===
program: $(PROGRAM_OUT)
testbin: $(TEST_OUT)
test: $(TEST_OUT)
	$(TEST_OUT)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean program testbin test

# Include auto-generated header dependencies (safe if missing)
-include $(DEPS)
