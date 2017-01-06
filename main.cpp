#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <cstring>
#include <map>
#include <algorithm>
#include "entry.hpp"

extern "C" {
	extern char _binary_source_start;
	extern char _binary_source_end;
};

using ptr_t = std::shared_ptr<Entry>;
using map_t = std::multimap<std::string, ptr_t>;
using vec_t = std::vector<std::string>;


std::string lower(std::string const s) {
	std::string str = s;
	for (size_t i = 0; i < str.length();) {
		if (std::isalpha(str[i]) && std::isupper(str[i])) {
			str[i] = std::tolower(str[i]);
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


void searchEng(map_t & map, std::string & str) {
	auto result = std::map<std::string, std::string>();
	for (auto && i : map) {
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
		std::cout << "Word not found.\n";
	}
	else {
		std::cout << "A total of " << result.size();
		std::cout << ((result.size() > 1) ? " entries" : " entry");
		std::cout << " have been found."
			<< "\n------------------------------\n\n";
		for (auto i : result) {
			std::cout << i.first << ":  ";
			std::cout << ((i.second[0] == '\t') ? i.second.substr(1) : i.second)
				<< "\n------------------------------\n\n";
		}
	}
}

void searchTxt(map_t & map, std::string & str) {
	auto result = std::map<std::string, std::string>();
	for (auto && i : map) {
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
		std::cout << "Word not found.\n";
	}
	else {
		std::cout << "A total of " << result.size();
		std::cout << ((result.size() > 1) ? " entries" : " entry");
		std::cout << " have been found.\n"
			<< "------------------------------\n\n";
		for (auto i : result) {
			std::cout << i.first << ":  " << i.second.substr(1) << "\n------------------------------\n\n";
		}
	}
}

void searchIcl(map_t & dict, std::string & word) {
	auto itr = dict.find(word);
	std::cout << "\n------------------------------\n\n";
	if (itr != dict.end()) {
		do {
			itr->second->print();
			std::cout << "\n------------------------------\n\n";
			itr = std::next(itr);
		} while (std::next(itr)->first == itr->first);
	}
	else {
		std::cout << "Word not found.\n";
	}
}

int main(int argc, char ** argv)
{
	if (argc != 3) {
		std::cout << "Not the right amount of parameters.";
		return -1;
	}
	std::string flag = argv[1];
	std::string input = lower(std::string(argv[2]));
	std::string source = &_binary_source_start;
	std::istringstream file(source);
	map_t dict;
	for (std::string str; std::getline(file, str); ) { //for every line
		std::istringstream iss(str);
		std::string name;
		std::vector<std::string> vec;
		if (str[0] == '*') {
			iss >> name;
			name.erase(std::remove_if(name.begin(), name.end(), [](char i) { return (i == '-' || i == '*' || i == ',' || i == ')'); }), name.end());
		}
		bool flag = true;
		std::string temp;
		std::string sentence;
		size_t index = 1;
		while (iss >> temp) {
			if (std::find(temp.begin(), temp.end(), '[') != temp.end()) {
				flag = false;
			}
			if (std::find(temp.begin(), temp.end(), ']') != temp.end()) {
				flag = true;
			}
			if (temp[0] == '+') {
				sentence += (flag) ? "\n" : " ";
				vec.push_back(sentence);
				sentence = (flag) ? "\t" + std::to_string(index) + ". " : "";
				index += (flag) ? 1 : 0;
				temp = temp.substr(1);
			}
			temp += " ";
			sentence += temp;
		}
		ptr_t ent(new Entry(name, vec));
		dict.insert(std::make_pair(name, ent));
	};
	if (flag == "-e") {
		searchEng(dict, input);
	}
	else if (flag == "-i") {
		searchIcl(dict, input);
	}
	else if (flag == "-t") {
		searchTxt(dict, input);
	}
	else {
		std::cout << "Parameter invalid. Program now terminates.\n";
		return -1;
	}
    return 0;
}

