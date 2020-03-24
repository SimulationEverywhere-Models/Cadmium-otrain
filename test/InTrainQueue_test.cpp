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
#include "../data_structures/stationnumber.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/InTrainQueue.hpp"

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

/****** Passsenger ID Input Reader atomic model declarations *******************/
template<typename T>
class InputReader_PassengerID_t : public iestream_input<PassengerID_t,T> {
    public:
        InputReader_PassengerID_t () = default;
        InputReader_PassengerID_t (const char* file_path) : iestream_input<PassengerID_t,T>(file_path) {}
};

/****** Station Number Input Reader atomic model declarations *******************/
template<typename T>
class InputReader_StationNumber_t : public iestream_input<StationNumber_t,T> {
    public:
        InputReader_StationNumber_t () = default;
        InputReader_StationNumber_t (const char* file_path) : iestream_input<StationNumber_t,T>(file_path) {}
};

int main(){
	
	/****** Passenger ID Input Reader atomic model instantiation *******************/
    const char * i_input_data_passenger = "../input_data/InTrainQueue_input_test_PassengerIn.txt";
    shared_ptr<dynamic::modeling::model> input_reader_passenger = dynamic::translate::make_dynamic_atomic_model<InputReader_PassengerID_t, TIME, const char*>("input_reader_passenger", move(i_input_data_passenger));
	
	/****** Station Number Input Reader atomic model instantiation *******************/
    const char * i_input_data_station = "../input_data/InTrainQueue_input_test_StationNumberIn.txt";
    shared_ptr<dynamic::modeling::model> input_reader_station = dynamic::translate::make_dynamic_atomic_model<InputReader_StationNumber_t, TIME, const char*>("input_reader_station", move(i_input_data_station));

    /****** In Train Queue atomic model instantiation *******************/
	//  variable "3" is passed to atomic model which corresponds to Carleton as the destination for people in the queue
    shared_ptr<dynamic::modeling::model> ITQ_Carleton = dynamic::translate::make_dynamic_atomic_model<InTrainQueue,TIME,int>("ITQ_Carleton",3);

    /*******TOP MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(outp_passenger)};
    dynamic::modeling::Models submodels_TOP = {input_reader_passenger, input_reader_station, ITQ_Carleton};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<InTrainQueue_defs::PassengerOut,outp_passenger>("ITQ_Carleton")
    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<PassengerID_t>::out,InTrainQueue_defs::PassengerIn>("input_reader_passenger","ITQ_Carleton"),
		dynamic::translate::make_IC<iestream_input_defs<StationNumber_t>::out,InTrainQueue_defs::StationNumberIn>("input_reader_station","ITQ_Carleton")
	};
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

    /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/ATOMIC_TEST_InTrainQueue_outputs.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/ATOMIC_TEST_InTrainQueue_state.txt");
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
    r.run_until(NDTime("00:40:00:000"));
    return 0;
}