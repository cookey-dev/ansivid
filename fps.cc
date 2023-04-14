#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <boost/algorithm/string.hpp>
#include "exec.hpp"
#include "fs.hpp"
#include "spinners.hpp"

using namespace std;
using namespace spinners;
namespace algo = boost::algorithm;

void ffmpeg(string file) {
	if (!exists(file)) {
		cerr << "\033[91mFile " << file << " doesn't exist\033[0m" << endl;
		exit(1);
	}
	if (exists("fps.txt")) deleteFile("fps.txt");
	if (exists("out.txt")) deleteFile("out.txt");
	if (exists("out.mp3")) deleteFile("out.mp3");
	if (dirExists("frames")) {
		deleteDir("frames");
	}
	createDir("frames");
	string fps = exec(("ffprobe -v quiet -select_streams v -of default=noprint_wrappers=1:nokey=1 -show_entries stream=r_frame_rate " + file).c_str());
	string bitrate = exec(("ffprobe -v quiet -select_streams a -of default=noprint_wrappers=1:nokey=1 -show_entries stream=bit_rate " + file).c_str());
	bitrate.erase(bitrate.find_last_not_of(" \n\r\t") + 1);

	vector<string> exprVec;
	algo::split(exprVec, fps, algo::is_any_of("/"), algo::token_compress_on);
	int dividend = stoi(exprVec.at(0));
	int divisor = stoi(exprVec.at(1));
	int roundFps = round(dividend / divisor);

	cout << "FPS: " << roundFps << endl;
	cout << "Bitrate: " << bitrate << endl;
	writeFile("fps.txt", to_string(roundFps));

	Spinner *spin = new Spinner();
	spin->setText("Running FFMpeg");
	spin->setInterval(100);
	spin->setSymbols("dots4");
	spin->start();
	exec(("ffmpeg -v quiet -i " + file + " frames/\%03d.png").c_str());
	exec(("ffmpeg -v quiet -i " + file + " -f mp3 -ab " + bitrate + " -vn out.mp3").c_str());
	spin->stop();
}

int main(int argc, char **argv) {
	if (!argv[1]) {
		cerr << "\033[91mFile not provided\nSyntax: ./fps file.mp4\033[0m"
		     << endl;
		exit(1);
	}
	ffmpeg(argv[1]);
	return 0;
}

