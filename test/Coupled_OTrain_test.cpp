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
struct outp_passenger: public out_port<PassengerID_t>{};
struct outp_station: public out_port<StationNumber_t>{};

/****** Passsenger ID Input Reader atomic model declarations *******************/
template<typename T>
class InputReader_PassengerID_t : public iestream_input<PassengerID_t,T> {
    public:
        InputReader_PassengerID_t () = default;
        InputReader_PassengerID_t (const char* file_path) : iestream_input<PassengerID_t,T>(file_path) {}
};

int main(){

	/****** PassengerID Input Reader atomic model instantiation *******************/
    const char * i_input_data_passenger = "../input_data/OTrain_input_test_PassengerIn.txt";
    shared_ptr<dynamic::modeling::model> input_reader_passenger = dynamic::translate::make_dynamic_atomic_model<InputReader_PassengerID_t, TIME, const char*>("input_reader_passenger", move(i_input_data_passenger));

	/****** OTrain coupled model instantiation *******************/
	shared_ptr<dynamic::modeling::coupled<TIME>> OTrain1 = make_shared<dynamic::modeling::coupled<TIME>>("OTrain1", submodels_OTrain, iports_OTrain, oports_OTrain, eics_OTrain, eocs_OTrain, ics_OTrain);

	/*******TOP MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(outp_passenger),typeid(outp_station)};
    dynamic::modeling::Models submodels_TOP = {input_reader_passenger,OTrain1};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<OTrain_defs::OT_PassengerOut,outp_passenger>("OTrain1"),
		dynamic::translate::make_EOC<OTrain_defs::OT_StationNumber,outp_station>("OTrain1")
		};
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<PassengerID_t>::out,OTrain_defs::OT_PassengerIn>("input_reader_passenger","OTrain1")
	};
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;

    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/COUPLED_TEST_OTrain_outputs.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/COUPLED_TEST_OTrain_state.txt");
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
    r.run_until(NDTime("00:41:00:000"));
    return 0;
}