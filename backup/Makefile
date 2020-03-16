# Name of the project
PROJ_NAME=main

#project paths
CPP_PATH=./src
HPP_PATH=./headers
BUILD_PATH=./build

# .cpp files
CPP_SOURCE=$(wildcard $(CPP_PATH)/*.cpp)
 
# .hpp files
HPP_SOURCE=$(wildcard $(HPP_PATH)/*.hpp)
 
# Object files
OBJ=$(subst .cpp,.o,$(subst $(CPP_PATH),$(BUILD_PATH),$(CPP_SOURCE)))
 
# Compiler
CC=g++
 
# Flags for compiler
CC_FLAGS=-c         \
         -W         \
         -Wall      \
         -g         \
         -I$(HPP_PATH)/

#
# Compilation and linking
#
all: buildFolder $(PROJ_NAME)
 
$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $^

$(BUILD_PATH)/%.o: $(CPP_PATH)/%.cpp $(HPP_PATH)/%.hpp
	$(CC) $(CC_FLAGS) -o $@ $<


$(BUILD_PATH)/main.o: $(CPP_PATH)/main.cpp $(HPP_SOURCE)
	$(CC) $(CC_FLAGS) -o $@ $<

clean:
	@ $(RM) $(BUILD_PATH)/*.o $(PROJ_NAME) *~
	@ rmdir $(BUILD_PATH)

buildFolder:
	@ mkdir -p $(BUILD_PATH)

.PHONY: all clean