#include "gdt.hpp"
#include "interrupts.hpp"
#include "types.hpp"

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

  for (int i = 0; str[i] != '\0'; ++i) {
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
    (*i)(); // déférencée, la fonction est appelée
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
  printf("Hello, World depuis le fichier kernel.cpp!\n");
  printf("Saut de ligne? Affichage de texte");

  GlobalDescriptorTable gdt;
  InterruptManager interrupts(&gdt);

  interrupts.activate();

  while (1)
    ;
}
