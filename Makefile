# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jihoolee <jihoolee@student.42SEOUL.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/02 20:30:41 by bylee             #+#    #+#              #
#    Updated: 2021/11/03 15:32:29 by jihoolee         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				=	minishell

CC					=	gcc
CFLAGS				=	-Wall -Wextra -Werror
RM					=	rm -f

LIBFT_DIR			=	lib/libft/
LIBFT_FLAG			= 	-lft

READLINE_INC_DIR	=	./lib/readline/include/
READLINE_LIB_DIR	=	./lib/readline/lib/
READLINE_FLAG		=	-lreadline

INCLUDE_DIR			=	./include

INTER_SRC_DIR		=	./srcs/interpreter/
INTER_SRCS			=	converter.c\
						destructor.c\
						interpeter.c\
						lexer_get_item.c\
						lexer.c\
						parse_cmd.c\
						parse_get_node.c\
						parse_type_check.c\
						parser.c\
						semantic_analyzer.c\
						util.c\
INTER_OBJS			=	$(addprefix $(INTER_SRCS_DIR), $(INTER_SRCS:.c=.o))

PIPE_SRC_DIR		=	./srcs/pipeline
PIPE_SRCS			=	pipeline_utils.c\
						pipeline.c
PIPE_OBJS			=	$(addprefix $(PIPE_SRC_DIR), $(PIPE_SRCS:.c=.o))

REDIR_SRC_DIR		=	./srcs/redirection/
REDIR_SRCS			=	fd_new.c\
						handle_redir.c\
						init_fd_table.c\
						search_proc_fd.c
REDIR_OBJS			=	$(addprefix $(REDIR_SRC_DIR), $(REDIR_SRCS:.c=.o))

SRC_DIR				=	src/
SRCS				=	error.c\
						execute.c\
						init.c\
						minishell.c\
						print_JSON.c\
						redir_and_exe.c
OBJS				=	$(addprefix $(SRC_DIR), $(SRCS:.c=.o))

.c.o :
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -I $(READLINE_INC_DIR) -I $(LIBFT_DIR) -c $< -o $(<:.c=.o)

$(NAME) : $(OBJS) $(INTER_OBJS) $(PIPE_OBJS) $(REDIR_OBJS)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) -o $(NAME) -L $(LIBFT_DIR) -L $(READLINE_LIB_DIR) $(LIBFT_FLAG) $(READLINE_FLAG) $^

all : $(NAME)

clean :
	$(RM) $(OBJS) $(INTER_OBJS) $(PIPE_OBJS) $(REDIR_OBJS)
	make clean -C $(LIBFT_DIR)

fclean : clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT_DIR)

re : fclean all

test : $(OBJS) $(INTER_OBJS) $(PIPE_OBJS) $(REDIR_OBJS)
	$(CC) $(CFLAGS) -g3 -fsanitize=address -o $(NAME) -L $(LIBFT_DIR) -L $(READLINE_LIB_DIR) $(LIBFT_FLAG) $(READLINE_FLAG) $^

.PHONY : all clean fclean re test
