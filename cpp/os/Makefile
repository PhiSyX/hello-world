GCCPARAMS = -m32 -Iinclude -nostdlib \
	-fno-builtin -fno-exceptions -fno-leading-underscore \
	-fno-rtti -fno-use-cxa-atexit -Wno-write-strings

ASPARAMS = --32
LDPARAMS = -melf_i386

objects = \
	obj/loader.o \
	obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin
