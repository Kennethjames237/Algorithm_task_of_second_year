# directories variables
DATADIR = data
SRCDIR = src
OBJDIR = build

TARGET = $(OBJDIR)/main$(EXT_BIN)

# extensions
EXT_SRC = .cpp
EXT_OBJ = .o
EXT_BIN = .out
EXT_DOT = .dot

# compiler variables
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# files
SRCS = $(wildcard $(SRCDIR)/*$(EXT_SRC))
OBJS = $(SRCS:$(SRCDIR)/%$(EXT_SRC)=$(OBJDIR)/%$(EXT_OBJ))

DOT_FILE = ./graph$(EXT_DOT)
PDF_FILE = $(DATADIR)/graph.pdf

# rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%$(EXT_OBJ): $(SRCDIR)/%$(EXT_SRC)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	@mkdir -p $(DATADIR)
	./$(TARGET) -graph 
	@dot $(DOT_FILE) -Tpdf -o $(PDF_FILE)
	@rm -rf $(DOT_FILE)
clean:
	rm -rf $(OBJS) $(TARGET) $(DOT_FILE)

.PHONY: all run clean