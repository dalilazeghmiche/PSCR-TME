#include "String.h"
#include "strutil.h" // pour length, newcopy, compare, newcat
#include <iostream>

namespace pr {

// Constructeur depuis C-string
String::String(const char* s) {
    std::cout << "String constructor called for: " << s << std::endl;
    data = newcopy(s); // alloue et copie la chaîne
}

// Destructeur
String::~String() {
    std::cout << "String destructor called for: " 
              << (data ? data : "(null)") << std::endl;
    delete[] data; // libère la mémoire
    data = nullptr;
}

// Constructeur par copie
String::String(const String& other) {
    std::cout << "Copy constructor called for: " << other.data << std::endl;
    data = newcopy(other.data);
}

// Opérateur d’affectation par copie
String& String::operator=(const String& other) {
    if (this != &other) {
        delete[] data;
        data = newcopy(other.data);
    }
    return *this;
}

// Constructeur par déplacement
String::String(String&& other) noexcept {
    std::cout << "Move constructor called" << std::endl;
    data = other.data;
    other.data = nullptr;
}

// Opérateur d’affectation par déplacement
String& String::operator=(String&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}

// Opérateur d’affichage
std::ostream& operator<<(std::ostream& os, const String& str) {
    return os << (str.data ? str.data : "(null)");
}

// Opérateur d’égalité
bool operator==(const String& a, const String& b) {
    return compare(a.data, b.data) == 0;
}

// Opérateur <
bool String::operator<(const String& other) const {
    return compare(data, other.data) < 0;
}

// Fonction utilitaire newcat
char* newcat(const char* a, const char* b) {
    size_t len_a = length(a);
    size_t len_b = length(b);
    char* res = new char[len_a + len_b + 1];
    for (size_t i = 0; i < len_a; i++) res[i] = a[i];
    for (size_t i = 0; i < len_b; i++) res[len_a + i] = b[i];
    res[len_a + len_b] = '\0';
    return res;
}

// Opérateur de concaténation
String operator+(const String& a, const String& b) {
    return String(newcat(a.data, b.data));
}

} // namespace pr
