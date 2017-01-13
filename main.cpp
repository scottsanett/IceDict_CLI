#include "main.hpp"

int main() {
	initscr(); cbreak(); timeout(-1); noecho(); keypad(stdscr, TRUE);
	waddstr(stdscr, "Initializing...\n\n"); wrefresh(stdscr);
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	auto mapptr = std::shared_ptr<std::vector<mapptr_t>>(new std::vector<mapptr_t>);
	for (size_t i = 0; i < 7; ++i) {
		mapptr->push_back(mapptr_t(new infl_t));
	}
	
	auto index = std::shared_ptr<std::multimap<std::string, std::string>>(new std::multimap<std::string, std::string>);
	auto forms = std::shared_ptr<std::set<std::string>>(new std::set<std::string>);
	mapentry_t dict(new map_t);
	std::thread t_forms(importForm, forms);
	std::thread t(import, dict);
	std::thread t_index(importIndex, index);
	std::thread t1(import_infl, mapptr, 1);
	std::thread t2(import_infl, mapptr, 2);
	std::thread t3(import_infl, mapptr, 3);
	std::thread t4(import_infl, mapptr, 4);
	std::thread t5(import_infl, mapptr, 5);
	std::thread t6(import_infl, mapptr, 6);
	std::thread t7(import_infl, mapptr, 7);
	t.join(); t_index.join(); t_forms.join();
	t1.join(); t2.join(); t3.join();
	t4.join(); t5.join(); t6.join(); t7.join();
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	printw("Initialization completed within ");
	for (auto i : std::to_string(elapsed.count())) {
		waddch(stdscr, i | A_BOLD);
	}
	printw(" seconds.\n"); wrefresh(stdscr);
	while (true) {
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
		wrefresh(stdscr);
		int ch;
		fflush(stdin);
		while ((ch = getch()) && (ch != KEY_ENTER)) {
			waddch(stdscr, ch); wrefresh(stdscr);
			if (ch == 'e') {
				searchEng(dict);
			}
			else if (ch == 'i') {
				searchIcl(dict);
			}
			else if (ch == 't') {
				searchTxt(dict);
			}
			else if (ch == 'r') {
				searchReg(dict);
			}
			else if (ch == 'f') {
				find_orig(mapptr);
			}
			else if (ch == 'p') {
				plot(mapptr, index);
			}
			else if (ch == 's') {
				printForm(mapptr, index, forms);
			}
			else if (ch == 'q') { 
				printw("\nProgram now terminates.\n"); 
				endwin(); nocbreak(); echo(); keypad(stdscr, FALSE); system("reset");
				return 0;
		}
			else {
				printw("\nParameter invalid. Please try again.\n>> ");
			}
		}
	}
	return 0;
}

