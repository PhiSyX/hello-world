#include <iostream>

// Syntaxe :
//    while(<cond>) [ { <body> } ] [ <;> ]
void while_loop()
{
    std::cout << "While" << std::endl;

    int counter{0};

    std::cout << "Start counter: " << counter << std::endl;

    while (counter < 5)
    {
        std::cout << counter << std::endl;
        ++counter;
    }

    std::cout << "End counter: " << counter << std::endl;
}

void while_input()
{
    std::cout << "While input" << std::endl;

    int counter{0};
    char input{'?'};

    std::cout << "Type 'e' for break the loop: " << std::endl;

    while (counter < 5 && input != 'e')
    {
        std::cout << counter << std::endl;
        std::cin >> input;
        ++counter;
    }
}

void exercice_laverie()
{
    std::cout << "Exercice While" << std::endl;

    int const machine_1_cap_max{5};
    int const machine_2_cap_max{10};

    std::cout << "Combien de kg de linge as-tu?" << std::endl;
    int kg{0};
    std::cin >> kg;

    if (kg <= machine_1_cap_max)
    {
        std::cout << "Mettre dans la première machine" << std::endl;
    }
    else if (kg <= machine_2_cap_max)
    {
        std::cout << "Mettre dans la deuxième machine" << std::endl;
    }
    else
    {
        int nb_machines{0};
        while (kg > machine_1_cap_max)
        {
            kg -= machine_2_cap_max;
            ++nb_machines;
        }

        std::cout << "Tu as besoin de " << nb_machines << " machine(s) de " << machine_2_cap_max << "kg." << std::endl;
        if (kg >= 0)
        {
            std::cout << "Le reste entre dans une machne de " << machine_1_cap_max << "kg." << std::endl;
        }
    }
}

void exercice_pgcd()
{
    std::cout << "Exercice while" << std::endl;

    std::cout << "A" << std::endl;
    int a{0};
    std::cin >> a;

    std::cout << "B" << std::endl;
    int b{0};
    std::cin >> b;

    int r{a % b};

    while (r != 0)
    {
        a = b;
        b = r;
        r = a % b;
    }

    std::cout << "PGCD = " << b << std::endl;
}

// Syntaxe:
//    do { <body> }
//    while(<cond>) [ { <body> } ] [ <;> ]
void do_while_loop()
{
    std::cout << "Do While" << std::endl;
    do
    {
        std::cout << "On passe quand même ici." << std::endl;
    } while (false);
}

void err_handling()
{
    std::cout << "Exercice Do/While" << std::endl;

    std::cout << "Quel jour es-stu né? ";
    int day{0};
    while (!(std::cin >> day))
    {
        std::cout << "Entrée invalide. Recommence." << std::endl;
        std::cin.clear();
        std::cin.ignore(255, '\n');
    }

    std::cout << "Quel mois es-tu né? ";
    int month{0};
    do
    {
        if (std::cin.fail())
        {
            std::cout << "Entrée invalide. Recommence." << std::endl;
            std::cin.clear();
            std::cin.ignore(255, '\n');
        }
    } while (!(std::cin >> month));

    std::cout << "Tu es né le " << day << "/" << month << "." << std::endl;
}

// Syntaxe :
//    for (<init>; <cond>; <iter>) [ { <body> } ][ <;> ]
void for_loop()
{
    std::cout << "For loop" << std::endl;
    for (int counter{10}; counter > 0; --counter)
    {
        std::cout << counter << std::endl;
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
