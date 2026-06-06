CC = g++
CPPFLAGS = -g -Wall -Wextra -Werror -std=c++17 -I./inc -I./libs/glfw/include
CPPFLAGS += -D_GLFW_USE_OPENGL

NAME = scop

SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = libs
GLFW_DIR = $(LIB_DIR)/glfw
GLFW_BUILD = $(GLFW_DIR)/build

SRC = $(shell find $(SRC_DIR) -type f -name "*.cpp")
C_SRC = $(shell find $(SRC_DIR) -type f -name "*.c")
ALL_SRC = $(SRC) $(C_SRC)

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC)) \
       $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SRC))

LIBS = -lGL -lX11 -lXrandr -lXinerama -lXcursor -lXi -ldl -lpthread

all: configure $(NAME)

configure:
	@if [ ! -d $(GLFW_DIR) ]; then \
		echo "GLFW not found. Downloading..."; \
		mkdir -p $(LIB_DIR); \
		cd $(LIB_DIR) && wget https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip && \
		unzip glfw-3.4.zip && mv glfw-3.4 glfw && rm glfw-3.4.zip; \
	fi
	@if [ ! -f $(GLFW_BUILD)/Makefile ]; then \
		mkdir -p $(GLFW_BUILD); \
		cd $(GLFW_BUILD) && cmake .. -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_WAYLAND=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF; \
	fi
	$(MAKE) -C $(GLFW_BUILD)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CPPFLAGS) -o $(NAME) $(OBJS) $(GLFW_BUILD)/src/libglfw3.a $(LIBS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -rf $(GLFW_BUILD)

re: fclean all

.PHONY: all clean fclean re configure
