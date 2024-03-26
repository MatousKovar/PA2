
CXX=g++
CXXFLAGS=-Wall -pedantic -g -std=c++17

TARGET=kovarm46
SOURCES=$(wildcard src/*.cpp)
HEADERS = $(wildcard src/*.h)
#each source file has to have object file also

#patsubst from,to,source
OBJECTS= $(patsubst src/%.cpp,build/%.o,$(SOURCES))
DEPS = $(patsubst src/%.cpp, build/%.dep, $(SOURCES))

all: compile doc

run: compile
	./$(TARGET)

doc: Doxyfile $(HEADERS)
	doxygen Doxyfile

compile: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/%.o : src/%.cpp
	#creates directory if needed
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) build/ doc/

build/%.dep: src/%.cpp src/*
	mkdir -p build/
	$(CXX) -MM -MT $(patsubst src/%.cpp, build/%.o, $<) $< > $@

include $(DEPS)
