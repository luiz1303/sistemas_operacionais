CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# nome do executável
TARGET = main 

# Arquivos fonte
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Headers
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Lista de objetos
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean