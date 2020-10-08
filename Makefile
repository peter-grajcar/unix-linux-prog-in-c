CC = gcc
CC_FLAGS = -g -std=c99 -Wall -Wextra -Wshadow

SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(subst $(SRC_DIR),$(OBJ_DIR),$(subst .c,.o,$(SRCS)))

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(OBJ_DIR) 
	@printf "\033[31m"
	@./cstyle.pl $<
	@printf "\033[0m"
	$(CC) $(CC_FLAGS) -I$(INCLUDE_DIR) -o $@ -c $< 

$(BIN_DIR)/%.out: $(OBJ_DIR)/%.o $(BIN_DIR)
	$(CC) -L$(LIB_DIR) $(addprefix -l,$(LIBS)) -o $@ $<

$(LIB_DIR)/%.so: $(OBJ_DIR)/%.o $(LIB_DIR)
	$(CC) -shared -o $@ $<

.PRECIOUS: $(BIN_DIR)/%.out
.PRECIOUS: $(OBJ_DIR)/%.o

%\#run: $(BIN_DIR)/%.out	
	@printf "=%.0s" {1..20}
	@printf " %s " $< 
	@printf "=%.0s" {1..20}
	@printf "\n"
	@$<
	@printf "=%.0s" {1..20}
	@printf "%%%ss\n" `echo $< | wc -m` | xargs -I{} printf {} " " | tr " " "="
	@printf "=%.0s" {1..21}
	@printf "\n"

distclean:
	@rm -rf $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)
	@rm -rf $(LIB_DIR)

