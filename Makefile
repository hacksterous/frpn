PROJECT_SOURCE_DIR := .
CXX := g++
CXXFLAGS := -Os -Wall -std=c++20 -I./linenoise-ng/include

RPN_SRC_FILES := \
    $(PROJECT_SOURCE_DIR)/src/object.cpp \
    $(PROJECT_SOURCE_DIR)/src/program.cpp \
    $(PROJECT_SOURCE_DIR)/src/main.cpp \
    $(PROJECT_SOURCE_DIR)/src/real-out.cpp \
    $(PROJECT_SOURCE_DIR)/src/lexer.cpp \
    $(PROJECT_SOURCE_DIR)/src/input.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-branch.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-complex.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-general.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-logs.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-program.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-real.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-stack.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-store.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-string.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-test.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-test-framework.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-time.cpp \
    $(PROJECT_SOURCE_DIR)/src/rpn-trig.cpp

LINENOISE_NG_SRC_FILES := \
    $(PROJECT_SOURCE_DIR)/linenoise-ng/src/ConvertUTF.cpp \
    $(PROJECT_SOURCE_DIR)/linenoise-ng/src/linenoise.cpp \
    $(PROJECT_SOURCE_DIR)/linenoise-ng/src/wcwidth.cpp

SRC_FILES := $(RPN_SRC_FILES) $(LINENOISE_NG_SRC_FILES)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

TARGET := frpn

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)

cleanall:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
