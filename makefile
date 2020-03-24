vokey.out: ./source/vokey.cpp
	g++ $(pkg-config --cflags --libs Qt5Widgets) -fPIC ./source/vokey.cpp -o vokey.out
	chmod +x vokey.out
