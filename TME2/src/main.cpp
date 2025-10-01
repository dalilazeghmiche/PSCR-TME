#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <string>
#include <algorithm>
#include <vector>

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
			if (nombre_lu % 100 == 0)
				// on affiche un mot "propre" sur 100
				cout << nombre_lu << ": "<< word << endl;
			nombre_lu++;
		}
	input.close();
	cout << "Finished parsing." << endl;
	cout << "Found a total of " << nombre_lu << " words." << endl;

	} else if (mode == "unique") {
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
}


 else {
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


