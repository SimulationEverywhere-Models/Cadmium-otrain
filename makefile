CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE ALL THE TESTS TOGETHER 
stationnumber.o: data_structures/stationnumber.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/stationnumber.cpp -o build/stationnumber.o

passengerID.o: data_structures/passengerID.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/passengerID.cpp -o build/passengerID.o

StationArrivalGenerator_test.o: test/StationArrivalGenerator_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/StationArrivalGenerator_test.cpp -o build/StationArrivalGenerator_test.o

StationPassengerGenerator_test.o: test/StationPassengerGenerator_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/StationPassengerGenerator_test.cpp -o build/StationPassengerGenerator_test.o

StationQueue_test.o: test/StationQueue_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/StationQueue_test.cpp -o build/StationQueue_test.o

InTrainQueue_test.o: test/InTrainQueue_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/InTrainQueue_test.cpp -o build/InTrainQueue_test.o

Coupled_OTrain_test.o: test/Coupled_OTrain_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/Coupled_OTrain_test.cpp -o build/Coupled_OTrain_test.o

tests: StationArrivalGenerator_test.o StationPassengerGenerator_test.o StationQueue_test.o InTrainQueue_test.o Coupled_OTrain_test.o stationnumber.o passengerID.o
	$(CC) -g -o bin/STATIONARRIVALGENERATOR_TEST build/StationArrivalGenerator_test.o build/stationnumber.o
	$(CC) -g -o bin/STATIONPASSENGERGENERATOR_TEST build/StationPassengerGenerator_test.o build/passengerID.o
	$(CC) -g -o bin/STATIONQUEUE_TEST build/StationQueue_test.o build/passengerID.o build/stationnumber.o
	$(CC) -g -o bin/INTRAINQUEUE_TEST build/InTrainQueue_test.o build/passengerID.o build/stationnumber.o
	$(CC) -g -o bin/COUPLED_OTRAIN_TEST build/Coupled_OTrain_test.o build/passengerID.o build/stationnumber.o

#TARGET TO COMPILE ONLY SIMULATOR
main_top.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main.cpp -o build/main_top.o
simulator: main_top.o stationnumber.o passengerID.o
	$(CC) -g -o bin/OTRAIN_MAIN_SIMULATION build/main_top.o build/passengerID.o build/stationnumber.o

#TARGET TO COMPILE EVERYTHING
all: simulator tests

#CLEAN COMMANDS
clean:
	rm -f bin/* build/*

