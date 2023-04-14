#ifndef FS_HPP
#define FS_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <cstdio>
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

bool exists(const string fname) {
	struct stat buf;
	return (stat(fname.c_str(), &buf) == 0);
}
bool dirExists(const string dname) {
	DIR *dir = opendir(dname.c_str());
	bool res;
	if (dir) {
		res = true;
		closedir(dir);
	} else if (ENOENT == errno) {
		res = false;
	} else {
		cerr << "\033[91mopendir() failed for some reason\033[0m" << endl;
		exit(1);
	}
	return res;
}

void deleteFile(const string fname) {
	remove(fname.c_str());
}
void deleteDir(const string dname) {
	fs::path dpath(dname);
	fs::remove_all(dpath);
}

void createDir(const string dname) {
	int res = mkdir(dname.c_str(), 0777);
	if (res != 0) {
		cerr << "\033[91mCould not create directory "
		     << dname << "\033[0m" << endl;
		exit(1);
	}
}

void writeFile(const string fname, const string content) {
	ofstream File(fname);
	File << content;
	File.close();
}

#endif

