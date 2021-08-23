#include <iostream>

// Les caractères :
//   Type: char
void characters()
{
    std::cout << 'A' << std::endl;
    std::cout << '7' << std::endl;
}

// Les nombres :
//   Types: int, double
void numbers()
{
    std::cout << -1 << std::endl; //? -> -1
    std::cout << 0 << std::endl;  //? ->  0
    std::cout << 1 << std::endl;  //? ->  1
    //! std::cout << 1_000_000 << std::endl; //* Erreur. Dommage.

    std::cout << 1. << std::endl;  //? -> 1
                                   //* Ok, je trouve ça bizarre, je m'attendais
                                   //* plutôt à ce que cela m'affiche: 1.0   ;
    std::cout << 1.0 << std::endl; //? -> 1
                                   //* Même remarque pour la ligne ci-dessus ;

    std::cout << 1.2 << std::endl; //? -> 1.2
}

// Variables :
//
//   Syntaxe: <type> <name>[ { <data> } ];
//   Syntaxe: <type> <name>[ = <data> ];
//
//   Point négatif: mutable par défaut.
void variables()
{
    int response{42};
    std::cout << "La réponse à la Grande Question est " << response << std::endl;

    double const PI{3.1415926};
    std::cout << "Voici la valeur du célèbre nombre PI : " << PI << std::endl;

    std::string name = "Hello, world";
    name += "!";
    std::cout << name << std::endl;
}

// Mot-clé auto
void variables_auto()
{
    auto const const_int = 42;
    std::cout << const_int << std::endl;

    auto const const_double = 3.14;
    std::cout << const_double << std::endl;

    auto let_char = "une suite de plusieurs caractères"; //? const char *let_char
    std::cout << let_char << std::endl;

    //* Pour utiiser auto avec des std::string,
    //* il faut déclarer la ligne suivante.
    using namespace std::literals; //* qu'une seule fois suffit, en dehors de
                                   //* la portée aussi.

    auto let_str = "chaine de caractères"s; //? std::__cxx11::string let_str
                                            //* à noter qu'on doit rajouter le
                                            //* 's' après la double quote de fin.
    std::cout << let_str << std::endl;
}

// Les entrées
//
// Point positif : 
//   simple à utiliser, comprendre
//
// Point négatif :
//   Si on tape des lettres pour l'âge, le programme se stoppe sans 
//   nous donner aucune explications. 
void inputs()
{
    int age = 0;
    std::string name = "";

    std::cout << "Entre ton âge : " << std::endl;
    std::cin >> age;
    std::cout << "Tu as " << age << " ans." << std::endl;

    std::cout << "Entre ton prénom :" << std::endl;
    std::cin >> name;
    std::cout << "Tu t'appelles " << name << std::endl;
}

int main()
{
    characters();
    numbers();
    variables();
    variables_auto();
    inputs();

    return EXIT_SUCCESS;
}
