#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__
#include <thread>
#include "search.hpp"
#include "autocomplete.hpp"

void importIndex(std::shared_ptr<std::multimap<std::string, std::string>> index) {
	std::string sourceFile(&_binary___alphabet_index_start, &_binary___alphabet_index_end);
	std::istringstream iss(sourceFile);
	std::string line;
	while (std::getline(iss, line)) {
		std::string key;
		std::string value;
		std::istringstream temp(line);
		temp >> key;
		key = key.substr(0, key.length() - 1);
		temp >> value;
		value = value.substr(0, value.length() - 1);
		index->insert(std::make_pair(key, value));
	}
}

void plotQuery(mapvecptr_t const maps, mapvecptr_t results, size_t index, std::string word) {
	auto thisMap = maps->at(index);
	auto thisResult = results->at(index);
	auto pos = thisMap->find(word);
	if (pos == thisMap->end()) { return; }
	std::string initialKey = pos->first;
	std::string initialValue = pos->second[0];
	size_t max = initialKey.length() + 10;
	vec_t entry;
	while (true) {
		std::string currentKey = pos->first;
		std::string currentValue = pos->second[0];
		if (currentKey != initialKey) { 
			thisResult->insert(std::make_pair(initialKey, entry));
			break; 
		}
		std::string str;
		size_t sz = 0;
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
		if (currentValue != initialValue) {
			thisResult->insert(std::make_pair(initialKey, entry));
			initialValue = currentValue;
			entry.clear();
		}
		pos = std::next(pos);
	}
}

void plot(mapvecptr_t const maps, std::shared_ptr<std::multimap<std::string, std::string>> const & index) {
	printw("\n\nPlease insert your word here: ");
	std::string word;
	input(word, index);
	std::ofstream fout("temp", std::ios::trunc);
	mapvecptr_t results = std::make_shared<vecptr_t>();
	for (size_t i = 0; i < 7; ++i) {
		results->push_back(mapptr_t(new infl_t()));
	}
	std::thread t1(plotQuery, maps, results, 0, word);
	std::thread t2(plotQuery, maps, results, 1, word);
	std::thread t3(plotQuery, maps, results, 2, word);
	std::thread t4(plotQuery, maps, results, 3, word);
	std::thread t5(plotQuery, maps, results, 4, word);
	std::thread t6(plotQuery, maps, results, 5, word);
	std::thread t7(plotQuery, maps, results, 6, word);
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

#endif