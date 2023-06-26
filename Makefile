NAME = ircserv

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS_DIR = src
SRCS = $(shell find $(SRCS_DIR) -name "*.cpp" -type f)

OBJS_DIR = obj
OBJS = $(subst $(SRCS_DIR), $(OBJS_DIR), $(SRCS:%.cpp=%.o))

INC = include

RM = rm -rf

all: $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(@D)
	@$(CXX) -I$(INC) $(CXXFLAGS) -o $@ -c $<

$(NAME): $(OBJS)
	@echo "\n\033[94m[Making ircserv]\033[37m"
	@$(CXX) -I$(INC) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@echo "\033[32m\n...make Done!\n\033[37m"

clean:
	@echo "\n\033[94m[Removing Object Files and Directory]\033[37m"
	@rm -rf $(OBJS)
	@rm -rf $(OBJS_DIR)
	@echo "\033[32m\n...clean Done!\n\033[37m"

fclean:
	@echo "\033[94m[Removing Binary File]\033[37m"
	@rm -rf $(OBJS)
	@rm -rf $(OBJS_DIR)
	@rm -rf $(NAME)
	@echo "\033[32m\n...fclean Done!\n\033[37m"

re:
	@make fclean
	@make all

.PHONY: all clean fclean re