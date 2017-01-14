#ifndef __SEARCH_INFLECTION__
#define __SEARCH_INFLECTION__
#include "thread"
#include "alias.hpp"
#include "import.hpp"
#include "search.hpp"
#include "autocomplete.hpp"

void searchInfl(mapvecptr_t const maps, resultptrmapvecptr_t results, size_t index, std::string word) {
	std::ofstream fout("temp", std::ios::app);
	auto thisMap = maps->at(index);
	auto thisResult = results->at(index);
	for (auto i : *thisMap) {
		if (i.second.size() == 0) { continue; } //so segfault won't take place
		std::string j = i.second.at(1);
		if (word == j) {
			std::string key = i.first;
			std::string value;
			size_t pos = 0;
			for (auto && k : i.second) {
				if (pos == 1) { value += "\033[1;31m" + k + "\033[2;35m ";}
				else { value += k + " "; }
				++pos;
			}
			thisResult->insert(std::make_pair(key, value));
		}
	}
}

void find_orig(mapvecptr_t const maps, std::shared_ptr<std::set<std::string>> const inflections) {
	printw("\n\nPlease insert your word here: ");
	std::string word;
	input_inflections(word, inflections);
	std::ofstream fout("temp", std::ios::trunc);
	resultptrmapvecptr_t results = std::make_shared<resultmapvecptr_t>();
	for (size_t i = 0; i < 7; ++i) {
		results->push_back(resultvecptr_t(new resultvec_t()));
	}
	std::thread t1(searchInfl, maps, results, 0, word);
	std::thread t2(searchInfl, maps, results, 1, word);
	std::thread t3(searchInfl, maps, results, 2, word);
	std::thread t4(searchInfl, maps, results, 3, word);
	std::thread t5(searchInfl, maps, results, 4, word);
	std::thread t6(searchInfl, maps, results, 5, word);
	std::thread t7(searchInfl, maps, results, 6, word);
	t1.join(); t2.join(); t3.join(); 
	t4.join(); t5.join(); t6.join(); t7.join();
	auto resultSize = [&]() { size_t sz = 0; for (auto i : *results) { sz += i->size(); } return sz; }();
	if ( resultSize == 0 ) {
        printw("Word not found.");
		display();
    }
    else {
		std::string line;
		int y, x;
		getmaxyx(stdscr, y, x);
		for (size_t i = 0; i < x; ++i) { line += "-"; }
		line = "\n" + line + "\n\n";
		fout << "A total of " << resultSize;
		fout << ((resultSize > 1) ? " entries have" : " entry has");
		fout << " been found.\n" << line;
        for (auto i : *results) {
			for (auto j : *i) {
				fout << "\033[1m" << j.first << "\033[0m\t" << j.second << line;
			}
        }
    }
	fout.close(); display();
	results->clear();
}


#endif
