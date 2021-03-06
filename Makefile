CC=icpc
#MPICC=mpicxx.openmpi
MPICC=/usr/lib64/openmpi/bin/mpicxx
COMMON_FLAGS=-O3 -std=c++0x -o solctra -fopenmp -Wall
NO_OMP=-O3 -std=c++11 -o solctra -qopenmp-stubs
#SOURCE=solctra.h solctra.cpp main.cpp utils.h utils.cpp FileHandler.cpp FileHandler.h Coil.cpp Coil.h GlobalData.cpp GlobalData.h
SOURCE=solctra.h solctra.cpp main.cpp utils.h utils.cpp
#SOURCE=solctra.h solctra.cpp main.cpp utils.h utils.cpp
#RPT_FLAGS=-qopt-report=5 -qopt-report-phase:vec -qopt-report-phase:openm
RPT_FLAGS=
#FP_FLAGS=-fp-model precise -fp-model source 
FP_FLAGS=


all: clean fp-fast

fp-fast: $(SOURCE)
	$(CC) $(COMMON_FLAGS) $(SOURCE)

fp-precise: $(SOURCE)
	$(CC) -fp-model precise $(COMMON_FLAGS) $(SOURCE)

fp-source: $(SOURCE)
	$(CC) -fp-model source $(COMMON_FLAGS) $(SOURCE)

fp-all: $(SOURCE)
	$(CC) -g ${FP_FLAGS} ${RPT_FLAGS} $(COMMON_FLAGS) $(SOURCE)

debug: $(SOURCE)
	$(CC) -g $(COMMON_FLAGS) $(SOURCE)

noomp: $(SOURCE)
	$(CC) ${FP_FLAGS} $(NO_OMP) $(SOURCE)

gcc: $(SOURCE)
	g++ -O3 -std=c++11 -o solctra $(SOURCE)

mic: $(SOURCE)
	$(CC) -mmic -g ${FP_FLAGS} ${RPT_FLAGS} $(COMMON_FLAGS) $(SOURCE)

mpi: $(SOURCE)
	$(MPICC) -g ${FP_FLAGS} ${RPT_FLAGS} $(COMMON_FLAGS) $(SOURCE)

mpi-icpc: $(SOURCE)
	mpiicpc -g -xHost -fp-model precise -fp-model source -qopt-report=5 -qopt-report-phase:vec -qopt-report-phase:openmp -O3 -std=c++11 -o bs-solctra -qopenmp -Wall $(SOURCE)

mpi-icpc-serial: $(SOURCE)
	mpic++ -g -xHost -fp-model precise -fp-model source -qopt-report=5 -qopt-report-phase:vec -qopt-report-phase:openmp -O3 -std=c++11 -o solctra.serial -qopenmp-stubs -no-vec $(SOURCE)

mpi-icpc-knl: $(SOURCE)
	mpiicpc -g -xMIC-AVX512 -fp-model precise -fp-model source -qopt-report=5 -qopt-report-phase:vec -qopt-report-phase:openmp -O3 -std=c++11 -o bs-solctra       -qopenmp -Wall $(SOURCE) -DKNL=8
#	mpiicpc -g -xMIC-AVX512 -fp-model precise -fp-model source -qopt-report=5 -qopt-report-phase:vec -qopt-report-phase:openmp -O3 -std=c++11 -o solctra.knl.8 -qopenmp -Wall $(SOURCE) -DKNL=8
#	mpiicpc -g -xMIC-AVX512 -fp-model precise -fp-model source -qopt-report=5 -qopt-report-phase:vec -qopt-report-phase:openmp -O3 -std=c++11 -o solctra.knl.16 -qopenmp -Wall $(SOURCE) -DKNL=16

mpi-icpc-mic: $(SOURCE)
	mpic++ -mmic -g -fp-model precise -fp-model source -qopt-report=5 -qopt-report-phase:vec -qopt-report-phase:openmp -O3 -std=c++11 -o solctra.mic -qopenmp -Wall $(SOURCE)

clean:
	rm -f solctra
