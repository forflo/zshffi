
FLEX_SRC = ffi_scanner.l 
BISON_SRC = ffi_parser.y
AUX_SRC = ffi_parser_util.c ffi_scanner_util.c ffi_nary_tree.c ffi_generate_ops.c ffi_util.c

clean:
	-rm *.o

all: $(FLEX_SRC) $(BISON_SRC) $(AUX_SRC)
	bison --debug -d $(BISON_SRC)
	flex --debug $(FLEX_SRC)
	gcc -g -o pars ffi_parser.c $(AUX_SRC) ffi_scanner.c ffi_main.c

opt: $(FLEX_SRC) $(BISON_SRC) $(AUX_SRC)
	bison -d $(BISON_SRC)
	flex $(FLEX_SRC)
	gcc -g -o pars ffi_parser.c $(AUX_SRC) ffi_scanner.c ffi_main.c
