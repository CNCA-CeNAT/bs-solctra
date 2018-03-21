/**
   	BS-SOLCTRA, main.cpp
    Purpose: Main function file. Defines functions for
    input parameter parsing and the main execution function
    where the MPI parallel programming standard is used to parallelize the 
    execution flow.

	Version 1
    @author Luis Diego Chavarria
    @Owners: Plasma Physics Laboratory, Costa Rica Institute of Technology
    		 Advanced Computing Laboratory, Costa Rica National High Technology Center

	Version 1, review 1
	@author: Diego Jimenez
	Changes:
		- Added documentation to code
*/

#include "solctra.h"
#include <mpi.h>
#include <omp.h>
#include <fstream>
#include <iostream>
#include <cstring>

const unsigned DEFAULT_STRING_BUFFER = 100;
const unsigned DEFAULT_STEPS = 500000;
const double DEFAULT_STEP_SIZE = 0.001;
const unsigned DEFAULT_PRECISION = 5;
const unsigned DEFAULT_PARTICLES= 1;
const unsigned DEFAULT_MODE= 1;
const std::string DEFAULT_OUTPUT = "results";
const std::string DEFAULT_RESOURCES = "resources";


/**
    Parses through input parameters to obtain the precision for 
    the simulation. Searches for the -precision flag and obtains 
    the value right after it.

    @param argc: amount of input parameters (numeric value).
    	   argv: array of values of the different input parameters.
    @return value of the input Precision if given. If not, returns the
    default value for PRECISION = 5
*/
unsigned getPrintPrecisionFromArgs(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-precision")
        {
            return static_cast<unsigned>(atoi(argv[i+1]));
        }
    }
    return DEFAULT_PRECISION;
}

/**
    Parses through input parameters to obtain the amount of steps to perform 
    in the simulation. Searches for the -steps flag and obtains 
    the value right after it.

    @param argc: amount of input parameters (numeric value).
    	   argv: array of values of the different input parameters.
    @return amount of steps to perform if given. If not, returns the
    default value for steps = 500000
*/
unsigned getStepsFromArgs(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-steps")
        {
            return static_cast<unsigned>(atoi(argv[i+1]));
        }
    }
    return DEFAULT_STEPS;
}


/**
    Parses through input parameters to obtain the stepsize to use 
    in the simulation. Searches for the -stepSize flag and obtains 
    the value right after it.

    @param argc: amount of input parameters (numeric value).
    	   argv: array of values of the different input parameters.
    @return value of stepSize to use if given. If not, returns the
    default value for stepSize = 0.001
*/
double getStepSizeFromArgs(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-stepSize")
        {
            return strtod(argv[i+1], nullptr);
        }
    }
    return DEFAULT_STEP_SIZE;
}


/**
    Function used to load the different input particles from an input file.
    This function calls the loadFile() function from the utils.cpp file to read 
    values from file.

    @param argc: amount of input parameters (numeric value).
    	   argv: array of values of the different input parameters.
    	   Coil& particles: pointer to SoA to save the different particles x,y and z information.
    	   const int length: number of particles to load for simulation.
    @return void function. Position values of particles are passed through pointers. 
*/
void LoadParticles(const int& argc, char** argv, Coil& particles, const int length)
{
    bool found = false;
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-particles")
        {
            loadFile(particles.x, particles.y, particles.z, length, argv[i+1]);
            found = true;
            break;
        }
    }
    if(!found)
    {
        printf("ERROR: particles path must be given!!\n");
        exit(1);
    }
}

/**
    Parses through input parameters to obtain the resourcePath to use 
    in the simulation. The resourcePath is the directory where the different coil files
    are stored. Searches for the -resource flag and obtains 
    the value right after it.

    @param argc: amount of input parameters (numeric value).
    	   argv: array of values of the different input parameters.
    @return string of the resourcePath if found. If not, returns the
    default value for resourcePath = "resources"
*/
std::string getResourcePath(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string param = argv[i];
        if("-resource" == param)
        {
            return std::string(argv[i+1]);
        }
    }
    return DEFAULT_RESOURCES;
}


/**
    Parses through input parameters to obtain the length (amount of particles) to use 
    in the simulation. Searches for the -length flag and obtains 
    the value right after it.

    @param argc: amount of input parameters (numeric value).
    	   argv: array of values of the different input parameters.
    @return value or amount of particles to use.
*/
unsigned getParticlesLengthFromArgs(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-length")
        {
            return static_cast<unsigned>(atoi(argv[i+1]));
        }
    }
    printf("ERROR: length of particles path must be given!!\n");
    exit(1);
}

/**
    Parses through input parameters to obtain the mode to use 
    in the simulation. Searches for the -mode flag and obtains 
    the value right after it.

    @param argc: amount of input parameters (numeric value).
    	   argv: array of values of the different input parameters.
    @return value of mode to use for simulation if found. If not DEFAULT_MODE is 
    used.
*/
unsigned getModeFromArgs(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-mode")
        {
            return static_cast<unsigned>(atoi(argv[i+1]));
        }
    }
    return DEFAULT_MODE;
}


