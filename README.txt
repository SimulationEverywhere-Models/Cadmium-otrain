This folder contains the OTrain model implemented in Cadmium

/**************************/
/****FILES ORGANIZATION****/
/**************************/

README.txt	
Cadmium-OTrainTripTimeAnalysis.pdf
form_otrain_cadmium.doc
ModelDescription-otrain_cadmium.xml
makefile

atomics [This folder contains atomic models implemented in Cadmium]
	InTrainQueue.hpp
	StationArrivalGenerator.hpp
	StationPassengerGenerator.hpp
	StationQueue.hpp
	
bin [This folder will be created automatically the first time you compile the poject.
	 It will contain all the executables]
	 
build [This folder will be created automatically the first time you compile the poject.
       It will contain all the build files (.o) generated during compilation]

coupled [This folder contains coupled models implemented in Cadmium other than the top model]
	OTrain.cpp

data_structures [This folder contains message data structure used in the model]
	passengerID.hpp
	passengerID.cpp
	stationnumber.hpp
	stationnumber.cpp
	
input_data [This folder contains all the input data to run the tests]
	InTrainQueue_input_test_PassengerIn.txt
	InTrainQueue_input_test_StationNumberIn.txt
	StationQueue_input_test_PassengerIn.txt
	StationQueue_input_test_StationNumberIn.txt
	OTrain_input_test_PassengerIn.txt
	
simulation_results [This folder will be created automatically the first time you compile the poject.
                    It will store the outputs from your simulations and tests]

test [This folder contains tests of atomic models and coupled models]
	InTrainQueue_test.cpp
	StationArrivalGenerator_test.cpp
	StationPassengerGenerator_test.cpp
	StationQueue_test.cpp
	Coupled_OTrain_test.cpp
	
top_model [This folder contains the OTrain top model]	
	main.cpp
	
/*************/
/****STEPS****/
/*************/

0 - Cadmium-OTrainTripTimeAnalysis.docx contains the explanation of this model

1 - Compile the project and the tests
	1 - Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the otrain_cadmium folder
	2 - To compile the project and the tests, type in the terminal:
			make clean; make all
			
2 - Run Station Arrival Generator test
	1 - Open the terminal in the bin folder. 
	2 - To run the test, type in:
			./STATIONARRIVALGENERATOR_TEST (or ./STATIONARRIVALGENERATOR_TEST.exe for Windows)
		OR
			select the executable test file STATIONARRIVALGENERATOR_TEST.exe from the bin folder
	3 - To check the output of the test, go to the folder simulation_results and open:
			"ATOMIC_TEST_StationArrivalGenerator_outputs.txt" and "ATOMIC_TEST_StationArrivalGenerator_state.txt"
	
	
3 - Repeat step 3 for the remaining atomic tests.
	The output and state logs can be found in simulation_results with the same naming convention shown in step 3
	Atomic model test logs will have the prefix "ATOMIC_TEST_..."
	Select executable tests from bin folder or in terminal type:
	1 - ./STATIONPASSENGERGENERATOR_TEST
	2 - ./STATIONQUEUE_TEST
	3 - ./INTRAINQUEUE_TEST
	
4 - Run the coupled model tests following the same procedure as step 3.
	The output and state logs can be found in simulation_results with the same naming convention shown in step 3.
	Coupled model test logs will have the prefix "COUPLED_TEST_..."
	Select executable tests from bin folder or in terminal type:
	1 - ./COUPLED_OTRAIN_TEST
			
5 - Run the top model
	1 - Open the terminal (Ubuntu terminal for Linux and Cygwin for Windows) in the bin folder.
	2 - To run the test, type in:
			./OTRAIN_MAIN_SIMULATION (or ./OTRAIN_MAIN_SIMULATION.exe for Windows)
		OR
			select te executable main simulation file OTRAIN_MAIN_SIMULATION.exe from the bin folder
	3 - To check the output of the model, go to the folder simulation_results and open:
			"OTRAIN_MAIN_SIMULATION_outputs.txt" 
			"OTRAIN_MAIN_SIMULATION_state.txt"
	