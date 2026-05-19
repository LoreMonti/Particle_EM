CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wall -Wextra

TARGET = main

SOURCES = main.cpp fields.cpp integrators.cpp initialization.cpp output.cpp diagnostics.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp particle_em.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)