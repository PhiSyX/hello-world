#include "drivers/AMD/am79c973.hpp"
#include "drivers/driver.hpp"
#include "drivers/keyboard.hpp"
#include "drivers/mouse.hpp"
#include "drivers/vga.hpp"
#include "gdt.hpp"
#include "gui/desktop.hpp"
#include "gui/window.hpp"
#include "hardware/interrupts.hpp"
#include "hardware/pci.hpp"
#include "memory.hpp"
#include "types.hpp"

#define GRAPHICS_MODE

/// Affiche des informations en console
///
/// Du fait que l'on construit notre propre système d'exploitation,
/// que l'on part de zéro, sans glibc, on ne peut pas utiliser la fonction
/// printf. On doit donc écrire notre propre fonction printf.
///
/// La mémoire vidéo de l'écran texte pour les moniteurs couleurs se trouve à
/// l'adresse 0xB8000, et pour les moniteurs noir et blanc se trouve à
/// l'adresse 0xB0000.
void
printf(char* str)
{
  // L'utilisation du mot clé static sur une variable locale change sa durée de
  // durée automatique à durée statique. Cela signifie que la variable est
  // maintenant créée au début du programme, et détruite à la fin du programme
  // (tout comme une variable globale). Par conséquent, la variable statique
  // conservera sa valeur même après être sortie de sa portée.
  static u16* video_memory = (u16*)0xb8000;

  static u8 x = 0;
  static u8 y = 0;

  for (usize i = 0; str[i] != '\0'; ++i) {
    switch (str[i]) {
      case '\n':
        x = 0;
        y++;
        break;

      default:
        video_memory[80 * y + x] = (video_memory[80 * y + x] & 0xFF00) | str[i];
        x++;
        break;
    }

    if (x >= 80) {
      x = 0;
      y++;
    }

    if (y >= 25) {
      for (y = 0; y < 25; y++) {
        for (x = 0; x < 80; x++) {
          video_memory[80 * y + x] = (video_memory[80 * y + x] & 0xFF00) | ' ';
        }
      }

      x = 0;
      y = 0;
    }
  }
}

void
printh(u8 key)
{
  char* foo = "00";
  char* hex = "0123456789ABCDEF";
  foo[0] = hex[(key >> 4) & 0xF];
  foo[1] = hex[key & 0xF];
  printf(foo);
}

void
printh16(u16 key)
{
  printh((key >> 8) & 0xFF);
  printh(key & 0xFF);
}

void
printh32(u32 key)
{
  printh((key >> 24) & 0xFF);
  printh((key >> 16) & 0xFF);
  printh((key >> 8) & 0xFF);
  printh(key & 0xFF);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
  void on_keydown(char ch)
  {
    char* foo = " ";
    foo[0] = ch;
    printf(foo);
  }
};

class MouseToConsole : public MouseEventHandler
{
  i8 x, y;

public:
  MouseToConsole() {}

  virtual void on_activate()
  {
    u16* video_memory = (u16*)0xb8000;
    x = 40;
    y = 12;
    video_memory[80 * y + x] = (video_memory[80 * y + x] & 0x0F00) << 4 |
                               (video_memory[80 * y + x] & 0xF000) >> 4 |
                               (video_memory[80 * y + x] & 0x00FF);
  }

  virtual void on_mousemove(i32 offset_x, i32 offset_y)
  {
    static u16* video_memory = (u16*)0xb8000;
    video_memory[80 * y + x] = (video_memory[80 * y + x] & 0x0F00) << 4 |
                               (video_memory[80 * y + x] & 0xF000) >> 4 |
                               (video_memory[80 * y + x] & 0x00FF);

    x += offset_x;
    if (x >= 80) {
      x = 79;
    }
    if (x < 0) {
      x = 0;
    }

    y += offset_y;
    if (y >= 25) {
      y = 24;
    }
    if (y < 0) {
      y = 0;
    }

    video_memory[80 * y + x] = (video_memory[80 * y + x] & 0x0F00) << 4 |
                               (video_memory[80 * y + x] & 0xF000) >> 4 |
                               (video_memory[80 * y + x] & 0x00FF);
  }
};

