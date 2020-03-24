//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Data structures
#include "../data_structures/passengerID.hpp"
#include "../data_structures/stationnumber.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../coupled/OTrain.cpp"
#include "../atomics/StationPassengerGenerator.hpp"
#include "../atomics/StationQueue.hpp"

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
struct PassengerArrived: public out_port<PassengerID_t>{};

int main(){
	
	/****** Station Passenger Generator instantiations *******************/
	shared_ptr<dynamic::modeling::model> SPG_Bayview = dynamic::translate::make_dynamic_atomic_model<StationPassengerGenerator,TIME,int>("SPG_Bayview",1);
	shared_ptr<dynamic::modeling::model> SPG_Carling = dynamic::translate::make_dynamic_atomic_model<StationPassengerGenerator,TIME,int>("SPG_Carling",2);
	shared_ptr<dynamic::modeling::model> SPG_Carleton = dynamic::translate::make_dynamic_atomic_model<StationPassengerGenerator,TIME,int>("SPG_Carleton",3);
	shared_ptr<dynamic::modeling::model> SPG_Confed = dynamic::translate::make_dynamic_atomic_model<StationPassengerGenerator,TIME,int>("SPG_Confed",4);
	shared_ptr<dynamic::modeling::model> SPG_Greenboro = dynamic::translate::make_dynamic_atomic_model<StationPassengerGenerator,TIME,int>("SPG_Greenboro",5);
	
	/****** Staion Queue instantiations *******************/
	shared_ptr<dynamic::modeling::model> SQ_Bayview = dynamic::translate::make_dynamic_atomic_model<StationQueue,TIME,int>("SQ_Bayview",1);
	shared_ptr<dynamic::modeling::model> SQ_Carling = dynamic::translate::make_dynamic_atomic_model<StationQueue,TIME,int>("SQ_Carling",2);
	shared_ptr<dynamic::modeling::model> SQ_Carleton = dynamic::translate::make_dynamic_atomic_model<StationQueue,TIME,int>("SQ_Carleton",3);
	shared_ptr<dynamic::modeling::model> SQ_Confed = dynamic::translate::make_dynamic_atomic_model<StationQueue,TIME,int>("SQ_Confed",4);
	shared_ptr<dynamic::modeling::model> SQ_Greenboro = dynamic::translate::make_dynamic_atomic_model<StationQueue,TIME,int>("SQ_Greenboro",5);

	/****** OTrain coupled model instantiation *******************/
	shared_ptr<dynamic::modeling::coupled<TIME>> OTrain1 = make_shared<dynamic::modeling::coupled<TIME>>("OTrain1", submodels_OTrain, iports_OTrain, oports_OTrain, eics_OTrain, eocs_OTrain, ics_OTrain);

	/*******TOP MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(PassengerArrived)};
    dynamic::modeling::Models submodels_TOP = {OTrain1,SPG_Bayview,SPG_Carling,SPG_Carleton,SPG_Confed,SPG_Greenboro,SQ_Bayview,SQ_Carling,SQ_Carleton,SQ_Confed,SQ_Greenboro};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<OTrain_defs::OT_PassengerOut,PassengerArrived>("OTrain1")
		};
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<StationPassengerGenerator_defs::PassengerInitial,StationQueue_defs::PassengerIn>("SPG_Bayview","SQ_Bayview"),
		dynamic::translate::make_IC<StationPassengerGenerator_defs::PassengerInitial,StationQueue_defs::PassengerIn>("SPG_Carling","SQ_Carling"),
		dynamic::translate::make_IC<StationPassengerGenerator_defs::PassengerInitial,StationQueue_defs::PassengerIn>("SPG_Carleton","SQ_Carleton"),
		dynamic::translate::make_IC<StationPassengerGenerator_defs::PassengerInitial,StationQueue_defs::PassengerIn>("SPG_Confed","SQ_Confed"),
		dynamic::translate::make_IC<StationPassengerGenerator_defs::PassengerInitial,StationQueue_defs::PassengerIn>("SPG_Greenboro","SQ_Greenboro"),
		dynamic::translate::make_IC<StationQueue_defs::PassengerOut,OTrain_defs::OT_PassengerIn>("SQ_Bayview","OTrain1"),
		dynamic::translate::make_IC<StationQueue_defs::PassengerOut,OTrain_defs::OT_PassengerIn>("SQ_Carling","OTrain1"),
		dynamic::translate::make_IC<StationQueue_defs::PassengerOut,OTrain_defs::OT_PassengerIn>("SQ_Carleton","OTrain1"),
		dynamic::translate::make_IC<StationQueue_defs::PassengerOut,OTrain_defs::OT_PassengerIn>("SQ_Confed","OTrain1"),
		dynamic::translate::make_IC<StationQueue_defs::PassengerOut,OTrain_defs::OT_PassengerIn>("SQ_Greenboro","OTrain1"),
		dynamic::translate::make_IC<OTrain_defs::OT_StationNumber,StationQueue_defs::StationNumberIn>("OTrain1","SQ_Bayview"),
		dynamic::translate::make_IC<OTrain_defs::OT_StationNumber,StationQueue_defs::StationNumberIn>("OTrain1","SQ_Carling"),
		dynamic::translate::make_IC<OTrain_defs::OT_StationNumber,StationQueue_defs::StationNumberIn>("OTrain1","SQ_Carleton"),
		dynamic::translate::make_IC<OTrain_defs::OT_StationNumber,StationQueue_defs::StationNumberIn>("OTrain1","SQ_Confed"),
		dynamic::translate::make_IC<OTrain_defs::OT_StationNumber,StationQueue_defs::StationNumberIn>("OTrain1","SQ_Greenboro")	};
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;

    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/OTRAIN_MAIN_SIMULATION_outputs.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/OTRAIN_MAIN_SIMULATION_state.txt");
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
    dynamic::engine::runner<NDTime, logger_top> r(TOP,{0});
    r.run_until(NDTime("01:01:00:000"));
    return 0;
}