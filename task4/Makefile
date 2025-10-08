DATADIR = data
SRCDIR = src
OBJDIR = build
BINDIR = build

EXT_SRC = .cpp
EXT_OBJ = .o
EXT_BIN = .out
EXT_DOT = .dot

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

DIJKSTRA_SRC = $(SRCDIR)/dijkstra.cpp
BELLMANFORD_SRC = $(SRCDIR)/bellmanford.cpp
DIJKSTRA_OBJ = $(OBJDIR)/dijkstra.o
BELLMANFORD_OBJ = $(OBJDIR)/bellmanford.o

DOT_DIJKSTRA = $(DATADIR)/dijkstra.dot
DOT_BELLMANFORD = $(DATADIR)/bellmanford.dot
PDF_DIJKSTRA = $(DATADIR)/dijkstra.pdf
PDF_BELLMANFORD = $(DATADIR)/bellmanford.pdf


all: $(BINDIR)/dijkstra$(EXT_BIN) $(BINDIR)/bellmanford$(EXT_BIN)

run: dijkstra bellmanford

$(BINDIR)/dijkstra$(EXT_BIN): $(DIJKSTRA_OBJ) | $(DATADIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINDIR)/bellmanford$(EXT_BIN): $(BELLMANFORD_OBJ) | $(DATADIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

dijkstra: $(BINDIR)/dijkstra$(EXT_BIN)
	./$< -graph
	dot $(DOT_DIJKSTRA) -Tpdf -o $(PDF_DIJKSTRA)

bellmanford: $(BINDIR)/bellmanford$(EXT_BIN)
	./$< -graph
	dot $(DOT_BELLMANFORD) -Tpdf -o $(PDF_BELLMANFORD)

$(OBJDIR) $(DATADIR):
	@mkdir -p $@

clean:
	@rm -rf $(OBJDIR)/*.o $(BINDIR)/*.out $(DATADIR)/*.pdf $(DATADIR)/*.dot
