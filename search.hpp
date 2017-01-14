#ifndef __SEARCH_HPP__
#define __SEARCH_HPP__
#include <string>
#include <map>
#include <fstream>
#include <memory>
#include <regex>
#include <ncurses.h>
#include "entry.hpp"
#include "import.hpp"
#include "autocomplete.hpp"

void display() {
	system("cat temp | less && rm temp");
	clear();
	printw("Please choose one of the options below: \n");
	printw("\ti: Find definition in the Icelandic - English dictionary;\n");
	printw("\te: Search English words in the mentioned dictionary;\n");
	printw("\tt: Search text in the mentioned dictionary;\n");
	printw("\tr: Search with regex in the mentioned dictionary;\n");
	printw("\tf: Insert a word and find its original form;\n");
	printw("\ts: Print out a specific inflection of the inserted word;\n");
	printw("\tp: Print out all inflectional forms of the inserted word;\n");
	printw("\tq: Quit.\n");
	printw(">> ");
}


void insert(std::string & word) {
	int c;
	while ((c = getch()) && (c != '\n')) {
		clear();
		if (c == KEY_BACKSPACE) {
			clear();
			delch();
			if (charType(word.back()) == 0 || charType(word.back()) == 1) {
				word = word.substr(0, word.length() - 1);
			}
			else if (charType(word.back()) == 5) {
				delch();
				word = word.substr(0, word.length() - 2);
			}
		}
		else if (charType(c) == 0 || charType(c) == 1) {
			word += c;
		}
		else if (charType(c) == 5) { // special characters
			word += c; c = getch(); word += c;
		}
		printw(">> ");
		printw(word.c_str());
		refresh();
	}
}


std::string lower(std::string const s) {
	std::string str = s;
	for (size_t i = 0; i < str.length();) {
		if (std::isalpha(str[i]) && std::isupper(str[i])) {
			str[i] = str[i] - 'A';
			i += 1;
		}
		// not ascii characters
		else {
			std::string temp;
			temp += str[i];
			temp += str[i + 1];
			if (temp == "Þ") { temp = "þ"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "Ð") { temp = "ð"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "Æ") { temp = "æ"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "Œ") { temp = "œ"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "Ö") { temp = "ö"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "Á") { temp = "á"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "É") { temp = "é"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "Í") { temp = "í"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "Ó") { temp = "ó"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "Ú") { temp = "ú"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "Ý") { temp = "ý"; str.erase(i, 2); str.insert(i, temp); }
			else if (temp == "Ø") { temp = "ø"; str.erase(i, 2); str.insert(i, temp); }
			else {}
			i += 2;
		}
	}
	return str;
}

void searchEng(std::shared_ptr<std::vector<mapentry_t>> dict) {
	printw("\n\nPlease insert your word here: ");
	std::string str;
	insert(str);
	std::ofstream fout("temp");
	auto result = std::map<std::string, std::string>();
	for (auto && map : *dict) {
		for (auto && i : *map) {
			auto word = i.second->getDef();
			for (auto j : *word) {
				auto orig = lower(j);
				auto pos = orig.find(" " + str + ",");
				if (pos != std::string::npos) {
					std::string temp = j.substr(pos + 1, str.length());
					temp = "\033[1;31;4m" + temp + "\033[0m";
					j.replace(pos + 1, str.length(), temp);
					std::string key = "\033[1m" + i.first + "\033[0m";
					result.insert(std::make_pair(key, j));
				}
			}
		}
	}
	if (result.empty()) {
		printw("Word not found.\n");
	}
	else {
		fout << "\n------------------------------\n\n";
		fout << "A total of " << result.size();
		fout << ((result.size() > 1) ? " entries have" : " entry has");
		fout << " been found."
			<< "\n------------------------------\n\n";
		for (auto i : result) {
			if (i.second[0] == '\t') {
				i.second.erase(i.second.begin());
			}
			fout << i.first << ":  ";
			fout << i.second << "\n------------------------------\n\n";
		}
	}
	fout.close(); display();
}

