# Define compiler to use
CC = gcc

SRC_FOLDER = src
OUT_FOLDER = build
OBJ_FOLDER = $(OUT_FOLDER)/obj

# Define any compile-time flags
CFLAGS = -Wall -Werror -std=c99

# Define any directories containing header files other than /usr/include
INCLUDES =	-I. \
			-Iinclude

# Define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
#LFLAGS = -L/home/newhall/lib  -L../lib
#LFLAGS = -Llib

# Define any libraries and their paths to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = 

# Define the C source files
SRCS = $(wildcard $(SRC_FOLDER)/*.c)

# define the C object files
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
OBJS := $(addprefix $(OBJ_FOLDER)/,$(notdir $(SRCS:.c=.o)))

# define the executable file
MAIN = $(OUT_FOLDER)/agbes



.PHONY: clean dosclean

all:    $(MAIN)
	@echo  Compilation completed!

$(MAIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c or cpp file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(shell mkdir -p $(OBJ_FOLDER))

clean:
	$(RM) $(OBJ_FOLDER)/*.o $(SRC_FOLDER)/*.o $(OUT_FOLDER)/*.exe $(MAIN)
