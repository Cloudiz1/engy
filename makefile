debug:
	g++ -o build/debug src/*.cpp -I include/ -Wall -Wpedantic -g -O0 -DDEBUG

prod:
	g++ -o build/prod src/*.cpp -I include/ -Wall -Wpedantic -O3 -DNDEBUG

magic_d:
	g++ -o build/magics magics/main.cpp -Wall -Wpedantic -g -O0 -DDEBUG
	./build/magics

magic_r:
	g++ -o build/magics magics/main.cpp -Wall -Wpedantic -O3 -DNDEBUG
	./build/magics
