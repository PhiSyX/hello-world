#include "print.hpp";

// Syntaxe :
//    while(<cond>) [ { <body> } ] [ <;> ]
void while_loop()
{
    println("While Loop");

    int counter{0};

    println("Start counter:", counter);

    while (counter < 5)
    {
        println(counter);
        ++counter;
    }

    println("End counter:", counter);
}

void while_input()
{
    println("While input");

    int counter{0};
    char input{'?'};

    println("Type 'e' for break the loop: ");

    while (counter < 5 && input != 'e')
    {
        println(counter);
        std::cin >> input;
        ++counter;
    }
}

void exercice_laverie()
{
    println("Exercice While");

    int const machine_1_cap_max{5};
    int const machine_2_cap_max{10};

    println("Combien de kg de linge as-tu?");
    int kg{0};
    std::cin >> kg;

    if (kg <= machine_1_cap_max)
    {
        println("Mettre dans la première machine");
    }
    else if (kg <= machine_2_cap_max)
    {
        println("Mettre dans la deuxième machine");
    }
    else
    {
        int nb_machines{0};
        while (kg > machine_1_cap_max)
        {
            kg -= machine_2_cap_max;
            ++nb_machines;
        }

        println("Tu as besoin de", nb_machines, "machine(s) de", machine_2_cap_max, "kg.");
        if (kg >= 0)
        {
            println("Le reste entre dans une machne de", machine_1_cap_max, "kg.");
        }
    }
}

void exercice_pgcd()
{
    println("Exercice while");

    println("A");
    int a{0};
    std::cin >> a;

    println("B");
    int b{0};
    std::cin >> b;

    int r{a % b};

    while (r != 0)
    {
        a = b;
        b = r;
        r = a % b;
    }

    println("PGCD = ", b);
}

// Syntaxe:
//    do { <body> }
//    while(<cond>) [ { <body> } ] [ <;> ]
void do_while_loop()
{
    println("Do While");
    do
    {
        println("On passe quand même ici.");
    } while (false);
}

void err_handling()
{
    println("Exercice Do/While");

    std::cout << "Quel jour es-stu né? ";
    int day{0};
    while (!(std::cin >> day))
    {
        println("Entrée invalide. Recommence.");
        std::cin.clear();
        std::cin.ignore(255, '\n');
    }

    std::cout << "Quel mois es-tu né? ";
    int month{0};
    do
    {
        if (std::cin.fail())
        {
            println("Entrée invalide. Recommence.");
            std::cin.clear();
            std::cin.ignore(255, '\n');
        }
    } while (!(std::cin >> month));

    println("Tu es né le", day, "/", month, ".");
}

// Syntaxe :
//    for (<init>; <cond>; <iter>) [ { <body> } ][ <;> ]
void for_loop()
{
    println("For loop");
    for (int counter{10}; counter > 0; --counter)
    {
        println(counter);
    }
}

int main()
{
    while_loop();
    while_input();
    exercice_laverie();
    exercice_pgcd();
    do_while_loop();
    err_handling();
    for_loop();

    return EXIT_SUCCESS;
}
