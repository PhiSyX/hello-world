#include "print.hpp";

// Les caractères :
//   Type: char
void characters()
{
    println('A');
    println('7');
}

// Les nombres :
//   Types: int, double
void numbers()
{
    println(-1); //? -> -1
    println(0);  //? ->  0
    println(1);  //? ->  1
    //! println(1_000_000);  //* Erreur. Dommage.

    println(1.);  //? -> 1
                  //* Ok, je trouve ça bizarre, je m'attendais
                  //* plutôt à ce que cela m'affiche: 1.0   ;
    println(1.0); //? -> 1
                  //* Même remarque pour la ligne ci-dessus ;

    println(1.2); //? -> 1.2
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
    println("La réponse à la Grande Question est", response);

    double const PI{3.1415926};
    println("Voici la valeur du célèbre nombre PI :", PI);

    std::string name = "Hello, world";
    name += "!";
    println(name);
}

// Mot-clé auto
void variables_auto()
{
    auto const const_int = 42;
    println(const_int);

    auto const const_double = 3.14;
    println(const_double);

    auto let_char = "une suite de plusieurs caractères"; //? const char *let_char
    println(let_char);

    //* Pour utiiser auto avec des std::string,
    //* il faut déclarer la ligne suivante.
    using namespace std::literals; //* qu'une seule fois suffit, en dehors de
                                   //* la portée aussi.

    auto let_str = "chaine de caractères"s; //? std::__cxx11::string let_str
                                            //* à noter qu'on doit rajouter le
                                            //* 's' après la double quote de fin.
    println(let_str);
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
    std::string first_name = "";

    println("Entre ton âge :");
    std::cin >> age;
    println("Tu as", age, "ans.");

    println("Entre ton prénom :");
    std::cin >> first_name;
    println("Tu t'appelles", first_name);
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
