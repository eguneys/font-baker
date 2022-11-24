GCC = x86_64-w64-mingw32-c++

SRC = src/main.cpp src/font.cpp src/file.cpp

hello $(SRC):
	$(GCC) $(SRC) -o dist/fb -static -static-libgcc -static-libstdc++
