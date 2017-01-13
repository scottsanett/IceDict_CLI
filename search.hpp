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
	printw("\tr: Search with regular expression in the mentioned dictionary;\n");
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

void searchEng(mapentry_t & map) {
	printw("\n\nPlease insert your word here: ");
	std::string str;
	insert(str);
	std::ofstream fout("temp");
	auto result = std::map<std::string, std::string>();
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

void searchTxt(mapentry_t & map) {
	printw("\n\nPlease insert your word here: ");
	std::string str;
	insert(str);
	std::ofstream fout("temp");
	auto result = std::map<std::string, std::string>();
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

void searchReg(mapentry_t & map) {
	printw("\n\nPlease insert your word here: ");
	std::string str;
	insert(str);
	std::ofstream fout("temp");
	auto result = std::map<std::string, std::string>();
	std::regex pattern(str);
	std::smatch res;
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

void searchIcl(mapentry_t & dict) {
	std::ofstream fout("temp");
	std::string word;
	printw("\n\nPlease insert your word here: ");
	int c;
	size_t position;
	std::string chosen;
	std::vector<std::string> options;
	while ((c = getch()) && (c != '\n')) {
		clear();
		if (c == KEY_BACKSPACE) {
			position = 0;
			options.clear();
			clear();
			delch(); 
			if (charType(word.back()) == 0 || charType(word.back()) == 1) {
				word = word.substr(0, word.length() - 1);
			}
			else if (charType(word.back()) == 5) {
				delch();
				word = word.substr(0, word.length() - 2);
			}
			autocomplete(dict, word, options);
		}
		else if (c == KEY_DOWN) { // autocomplete
			clear();
			if (position < options.size()) {
				++position;
			}
			word = options[position];
		}
		else if (c == KEY_UP) {
			clear();
			if (position != 0) {
				--position;
			}
			word = options[position];
		}
		else if (charType(c) == 0 || charType(c) == 1) { 
			position = 0; 
			options.clear();
			word += c; 
			autocomplete(dict, word, options);
		}
		else if (charType(c) == 5) { // special characters
			position = 0;
			options.clear();
			word += c; c = getch(); word += c;
			autocomplete(dict, word, options);
		}
		if (options.size() > 0) {
			printw(">> ");
			for (auto i : word) {
				addch(i | A_BOLD);
			}
			printw("\n\n");
			for (size_t pos = 0; pos < options.size(); ++pos) {
				if (pos == position) {
					for (auto && i : options[pos]) {
						addch(i | A_STANDOUT);
					}
					addch('\n');
				}
				else {
					for (auto && i : options[pos]) {
						addch(i | A_NORMAL);
					}
					addch('\n');
				}
			}
		}
		else { 
			printw(">> ");
			for (auto i : word) {
				waddch(stdscr, i | A_BOLD);
			}
		}
		refresh();
	}
	auto itr = dict->find(word);
	if (options.size() != 0 && itr != dict->end()) {
		do {
			itr->second->print(fout);
			fout << "\n------------------------------\n\n";
			itr = std::next(itr);
		} while (std::next(itr)->first == itr->first);
	}
	else {
		fout << "Word not found.\n";
	}
	fout.close(); display();
}

#endif
