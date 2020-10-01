CC = gcc
CC_FLAGS = -g -std=c99 -Wall -Wextra -Wshadow

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(subst $(SRC_DIR),$(OBJ_DIR),$(subst .c,.o,$(SRCS)))

compile: $(OBJS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(OBJ_DIR) 
	$(CC) $(CC_FLAGS) -o $@ -c $< 

$(BIN_DIR)/%.out: $(OBJ_DIR)/%.o $(BIN_DIR)
	$(CC) -o $@ $<

.PRECIOUS: $(BIN_DIR)/%.out

%\#run: $(BIN_DIR)/%.out
	#===============================================#
	$<
	#===============================================#

distclean:
	rm -rf $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

