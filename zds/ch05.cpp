#include <string>
#include <vector>
#include "print.hpp"

// Vecteur : tableau dynamique
//    Taille non connue à la compilation
//
//   Point négatif :
//     - Récupérer un index inexistant d'un tableau nous renvoie n'importe quoi.
void use_vector()
{
    std::vector<int> const v_int{1, 2, 3, 4, 5};
    std::vector<double> const v_double{2.71828, 3.14159};
    std::vector<std::string> const v_string{"foo", "bar", "foobar"};
    std::vector<std::string> v_mut_string{};

    println(v_int[0]);
    println(v_double[1]);
    println(v_string[2]);
    println("");

    // Que se passe-t-il si on accède à un élément qui n'existe pas ?
    println(v_int[42]);    //!\ Non sûr
    println(v_double[42]); //!\ Non sûr
    println(v_string[42]); //!\ Non sûr

    // Récupérer la première et la dernière valeur d'un tableau
    auto const first_v_int = v_int.front();
    println(first_v_int);

    auto const last_v_int = v_int.back();
    println(last_v_int);

    // Afficher tous les éléments d'un tableau
    for (auto const &item : v_string)
    {
        println("For", item);
    }

    // Ajouter des éléments à un tableau
    if (v_mut_string.empty())
    {
        v_mut_string.push_back("foobar");
        v_mut_string.push_back("bar");
        v_mut_string.push_back("foo");
    }

    // Supprimer un élément d'un tableau
    v_mut_string.pop_back();

    for (auto const &item : v_mut_string)
    {
        println("For mut", item);
    }

    // Que se passera-t-il si on supprime un élément qui n'existe pas ?
    v_mut_string.pop_back();
    v_mut_string.pop_back();
    //! v_mut_string.pop_back(); //! Panic : Segmentation fault

    // Modifier un élément d'un tableau
    v_mut_string[0] = "foobar";
    println(v_mut_string[0]);
}

// Calcul de moyenne
void exercices_vector()
{
    std::vector<double> notes{};

    while (true)
    {
        println("Entre une note ? (-0 pour break)");
        double note{0.0};
        std::cin >> note;

        if (note < 0)
        {
            break;
        }

        notes.push_back(note);
    }

    if (notes.empty())
    {
        println("Aucune note");
    }
    else
    {
        double total{};
        for (auto const &note : notes)
        {
            total += note;
        }

        println("La moyenne est de :", total / notes.size(), ".");
    }
}

// Minimum est maximum
void exercices_vector_2()
{
    // Remplir un tableau de chiffre aléatoires
    std::vector<int> v_int{};

    for (int i = 0; i < 10; ++i)
    {
        v_int.push_back(rand() % 100);
    }

    println("les valeurs sont:", v_int);

    // Trouver le minimum et le maximum
    int min{v_int[0]};
    int max{v_int[0]};

    for (auto const &item : v_int)
    {
        if (item < min)
        {
            min = item;
        }

        if (item > max)
        {
            max = item;
        }
    }

    println("Min :", min, "Max :", max);
}

// Séparer les pairs des impairs
void exercices_vector_3()
{
    std::vector<int> v_pair{};
    std::vector<int> v_impair{};

    for (int i = 0; i < 10; ++i)
    {
        int const n = rand() % 100;
        if (n % 2 == 0)
        {
            v_pair.push_back(n);
        }
        else
        {
            v_impair.push_back(n);
        }
    }

    println("Les valeurs pairs sont :", v_pair);
    println("Les valeurs impairs sont :", v_impair);
}

// Compter les occurrences d'une valeur
void exercices_vector_4()
{
    std::vector<char> const v_char{'a', 'b', 'b', 'c', 'c', 'c', 'd', 'd', 'd', 'd'};
    char const letter_to_count = 'd';
    int count{};
    for (auto const &item : v_char)
    {
        if (item == letter_to_count)
        {
            count++;
        }
    }

    println("Le nombre de d est :", count);
}

int main()
{
    use_vector();
    // exercices_vector();
    exercices_vector_2();
    exercices_vector_3();
    exercices_vector_4();

    return EXIT_SUCCESS;
}
