#ifndef _SPINNERS_HPP_
#define _SPINNERS_HPP_



#include <iostream>
#include <sstream>
#include <map>
#include <chrono>
#include <thread>
#include <string>
#include <string.h>

using namespace std;

/*
MIT License

Copyright (c) 2018 Jan Kuri <jan@bleenco.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
 * Modified slightly by CookeyDev
 */

namespace spinners {
map<const char *, const char *> spinnerType = {{"dots", u8"⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏"}, {"dots2", u8"⣾⣽⣻⢿⡿⣟⣯⣷"}, {"dots3", u8"⠋⠙⠚⠞⠖⠦⠴⠲⠳⠓"}, {"dots4", u8"⠄⠆⠇⠋⠙⠸⠰⠠⠰⠸⠙⠋⠇⠆"}, {"dots5", u8"⠋⠙⠚⠒⠂⠂⠒⠲⠴⠦⠖⠒⠐⠐⠒⠓⠋"}, {"dots6", u8"⠁⠉⠙⠚⠒⠂⠂⠒⠲⠴⠤⠄⠄⠤⠴⠲⠒⠂⠂⠒⠚⠙⠉⠁"}, {"dots7", u8"⠈⠉⠋⠓⠒⠐⠐⠒⠖⠦⠤⠠⠠⠤⠦⠖⠒⠐⠐⠒⠓⠋⠉⠈"}, {"dots8", u8"⠁⠁⠉⠙⠚⠒⠂⠂⠒⠲⠴⠤⠄⠄⠤⠠⠠⠤⠦⠖⠒⠐⠐⠒⠓⠋⠉⠈⠈"}, {"dots9", u8"⢹⢺⢼⣸⣇⡧⡗⡏"}, {"dots10", u8"⢄⢂⢁⡁⡈⡐⡠"}, {"dots11", u8"⠁⠂⠄⡀⢀⠠⠐⠈"}, {"pipe", u8"┤┘┴└├┌┬┐"}, {"star", u8"✶✸✹✺✹✷"}, {"star2", u8"+x*"}, {"flip", u8"___-``'´-___"}, {"hamburger", u8"☱☲☴"}, {"growVertical", u8"▁▃▄▅▆▇▆▅▄▃"}, {"growHorizontal", u8"▏▎▍▌▋▊▉▊▋▌▍▎"}, {"balloon", u8" .oO@* "}, {"balloon2", u8".oO°Oo."}, {"noise", u8"▓▒░"}, {"bounce", u8"⠁⠂⠄⠂"}, {"boxBounce", u8"▖▘▝▗"}, {"boxBounce2", u8"▌▀▐▄"}, {"triangle", u8"◢◣◤◥"}, {"arc", u8"◜◠◝◞◡◟"}, {"circle", u8"◡⊙◠"}, {"squareCorners", u8"◰◳◲◱"}, {"circleQuarters", u8"◴◷◶◵"}, {"circleHalves", u8"◐◓◑◒"}, {"squish", u8"╫╪"}, {"toggle", u8"⊶⊷"}, {"toggle2", u8"▫▪"}, {"toggle3", u8"□■"}, {"toggle4", u8"■□▪▫"}, {"toggle5", u8"▮▯"}, {"toggle6", u8"ဝ၀"}, {"toggle7", u8"⦾⦿"}, {"toggle8", u8"◍◌"}, {"toggle9", u8"◉◎"}, {"toggle10", u8"㊂㊀㊁"}, {"toggle11", u8"⧇⧆"}, {"toggle12", u8"☗☖"}, {"toggle13", u8"=*-"}, {"arrow", u8"←↖↑↗→↘↓↙"}};

const char *getSpinner(const char *key) {
  auto search = spinnerType.find(key);
  if (search != spinnerType.end()) {
    return search->second;
  } else {
    search = spinnerType.find("dots");
    return search->second;
  }
}

class Spinner {
public:
Spinner() : interval(80), text(""), stop_spinner(false), symbols(getSpinner("dots")){};
Spinner(int _interval, std::string _text, const char *_symbols) : interval(_interval), text(_text), stop_spinner(false), symbols(getSpinner(_symbols)){};
~Spinner() { stop(); };

void setInterval(int _interval) {
	interval = _interval;
}
void setText(string _text) {
	text = _text;
}
void setSymbols(const char *_symbols) {
	symbols = getSpinner(_symbols);
}
void startSpinner() {
	int len = strlen(symbols) / 3;
	int i = 0;
	char ch[4];

	hideCursor();
	while (!stop_spinner) {
		i = (i >= (len - 1)) ? 0 : i + 1;
		strncpy(ch, symbols + i * 3, 3);
		cout << ch << " " << text << " \r";
		cout.flush();
		this_thread::sleep_for(std::chrono::milliseconds(interval));
	}
	showCursor();
}
void start() {
	t = thread(&Spinner::startSpinner, this);
}
void stop() {
	stop_spinner = true;
	if (t.joinable()) {
		t.join();
	}
}

private:
int interval;
string text;
bool stop_spinner;
const char *symbols;
thread t;

void hideCursor() {
	cout << "\u001b[?25l";
}

void showCursor() {
	cout << "\u001b[?25h";
}
};
}

#endif
