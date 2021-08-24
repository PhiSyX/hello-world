#include "print.hpp";

// Les booléens
void booleans()
{
    bool const vrai = true;
    bool const faux = false;
}

// Les conditions
void conditions()
{
    int const a{10};
    int const b{20};

    //* Directive permettant d'afficher true ou false. Par défaut, ça nous
    //* afficherait 1 ou 0 respectivement.
    std::cout << std::boolalpha;

    println("a == b donne", (a == b));
    println("a != b donne", (a != b));
    println("a < b donne", (a < b));
    println("a <= b donne", (a <= b));

    bool const plus_grand{a > b};
    println("a > b donne", plus_grand);

    bool const plus_grand_ou_egal{a >= b};
    println("a >= b donne", plus_grand_ou_egal);

    println("Indique-moi ta note :");
    int note{0};
    std::cin >> note;
    println("Tu as obtenu la note de :", note);

    if (note == 20)
    {
        println("Tu es au top!");
    }
    else if (note > 10 && note < 20)
    {
        println("Félicitations, c'est une très bonne note!");
    }
    else
    {
        println("Tu feras mieux les prochaines fois. Courage!");
    }
}

int main()
{
    conditions();

    return EXIT_SUCCESS;
}
