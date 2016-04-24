CC=icpc
COMMON_FLAGS=-O3 -std=c++11 -o solctra -qopenmp
NO_OMP=-O3 -std=c++11 -o solctra -qopenmp-stubs
SOURCE=solctra.h solctra.cpp main.cpp utils.h utils.cpp FileHandler.cpp FileHandler.h Coil.cpp Coil.h
#SOURCE=solctra.h solctra.cpp main.cpp utils.h utils.cpp


all: clean fp-fast

fp-fast: $(SOURCE)
	$(CC) $(COMMON_FLAGS) $(SOURCE)

fp-precise: $(SOURCE)
	$(CC) -fp-model precise $(COMMON_FLAGS) $(SOURCE)

fp-source: $(SOURCE)
	$(CC) -fp-model source $(COMMON_FLAGS) $(SOURCE)

fp-all: $(SOURCE)
	$(CC) -fp-model precise -fp-model source $(COMMON_FLAGS) $(SOURCE)

debug: $(SOURCE)
	$(CC) -g -fp-model precise -fp-model source $(COMMON_FLAGS) $(SOURCE)

noomp: $(SOURCE)
	$(CC) -fp-model precise -fp-model source $(NO_OMP) $(SOURCE)

gcc: $(SOURCE)
	g++ -O3 -std=c++11 -o solctra $(SOURCE)

clean:
	rm -f solctra
