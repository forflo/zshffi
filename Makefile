
FLEX_SRC = ffi_scanner.l 
BISON_SRC = ffi_parser.y
SRC_1 = ffi_parser.c ffi_scanner.c 
SRC_2 = ffi_parser_util.c ffi_scanner_util.c ffi_nary_tree.c ffi_generate_ops.c
SRC_3 = ffi_util.c ffi_dstru_util.c ffi_dstru.c ffi_storage.c ffi_offset_table.c

clean:
	-rm *.o

all: $(FLEX_SRC) $(BISON_SRC) $(SRC_1) $(SRC_2) $(SRC_3) ffi_main.c
	bison --debug -d $(BISON_SRC)
	flex --debug $(FLEX_SRC)
	flex --debug ffi_scanner_location.l
	gcc -g -o pars $(SRC_1) $(SRC_2) $(SRC_3) ffi_main.c

opt: $(FLEX_SRC) $(BISON_SRC) $(SRC_1) $(SRC_2) $(SRC_3) ffi_main.c
	bison -d $(BISON_SRC)
	flex $(FLEX_SRC)
	flex ffi_scanner_location.l
	gcc -g -o pars $(SRC_1) $(SRC_2) $(SRC_3) ffi_main.c

ntest: $(FLEX_SRC) $(BISON_SRC) $(SRC_1) $(SRC_2) $(SRC_3) ffi_main_generate.c
	bison --debug -d $(BISON_SRC)
	flex --debug $(FLEX_SRC)
	gcc -g -o ntest $(SRC_1) $(SRC_2) $(SRC_3) ffi_main_generate.c
