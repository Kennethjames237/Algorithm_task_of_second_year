# Directory structure
SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := $(BUILD_DIR)/bin
OBJ_DIR   := $(BUILD_DIR)/obj
DATA_DIR  := $(BUILD_DIR)/data

# Compiler and flags
CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -pedantic --debug

# Source files and targets
SRC       := $(SRC_DIR)/main.cpp
TARGET    := $(BIN_DIR)/program
OBJ       := $(OBJ_DIR)/main.o

# Ensure directory structure exists
DIRS      := $(BIN_DIR) $(OBJ_DIR) $(DATA_DIR)

# Default target
.PHONY: all
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJ) | $(BIN_DIR)
	@echo "Linking $@..."
	@$(CXX) $(LDFLAGS) $^ -o $@
	@echo "Build complete!"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Create necessary directories
$(DIRS):
	@mkdir -p $@

# Run the program to generate dot files and convert to PDF
.PHONY: run
run: $(TARGET)
	@echo "Running program..."
	@mkdir -p $(DATA_DIR)
	@cd $(BUILD_DIR) && ../$(TARGET)
	@echo "Program execution complete."
	@echo "Converting DOT files to PDF..."
	@for file in $(BUILD_DIR)/*.dot; do \
		if [ -f "$$file" ]; then \
			filename=$$(basename "$$file" .dot); \
			echo "Processing $$filename..."; \
			dot -Tpdf "$$file" -o "$(DATA_DIR)/$$filename.pdf"; \
			mv "$$file" "$(DATA_DIR)/$$filename.dot"; \
		fi; \
	done
	@echo "PDF conversion complete."


# Clean the build files
.PHONY: clean
clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR)
	@echo "Clean complete."

# Show help information
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all       - Build the executable (default)"
	@echo "  run       - Run the program to generate DOT files and convert to PDF"
	@echo "  clean     - Remove all build files"
	@echo "  help      - Display this help message"
