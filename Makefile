CC  := cc
CXX := c++

CFLAGS   := -g -Wall -Wextra -Werror -I./inc -I./libs/glfw/include
CXXFLAGS := -g -Wall -Wextra -Werror -std=c++17 -I./inc -I./libs/glfw/include

CFLAGS   += -D_GLFW_USE_OPENGL
CXXFLAGS += -D_GLFW_USE_OPENGL

NAME = scop

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = libs
GLFW_DIR = $(LIB_DIR)/glfw
GLFW_BUILD = $(GLFW_DIR)/build

SRC_CPP := $(shell find $(SRC_DIR) -type f -name "*.cpp")
SRC_C   := $(shell find $(SRC_DIR) -type f -name "*.c")

OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_CPP)) \
        $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_C))

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	LIBS := -lGL -lX11 -lXrandr -lXinerama -lXcursor -lXi -ldl -lpthread
endif

ifeq ($(UNAME_S),Darwin)
	LIBS := \
	-framework Cocoa \
	-framework IOKit \
	-framework CoreVideo \
	-framework OpenGL
endif

all: configure $(NAME)

configure:
	@if [ ! -d "$(GLFW_DIR)" ]; then \
		echo "GLFW not found. Downloading..."; \
		mkdir -p $(LIB_DIR); \
		cd $(LIB_DIR) && \
		curl -L -o glfw-3.4.zip https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip && \
		unzip glfw-3.4.zip && \
		mv glfw-3.4 glfw && \
		rm glfw-3.4.zip; \
	fi

	@if [ ! -f "$(GLFW_BUILD)/Makefile" ]; then \
		mkdir -p $(GLFW_BUILD); \
		cd $(GLFW_BUILD) && \
		cmake .. \
			-DGLFW_BUILD_EXAMPLES=OFF \
			-DGLFW_BUILD_TESTS=OFF \
			-DGLFW_BUILD_DOCS=OFF \
			-DGLFW_BUILD_WAYLAND=OFF; \
	fi

	$(MAKE) -C $(GLFW_BUILD)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(GLFW_BUILD)/src/libglfw3.a $(LIBS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -rf $(GLFW_BUILD)

re: fclean all

.PHONY: all configure clean fclean re
