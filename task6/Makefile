SRCDIR = src
DATADIR = data
OBJDIR = build
INCDIR = include

TARGET = $(OBJDIR)/main$(EXT_BIN)

MODIFIED_FILE=$(DATADIR)/modified.cpp
ORIGINAL_FILE=$(DATADIR)/original.cpp
DIFFER_FILE=$(DATADIR)/differ.txt

EXT_SRC = .cpp
EXT_HDR = .hpp
EXT_OBJ = .o
EXT_BIN = .out

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

SRCS = $(wildcard $(SRCDIR)/*$(EXT_SRC))
OBJS = $(SRCS:$(SRCDIR)/%$(EXT_SRC)=$(OBJDIR)/%$(EXT_OBJ))
HEADERS = $(wildcard $(INCDIR)/*$(EXT_HDR))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%$(EXT_OBJ): $(SRCDIR)/%$(EXT_SRC) $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

run: $(TARGET)
	./$(TARGET) $(ORIGINAL_FILE) $(MODIFIED_FILE) $(DIFFER_FILE)

debug: $(TARGET)
	gdb --args ./$(TARGET) $(ORIGINAL_FILE) $(MODIFIED_FILE) $(DIFFER_FILE)


clean:
	rm -rf $(OBJS) $(TARGET)

.PHONY: all run clean