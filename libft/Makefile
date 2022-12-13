# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/29 21:51:32 by hsarhan           #+#    #+#              #
#    Updated: 2022/11/20 16:22:48 by hsarhan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = ft_strchr.c ft_split.c ft_strtrim.c ft_bzero.c ft_strnstr.c ft_strmapi.c ft_isprint.c ft_atoi.c ft_strdup.c \
	   ft_tolower.c ft_toupper.c ft_substr.c ft_striteri.c ft_memchr.c ft_putnbr_fd.c ft_strlcpy.c ft_itoa.c \
	   ft_memset.c ft_calloc.c ft_strlen.c ft_putchar_fd.c ft_isascii.c ft_strlcat.c ft_putendl_fd.c ft_memcmp.c \
	   ft_isdigit.c ft_strrchr.c ft_putstr_fd.c ft_isalpha.c ft_strncmp.c ft_isalnum.c ft_memcpy.c ft_memmove.c \
	   ft_strjoin.c get_next_line.c get_next_line_utils.c ft_atof.c ft_split_whitespace.c ft_strcmp.c

BONUS_SRCS = ft_lstadd_front_bonus.c ft_lstmap_bonus.c ft_lstnew_bonus.c ft_lstclear_bonus.c \
			 ft_lstadd_back_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstdelone_bonus.c \
			 ft_lstiter_bonus.c

OBJ_DIR = .obj
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o) $(BONUS_SRCS:.c=.o))
BONUS_OBJS := $(addprefix $(OBJ_DIR)/, $(BONUS_SRCS:.c=.o))

NAME = libft.a
CC = gcc

OPTIMIZATION_FLAGS = -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -funroll-loops

CFLAGS = -Wall -Wextra -g3 \
		$(OPTIMIZATION_FLAGS) \
		# -fsanitize=address \

all: $(NAME) bonus

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(NAME): $(OBJS)
	ar -rc $(NAME) $(OBJS)
	ranlib $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $^ 

bonus: $(NAME) $(BONUS_OBJS)
	ar -r $(NAME) $(BONUS_OBJS)
	ranlib $(NAME)

norm:
	norminette $(SRCS) $(BONUS_SRCS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
	
