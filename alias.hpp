#ifndef __ALIAS_HPP__
#define __ALIAS_HPP__
#include <map>
#include <memory>
#include "entry.hpp"

using ptr_t = std::shared_ptr<Entry>;
using map_t = std::multimap<std::string, std::shared_ptr<Entry>>;
using mapentry_t = std::shared_ptr<map_t>;
using vec_t = std::vector<std::string>;
using infl_t = std::multimap<std::string, std::vector<std::string>>;
using resultvec_t = std::multimap<std::string, std::string>;
using resultvecptr_t = std::shared_ptr<std::multimap<std::string, std::string>>;
using resultmapvecptr_t = std::vector<std::shared_ptr<std::multimap<std::string, std::string>>>;
using resultptrmapvecptr_t = std::shared_ptr<std::vector<std::shared_ptr<std::multimap<std::string, std::string>>>>;
using mapptr_t = std::shared_ptr<std::multimap<std::string, std::vector<std::string>>>;
using vecptr_t = std::vector<std::shared_ptr<std::multimap<std::string, std::vector<std::string>>>>;
using mapvecptr_t = std::shared_ptr<std::vector<std::shared_ptr<std::multimap<std::string, std::vector<std::string>>>>>;

#endif