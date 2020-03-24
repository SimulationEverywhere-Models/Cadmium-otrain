/*
* Zachary Dunnigan
*
* StationPassengerGenerator:
* Cadmium implementation of CD++ StationGenerator.cpp atomic model
*/

#ifndef _STATIONPASSENGERGENERATOR_HPP__
#define _STATIONPASSENGERGENERATOR_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>
#include <cmath> 

#include "../data_structures/passengerID.hpp"

using namespace cadmium;
using namespace std;

// Distributions for wait time and passenger destination
std::default_random_engine generator;
std::normal_distribution<double> waitdist(5,5);
std::uniform_int_distribution<int> stationdist(1,5);

//Port definition
struct StationPassengerGenerator_defs{
    struct PassengerInitial : public out_port<PassengerID_t> {};
};

template<typename TIME> 
class StationPassengerGenerator{
    public:
    // ports definition
    using output_ports=tuple<typename StationPassengerGenerator_defs::PassengerInitial>;
	using input_ports=tuple<>;	//unused, no input port for generator
	
    // state definition
    struct state_type{
        bool active;
		double WaitTime;			// Wait time between passenger generation
		int StationStartNumber;		// Constant, corresponds to what station queue the generator connects to, varies by station
		int Station;				// Corresponds destination station of the generated passenger
		int PassengerNumber;		// Keeps track of total number of passenger generated at a given station
		int PassengerOutput;		// Unique passenger ID to be outputted: identifies passenger#, start station, and destination
    }; 
    state_type state;
	
    // default constructor
	StationPassengerGenerator() = default;
	
	// constructor with passed variable
    StationPassengerGenerator(int var) {
        state.active = true;
		state.WaitTime = 0;
		state.Station = 0;
		state.PassengerNumber = 0;
		state.PassengerOutput = 0;
		state.StationStartNumber = var;	// Each instance of the passeneger generator corresponds to a specific Station
	}
	
	// external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
	
    }
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
		internal_transition();
        external_transition(TIME(), move(mbs));
    }
	
    // internal transition
    void internal_transition() {
        switch(state.active){
			case(true) :
					// Increment Passenger number
					state.PassengerNumber++;
					
					// Get wait time and desination for next passenger
					state.WaitTime = waitdist(generator);
					while (round(state.WaitTime)<=0 || round(state.WaitTime)>=10){
						state.WaitTime = waitdist(generator);
					}
					state.Station = stationdist(generator);
					while (state.Station == state.StationStartNumber){
						state.Station = stationdist(generator);
					}
					
					if(state.Station == 1){
						// Destination: Bayview						
						state.PassengerOutput = state.PassengerNumber*100+state.StationStartNumber*10+1;
					} else if(state.Station == 2){
						// Destination: Carling
						state.PassengerOutput = state.PassengerNumber*100+state.StationStartNumber*10+2;
					} else if(state.Station == 3){
						// Destination: Carleton
						state.PassengerOutput = state.PassengerNumber*100+state.StationStartNumber*10+3;
					} else if(state.Station == 4){
						// Destination: Confed
						state.PassengerOutput = state.PassengerNumber*100+state.StationStartNumber*10+4;
					} else {
						// Destination: Greenboro	
						state.PassengerOutput = state.PassengerNumber*100+state.StationStartNumber*10+5;
					}
					
			case(false) :;	// Never happens, generator is always active while simulation running
		}
    }
	

    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        PassengerID_t out;
		if(state.PassengerOutput%10 == state.StationStartNumber && state.PassengerNumber>1){
			// Do nothing, passenger is already at their destination station 
		} else if(state.PassengerOutput == 0){
			// DO nothing, first passenger has not been generated yet
		} else {
			// Send passenger ID to output
			out.passengerID = state.PassengerOutput;
			get_messages<typename StationPassengerGenerator_defs::PassengerInitial>(bags).push_back(out);
			return bags;
		}			
    }
	
    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
		if (state.PassengerNumber == 0) {  	// initializing, begin generation after 1 minute to avoid simultaneuos inputs with StationNumberIn
			string s = "00:00:00:500";
			next_internal = TIME(s);
		} else if (state.active == true) {  
			string s = "00:" + to_string(round(state.WaitTime)) + ":00:00";
			next_internal = TIME(s);
		} else if(state.active == false) {
			next_internal = numeric_limits<TIME>::infinity();
		}
		return next_internal;

    }

    friend ostringstream& operator<<(ostringstream& os, const typename StationPassengerGenerator<TIME>::state_type& i) {
		int x = round(i.WaitTime);
		if (i.PassengerNumber == 0){
			os << "initializing, passengers will begin generating in 0.5 seconds";
		} else {
			os << "creating next passenger -> Wait Time: " << x << " min & Passenger Number: " << i.PassengerNumber << " & Start Station: " << i.StationStartNumber << " & Desination: " << i.Station;
		}
		return os;
    }
	
};    
#endif // _STATIONPASSSENGERGENERATOR_HPP_