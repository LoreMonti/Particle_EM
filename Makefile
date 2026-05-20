CXX = g++
CXXFLAGS = -O3 -std=c++11 -Wall -Wextra

SRC = main.cpp fields.cpp integrators.cpp initialization.cpp output.cpp diagnostics.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)
	@rm -f $(OBJ)
	@echo "Build completed successfully."

%.o: %.cpp particle_em.h
	@$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	@./$(TARGET)

clean:
	@rm -f $(OBJ) $(TARGET)
	@echo "Clean completed."

.PHONY: all run clean