/**
    Parses through input parameters to obtain the id of the current execution of
    the simulation. Searches for the -id flag and obtains the value right after it.

    @param argc: amount of input parameters (numeric value).
    	   argv: array of values of the different input parameters.
    @return value of id for the simulation.
*/
std::string getJobId(const int& argc, char** argv)
{
    for(int i = 1 ; i < argc - 1 ; ++i)
    {
        std::string tmp = argv[i];
        if(tmp == "-id")
        {
            return std::string(argv[i+1]);
        }
    }
    printf("ERROR: job id must be given!!\n");
    exit(1);
}


/**
    Main program flow function. This function puts all the simulation 
    together by calling the different functions of BS-SOLCTRA. In here,
    parallel execution with MPI is performed and data is distributed 
    among working units. 

    @param argc: amount of input parameters (numeric value).
    	   argv: array of values of the different input parameters.
    @return integer return code. 
*/
int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv); //MPI Parallel Region is stared. From here on, parallel process execute the code
    int myRank; //Variable that will hold the ID for each parallel process. Each process has a unique ID
    int commSize; //Variable that holds the value for the amount of parallel processes in the rank universe
    MPI_Comm_size(MPI_COMM_WORLD, &commSize); //This MPI function returns the amount of processes that are available
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); // This MPI function assigns a unique ID to each rank
    const int ompSize = omp_get_max_threads(); //This OpenMP function returns the value for the maximum number of threads available on a platform
    std::string resourcePath; //variable that will hold the path to the input Coil files
    unsigned steps; // Variaable for simulations steps to perform
    double stepSize; // Variable for simulation step size ("Simulation resolution")
    unsigned precision; //Precision to be used for output results
    unsigned int length; // Variable used to set the amount of points to read from the file of particles
    unsigned int mode; //Use algorithm convergence as stop condition or execute the given amount of steps
    std::string output;
    std::string jobId;
    std::ofstream handler;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len); //MPI function to get the name of a given processor
    if(0 == myRank) // Only Rank with 0 ID executes this block: Input parameters are parsed using the different functions present in this cpp file
    {
        std::cout << "Communicator Size=[" << commSize << "]." << std::endl;
        resourcePath = getResourcePath(argc, argv);
        steps = getStepsFromArgs(argc, argv);
        stepSize = getStepSizeFromArgs(argc, argv);
        precision = getPrintPrecisionFromArgs(argc, argv);
        length = getParticlesLengthFromArgs(argc, argv);
        mode = getModeFromArgs(argc, argv);
        jobId = getJobId(argc, argv);
        output = "results_" + jobId;
        createDirectoryIfNotExists(output);
        std::cout.precision(precision);
        std::cout << "Running with:" << std::endl;
        std::cout << "Resource Path=[" << resourcePath << "]." << std::endl;
        std::cout << "JobId=[" << jobId << "]." << std::endl;
        std::cout << "Steps=[" << steps << "]." << std::endl;
        std::cout << "Steps size=[" << stepSize << "]." << std::endl;
        std::cout << "Particles=[" << length << "]." << std::endl;
        std::cout << "Mode=[" << mode << "]." << std::endl;
        std::cout << "Output path=[" << output << "]." << std::endl;
        std::cout << "MPI size=[" << commSize << "]." << std::endl;
        std::cout << "OpenMP size=[" << ompSize << "]." << std::endl;
        std::string file_name = "stdout_"+jobId+".log";
        handler.open(file_name);
        if(!handler.is_open())
        {
            std::cerr << "Unable to open stdout.log for appending. Nothing to do." << std::endl;
            exit(0);
        }

        handler << "Running with:" << std::endl;
        handler << "Steps=[" << steps << "]." << std::endl;
        handler << "Steps size=[" << stepSize << "]." << std::endl;
        handler << "Particles=[" << length << "]." << std::endl;
        handler << "Mode=[" << mode << "]." << std::endl;
        handler << "Output path=[" << output << "]." << std::endl;
        handler << "MPI size=[" << commSize << "]." << std::endl;
        handler << "OpenMP size=[" << ompSize << "]." << std::endl;
        handler << "Rank=[" << myRank << "] => Processor Name=[" << processor_name << "]." << std::endl;
    }

    /*All relevant values are broadcasted or passed on to all processes or ranks in the universe*/
    MPI_Bcast(&steps, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&stepSize, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&precision, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&length, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&mode, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    unsigned int outputSize = static_cast<unsigned int>(output.size()); 
    MPI_Bcast(&outputSize, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    
    char* tmp = new char[outputSize + 1];
    if(0 == myRank)
    {
        std::strcpy(tmp, output.c_str());
    }
    MPI_Bcast(tmp, outputSize + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    if(0 != myRank)
    {
       output = std::string(tmp);
    }
    delete[] tmp;

    /*--------Particle Loading and distribution------------------*/
    /*A Coil structure is declared and its components are initialized with defined sizes*/ 
    Coil particles; 
    particles.x = static_cast<double*>(_mm_malloc(sizeof(double) * length, ALIGNMENT_SIZE));
    particles.y = static_cast<double*>(_mm_malloc(sizeof(double) * length, ALIGNMENT_SIZE));
    particles.z = static_cast<double*>(_mm_malloc(sizeof(double) * length, ALIGNMENT_SIZE));
    if(0 == myRank) // Rank 0 calls the LoadParticles() function which loads the position information for the different particles
    {
        LoadParticles(argc, argv, particles, length);
    }

    //Particles' information is distributed among the different parallel MPI ranks.
    MPI_Bcast(particles.x, length, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(particles.y, length, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(particles.z, length, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    std::cout << "Rank=[" << myRank << "] => Running with:" << std::endl;
    std::cout << "Rank=[" << myRank << "] => Steps=[" << steps << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => Steps size=[" << stepSize << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => Particles=[" << length << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => Mode=[" << mode << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => Output path=[" << output << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => MPI size=[" << commSize << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => OpenMP size=[" << ompSize << "]." << std::endl;
    std::cout << "Rank=[" << myRank << "] => Processor Name=[" << processor_name << "]." << std::endl;

    /*--------Coil Loading and distribution------------------*/
    /*A GlobalData structure is initialized and the coil components are initialized*/
    const size_t sizeToAllocate = sizeof(double) * TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS;
    GlobalData data;
    data.coils.x = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.coils.y = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.coils.z = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    if(0 == myRank) //Rank 0 calls the load_coil_data function to read in the different Coil files from the resourcePath
    {
        load_coil_data(data.coils.x, data.coils.y, data.coils.z, resourcePath);
        //std::cout << x[TOTAL_OF_GRADES_PADDED] << "|" << y[TOTAL_OF_GRADES_PADDED] << "|" << z[TOTAL_OF_GRADES_PADDED] << std::endl;
        //std::cout << x[TOTAL_OF_GRADES_PADDED*2] << "|" << y[TOTAL_OF_GRADES_PADDED*2] << "|" << z[TOTAL_OF_GRADES_PADDED*2] << std::endl;
    }
    //Coils' information is distributed among the different parallel MPI ranks.
    MPI_Bcast(data.coils.x, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data.coils.y, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data.coils.z, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  

  	//The e_roof components of the GlobalData data variable are initialized
    data.e_roof.x = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.e_roof.y = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.e_roof.z = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    data.leng_segment = static_cast<double*>(_mm_malloc(sizeToAllocate, ALIGNMENT_SIZE));
    if(0 == myRank) //Rank 0 uses the e_roof function to calculate the value of e_roof for the different filamentary segments of each coil
    {
        e_roof(data); // e roof is the unit vector along each segment (See: Hanson, "Compact expressions for the BiotSavart fields of a filamentary segment") 
    }
    //e_roof information is distributed among parallel processes
    MPI_Bcast(data.e_roof.x, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data.e_roof.y, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data.e_roof.z, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(data.leng_segment, TOTAL_OF_GRADES_PADDED * TOTAL_OF_COILS, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    
    const double startTime = getCurrentTime(); //Start time measurement for simulation
    runParticles(data, output, particles, length, steps, stepSize, mode); //Actual simulation function call
    const double endTime = getCurrentTime(); // Finish time measurement for simulation
    std::cout << "Rank=" << myRank << " before finalize barrier on processor=[" << processor_name << "] with time=[" << (endTime - startTime) << "]!" << std::endl;
    MPI_Barrier(MPI_COMM_WORLD);
    //endTime = getCurrentTime();
    _mm_free(data.coils.x);
    _mm_free(data.coils.y);
    _mm_free(data.coils.z);
    _mm_free(data.e_roof.x);
    _mm_free(data.e_roof.y);
    _mm_free(data.e_roof.z);
    _mm_free(data.leng_segment);
    _mm_free(particles.x);
    _mm_free(particles.y);
    _mm_free(particles.z);

    if(0 == myRank)
    {
        handler << "Total execution time=[" << (endTime - startTime) << "]." << std::endl;
	handler.close();
        handler.open("stats.csv", std::ofstream::out | std::ofstream::app);
        if(!handler.is_open())
        {
            std::cerr << "Unable to open stats.csv for appending. Nothing to do." << std::endl;
            exit(0);
        }
        handler << jobId << "," << commSize << "," << ompSize << "," << length << "," << steps << "," <<  stepSize << "," << output << "," << (endTime - startTime) << "," << processor_name << std::endl;
        handler.close();
	std::cout << "Total execution time=[" << (endTime - startTime) << "]." << std::endl;
    }
    MPI_Finalize();
    return 0;
}
