#ifndef __IMPORT_INFLECTION__
#define __IMPORT_INFLECTION__
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include "alphabet.hpp"
#include "import.hpp"
#include "alias.hpp"

void import_infl(std::shared_ptr<std::vector<mapptr_t>> mapvec, size_t i) {
	std::string inf = *alphabet[i];
	std::istringstream file(inf);
    std::string line;
	auto map = mapvec->at(i - 1);
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        std::string word;
        std::string temp;
        vec_t tmpvec;
        while (iss >> word) {
            if (word.back() == ';') {
				auto modif = word.substr(0, word.length() - 1);
                if (temp.length() == 0) { temp = modif; }
				else { temp += " " + modif; }
				if (key.length() == 0) { key = modif; }
				else { tmpvec.emplace_back(temp); }
                temp = "";
            }
            else { temp += " " + word; }
        }
        map->insert(std::make_pair(key, tmpvec));
    }
}


#endif
