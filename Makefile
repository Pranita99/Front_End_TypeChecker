CXX = g++
CXXFLAGS = -std=c++14 -Wall -g
LEX = flex
YACC = bison

all: api_parser

# First, generate the parser
parser.tab.cc parser.tab.hh: parser.yy
	$(YACC) -d parser.yy

# Then, generate the lexer (after parser.tab.hh is available)
lex.yy.c: lexer.ll parser.tab.hh
	$(LEX) -o lex.yy.c lexer.ll

# Build the parser
api_parser: parser.tab.cc lex.yy.c ast.cc ASTVis.cc main.cc
	$(CXX) $(CXXFLAGS) -o api_parser parser.tab.cc lex.yy.c ast.cc ASTVis.cc main.cc

clean:
	rm -f api_parser lex.yy.c parser.tab.cc parser.tab.hh
	rm -f *.o *~

.PHONY: all clean
