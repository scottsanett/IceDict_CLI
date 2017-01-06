#ifndef __ENTRY_HPP__
#define __ENTRY_HPP__
#include <string>
#include <vector>
#include <iostream>

using vec_t = std::vector<std::string>;

class Entry {
	std::string name;
	vec_t definition;
public:
	Entry() = default;
	Entry(std::string str, std::vector<std::string> vec): name(str), definition(vec){}
	void print() {
		std::cout << "\n------------------------------\n\n";
		std::cout << "\033[1m" + name + "\033[0m" << ", ";
		for (auto && i : definition) { std::cout << i; } 
	}
	std::shared_ptr<vec_t> getDef() const { return std::make_shared<vec_t>(definition); }
};

#endif