#include <iostream>

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

    std::cout << "a == b donne " << (a == b) << std::endl;
    std::cout << "a != b donne " << (a != b) << std::endl;
    std::cout << "a < b donne " << (a < b) << std::endl;
    std::cout << "a <= b donne " << (a <= b) << std::endl;

    bool const plus_grand{a > b};
    std::cout << "a > b donne " << plus_grand << std::endl;

    bool const plus_grand_ou_egal{a >= b};
    std::cout << "a >= b donne " << plus_grand_ou_egal << std::endl;

    std::cout << "Indique-moi ta note : ";
    int note{0};
    std::cin >> note;
    std::cout << "Tu as obtenu la note de :" << note << std::endl;

    if (note == 20)
    {
        std::cout << "Tu es au top!" << std::endl;
    }
    else if (note > 10 && note < 20)
    {
        std::cout << "Félicitations, c'est une très bonne note!" << std::endl;
    }
    else
    {
        std::cout << "Tu feras mieux les prochaines fois. Courage!" << std::endl;
    }
}

int main()
{
    conditions();

    return EXIT_SUCCESS;
}