void searchTxt(std::shared_ptr<std::vector<mapentry_t>> dict) {
	printw("\n\nPlease insert your word here: ");
	std::string str;
	insert(str);
	std::ofstream fout("temp");
	auto result = std::map<std::string, std::string>();
	for (auto && map : *dict) {
		for (auto && i : *map) {
			auto word = i.second->getDef();
			for (auto j : *word) {
				auto orig = lower(j);
				auto pos = orig.find(str);
				if (pos != std::string::npos) {
					std::string temp = j.substr(pos, str.length());
					temp = "\033[1;31;4m" + temp + "\033[0m";
					j.replace(pos, str.length(), temp);
					std::string key = "\033[1m" + i.first + "\033[0m";
					result.insert(std::make_pair(key, j));
				}
			}
		}
	}
	if (result.empty()) {
		printw("Word not found.\n");
	}
	else {
		fout << "\n------------------------------\n\n";
		fout << "A total of " << result.size();
		fout << ((result.size() > 1) ? " entries have" : " entry has");
		fout << " been found.\n"
			<< "------------------------------\n\n";
		for (auto i : result) {
			if (i.second[0] == '\t') {
				i.second.erase(i.second.begin());
			}
			fout << i.first << ":  ";
			fout << i.second << "\n------------------------------\n\n";
		}
	}
	fout.close(); display();
}

void searchReg(std::shared_ptr<std::vector<mapentry_t>> dict) {
	printw("\n\nPlease insert your word here: ");
	std::string str;
	insert(str);
	std::ofstream fout("temp");
	auto result = std::map<std::string, std::string>();
	std::regex pattern(str);
	std::smatch res;
	for (auto && map : *dict) {
		for (auto && i : *map) {
			auto word = i.second->getDef();
			for (auto j : *word) {
				auto orig = lower(j);
				if (std::regex_search(orig, res, pattern)) {
					auto pos = orig.find(res.str());
					if (pos != std::string::npos) {
						std::string temp = j.substr(pos, str.length());
						temp = "\033[1;31;4m" + temp + "\033[0m";
						j.replace(pos, str.length(), temp);
					}
					std::string key = "\033[1m" + i.first + "\033[0m";
					result.insert(std::make_pair(key, j));
				}
			}
		}
	}
	if (result.empty()) {
		printw("Word not found.\n");
	}
	else {
		fout << "\n------------------------------\n\n";
		fout << "A total of " << result.size();
		fout << ((result.size() > 1) ? " entries have" : " entry has");
		fout << " been found.\n"
			<< "------------------------------\n\n";
		for (auto i : result) {
			if (i.second[0] == '\t') {
				i.second.erase(i.second.begin());
			}
			fout << i.first << ":  ";
			fout << i.second << "\n------------------------------\n\n";
		}
	}
	fout.close(); display();
}

void searchIcelandic(std::shared_ptr<std::vector<mapentry_t>> dict, resultptrmapvecptr_t results, std::string word, size_t index) {
	std::ofstream fout("temp", std::ios::app);
	auto thisMap = dict->at(index);
	auto thisResult = results->at(index);
	auto itr = thisMap->find(word);
	if (itr == thisMap->end()) { return; }
	else {
		while (itr->first == word) {
			std::vector<std::string> temp;
			std::string value;
			auto ptr = itr->second->getDef();
			for (auto && i : *ptr) {
				value += i + ' ';
			}
			thisResult->insert(std::make_pair(itr->first, value));
			itr = std::next(itr);
		}
	}
}

void searchIcl(std::shared_ptr<std::vector<mapentry_t>> & dict, std::shared_ptr<std::set<std::string>> wordindex) {
	std::ofstream fout("temp");
	std::string word;
	auto results = resultptrmapvecptr_t(new resultmapvecptr_t());
	for (auto i = 0; i < 2; ++i) {
		results->push_back(resultvecptr_t(new resultvec_t()));
	}
	printw("\n\nPlease insert your word here: ");
	input_icelandic(word, wordindex);
	std::thread t1(searchIcelandic, dict, results, word, 0);
	std::thread t2(searchIcelandic, dict, results, word, 1);
	t1.join(); t2.join();
	auto size = [&]() { size_t sz = 0; for (auto && i : *results) { sz += i->size(); } return sz; }();
	if (size == 0) { printw("Word not found.\n"); }
	else {
		for (auto && i : *results) {
			for (auto && j : *i) {
				fout << "\n------------------------------\n\n";
				fout << "\033[1m" + j.first + "\033[0m" << ", ";
				fout << j.second << '\n';
			}
			fout << "\n------------------------------\n\n";
		}
	}
	fout.close(); display();
}

#endif
