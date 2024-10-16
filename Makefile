# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: padam <padam@student.42heilbronn.de>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/09 15:34:56 by padam             #+#    #+#              #
#    Updated: 2024/10/16 23:46:23 by padam            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++
CFLAGS =  $(INCLUDES) #-Wall -Wextra -Werror -std=c++17
INCLUDES = -I./include
LIBS =

SRC_PATH = src
OBJ_PATH = obj

OBJ_DIRS =

SRCS_MAIN =	main.cpp Server.cpp Client.cpp Channel.cpp

SRC_NAME =										$(SRCS_MAIN)	\
#			$(addprefix ,							$())		\

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
RESET = \033[0m

OBJ_NAME = $(SRC_NAME:.cpp=.o)
OBJS = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all: $(NAME)

$(NAME): ascii_art $(OBJS)
	@$(CC) -o $(NAME) $(CFLAGS) $(LIBS) $(OBJS)
	@printf "$(GREEN)%-100s\n$(RESET)" "$(NAME) compiled"

$(OBJ_PATH)	:
	@mkdir -p $(OBJ_PATH)
ifneq ($(OBJ_DIRS),)
	@mkdir -p $(addprefix $(OBJ_PATH)/,$(OBJ_DIRS))
endif

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp | $(OBJ_PATH)
	@printf "%-100s\r" "$(CC) $(CFLAGS) -o $@"
	@$(CC) $(CFLAGS) -o $@ -c $<

init_submodules:
	git submodule update --init --recursive

update_submodules:
	git submodule update --recursive --remote

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)

re: fclean all

ascii_art:
	@clear
	@printf "$(GREEN)	                                                                                                                                         \n\
	                                                                                                                                         \n\
                                                                                                                                             \n\
                                                                                                                                             \n\
                                                                                                                     $(BLUE)by: padam && aweizman$(RESET) \n\
                                                                                                                                             \n"

.PHONY: all, $(NAME), $(LIBFT_DIR)/libft.a, $(OBJ_PATH), $(OBJ_PATH), clean, fclean, re