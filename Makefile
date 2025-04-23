CXX = g++
CXXFLAGS = -std=c++14 -Wall -I.

TARGET = simple_parser_demo

SOURCES = main.cc ast.cc ASTVis.cc simple_parser.cc

OBJECTS = $(SOURCES:.cc=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET) sample_api.txt

# Object rules
main.o: main.cc simple_parser.hh ast.hh ASTVis.hpp
	$(CXX) $(CXXFLAGS) -c main.cc

ast.o: ast.cc ast.hh ASTVis.hpp
	$(CXX) $(CXXFLAGS) -c ast.cc

ASTVis.o: ASTVis.cc ASTVis.hpp
	$(CXX) $(CXXFLAGS) -c ASTVis.cc

simple_parser.o: simple_parser.cc simple_parser.hh ast.hh
	$(CXX) $(CXXFLAGS) -c simple_parser.cc

.PHONY: all clean run
