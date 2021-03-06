MAIN_TARGET	:= finale

SRC_DIR		:= src
INC_DIR		:= src
DEP_DIR     := .dep
BUILD_DIR	:= .build

CC              := clang++
CFLAGS          := -std=c++14 -O2
CFLAGS1	        := -std=c++14 -O0 -g
INCS			:= -I$(INC_DIR)
HEADERS         := $(shell find $(INC_DIR) -name '*.h' -type 'f' | sort)
MAIN_SOURCES    := $(shell find $(SRC_DIR) -name '*.cpp' -type 'f' | sort)
MAIN_OBJECTS    := $(MAIN_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPFILES        := $(MAIN_SOURCES:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.dep)

all : $(DEPFILES) $(MAIN_TARGET)

# Linking all objects with libs into executable.
$(MAIN_TARGET): $(MAIN_OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Compilation of source files into object files, with help of depedency files.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.dep
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

# Rule for creating dependency files.
$(DEP_DIR)/%.dep: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCS) -MM -MT '$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$<)' $< -MF $@

include $(DEPFILES)

clean:
	rm -rf $(BUILD_DIR) $(DEP_DIR) $(MAIN_TARGET)
