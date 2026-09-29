PKGS=libtiff-4 libpng
DEFINES=
BUILD_DIR=build/
INC_DIR=include/

# Source files
SRC_DIR=src/
SRC_OBJ_DIR=$(BUILD_DIR)obj/$(SRC_DIR)
SRC_SRCS=$(shell find $(SRC_DIR) -iname \*.c)
SRC_OBJS_sub=$(subst $(SRC_DIR),$(SRC_OBJ_DIR),$(SRC_SRCS))
SRC_OBJS=$(SRC_OBJS_sub:.c=.o)
SRC_OBJ_DIRS_sub=$(shell find $(SRC_DIR) -type d)
SRC_OBJ_DIRS=$(subst $(SRC_DIR),$(SRC_OBJ_DIR),$(SRC_OBJ_DIRS_sub))

# Library files
LIB_DIR=lib/
LIB_OBJ_DIR=$(BUILD_DIR)obj/$(LIB_DIR)
LIB_SRCS=$(shell find $(LIB_DIR) -iname \*.c)
LIB_OBJS_sub=$(subst $(LIB_DIR),$(LIB_OBJ_DIR),$(LIB_SRCS))
LIB_OBJS=$(LIB_OBJS_sub:.c=.o)
LIB_OBJ_DIRS_sub=$(shell find $(LIB_DIR) -type d)
LIB_OBJ_DIRS=$(subst $(LIB_DIR),$(LIB_OBJ_DIR),$(LIB_OBJ_DIRS_sub))

# Library files
TEST_DIR=test/
TEST_OBJ_DIR=$(BUILD_DIR)obj/$(TEST_DIR)
TEST_SRCS=$(shell find $(TEST_DIR) -iname \*.c)
TEST_OBJS_sub=$(subst $(TEST_DIR),$(TEST_OBJ_DIR),$(TEST_SRCS))
TEST_OBJS=$(TEST_OBJS_sub:.c=.o)
TEST_OBJ_DIRS_sub=$(shell find $(TEST_DIR) -type d)
TEST_OBJ_DIRS=$(subst $(TEST_DIR),$(TEST_OBJ_DIR),$(TEST_OBJ_DIRS_sub))

# Programs
PROG_DIR=
PROG_OUT_DIR=$(BUILD_DIR)$(PROG_DIR)
PROG_SRCS=$(shell find $(SRC_DIR) -iname \*.c)
PROG_OBJS=$(PROG_SRCS:.c=.o)
PROG_DIRS_sub=$(shell find $(SRC_DIR) -type d)
PROG_DIRS=$(subst $(SRC_DIR),$(PROG_OUT_DIR),$(PROG_DIRS_sub))
PROGS_sub=$(subst $(SRC_DIR),$(PROG_OUT_DIR),$(PROG_SRCS))
PROGS=$(PROGS_sub:.c=)

# Include raylib if we want a visual experience
ifdef RAYLIB
$(info Visual Experience Selected)
PKGS+=raylib
DEFINES+=-DVISUAL
endif

# Dump AVL tree info?
ifdef AVL_INFO
$(info Including AVL Dump)
DEFINES+=-DAVL_INFO
endif

# Suppress Test Passings
ifdef TEST_SHOW_PASS
$(info Showing test pass results)
DEFINES+=-DTEST_SHOW_PASS
endif

ifeq ($(shell uname -s),Linux)
	PKGCONF=pkgconf
endif
ifeq ($(shell uname -s),Darwin)
	PKGCONF=pkg-config
endif

CFLAGS=
CFLAGS+=$(shell $(PKGCONF) --cflags $(PKGS))
CFLAGS+=-I$(INC_DIR)
CFLAGS+=$(DEFINES)
CFLAGS+=-Wall

LDFLAGS=
LDFLAGS+=$(shell $(PKGCONF) --libs $(PKGS))


default: clean build

.PHONY: clean build test run

build: $(PROGS)

$(BUILD_DIR)$(PROG_DIR)%: $(SRC_OBJ_DIR)%.o $(LIB_OBJS)
	@echo LD --\> $@
	@gcc -o $@ $^ $(LDFLAGS)

build/test: $(TEST_OBJS) $(LIB_OBJS)
	@echo LD --\> $@
	@gcc -o $@ $^ $(LDFLAGS)

$(SRC_OBJ_DIR)%.o: $(SRC_DIR)%.c
	@echo CC $< --\> $@
	@gcc -o $@ $(CFLAGS) -c $<

$(LIB_OBJ_DIR)%.o: $(LIB_DIR)%.c
	@echo CC $< --\> $@
	@gcc -o $@ $(CFLAGS) -c $<

$(TEST_OBJ_DIR)%.o: $(TEST_DIR)%.c
	@echo CC $< --\> $@
	@gcc -o $@ $(CFLAGS) -c $<

test: clean build/test
	@echo Test beginning...
	@./build/test

clean:
	@echo Cleaning build files...
	@-rm -rf $(OBJ_DIR) $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR) $(SRC_OBJ_DIRS) $(LIB_OBJ_DIRS) $(TEST_OBJ_DIRS) $(PROG_DIRS)
