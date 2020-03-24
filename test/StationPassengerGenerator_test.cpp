//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Data structures
#include "../data_structures/passengerID.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/StationPassengerGenerator.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
// none for this test

/***** Define output ports for coupled model *****/
struct Carleton_out: public out_port<PassengerID_t>{};
struct Greenboro_out: public out_port<PassengerID_t>{};


int main(){

    /****** Station Passenger Generator atomic model instantiation *******************/
	// variables 3 and 5 passed to atomic model instances correspond to Carleton and Bayview stations respectively
    shared_ptr<dynamic::modeling::model> SPG_Carleton = dynamic::translate::make_dynamic_atomic_model<StationPassengerGenerator, TIME, int>("SPG_Carleton",3);
	shared_ptr<dynamic::modeling::model> SPG_Greenboro = dynamic::translate::make_dynamic_atomic_model<StationPassengerGenerator, TIME, int>("SPG_Greenboro",5);
	
    /*******TOP MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(Carleton_out),typeid(Greenboro_out)};
    dynamic::modeling::Models submodels_TOP = {SPG_Carleton,SPG_Greenboro};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP;
    eocs_TOP = {
        dynamic::translate::make_EOC<StationPassengerGenerator_defs::PassengerInitial,Carleton_out>("SPG_Carleton"),
		dynamic::translate::make_EOC<StationPassengerGenerator_defs::PassengerInitial,Greenboro_out>("SPG_Greenboro")
	};
    dynamic::modeling::ICs ics_TOP = {};
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/ATOMIC_TEST_StationPassengerGenerator_outputs.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/ATOMIC_TEST_StationPassengerGenerator_state.txt");
    struct oss_sink_state{
        static ostream& sink(){          
            return out_state;
        }
    };
    
    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/ 
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("00:31:00:000"));
    return 0;
}
