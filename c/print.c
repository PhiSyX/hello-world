#include <stdio.h>

// ------ //
// Header // -> définition des fonctions
// ------ //

#define LF "\n"

void printw(char *w1, char *w2);

// ---- //
// Main //
// ---- //

int main()
{
    printw("Hello", "world");
    printw("Salut", "comment va-t'on?");
    printw("Ho", "petit sacripant");
    printw("Nous te souhaitons la bienvenue cher visiteur", "profite bien bien de ce code source");
    return 0;
}

// Application des en-têtes de fonctions

void printw(char *w1, char *w2)
{
    printf("%s, %s!%s", w1, w2, LF);
}

/* eof ; end of line */
