#ifndef __IMPORT_HPP__
#define __IMPORT_HPP__
#include <fstream>
#include <string>
#include <memory>
#include <sstream>
#include <map>
#include <algorithm>

extern "C" {
	extern char _binary___alphabet_source_start;
	extern char _binary___alphabet_source_end;
};

using ptr_t = std::shared_ptr<Entry>;
using map_t = std::multimap<std::string, ptr_t>;
using mapentry_t = std::shared_ptr<map_t>;

void import(mapentry_t dict) {
	std::string source(&_binary___alphabet_source_start, &_binary___alphabet_source_end);
	std::istringstream file(source);
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
			if (temp[0] != '+') {
				if (std::find(temp.begin(), temp.end(), '[') != temp.end()) {
					flag = false;
					auto pos4 = temp.find('[');
					if (pos4 != std::string::npos) { temp.insert(pos4 + 1, "\033[36m"); }
				}
				if (std::find(temp.begin(), temp.end(), ']') != temp.end()) {
					flag = true;
					auto pos5 = temp.find(']');
					if (pos5 != std::string::npos) { temp.insert(pos5, "\033[0m"); }
				}
				auto pos1 = temp.find('(');
				if (pos1 != std::string::npos) { temp.insert(pos1 + 1, "\033[1;34m"); }
				auto pos2 = temp.find(')');
				if (pos2 != std::string::npos) { temp.insert(pos2, "\033[0m"); }
				auto pos3 = temp.find('#');
				if (pos3 != std::string::npos) {
					temp.erase(temp.begin() + pos3);
					temp = "\033[1;35m" + temp + "\033[0m";
				}
				auto sz = name.length();
				if (sz > 3) {
					auto pos = temp.find(name.substr(0, sz - 1));
					if (pos != std::string::npos) {
						temp.insert(pos, "\033[1;32m");
						temp += "\033[0m";
					}
				}
			}
			else if (temp[0] == '+') {
				sentence += (flag) ? "\n" : " ";
				vec.push_back(sentence);
				auto sz = name.length();
				if (sz > 3) {
					auto pos = temp.find(name.substr(0, sz - 1));
					if (pos != std::string::npos) {
						temp.insert(pos, "\033[1;32m");
						temp += "\033[0m";
					}
				}
				sentence = (flag) ? "\t" + std::to_string(index) + ". " : "";
				index += (flag) ? 1 : 0;
				temp = temp.substr(1);
			}
			temp += " ";
			sentence += temp;
		}
		ptr_t ent(new Entry(name, vec));
		dict->insert(std::make_pair(name, ent));
	};
}


#endif