/// Défini un constructeur comme un pointeur vers une fonction
typedef void (*constructor)();
/// `start_ctors` est une variable de type `constructor` (pointeur vers une
/// fonction)
extern "C" constructor start_ctors;
/// `end_ctors` est une variable de type `constructor` (pointeur vers une
/// fonction)
extern "C" constructor end_ctors;

extern "C" void
call_ctors()
{
  /// i pointe d'abord sur le premier pointeur vers un élément de fonction
  /// (start_ctors) dans le tableau de pointeurs vers les fonctions
  /// (.init_array).
  for (constructor* i = &start_ctors; i != &end_ctors; i++) {
    (*i)(); // déréférencée, la fonction est appelée
  }
}

void
taskA()
{
  while (1) {
    printf("A");
  }
}
void
taskB()
{
  while (1) {
    printf("B");
  }
}

/// Fonction de démarrage
///
/// multiboot_struct et multiboot_magic sont transmis par le bootloader.
///
/// Le compilateur G++ modifie le nom de la fonction kernel_main lorsqu'il
/// compile. On utilise extern "C" pour empêcher ça.
extern "C" void
kernel_main(void* multiboot_struct, u32 magicnumber)
{
  printf("Demarrage du kernel!\n");

  GlobalDescriptorTable gdt;

  u32* memupper = (u32*)(((usize)multiboot_struct) + 8);
  usize heap = 10 * 1024 * 1024;

  MemoryManager memory_manager(heap, (*memupper) * 1024 - heap - 10 * 1024);

  printf("heap: 0x");
  printh((heap >> 24) & 0xFF);
  printh((heap >> 16) & 0xFF);
  printh((heap >> 8) & 0xFF);
  printh((heap)&0xFF);

  void* allocated = memory_manager.malloc(1024);
  printf("\nallocated: 0x");
  printh(((usize)allocated >> 24) & 0xFF);
  printh(((usize)allocated >> 16) & 0xFF);
  printh(((usize)allocated >> 8) & 0xFF);
  printh(((usize)allocated) & 0xFF);
  printf("\n");

  TaskManager task_manager;
  /*
  Task task1(&gdt, taskA);
  Task task2(&gdt, taskB);
  task_manager.add(&task1);
  task_manager.add(&task2);
  */

  InterruptManager interrupts(0x20, &gdt, &task_manager);
  printf("Initialisation du materiel\n");

#ifdef GRAPHICS_MODE
  printf("Mode: Graphique\n");
  Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
#else
  printf("Mode: Normal\n");
#endif

  DriverManager driver_manager;

#ifdef GRAPHICS_MODE
  KeyboardDriver keyboard(&interrupts, &desktop);
#else
  PrintfKeyboardEventHandler keyboard_handler;
  KeyboardDriver keyboard(&interrupts, &keyboard_handler);
#endif
  driver_manager.add(&keyboard);

#ifdef GRAPHICS_MODE
  MouseDriver mouse(&interrupts, &desktop);
#else
  MouseToConsole mouse_handler;
  MouseDriver mouse(&interrupts, &mouse_handler);
#endif
  driver_manager.add(&mouse);

  PCIController PCIController;
  PCIController.select_drivers(&driver_manager, &interrupts);

  VGA vga;

  driver_manager.enable_all();

#ifdef GRAPHICS_MODE
  vga.set_mode(320, 200, 8);
  Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
  desktop.add_child(&win1);
  Window win2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
  desktop.add_child(&win2);
#endif

  amd_am79c973* eth0 = (amd_am79c973*)(driver_manager.drivers[2]);
  eth0->send((u8*)"Hello Network", (usize)13);

  interrupts.activate();

  while (1) {
#ifdef GRAPHICS_MODE
    desktop.draw(&vga);
#endif
  }
}
