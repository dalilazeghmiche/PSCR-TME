#include <iostream>
using namespace std;

int main() {
    int tab[10];

    // Remplir le tableau avec les entiers 0 à 9
    for (int i = 0; i < 10; ++i) {
        tab[i] = i;
    }

    // Afficher le contenu du tableau
    cout << "Contenu du tableau : ";
    for (int i = 0; i < 10; ++i) {
        cout << tab[i] << " ";
    }
    cout << endl;

    // Vérification
    for (int i = 9; i >= 0; i--) {
    if (tab[i] - tab[i-1] != 1) {
        cout << "probleme !";
    }
}

    return 0;
}
