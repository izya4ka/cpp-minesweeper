TARGET := Minesweeper
CXX := clang++
CXXFLAGS := -march=native -O2 -pipe -Wall -Wextra -std=c++20

PREF_SRC := ./src
PREF_OBJ := ./build

all: $(TARGET) run

SRC := $(shell find . -name "*.cpp")
OBJ := $(patsubst $(PREF_SRC)/%.cpp, $(PREF_OBJ)/%.o, $(SRC))

$(PREF_OBJ)/%.o : $(PREF_SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET) : $(OBJ)
	$(CXX) $(CXXFLAGS) -lncurses $(OBJ) -o $(TARGET)

run : $(TARGET)
	./$(TARGET)

clean :
	rm $(TARGET) $(OBJ)