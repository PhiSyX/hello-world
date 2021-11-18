#include <algorithm>
#include <list>
#include <vector>
#include "../print.hpp"

// Syntaxe fonction
//
//  <fn_return_type> <fn_name>((<param_type> <param_name>),*)
//  { <body> }

// Exemple de fonctions : cas les plus simples
void simple_case()
{
    std::string s{"Salut toi"};
    s.pop_back();
    s.clear();
    println(s);

    std::list<int> l{5, 4, 3, 2, 1};
    l.sort();
    println(l);
}

// Exemples de fonctions : renvoyant une valeur
void ret_val()
{
    std::vector<int> const v_int{1, 2, 3, 4, 5};
    auto const first{v_int.front()};
    auto const last{v_int.back()};
}

// Exemples de fonction : fonction attendant une valeur
void waiting_val()
{
    std::string s{"du texte"};
    s.push_back('!');

    std::vector<char> v_char{};
    v_char.assign(5, 'C');
}

// Exemple de fonctions : Fusion
void fusion()
{
    std::vector<int> v_int{1, 2, 3, 4, 5};
    auto it = std::begin(v_int);
    std::count(std::begin(v_int), std::end(v_int), 2);

    std::string s{""};
    std::getline(std::cin, s);
    auto len{std::size(s)};
}

// PGCD
int pgcd(int a, int b)
{
    int r{a % b};

    while (r != 0)
    {
        a = b;
        b = r;
        r = a % b;
    }

    return b;
}

// Somme
int somme(int n)
{
    return (n * (n + 1)) / 2;
}

// Ou exclusif
bool ou_exclusif(bool a, bool b)
{
    return (a && !b) || (b && !a);
}

void exercice_1()
{
    int const a{845};
    int const b{314};

    println("Le PGCD de", a, "et", b, "vaut", pgcd(a, b), ".");
    println("La somme de tous les entiers de 1 à 25 vaut", somme(25), ".");

    std::cout << std::boolalpha;
    println("XOR(true, false) vaut", ou_exclusif(true, false), ".");
}

// Afficher un rectangle
void exercice_rect(int h, int w)
{
    for (auto i{0}; i < h; ++i)
    {
        for (auto j{0}; j < w; ++j)
        {
            std::cout << "*";
        }

        std::cout << std::endl;
    }
}

void setup_rect()
{
    exercice_rect(2, 5);
    exercice_rect(4, 3);
}

// Distributeur d’argent
std::vector<int> cash_dispenser(int total, std::vector<int> available_denom)
{
    std::vector<int> result{};

    for (auto denom : available_denom)
    {
        result.push_back(total / denom);
        total %= denom;
    }

    return result;
}

void show_dispenser(std::vector<int> total, std::vector<int> available_denom)
{
    int i{0};
    for (auto denom : total)
    {
        if (denom != 0)
        {
            println(denom, "x", available_denom[i], "euros");
        }
        ++i;
    }
}

void setup_cash_dispenser()
{
    std::vector<int> const available_denom{500, 200, 100, 50, 20, 10, 5, 2, 1};
    auto const cash_denom{cash_dispenser(285, available_denom)};
    show_dispenser(cash_denom, available_denom);

    std::vector<int> const other_available_denom{200, 50, 10, 1};
    auto const cash_denom_45874{cash_dispenser(45874, other_available_denom)};
    show_dispenser(cash_denom_45874, other_available_denom);
}

// Parenthésage
bool exercice_parentheses(std::string expr)
{
    int opening{0};
    int closing{0};

    for (auto ch : expr)
    {
        if (ch == '(')
        {
            ++opening;
        }
        else
        {
            ++closing;
        }

        if (closing > opening)
        {
            return false;
        }
    }

    return opening == closing;
}

void setup_parentheses()
{
    std::cout << std::boolalpha;

    println(exercice_parentheses("((()))"));
    println(exercice_parentheses("((())"));
    println(exercice_parentheses("))(("));
}

int main(int argc, char const *argv[])
{
    simple_case();
    ret_val();
    waiting_val();
    fusion();
    exercice_1();
    setup_rect();
    setup_cash_dispenser();
    setup_parentheses();

    return EXIT_SUCCESS;
}
