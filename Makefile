cc: $(wildcard *.asm)
	$(foreach file, $?, nasm -g -f elf64 $(file) -o $(subst .asm,.o,$(file)) && ld $(subst .asm,.o,$(file)) -o $(subst .asm,.out,$(file)) ;)

.PHONY: cc
