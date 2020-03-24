/*
* Zachary Dunnigan
*
* InTrainQueue:
* Cadmium implementation of CD++ InTrainQueue.cpp atomic model
*/

#ifndef __INTRAINQUEUE_HPP__
#define __INTRAINQUEUE_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>
#include <list>

#include "../data_structures/passengerID.hpp"
#include "../data_structures/stationnumber.hpp"


using namespace cadmium;
using namespace std;

//Port definition
struct InTrainQueue_defs{
	struct PassengerOut : public out_port<PassengerID_t> {};
	struct StationNumberIn : public in_port<StationNumber_t> {};
	struct PassengerIn : public in_port<PassengerID_t> {};	
};

template<typename TIME> 
class InTrainQueue{
    public:
    // ports definition
	using input_ports=tuple<typename InTrainQueue_defs::StationNumberIn, typename InTrainQueue_defs::PassengerIn>;
    using output_ports=tuple<typename InTrainQueue_defs::PassengerOut>;

	
    // state definition
    struct state_type{
        bool active;
		bool FillQueue;
		bool EmptyQueue;
		int StationDest;
		typedef list<int> ElementList;
		ElementList PassengerQueue;
		TIME next_internal;
    }; 
    state_type state;
	
	
	// default constructor
    InTrainQueue() = default;
	
    //  constructor with passed variable
    InTrainQueue(int var) {
        state.active = false;		// Initially passive
		state.FillQueue = false;	// If passive, queue is not being filled 
		state.EmptyQueue = false;	// If passive, queue is not being emptied
		state.PassengerQueue.erase(state.PassengerQueue.begin(), state.PassengerQueue.end());
		state.StationDest = var;	// Station destination of a passennger queue is determined by the main simulator
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}
	
	// external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
		
		if((get_messages<typename InTrainQueue_defs::PassengerIn>(mbs).size()+get_messages<typename InTrainQueue_defs::StationNumberIn>(mbs).size())>1) 
             assert(false && "one input per time unit");
		
		for(const auto &x : get_messages<typename InTrainQueue_defs::PassengerIn>(mbs)){
			if(state.active == false){
				if (x.passengerID%10 == state.StationDest ){
					state.FillQueue = true;
					state.active = true;
					state.PassengerQueue.push_back(x.passengerID);
					state.next_internal = TIME("00:00:00:000");
				}
			} else if (state.active == true){
				// If passenger is inputted while active, check if queue emptying
				// This should never happen as the passenger generator cannot create passengers with the same destination as start station
				// This is simply here as a precaution
				if (x.passengerID%10 == state.StationDest ){
					// If queue is being emptied and a passenger boards whos destination is the same as the current station
					// Then the passenger simply gets on the train and then gets right back off
					state.PassengerQueue.push_back(x.passengerID);
					state.next_internal = state.next_internal - e;
				}
			}
		}
		
		for(const auto &x : get_messages<typename InTrainQueue_defs::StationNumberIn>(mbs)){
			if(state.active == false){
				if (x.station == state.StationDest && state.PassengerQueue.size() > 0){
					state.EmptyQueue = true;
					state.active = true;
					state.next_internal = TIME("00:00:00:025");
				}
			}
		}
		
	}	
	
	
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
		internal_transition();
        external_transition(TIME(), move(mbs));
    }
	
	
    // internal transition
    void internal_transition(){
		
		if(state.EmptyQueue){
			state.PassengerQueue.pop_front();
			if (state.PassengerQueue.size() > 0){
					state.active = true;
					state.EmptyQueue = true;
					state.next_internal = TIME("00:00:00:025");
			} else {
				state.active = false;
				state.EmptyQueue = false;
				state.FillQueue = false;
				state.next_internal = std::numeric_limits<TIME>::infinity();
			}
		}
				
		if(state.FillQueue){
			state.FillQueue = false;
			state.EmptyQueue = false;
			state.active = false;
			state.next_internal = std::numeric_limits<TIME>::infinity();
		}
		
    }
	
	
    // output function
    typename make_message_bags<output_ports>::type output() const {
		typename make_message_bags<output_ports>::type bags;
		if (state.EmptyQueue && state.PassengerQueue.size() > 0){
			PassengerID_t out;
			out.passengerID = state.PassengerQueue.front();
			get_messages<typename InTrainQueue_defs::PassengerOut>(bags).push_back(out);
		}
		return bags;
    }
	
    // time_advance function
    TIME time_advance() const {
		return state.next_internal;
    }
	
    friend ostringstream& operator<<(ostringstream& os, const typename InTrainQueue<TIME>::state_type& i) {

		os << "next internal: " << i.next_internal << "\n    "
		   << "Active: " << i.active << " & Queue Destination: " << i.StationDest << " & Fill Queue: " << i.FillQueue << " & Empty Queue: " << i.EmptyQueue << " & Passengers in Queue: "  << i.PassengerQueue.size();
		return os;
    }
	
};    
#endif // _INTRAINQUEUE_HPP_