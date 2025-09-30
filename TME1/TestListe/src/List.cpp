//ajout des bib
#include <cstddef>
#include <string>
#include <ostream>

// FAUTE : Il manque l’inclusion du fichier d’en-tête qui déclare List et Chainon
// => Sans `#include "List.hpp"`, le compilateur ne connaît pas ces classes.
#include "List.h"

namespace pr {

// ******************* Chainon
// FAUTE : Boucle récursive infinie dans length() (appel à length() au lieu de len)
// => Cela provoque un crash par stack overflow.
size_t Chainon::length() {
    size_t len = 1;
    if (next != nullptr) {
        len += next->length();
    }
    return len;  // correction ici
}

// Problème : le const dans le header n’est pas respecté dans 
//le cpp → ligne rouge.
//Solution : ajouter const
void Chainon::print(std::ostream & os) const{
    os << data;
    if (next != nullptr) {
        os << ", ";
        next->print(os); // déplacé dans le if
    }
}

// ******************  List
const std::string & List::operator[](size_t index) const {
    Chainon * it = tete;
    for (size_t i = 0; i < index; i++) {
        // FAUTE : Pas de vérification de null
        // => accès hors limites si index > taille
        if (it == nullptr) {
            throw std::out_of_range("Index hors limites"); 
        }
        it = it->next;
    }
    return it->data;
}

void List::push_back(const std::string& val) {
    if (tete == nullptr) {
        // FAUTE : Appel de constructeur Chainon avec un seul paramètre
        // alors que le constructeur défini prend (val, next).
        // Correction : fournir un second paramètre (nullptr).
        tete = new Chainon(val, nullptr);
    } else {
        Chainon * fin = tete;
        while (fin->next) {
            fin = fin->next;
        }
        fin->next = new Chainon(val, nullptr);
    }
}

void List::push_front(const std::string& val) {
    tete = new Chainon(val, tete);
}

//  empty() n’est pas const

//Dans le header
bool List::empty() {
    return tete == nullptr;
}

size_t List::size() const {
    if (tete == nullptr) {
        return 0;
    } else {
        return tete->length();
    }
}

} // namespace pr

// FAUTE : operator<< accède à un membre privé (tete) de List
// => Doit être déclaré friend dans List.hpp ou utiliser un getter.
std::ostream & operator<<(std::ostream & os, const pr::List & vec) {
    os << "[";
    if (vec.tete != nullptr) {
        vec.tete->print(os);
    }
    os << "]";
    return os;
}
