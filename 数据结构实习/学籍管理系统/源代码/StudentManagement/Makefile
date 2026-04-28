# 定义编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I./include
LDFLAGS = 

# 定义目录
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# 定义源文件和目标文件
SOURCES = $(SRC_DIR)/Utils.cpp $(SRC_DIR)/User.cpp $(SRC_DIR)/Student.cpp \
          $(SRC_DIR)/Course.cpp $(SRC_DIR)/Selection.cpp $(SRC_DIR)/Score.cpp \
          $(SRC_DIR)/System.cpp main.cpp

OBJECTS = $(BUILD_DIR)/Utils.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Student.o \
          $(BUILD_DIR)/Course.o $(BUILD_DIR)/Selection.o $(BUILD_DIR)/Score.o \
          $(BUILD_DIR)/System.o $(BUILD_DIR)/main.o

# 定义可执行文件名
TARGET = $(BIN_DIR)/StudentManagement

# 默认目标
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "编译完成！可执行文件：$(TARGET)"

# 编译源文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: main.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理编译文件
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "清理完成！"

# 运行程序
run: $(TARGET)
	$(TARGET)

.PHONY: all clean run
