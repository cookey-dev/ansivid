#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <array>
#include <Magick++.h>
#include <stdexcept>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <boost/algorithm/string.hpp>
#include "spinners.hpp"
#include "exec.hpp"

using namespace std;
using namespace spinners;
namespace IM = Magick;
namespace fs = std::filesystem;
namespace algo = boost::algorithm;

typedef array<int, 4> Pixel;
typedef vector<Pixel> Image;

Pixel nl{300, 300, 300, 300};
Pixel nf{301, 301, 301, 301};


// Sort algorithm
bool cmp(string s1, string s2) {
	fs::path f1(s1);
	fs::path f2(s2);
	string fname1 = f1.stem();
	string fname2 = f2.stem();
	return fname1.size() == fname2.size() ? fname1 < fname2 : fname1.size() < fname2.size();
}

// Resize file
IM::Image resize(string path) {
	// Get stdout rows/columns
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	// Resize
	IM::Geometry new_size = IM::Geometry(size.ws_col - 1, size.ws_row);
	new_size.aspect(true);
	IM::Image img;
	img.read(path);
	img.resize(new_size);
	return img;
}

Image _rgby(IM::Image img) {
	Image res;
	try {
		// Rows/Columns
		int rows = img.rows();
		int col = img.columns();

		// Iterate over pixels
		for (int y = 0; y <= rows; y++) {
			for (int x = 0; x <= col; x++) {
				IM::Color px = img.pixelColor(x, y);
				string cl = px;
				int r, g, b;
				const char *hexColor = cl.c_str();
				sscanf(hexColor, "#%02x%02x%02x", &r, &g, &b);
				int l = 0.2126 * r + 0.7152 * g + 0.0722 * b;
				Pixel rgbRes = {r, g, b, l};
				res.push_back(rgbRes);
			}
			res.push_back(nl);
		}
	} catch (IM::Exception &err) {
		cerr << "Error: " << err.what() << endl;
		exit(1);
	}
	
	return res;
}

void append_file(string fname, string content) {
	ofstream out;
	out.open(fname, ios_base::app);
	out << content;
}

string chars[10] = {" ", ".", ":", "!", "+", "*", "e", "$", "#", "@"};

string image(Image rgby) {
	string res;
	for (array<int, 4> i : rgby) {
		int y = i[3];
		string r = to_string(i[0]);
		string g = to_string(i[1]);
		string b = to_string(i[2]);
		string current = y <= 25 ? chars[0] :
		y <= 51 && y > 25 ? chars[1] :
		y <= 76 && y > 51 ? chars[2] :
		y <= 102 && y > 76 ? chars[3] :
		y <= 127 && y > 102 ? chars[4] :
		y <= 153 && y > 127 ? chars[5] :
		y <= 178 && y > 153 ? chars[6] :
		y <= 204 && y > 153 ? chars[7] :
		y <= 229 && y > 204 ? chars[8] :
		y <= 255 && y > 229 ? chars[9] :
		y == 300 ? "\n" : "Error";
		current = "\033[38;2;" + r + ";" + g + ";" + b + "m" + current + "\033[0m";
		res += current;
	}
	
	return res;
}

int main() {
	cout << "\033[91m######################\n"
		 << "# \033[96mANSI Video Creator \033[91m#\n"
		 << "#    \033[92mBy Cookeydev    \033[91m#\n"
		 << "######################\033[0m\n";

	string magickOptions = exec("GraphicsMagick++-config --cppflags --cxxflags --ldflags --libs");
	IM::InitializeMagick(
		magickOptions.c_str()
	);
	cout << "\033[94mInitialized Magick++\033[0m" << endl;
	
	Spinner *sortSpin = new Spinner();
	sortSpin->setText("Sorting frames...");
	sortSpin->setInterval(100);
	sortSpin->setSymbols("dots4");
	sortSpin->start();
	vector<fs::path> _files;
	vector<string> files;
	copy(fs::directory_iterator("./frames"), fs::directory_iterator(), back_inserter(_files));
	for (const string & entry : _files) {
		files.push_back(entry);
	}
	sort(files.begin(), files.end(), cmp);
	sortSpin->stop();

	Spinner *spin = new Spinner();
	spin->setText("Processing...");
	spin->setInterval(100);
	spin->setSymbols("dots4");
	spin->start();
	for (string entry : files) {
		spin->setText(("Processing " + entry));
		IM::Image img = resize(entry);
		Image processedImage = _rgby(img);
		string fin = image(processedImage);
		fin += "nf\n";
		append_file("out.txt", fin);
	}
	spin->stop();
	cout << endl;

	return 0;
}
