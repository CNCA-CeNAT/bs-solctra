all: clean solctra

solctra: solctra.h solctra.cpp main.cpp
	icpc -std=c++11 -qopenmp -o solctra solctra.h solctra.cpp main.cpp utils.h utils.cpp

clean:
	rm -f solctra
