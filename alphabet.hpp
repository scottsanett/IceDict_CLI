#ifndef __ALPHABET_HPP__
#define __ALPHABET_HPP__

extern "C" {
	extern char _binary___alphabet_wordindex_start;
	extern char _binary___alphabet_wordindex_end;
};

extern "C" {
	extern char _binary___alphabet_vifgusson_start;
	extern char _binary___alphabet_vifgusson_end;
};

extern "C" {
	extern char _binary___alphabet_zoega_start;
	extern char _binary___alphabet_zoega_end;
}

extern "C" {
	extern char _binary___alphabet_index_start;
	extern char _binary___alphabet_index_end;
}

extern "C" {
	extern char _binary___alphabet_forms_start;
	extern char _binary___alphabet_forms_end;
}

extern "C" {
	extern char _binary___alphabet_inflection_start;
	extern char _binary___alphabet_inflection_end;
}

extern "C" {
	extern char _binary___alphabet_1_start;
	extern char _binary___alphabet_1_end;
}

extern "C" {
	extern char _binary___alphabet_2_start;
	extern char _binary___alphabet_2_end;
}

extern "C" {
	extern char _binary___alphabet_3_start;
	extern char _binary___alphabet_3_end;
}

extern "C" {
	extern char _binary___alphabet_4_start;
	extern char _binary___alphabet_4_end;
}

extern "C" {
	extern char _binary___alphabet_5_start;
	extern char _binary___alphabet_5_end;
}

extern "C" {
	extern char _binary___alphabet_6_start;
	extern char _binary___alphabet_6_end;
}

extern "C" {
	extern char _binary___alphabet_7_start;
	extern char _binary___alphabet_7_end;
}


using strptr_t = std::shared_ptr<std::string>;

std::map<size_t, strptr_t> alphabet = {
	std::make_pair(1, strptr_t(new std::string(&_binary___alphabet_1_start, &_binary___alphabet_1_end))),
	std::make_pair(2, strptr_t(new std::string(&_binary___alphabet_2_start, &_binary___alphabet_2_end))),
	std::make_pair(3, strptr_t(new std::string(&_binary___alphabet_3_start, &_binary___alphabet_3_end))),
	std::make_pair(4, strptr_t(new std::string(&_binary___alphabet_4_start, &_binary___alphabet_4_end))),
	std::make_pair(5, strptr_t(new std::string(&_binary___alphabet_5_start, &_binary___alphabet_5_end))),
	std::make_pair(6, strptr_t(new std::string(&_binary___alphabet_6_start, &_binary___alphabet_6_end))),
	std::make_pair(7, strptr_t(new std::string(&_binary___alphabet_7_start, &_binary___alphabet_7_end))),
};

void toLower(std::string & name) {
	std::vector<std::string> upper = { "Þ", "Ð", "Æ", "Ö", "Á", "É", "Í", "Ó", "Ú", "Ý", "Œ" };
	std::vector<std::string> lower = { "þ", "ð", "æ", "ö", "á", "é", "í", "ó", "ú", "ý", "œ" };
	[&]() { for (auto i = 0; i < name.length(); ++i) {
		if (std::isalpha(name[i])) {
			if (std::isupper(name[i])) {
				name[i] = std::tolower(name[i]);
			}
		}
		else {
			if ((name[i] == upper[0][0]) && (name[i + 1] == upper[0][1])) { name[i] = lower[0][0]; name[i + 1] = lower[0][1]; i += 1; }
			else if ((name[i] == upper[1][0]) && (name[i + 1] == upper[1][1])) { name[i] = lower[1][0]; name[i + 1] = lower[1][1]; i += 1; }
			else if ((name[i] == upper[2][0]) && (name[i + 1] == upper[2][1])) { name[i] = lower[2][0]; name[i + 1] = lower[2][1]; i += 1; }
			else if ((name[i] == upper[3][0]) && (name[i + 1] == upper[3][1])) { name[i] = lower[3][0]; name[i + 1] = lower[3][1]; i += 1; }
			else if ((name[i] == upper[4][0]) && (name[i + 1] == upper[4][1])) { name[i] = lower[4][0]; name[i + 1] = lower[4][1]; i += 1; }
			else if ((name[i] == upper[5][0]) && (name[i + 1] == upper[5][1])) { name[i] = lower[5][0]; name[i + 1] = lower[5][1]; i += 1; }
			else if ((name[i] == upper[6][0]) && (name[i + 1] == upper[6][1])) { name[i] = lower[6][0]; name[i + 1] = lower[6][1]; i += 1; }
			else if ((name[i] == upper[7][0]) && (name[i + 1] == upper[7][1])) { name[i] = lower[7][0]; name[i + 1] = lower[7][1]; i += 1; }
			else if ((name[i] == upper[8][0]) && (name[i + 1] == upper[8][1])) { name[i] = lower[8][0]; name[i + 1] = lower[8][1]; i += 1; }
			else if ((name[i] == upper[9][0]) && (name[i + 1] == upper[9][1])) { name[i] = lower[9][0]; name[i + 1] = lower[9][1]; i += 1; }
			else if ((name[i] == upper[10][0]) && (name[i + 1] == upper[10][1])) { name[i] = lower[10][0]; name[i + 1] = lower[10][1]; i += 1; }
			else {}
		}
	} }();
}

#endif