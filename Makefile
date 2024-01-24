NAME = ircserv

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98

SRC = $(wildcard ./src/*.cpp ./src/cmds/*.cpp)

OBJDIR = ./obj
OBJ = $(addprefix $(OBJDIR)/,$(notdir $(SRC:.cpp=.o)))

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@echo ircServer created

$(OBJDIR)/%.o: ./src/%.cpp
	@$(CC) $(FLAGS) -c -o $@ $<

$(OBJDIR)/%.o: ./src/cmds/%.cpp
	@$(CC) $(FLAGS) -c -o $@ $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(OBJDIR)

re: fclean all

.PHONY: all clean fclean re