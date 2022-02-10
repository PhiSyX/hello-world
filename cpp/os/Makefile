GCCPARAMS = -m32 -Iinclude -nostdlib \
	-fno-builtin -fno-exceptions -fno-leading-underscore \
	-fno-rtti -fno-use-cxa-atexit -Wno-write-strings

ASPARAMS = --32
LDPARAMS = -melf_i386

objects = \
	obj/loader.o \
    obj/gdt.o \
    obj/memory.o \
    obj/drivers/driver.o \
    obj/hardware/port.o \
    obj/hardware/interrupts_stub.o obj/hardware/interrupts.o \
    obj/multitasking.o \
    obj/drivers/AMD/am79c973.o \
    obj/hardware/pci.o \
    obj/drivers/keyboard.o \
    obj/drivers/mouse.o \
    obj/drivers/vga.o \
    obj/drivers/ata.o \
    obj/gui/widget.o \
    obj/gui/window.o \
    obj/gui/desktop.o \
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
	cp $< /boot/mykernel.bin || true

# Création de l'ISO pour une VM
mykernel.iso: mykernel.bin
	mkdir -p iso/boot/grub
	cp $< iso/boot/
	@echo 'set timeout=0' > iso/boot/grub/grub.cfg
	@echo 'set default=0' >> iso/boot/grub/grub.cfg
	@echo '' >> iso/boot/grub/grub.cfg
	@echo 'menuentry "Mon superbe OS" {' >> iso/boot/grub/grub.cfg
	@echo '  multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	@echo '  boot' >> iso/boot/grub/grub.cfg
	@echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

clean:
	rm -rf $(objects) mykernel.bin mykernel.iso obj
