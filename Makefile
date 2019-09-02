
include Compiler.mk

SRC_DIR = source
INC_DIR = include
MOD_DIR = modules
OBJ_DIR = .obj
DEP_DIR = .dep

CFLAGS  += -I $(SRC_DIR) -I $(INC_DIR)
LDLIBS  += -ldl

PROGRAM = chttpd
SOURCES = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/**/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
MODULES = $(wildcard $(MOD_DIR)/*)

ARGS = 

$(shell mkdir -p $(SRC_DIR) $(OBJ_DIR) $(DEP_DIR))

.PHONY: all
all: $(PROGRAM) $(MODULES)

.PHONY: modules
modules: $(MODULES)

.PHONY: $(MODULES)
$(MODULES):
	$(MAKE) -C $@ $(TARGET) CHTTP_ROOT=$(CURDIR)

.PHONY: clean
clean:
	rm -rf $(DEP_DIR) $(OBJ_DIR) $(PROGRAM)
	$(MAKE) modules TARGET=clean

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -MM -MP -MT $@ -o $(DEP_DIR)/$*.d $<
	$(CC) $(CFLAGS) -c -o $@ $<

$(PROGRAM): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

run: $(PROGRAM)
	./$(PROGRAM) $(ARGS)

valgrind: $(PROGRAM)
	valgrind ./$(PROGRAM) $(ARGS)

gdb: $(PROGRAM)
	gdb --args ./$(PROGRAM) $(ARGS)

$(DEP_DIR)/%.d: ;
.PRECIOUS: $(DEP_DIR)/%.d
-include $(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d,$(SOURCES))
