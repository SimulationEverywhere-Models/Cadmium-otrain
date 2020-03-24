/*
* Zachary Dunnigan
*
* StationQueue:
* Cadmium implementation of CD++ StationQueue.cpp atomic model
*/

#ifndef __STATIONQUEUE_HPP__
#define __STATIONQUEUE_HPP__

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
struct StationQueue_defs{
	struct PassengerOut : public out_port<PassengerID_t> {};
	struct StationNumberIn : public in_port<StationNumber_t> {};
	struct PassengerIn : public in_port<PassengerID_t> {};	
};

template<typename TIME> 
class StationQueue{
    public:
    // ports definition
	using input_ports=tuple<typename StationQueue_defs::StationNumberIn, typename StationQueue_defs::PassengerIn>;
    using output_ports=tuple<typename StationQueue_defs::PassengerOut>;

	
    // state definition
    struct state_type{
        bool active;
		bool FillQueue;
		bool EmptyQueue;
		int StationDest;
		typedef list<int> ElementList;
		ElementList StationQueue;
		TIME next_internal;
    }; 
    state_type state;
	
	
	// default constructor
    StationQueue() = default;
	
    //  constructor with passed variable
    StationQueue(int var) {
        state.active = false;		// Initially passive
		state.FillQueue = false;	// If passive, queue is not being filled 
		state.EmptyQueue = false;	// If passive, queue is not being emptied
		state.StationQueue.erase(state.StationQueue.begin(), state.StationQueue.end());
		state.StationDest = var;	// Station destination of a passennger queue is determined by the main simulator
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}
	
	// external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
		
		if((get_messages<typename StationQueue_defs::PassengerIn>(mbs).size()+get_messages<typename StationQueue_defs::StationNumberIn>(mbs).size())>1) 
             assert(false && "one input per time unit");
		
		for(const auto &x : get_messages<typename StationQueue_defs::PassengerIn>(mbs)){
			if(state.active == false){
				if (!state.EmptyQueue){
					// The 2nd condition on below if statement is an improvement on original CD++ code
					// Ensures that if a passenger generated from a different station tries to enter the queue at the current station, they are denied
					if (x.passengerID%10 != state.StationDest && (x.passengerID%100 - x.passengerID%10)/10 == state.StationDest){
						state.FillQueue = true;
						state.active = true;
						state.StationQueue.push_back(x.passengerID);
						state.next_internal = TIME("00:00:00:000");
					}
				}
			} else if (state.active = true){
				// If passenger is inputted while active, check if queue emptying
				if(state.EmptyQueue){
					// If queue is being emptied and a valid passenger is generated, that passengers joins the queue
					// The queue will continue to empty with the new passenger added to the end
					if (x.passengerID%10 != state.StationDest && (x.passengerID%100 - x.passengerID%10)/10 == state.StationDest){
						state.StationQueue.push_back(x.passengerID);
						state.next_internal = state.next_internal - e;
					}
				}
				
			}
		}
		
		for(const auto &x : get_messages<typename StationQueue_defs::StationNumberIn>(mbs)){
			if(state.active == false){
				if (x.station == state.StationDest && state.StationQueue.size() > 0){
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
			state.StationQueue.pop_front();
			state.FillQueue = false;
			if (state.StationQueue.size() > 0){
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
		PassengerID_t out;
		if (state.EmptyQueue && state.StationQueue.size() > 0){
			out.passengerID = state.StationQueue.front();
			get_messages<typename StationQueue_defs::PassengerOut>(bags).push_back(out);
			return bags;
		}
    }
	
    // time_advance function
    TIME time_advance() const {
		return state.next_internal;
    }
	
    friend ostringstream& operator<<(ostringstream& os, const typename StationQueue<TIME>::state_type& i) {

		os << "next internal: " << i.next_internal << "\n    "
		   << "Active: " << i.active << " & Station Location: " << i.StationDest << " & Fill Queue: " << i.FillQueue << " & Empty Queue: " << i.EmptyQueue << " & Passengers at Station: "  << i.StationQueue.size();
		return os;
    }
	
};    
#endif // _STATIONQUEUE_HPP_