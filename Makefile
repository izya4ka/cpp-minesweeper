TARGET := Minesweeper
CXX := clang++
CXXFLAGS := -march=native -Ofast -pipe -g -Wall -Wextra -lncurses

PREF_SRC := ./src
PREF_OBJ := ./build

all: $(TARGET) run

SRC := $(shell find . -name "*.cpp")
OBJ := $(patsubst $(PREF_SRC)/%.cpp, $(PREF_OBJ)/%.o, $(SRC))

$(PREF_OBJ)/%.o : $(PREF_SRC)/%.cpp
	$(CXX) -c $< -o $@

$(TARGET) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

run : $(TARGET)
	./$(TARGET)

clean :
	rm $(TARGET) $(OBJ)