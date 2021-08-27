#include <ctime>
#include <fstream>
#include <vector>
#include "../print.hpp"

/// Lire dans un fichier
void read_file()
{
    std::ifstream file{"./testdata/output.txt"};

    std::string line{""};
    while (std::getline(file, line))
        std::cout << line << std::endl;

    file.close();
}

/// Ouvrir un fichier et écrire
///
//!  Point négatif:
//!    - Si le dossier "./testdata" n'existe pas,
//!      le fichier ne sera pas crée.
//!
//!    - On doit fermer manuellement le fichier
//!      lorsqu'on a fini de travailler avec.
//!      Contrairement au langage Rust où le fermer
//!      se ferme automatiquement dès que l'on sort
//!      de la portée de la fonction.
void read_and_create_file()
{
    std::ofstream file{"./testdata/output.txt", std::ios::app};

    std::srand(std::time(nullptr));
    int number{std::rand() % 1000000};

    file << "Il a écrit \"f(" << number << ")\" au tableau." << std::endl;
    file.close();
}

/// Statistiques sur des fichiers
void exercice_file()
{
    std::ifstream file{"testdata/output.txt"};
    std::string ligne{""};

    int total_lines{0};
    int total_chars{0};
    int total_words{0};

    bool prev_space_char{false};
    while (std::getline(file, ligne))
    {
        ++total_lines;

        for (char c : ligne)
        {
            if (isspace(c))
            {
                if (!prev_space_char)
                {
                    ++total_words;
                }

                prev_space_char = true;
            }
            else
            {
                ++total_chars;
                prev_space_char = false;
            }
        }

        if (!prev_space_char)
        {
            ++total_words;
            prev_space_char = true;
        }
    }

    println("Total de caractères :", total_chars);
    println("Total de lignes :", total_lines);
    println("Total de mots :", total_words);
}

int main()
{
    read_file();
    read_and_create_file();
    exercice_file();

    return EXIT_SUCCESS;
}
