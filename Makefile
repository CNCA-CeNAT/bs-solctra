CC=icpc
#MPICC=mpicxx.openmpi
MPICC=/usr/lib64/openmpi/bin/mpicxx
COMMON_FLAGS=-O3 -o solctra -std=c++11 -Wall
PARALLEL=-qopenmp
SERIAL=-qopenmp-stubs -no-vec
SOURCE=solctra.h solctra.cpp main.cpp utils.h utils.cpp FileHandler.cpp FileHandler.h
RPT_FLAGS=-qopt-report=5 -qopt-report-phase:vec -qopt-report-phase:openm -qopt-report-phase:offload
FP_FLAGS=-fp-model precise -fp-model source


all: clean fp-fast

fp-fast: $(SOURCE)
	$(CC) $(PARALLEL $(COMMON_FLAGS) $(SOURCE)

fp-precise: $(SOURCE)
	$(CC) -fp-model precise $(PARALLEL) $(COMMON_FLAGS) $(SOURCE)

fp-source: $(SOURCE)
	$(CC) -fp-model source $(PARALLEL) $(COMMON_FLAGS) $(SOURCE)

fp-all: $(SOURCE)
	$(CC) -g $(FP_FLAGS) $(RPT_FLAGS) $(PARALLEL) $(COMMON_FLAGS) $(SOURCE)

debug: $(SOURCE)
	$(CC) -g $(PARALLEL) $(COMMON_FLAGS) $(SOURCE)

serial: $(SOURCE)
	$(CC) $(FP_FLAGS) $(SERIAL) $(COMMON_FLAGS) $(SOURCE)

gcc: $(SOURCE)
	g++  $(COMMON_FLAGS) $(SOURCE)

mic: $(SOURCE)
	$(CC) -qoffload -qopenmp-offload=mic -g $(PARALLEL) $(FP_FLAGS) $(RPT_FLAGS) $(COMMON_FLAGS) $(SOURCE)

mpi: $(SOURCE)
	$(MPICC) -g $(FP_FLAGS) $(RPT_FLAGS) $(COMMON_FLAGS) $(SOURCE)

clean:
	rm -f solctra results/*
