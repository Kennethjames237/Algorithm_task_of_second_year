# directories variables
SRCDIR = src
DATADIR = data
OBJDIR = build
INCDIR = include

TARGET = $(OBJDIR)/main$(EXT_BIN)

# extensions
EXT_SRC = .cpp
EXT_HDR = .hpp
EXT_OBJ = .o
EXT_BIN = .out
EXT_DOT = .dot

# compiler variables
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
LDFLAGS = $(shell pkg-config --libs --cflags libgvc) 

# files
SRCS = $(wildcard $(SRCDIR)/*$(EXT_SRC))
OBJS = $(SRCS:$(SRCDIR)/%$(EXT_SRC)=$(OBJDIR)/%$(EXT_OBJ))
HEADERS = $(wildcard $(INCDIR)/*$(EXT_HDR))

DOT_FILE = $(DATADIR)/tree$(EXT_DOT)
DOT_FILE_FLIP= $(DATADIR)/flipped$(EXT_DOT)
GVPR_SCRIPT = $(DATADIR)/binarytree.gvpr

# rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%$(EXT_OBJ): $(SRCDIR)/%$(EXT_SRC) $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@ $(LDFLAGS)

run: $(TARGET)
	@mkdir -p $(DATADIR)
	./$(TARGET) $(DOT_FILE) $(DOT_FILE_FLIP)
	@dot -Tdot $(DOT_FILE) | gvpr -c -f $(GVPR_SCRIPT) | neato -n -Tpdf -o $(DATADIR)/tree.pdf
	@dot -Tdot $(DOT_FILE_FLIP) | gvpr -c -f $(GVPR_SCRIPT) | neato -n -Tpdf -o $(DATADIR)/flipped.pdf

debug: $(TARGET)
	@mkdir -p $(DATADIR)
	gdb --args  ./$(TARGET) $(DOT_FILE) $(DOT_FILE_FLIP)
	@dot -Tdot $(DOT_FILE) | gvpr -c -f $(GVPR_SCRIPT) | neato -n -Tpdf -o $(DATADIR)/tree.pdf
	@dot -Tdot $(DOT_FILE_FLIP) | gvpr -c -f $(GVPR_SCRIPT) | neato -n -Tpdf -o $(DATADIR)/flipped.pdf

clean:
	rm -rf $(OBJS) $(TARGET) $(DATADIR)/tree.pdf $(DATADIR)/flipped.pdf $(DOT_FILE) $(DOT_FILE_FLIP)

.PHONY: all run clean