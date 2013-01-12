program_NAME := vbo

CC=g++

program_CXX_SRCS := $(wildcard *.cpp)
OBJ_DIR := .obj
program_CXX_OBJS  := $(patsubst %,$(OBJ_DIR)/%,${program_CXX_SRCS:.cpp=.o})

program_INCLUDE_DIRS := include/SDL

program_LIBRARIES := GL glfw SDL SDL_image

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))

LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))

LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

$(OBJ_DIR)/%.o:%.cpp
	$(CC) -c -ggdb -o $@ $<

$(program_NAME): $(program_CXX_OBJS)
	$(CC) $(program_CXX_OBJS) -ggdb -Llib -framework Cocoa -framework OpenGL -lglew -lglfw -framework IOKit -framework SDL -framework SDL_image -o $(program_NAME)

clean:
	@- $(RM) bin/$(program_NAME)
	@- $(RM) $(program_CXX_OBJS)
