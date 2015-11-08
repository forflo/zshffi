
FLEX_SRC = ffi_scanner.l 
BISON_SRC = ffi_parser.y
AUX_SRC = ffi_parser_util.c ffi_scanner_util.c ffi_nary_tree.c ffi_generate_ops.c ffi_util.c
SRC_1 = ffi_parser.c ffi_scanner.c 
SRC_2 = ffi_parser_util.c ffi_scanner_util.c ffi_nary_tree.c ffi_generate_ops.c
SRC_3 = ffi_util.c ffi_dstru_util.c ffi_dstru.c ffi_main.c

clean:
	-rm *.o

all: $(FLEX_SRC) $(BISON_SRC) $(AUX_SRC)
	bison --debug -d $(BISON_SRC)
	flex --debug $(FLEX_SRC)
	gcc -g -o pars $(SRC_1) $(SRC_2) $(SRC_3)

opt: $(FLEX_SRC) $(BISON_SRC) $(AUX_SRC)
	bison -d $(BISON_SRC)
	flex $(FLEX_SRC)
	gcc -g -o pars $(SRC_1) $(SRC_2) $(SRC_3)
