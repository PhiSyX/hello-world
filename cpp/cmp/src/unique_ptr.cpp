#include <iostream>
#include <memory>
#include <vector>

using namespace std;

/// `unique_ptr` est censé être un moyen d'indiquer qu'un pointeur donné est
///  le "propriétaire" unique d'un élément de données.
///
/// Si on veut transférer la propriété d'un unique_ptr à un autre, on doit
/// appeler `move` sur lui. Après ça, toute tentative d'accès aux données en
//  utilisant `unique_ptr` d'origine constitue une erreur d'exécution.
///
/// Ce qui est très bien, sauf pour deux choses :
///    - Par "erreur d'exécution", il faut comprendre que le pointeur
///      intégré de `unique_ptr` original est muté en `nullptr`,
///      de sorte que l'on doit perpétuellement vérifier `unique_ptr`
///      pour `nullptr`, sinon on obtiendrait un "segmentation fault".
///
///    - Dans de nombreuses situations, on préfère avoir une garantie au
///      moment de la compilation qu'on ne déréférencera jamais `nullptr`,
///      de sorte qu'on ait pas de bugs de sécurité mémoire cachés
///      dans le code qui ne se produisent que bien après le début du programme.
void use_unique_ptr_segfault()
{
    unique_ptr<int> orig(new int(5));
    cout << *orig << endl;
    auto stolen = move(orig);
    cout << *orig << endl;
}

void use_unique_ptr_smart_pointer()
{
    vector<unique_ptr<int>> v_int;
    v_int.push_back(make_unique<int>(5));
    cout << *v_int[0] << endl;

    auto ptr_to_5 = move(v_int[0]);
    cout << *ptr_to_5 << endl;

    // Segmentation fault
    cout << *v_int[0] << endl;
}

int main(int argc, char const *argv[])
{
    // use_unique_ptr_segfault();
    use_unique_ptr_smart_pointer();
    return EXIT_SUCCESS;
}
