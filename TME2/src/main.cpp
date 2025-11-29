#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <string>
#include <algorithm>
#include <vector>
#include "HashMap.h"
#include <unordered_map>


// helper to clean a token (keep original comments near the logic)
static std::string cleanWord(const std::string& raw) {
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	static const std::regex re( R"([^a-zA-Z])");
	// élimine la ponctuation et les caractères spéciaux
	std::string w = std::regex_replace(raw, re, "");
	// passe en lowercase
	std::transform(w.begin(), w.end(), w.begin(), ::tolower);
	return w;
}

int main(int argc, char** argv) {
	using namespace std;
	using namespace std::chrono;

	// Allow filename as optional first argument, default to project-root/WarAndPeace.txt
	// Optional second argument is mode (e.g. "count" or "unique").
	string filename = "../WarAndPeace.txt";
	string mode = "count";
	if (argc > 1) filename = argv[1];
	if (argc > 2) mode = argv[2];

	ifstream input(filename);
	if (!input.is_open()) {
		cerr << "Could not open '" << filename << "'. Please provide a readable text file as the first argument." << endl;
		cerr << "Usage: " << (argc>0?argv[0]:"TME2") << " [path/to/textfile]" << endl;
		return 2;
	}
	cout << "Parsing " << filename << " (mode=" << mode << ")" << endl;
	
	auto start = steady_clock::now();
	
	// prochain mot lu
	string word;

	if (mode == "count") {
		size_t nombre_lu = 0;
	
		// default counting mode: count total words
		while (input >> word) {
			// élimine la ponctuation et les caractères spéciaux
			word = cleanWord(word);

			// word est maintenant "tout propre"
			/* (nombre_lu % 100 == 0)
				// on affiche un mot "propre" sur 100
				cout << nombre_lu << ": "<< word << endl;*/
			nombre_lu++;
		}
	input.close();
	cout << "Finished parsing." << endl;
	cout << "Found a total of " << nombre_lu << " words." << endl;

	}

     else if (mode == "unique") {
    std::vector<std::string> seen;  // contiendra tous les mots uniques
    
    while (input >> word) {
        word = cleanWord(word);
        if (word.empty()) continue;  // ignorer les tokens vides
         
        // vérifier si le mot est déjà dans le vector
        bool found = false;
        for (const auto& w : seen) {
            if (w == word) { found = true; break; }
        }

        if (!found) {
            seen.push_back(word);
        }
    }

    input.close();
    std::cout << "Found " << seen.size() << " unique words." << std::endl;
    // Afficher tous les mots uniques
    //std::cout << "Unique words found:" << std::endl;
	
    /*for (const auto& w : seen) {
        std::cout << w << std::endl;
    }*/

}

else if (mode == "freq") {
    std::vector<std::pair<std::string, int>> freq;
    while (input >> word) {
        word = cleanWord(word);
        if (word.empty()) continue;

        bool found = false;
        for (auto& p : freq) {
            if (p.first == word) {
                p.second++;   // incrémenter le compteur
                found = true;
                break;
            }
        }

        if (!found) {
            freq.push_back({word, 1});  // nouveau mot rencontré
        }
    }

    input.close();

    // rechercher les mots demandés
    auto getFreq = [&](const std::string& target) {
        for (const auto& p : freq) {
            if (p.first == target) return p.second;
        }
        return 0; // si jamais le mot n’est pas trouvé
    };

    std::cout << "Occurrences:" << std::endl;
    std::cout << "war   : " << getFreq("war") << std::endl;
    std::cout << "peace : " << getFreq("peace") << std::endl;
    std::cout << "toto  : " << getFreq("toto") << std::endl;




	// 🔹 Étape 5 : trier par nombre d'occurrences décroissantes
std::sort(freq.begin(), freq.end(),
          [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
              return a.second > b.second;  // tri décroissant
          });

// 🔹 Afficher les 10 mots les plus fréquents
std::cout << "\nLes 10 mots les plus fréquents :" << std::endl;
for (size_t i = 0; i < 10 && i < freq.size(); ++i) {
    std::cout << freq[i].first << " (" << freq[i].second << " occurrences)" << std::endl;
}
}
  

else if (mode == "freqhash") {
    // Exemple : taille initiale = 100

    size_t initial_table_size = 1024; // tester 100, 1024, 10000
    HashMap<std::string,int> freqmap(initial_table_size);
auto start = std::chrono::steady_clock::now();  // début du chronomètre
    while (input >> word) {
        word = cleanWord(word);
        if (word.empty()) continue;

        int* val = freqmap.get(word);
        if (val) {
            (*val)++; // incrémenter si déjà présent
        } else {
            freqmap.put(word, 1); // sinon ajouter
        }
    }

    input.close();
auto end = std::chrono::steady_clock::now();    // fin du chronomètre
auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

std::cout << "Total runtime (wall clock) : " << elapsed << " ms" << std::endl;

    // afficher quelques mots spécifiques
    std::cout << "Occurrences:" << std::endl;
    std::cout << "war   : " << (freqmap.get("war") ? *freqmap.get("war") : 0) << std::endl;
    std::cout << "peace : " << (freqmap.get("peace") ? *freqmap.get("peace") : 0) << std::endl;
  
  std::cout << "toto  : " << (freqmap.get("toto") ? *freqmap.get("toto") : 0) << std::endl;

  auto kvPairs = freqmap.toKeyValuePairs();

// trier par nombre d'occurrences décroissantes
std::sort(kvPairs.begin(), kvPairs.end(), 
    [](const std::pair<std::string,int>& a, const std::pair<std::string,int>& b) {
        return a.second > b.second;
    });

// afficher les 10 mots les plus fréquents
std::cout << "\nLes 10 mots les plus fréquents :" << std::endl;
for (size_t i = 0; i < 10 && i < kvPairs.size(); ++i) {
    std::cout << kvPairs[i].first << " (" << kvPairs[i].second << " occurrences)" << std::endl;
}

}
if (mode == "freqstd") {
    std::unordered_map<std::string,int> freqmap;
    std::string word;

    // Lecture du fichier et comptage
    while (input >> word) {
        word = cleanWord(word);
        if (word.empty()) continue;
        freqmap[word]++;
    }
    input.close();

    // 2️⃣ Extraire les paires clé/valeur dans un vecteur et trier
    std::vector<std::pair<std::string,int>> kvPairs(freqmap.begin(), freqmap.end());
    std::sort(kvPairs.begin(), kvPairs.end(),
              [](const std::pair<std::string,int>& a,
                 const std::pair<std::string,int>& b) {
                     return a.second > b.second;
              });

    // 3️⃣ Afficher les 10 mots les plus fréquents et quelques mots spécifiques
    std::cout << "Occurrences:\n";
    std::cout << "war   : " << freqmap["war"] << "\n";
    std::cout << "peace : " << freqmap["peace"] << "\n";
    std::cout << "toto  : " << freqmap["toto"] << "\n";

    std::cout << "\nTop 10 mots les plus fréquents :" << std::endl;
    for (size_t i = 0; i < 10 && i < kvPairs.size(); ++i) {
        std::cout << kvPairs[i].first << " (" << kvPairs[i].second << " occurrences)" << std::endl;
    }

    // Mesure du temps total
    auto end = std::chrono::steady_clock::now();
    std::cout << "Total runtime (wall clock) : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms" << std::endl;
}


  else{
		// unknown mode: print usage and exit
		cerr << "Unknown mode '" << mode << "'. Supported modes: count, unique, freq" << endl;
		input.close();
		return 1;
	}

	// print a single total runtime for successful runs
	auto end = steady_clock::now();
	cout << "Total runtime (wall clock) : " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

	return 0;
}


