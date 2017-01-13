#include <bits/stdc++.h>
#include "import.hpp"
#include "alias.hpp"
#include "alphabet.hpp"

void importForm(std::shared_ptr<std::set<std::string>> map) {
	std::string forms(&_binary___alphabet_forms_start, &_binary___alphabet_forms_end);
	std::istringstream file(forms);
	std::string line;
	while (std::getline(file, line)) {
		map->insert(line);
	}
}

void findForm(mapvecptr_t const maps, mapvecptr_t results, size_t index, std::string word, std::string form) {
	std::ofstream fout("temp", std::ios::app);
	auto thisMap = maps->at(index);
	auto thisResult = results->at(index);
	auto pos = thisMap->find(word);
	if (pos == thisMap->end()) { return; }
	std::string initialKey = pos->first;
	std::string initialForm = pos->second.back();
	size_t max = initialKey.length() + 10;
	vec_t entry;
	while (true) {
		std::string currentKey = pos->first;
		std::string currentForm = pos->second.back();
		if (currentForm[0] == ' ') { currentForm = currentForm.substr(1, currentForm.length() - 1); }
		if (currentKey != initialKey) {
			thisResult->insert(std::make_pair(initialKey, entry));
			break;
		}
		std::string str;
		size_t sz = 0;
		if (currentForm.find(form) == std::string::npos) {
			pos = std::next(pos);
		}
		else {
			for (std::string & i : pos->second) {
				if (sz++ == 1) {
					auto length = i.length();
					auto count = [&]() {
						size_t num = 0;
						for (size_t a = 0; a < i.length(); ) {
							auto x = i[a];
							if (!std::isalpha(x)) {
								num += 1;
								a += 2;
							}
							else { a += 1; }
						} return num; }();
						for (size_t j = 0; j < max - length + count; ++j) {
							i += " ";
						}
						str += "\033[1;31m" + i + "\033[2;35m";
				}
				else { str += i + '\t'; }
			}
			entry.push_back(str);
			thisResult->insert(std::make_pair(initialKey, entry));
			break;
		}
	}
}

void printForm(mapvecptr_t const maps, std::shared_ptr<std::multimap<std::string, std::string>> const & index, std::shared_ptr<std::set<std::string>> set) {
	std::ofstream fout("temp");
	mapvecptr_t results = std::make_shared<vecptr_t>();
	for (size_t i = 0; i < 7; ++i) {
		results->push_back(mapptr_t(new infl_t()));
	}
	printw("\n\nPlease insert your word here: ");
	std::string word;
	input(word, index);
	printw("\n\nNow insert the name of the inflection you wish to inquire: ");
	std::string inflectional_form;
	input(inflectional_form, set);
	std::thread t1(findForm, maps, results, 0, word, inflectional_form);
	std::thread t2(findForm, maps, results, 1, word, inflectional_form);
	std::thread t3(findForm, maps, results, 2, word, inflectional_form);
	std::thread t4(findForm, maps, results, 3, word, inflectional_form);
	std::thread t5(findForm, maps, results, 4, word, inflectional_form);
	std::thread t6(findForm, maps, results, 5, word, inflectional_form);
	std::thread t7(findForm, maps, results, 6, word, inflectional_form);
	t1.join(); t2.join(); t3.join();
	t4.join(); t5.join(); t6.join(); t7.join();
	auto resultSize = [&]() { size_t sz = 0; for (auto i : *results) { sz += i->size(); } return sz; }();
	if (resultSize == 0) {
		printw("\n\nWord not found.\n");
		display();
		return;
	}
	fout << "A total of " << resultSize;
	fout << ((resultSize > 1) ? " entries have" : " entry has");
	fout << " been found.\n";
	fout << "\n------------------------------\n\n";
	for (auto && i : *results) {
		for (auto && j : *i) {
			fout << "\033[1m" << j.first << "\033[0m" << "\n\n";
			for (auto && k : j.second) {
				fout << k << '\n';
			}
			fout << "\n------------------------------\n\n";
		}
	}
	fout.close(); display();
	results->clear();
}

