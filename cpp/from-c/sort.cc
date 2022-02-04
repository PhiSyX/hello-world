#include <algorithm>
#include <vector>
#include <parallel/algorithm>
#include <memory>

int cmp(const void *a, const void *b);
void fn();

// C         qsort(): 13.4 secondes (13.4 CPU)
// C++   std::sort(): 8.0 secondes  (8.0 CPU)
// C++ parallel sort: 1.7 secondes  (11.8 secondes CPU)
int main(int argc, char **argv)
{
    // Le mot clé "auto" indique au compilateur d’utiliser l’expression
    // d’initialisation d’une variable déclarée pour déduire son type.
    //
    // Le mot clé "auto" est un espace reservé pour un type, mais
    // il n'est pas lui-même un type. Par conséquent, le mot clé "auto"
    // ne peut pas être utilisé dans les casts ou les opérateurs
    // tels que sizeof et typeid (pour C++/CLI).
    auto lim = atoi(argv[1]);

    // Les deux lignes suivantes définissent un vecteur contenant
    // des entiers, et réservent suffisamment d'espace pour
    // le nombre d'entrées que l'on veut. Il s'agit d'une optimisation
    // facultative.
    std::vector<int> vec;
    vec.reserve(lim);

    // La boucle while remplit le vecteur avec des nombres "aléatoires".
    while (--lim)
        vec.push_back(random());

    if (*argv[2] == 'q')
    {
        // On appele la fonction "qsort" de C pour opérer sur le vecteur C++.
        // std::vector est explicitement conçu pour être interopérable
        // avec les opérations sur les "raw pointers".
        // Il est censé pouvoir être transmis à la bibliothèque C ou
        // aux appels système.
        // Il stocke ses données dans un bloc de mémoire consécutif
        // qui peut être modifié à volonté.
        qsort(&vec[0], vec.size(), sizeof(int), cmp);
    }
    // Les lignes suivantes utilisent les fonctions de tri de C++.
    else if (*argv[2] == 'p')
    {
        __gnu_parallel::sort(
            vec.begin(), vec.end(),
            [](const auto &a, const auto &b)
            { return a < b; });
    }
    else
    {
        std::sort(
            vec.begin(), vec.end(),
            [](const auto &a, const auto &b)
            { return a < b; });
    }
}

int cmp(const void *a, const void *b)
{
    if (*(int *)a < *(int *)b)
    {
        return -1;
    }
    else if (*(int *)a > *(int *)b)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
