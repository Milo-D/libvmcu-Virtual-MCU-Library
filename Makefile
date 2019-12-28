CXX      := -g++
CXXFLAGS := -D
LDFLAGS  := -L /usr/lib -lstdc++ -lm
MCU_ARCH := ATMEGA32 
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := mdx
INCLUDE  := -I include/
SRC      :=                      	    \
   $(wildcard src/cli/*.cpp) 			\
   $(wildcard src/disassembler/*.cpp) 	\
   $(wildcard src/instructions/*.cpp) 	\
   $(wildcard src/misc/*.cpp)         	\
   $(wildcard src/parser/*.cpp)	      	\
   $(wildcard src/system/*.cpp)        	\
   $(wildcard src/table/*.cpp)       	\

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(MCU_ARCH) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(MCU_ARCH) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJECTS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*
