NAME = webserv

CXX = g++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g

SRCS_DIR = srcs
SRCS_SUBDIRS = $(shell find $(SRCS_DIR) -type d)
SRCS = $(wildcard $(SRCS_DIR)/*.cpp $(SRCS_DIR)/*/*.cpp)

OBJS_DIR = objs
OBJS = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(SRCS))

INCLUDES = -I$(SRCS_DIR) $(addprefix -I, $(SRCS_SUBDIRS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -r $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

docker:
	cd docker && docker compose up -d && docker exec -it debian /bin/bash

.PHONY: docker