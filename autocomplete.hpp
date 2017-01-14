#ifndef __AUTOCOMPLETE_HPP__
#define __AUTOCOMPLETE_HPP__
#include <string>
#include <ncurses.h>
#include "alias.hpp"

int charType(char ch) {
	if (std::isalpha(ch) || std::isdigit(ch)) { return 0; }
	else if (ch == '.' || ch == '-' || ch == ' ' || ch == '\'' || ch == '\"') { return 1; }
	else if (ch == KEY_BACKSPACE) { return 2; }
	else if (ch == KEY_ENTER) { return 3; }
	else if (ch == KEY_LEFT || ch == KEY_RIGHT) { return 4; }
	else { return 5; }
}
 
/* autocomplete for icelandic - english dictionary */
void autocomplete_icelandic(std::shared_ptr<std::set<std::string>> dict, std::string const & str, std::vector<std::string> & result) {
	int height, width;
	getmaxyx(stdscr, height, width);
	if (height > 2) { height -= 2; }
	size_t i = 0;
	for (auto && entry : *dict) {
		auto pos = entry.find(str);
		if (pos == 0 && i < height) {
			result.push_back(entry);
			++i;
		}
	}
}

/* autocomplete for inflectional dictionary */
void autocomplete(std::shared_ptr<std::multimap<std::string, std::string>> index, std::string const & str, std::vector<std::string> & result) {
	int height, width;
	getmaxyx(stdscr, height, width);
	if (height > 2) { height -= 2; }
	std::string key;
	size_t i = 0;
	for (auto && entry : *index) {
		key = entry.first;
		auto pos = key.find(str);
		if (pos == 0 && i < height) {
			result.push_back(key);
			++i;
		}
	}
}

/* autocomplete for a specific inflectional form */
void autocomplete(std::shared_ptr<std::set<std::string>> forms, std::string const & str, std::vector<std::string> & result) {
	int height, width;
	getmaxyx(stdscr, height, width);
	if (height > 2) { height -= 2; }
	size_t i = 0;
	std::vector<std::string> stored;
	std::istringstream iss(str);
	std::string word;
	while (iss >> word) {
		stored.push_back(word);
	}
	for (auto && entry : *forms) {
		auto flag = [&]() { for (auto && snippet : stored) { if (entry.find(snippet) == std::string::npos) return false; } return true; }();
		if (flag == true && i < height) {
			result.push_back(entry);
			++i;
		}
	}
}

/* autocomplete for a specific inflectional form */
void autocomplete_inflection(std::shared_ptr<std::set<std::string>> inflections, std::string const & str, std::vector<std::string> & result) {
	int height, width;
	getmaxyx(stdscr, height, width);
	if (height > 2) { height -= 2; }
	size_t i = 0;
	for (auto && entry : *inflections) {
		auto pos = entry.find(str);
		if (pos == 0 && i < height) {
			result.push_back(entry);
			++i;
		}
	}
}
/* input for two icelandic - english dictionaries */
void input_icelandic(std::string & word, std::shared_ptr<std::set<std::string>> dict) {
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
			autocomplete_icelandic(dict, word, options);
		}
		else if (c == KEY_DOWN) { // autocomplete
			clear();
			if (position < options.size() - 1) {
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
			autocomplete_icelandic(dict, word, options);
		}
		else if (charType(c) == 5) { // special characters
			position = 0;
			options.clear();
			word += c; c = getch(); word += c;
			autocomplete_icelandic(dict, word, options);
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
}

/* input function for inflection autocomplete*/
void input(std::string & word, std::shared_ptr<std::multimap<std::string, std::string>> const index) {
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
			autocomplete(index, word, options);
		}
		else if (c == KEY_DOWN) { // autocomplete
			clear();
			if (position < options.size() - 1) {
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
			autocomplete(index, word, options);
		}
		else if (charType(c) == 5) { // special characters
			position = 0;
			options.clear();
			word += c; c = getch(); word += c;
			autocomplete(index, word, options);
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
}

/* input function for inflection form autocomplete*/
void input(std::string & word, std::shared_ptr<std::set<std::string>> const index) {
	int c;
	size_t position;
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
			autocomplete(index, word, options);
		}
		else if (c == KEY_DOWN) { // autocomplete
			clear();
			if (position < options.size() - 1) {
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
			autocomplete(index, word, options);
		}
		else if (charType(c) == 5) { // special characters
			position = 0;
			options.clear();
			word += c; c = getch(); word += c;
			autocomplete(index, word, options);
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
}

/* input function for all inflectional forms */
void input_inflections(std::string & word, std::shared_ptr<std::set<std::string>> const inflections) {
	int c;
	size_t position;
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
			autocomplete_inflection(inflections, word, options);
		}
		else if (c == KEY_DOWN) { // autocomplete
			clear();
			if (position < options.size() - 1) {
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
			autocomplete_inflection(inflections, word, options);
		}
		else if (charType(c) == 5) { // special characters
			position = 0;
			options.clear();
			word += c; c = getch(); word += c;
			autocomplete_inflection(inflections, word, options);
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
}

#endif