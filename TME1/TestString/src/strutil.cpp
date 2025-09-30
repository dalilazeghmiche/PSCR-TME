#include "strutil.h"
#include <cstddef>  // pour size_t

namespace pr {

// Calcul de la longueur d'une chaîne C (comme strlen)
size_t length(const char* s) {
    size_t len = 0;
    while (s && s[len] != '\0') len++;
    return len;
}

// Création d'une nouvelle copie d'une chaîne (comme strdup)
char* newcopy(const char* s) {
    if (!s) return nullptr;
    size_t len = length(s);
    char* copy = new char[len + 1]; // +1 pour le '\0'
    for (size_t i = 0; i <= len; i++) // copie y compris '\0'
        copy[i] = s[i];
    return copy;
}

// Comparaison de deux chaînes (comme strcmp)
int compare(const char* a, const char* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    size_t i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i])
            return a[i] - b[i];
        i++;
    }
    return a[i] - b[i];
}

} // namespace pr
