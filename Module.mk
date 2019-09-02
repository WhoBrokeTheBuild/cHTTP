
include $(CHTTP_ROOT)/Compiler.mk

CFLAGS  += -fPIC
LDFLAGS += -shared

MODULE_NAME ?= $(shell basename $(CURDIR))_module
MODULE_EXT ?= so

SRC_DIR = source
OBJ_DIR = .obj
DEP_DIR = .dep

CFLAGS += -I $(SRC_DIR) -I $(CHTTP_ROOT)/include

LIBRARY = $(MODULE_NAME).$(MODULE_EXT)
SOURCES = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/**/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

$(shell mkdir -p $(SRC_DIR) $(OBJ_DIR) $(DEP_DIR))

.PHONY: all
all: $(LIBRARY)

.PHONY: clean
clean:
	rm -rf $(DEP_DIR) $(OBJ_DIR) $(LIBRARY)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MM -MP -MT $@ -o $(DEP_DIR)/$*.d $<
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIBRARY): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)