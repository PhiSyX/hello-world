cc: $(wildcard *.asm)
	$(foreach file, $?, \
		nasm -g -f elf64 $(file) -o $(subst .asm,.o,$(file)) && \
		ld $(subst .asm,.o,$(file)) -o $(subst .asm,.out,$(file)) && \
		chmod u+x $(subst .asm,.out,$(file)) && \
		rm $(subst .asm,.o,$(file)) ;)

clear: 
	rm -f *.out *.o

.PHONY: cc clear
