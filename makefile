SRC_DIR=src
HEADER_DIR=src/headers
BUILD_DIR=build
DEBUG_DIR=debug

all: $(BUILD_DIR)/compiler $(BUILD_DIR)/vm $(BUILD_DIR)/decompiler 

$(BUILD_DIR)/stack.o: $(HEADER_DIR)/stack.h $(HEADER_DIR)/stack.c
	gcc -c $(HEADER_DIR)/stack.c -o $(BUILD_DIR)/stack.o

$(BUILD_DIR)/constants.o: $(HEADER_DIR)/constants.h $(HEADER_DIR)/constants.c
	gcc -c $(HEADER_DIR)/constants.c -o $(BUILD_DIR)/constants.o

dependencies: $(BUILD_DIR)/constants.o $(BUILD_DIR)/stack.o

$(BUILD_DIR)/compiler: $(SRC_DIR)/compiler.c $(BUILD_DIR)/constants.o
	gcc $(SRC_DIR)/compiler.c $(BUILD_DIR)/constants.o -o $(BUILD_DIR)/compiler

$(BUILD_DIR)/decompiler: $(SRC_DIR)/decompiler.c $(BUILD_DIR)/constants.o
	gcc $(SRC_DIR)/decompiler.c $(BUILD_DIR)/constants.o -o $(BUILD_DIR)/decompiler

$(BUILD_DIR)/vm: $(SRC_DIR)/vm.c $(BUILD_DIR)/constants.o $(BUILD_DIR)/stack.o
	gcc $(SRC_DIR)/vm.c $(BUILD_DIR)/constants.o $(BUILD_DIR)/stack.o -o $(BUILD_DIR)/vm

$(DEBUG_DIR)/stack.o: $(HEADER_DIR)/stack.h $(HEADER_DIR)/stack.c
	gcc -c -g $(HEADER_DIR)/stack.c -o $(DEBUG_DIR)/stack.o

$(DEBUG_DIR)/constants.o: $(HEADER_DIR)/constants.h $(HEADER_DIR)/constants.c
	gcc -c -g $(HEADER_DIR)/constants.c -o $(DEBUG_DIR)/constants.o

dependencies_dbg: $(DEBUG_DIR)/constants.o $(DEBUG_DIR)/stack.o

$(DEBUG_DIR)/compiler_dbg: $(SRC_DIR)/compiler.c $(DEBUG_DIR)/constants.o
	gcc $(SRC_DIR)/compiler.c $(DEBUG_DIR)/constants.o -o $(DEBUG_DIR)/compiler_dbg

$(DEBUG_DIR)/decompiler_dbg: $(SRC_DIR)/decompiler.c $(DEBUG_DIR)/constants.o
	gcc $(SRC_DIR)/decompiler.c $(DEBUG_DIR)/constants.o -o $(DEBUG_DIR)/decompiler_dbg

$(DEBUG_DIR)/vm_dbg: $(SRC_DIR)/vm.c $(DEBUG_DIR)/constants.o $(DEBUG_DIR)/stack.o
	gcc $(SRC_DIR)/vm.c $(DEBUG_DIR)/constants.o $(DEBUG_DIR)/stack.o -o $(DEBUG_DIR)/vm_dbg

debug: $(DEBUG_DIR)/compiler_dbg $(DEBUG_DIR)/decompiler_dbg $(DEBUG_DIR)/vm_dbg

clean:
	rm $(BUILD_DIR)/* $(DEBUG_DIR)/*
