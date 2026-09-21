ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME            = libft_malloc_$(HOSTTYPE).so
LINK            = libft_malloc.so
CC              = gcc
CFLAGS          = -Wall -Wextra -Werror -fPIC -ggdb3
HEADER          = ./includes
LIBFT_DIR       = ./libft
LIBFT           = $(LIBFT_DIR)/libft.a
LIBFT_HEADER    = $(LIBFT_DIR)/includes
INCLUDES        = -I$(HEADER) -I$(LIBFT_HEADER)

SRCS            = srcs/blocks.c \
                  srcs/free.c \
                  srcs/malloc.c \
                  srcs/pages.c \
                  srcs/realloc.c \
                  srcs/utils.c

SRCS_BONUS      = srcs_bonus/blocks.c \
                  srcs_bonus/free.c \
                  srcs_bonus/malloc.c \
                  srcs_bonus/pages.c \
                  srcs_bonus/realloc.c \
                  srcs_bonus/utils.c \
                  srcs_bonus/debug.c \
                  srcs_bonus/print.c

OBJS_DIR        = objs
OBJS_BONUS_DIR  = objs_bonus
OBJS            = $(SRCS:srcs/%.c=$(OBJS_DIR)/%.o)
OBJS_BONUS      = $(SRCS_BONUS:srcs_bonus/%.c=$(OBJS_BONUS_DIR)/%.o)
MANDATORY_FLAG  = .mandatory
BONUS_FLAG      = .bonus


all: $(MANDATORY_FLAG)

$(MANDATORY_FLAG): $(LIBFT) $(OBJS)
	$(CC) -shared $(OBJS) $(LIBFT) -o $(NAME)
	ln -sf $(NAME) $(LINK)
	rm -f $(BONUS_FLAG)
	touch $(MANDATORY_FLAG)


bonus: $(BONUS_FLAG)

$(BONUS_FLAG): $(LIBFT) $(OBJS_BONUS)
	$(CC) -shared $(OBJS_BONUS) $(LIBFT) -pthread -o $(NAME)
	ln -sf $(NAME) $(LINK)
	rm -f $(MANDATORY_FLAG)
	touch $(BONUS_FLAG)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJS_DIR)/%.o: srcs/%.c includes/malloc.h
	@mkdir -p $(OBJS_DIR)
	@printf "Compiling $<...\n"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_BONUS_DIR)/%.o: srcs_bonus/%.c includes/malloc_bonus.h
	@mkdir -p $(OBJS_BONUS_DIR)
	@printf "Compiling $<...\n"
	$(CC) $(CFLAGS) -pthread $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR) $(OBJS_BONUS_DIR)
	rm -f $(MANDATORY_FLAG) $(BONUS_FLAG)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	rm -f $(LINK)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

re_bonus: fclean bonus

.PHONY: all bonus clean fclean re re_bonus