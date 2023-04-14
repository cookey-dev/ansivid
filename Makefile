main: process.cc exec.hpp fps.cc fs.hpp load.cc
	g++ -o process process.cc `GraphicsMagick++-config --cppflags --cxxflags --ldflags --libs` -std=c++17 -pthread
	g++ -o fps fps.cc -pthread -std=c++17 -DBOOST_NO_CXX11_SCOPED_ENUMS -lboost_system -lboost_filesystem
	g++ -o load load.cc -lcurses -std=c++17 -pthread -fconcepts-ts
