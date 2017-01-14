#ifndef __ALPHABET_HPP__
#define __ALPHABET_HPP__

extern "C" {
	extern char _binary___alphabet_wordindex_start;
	extern char _binary___alphabet_wordindex_end;
};

extern "C" {
	extern char _binary___alphabet_source_start;
	extern char _binary___alphabet_source_end;
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

#endif