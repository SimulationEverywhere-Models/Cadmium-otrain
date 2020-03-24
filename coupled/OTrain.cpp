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
#include "../atomics/StationArrivalGenerator.hpp"
#include "../atomics/InTrainQueue.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

//OTrain Port definition
struct OTrain_defs{
	struct OT_PassengerIn : public in_port<PassengerID_t> {};	
	struct OT_PassengerOut : public out_port<PassengerID_t> {};
	struct OT_StationNumber : public out_port<StationNumber_t> {};
};

/****** Station Arrival Generator atomic model instantiation ******/
shared_ptr<dynamic::modeling::model> SAG = dynamic::translate::make_dynamic_atomic_model<StationArrivalGenerator,TIME>("SAG");

/****** In Train Queue atomic model instantiations ****************/
shared_ptr<dynamic::modeling::model> ITQ_Bayview = dynamic::translate::make_dynamic_atomic_model<InTrainQueue,TIME,int>("ITQ_Bayview",1);
shared_ptr<dynamic::modeling::model> ITQ_Carling = dynamic::translate::make_dynamic_atomic_model<InTrainQueue,TIME,int>("ITQ_Carling",2);
shared_ptr<dynamic::modeling::model> ITQ_Carleton = dynamic::translate::make_dynamic_atomic_model<InTrainQueue,TIME,int>("ITQ_Carleton",3);
shared_ptr<dynamic::modeling::model> ITQ_Confed = dynamic::translate::make_dynamic_atomic_model<InTrainQueue,TIME,int>("ITQ_Confed",4);
shared_ptr<dynamic::modeling::model> ITQ_Greenboro = dynamic::translate::make_dynamic_atomic_model<InTrainQueue,TIME,int>("ITQ_Greenboro",5);

/*******OTRAIN MODEL********/
dynamic::modeling::Ports iports_OTrain = {typeid(OTrain_defs::OT_PassengerIn)};
dynamic::modeling::Ports oports_OTrain = {typeid(OTrain_defs::OT_PassengerOut),typeid(OTrain_defs::OT_StationNumber)};
dynamic::modeling::Models submodels_OTrain = {SAG,ITQ_Bayview,ITQ_Carling,ITQ_Carleton,ITQ_Confed,ITQ_Greenboro};
dynamic::modeling::EICs eics_OTrain = {
	dynamic::translate::make_EIC<OTrain_defs::OT_PassengerIn,InTrainQueue_defs::PassengerIn>("ITQ_Bayview"),
	dynamic::translate::make_EIC<OTrain_defs::OT_PassengerIn,InTrainQueue_defs::PassengerIn>("ITQ_Carling"),
	dynamic::translate::make_EIC<OTrain_defs::OT_PassengerIn,InTrainQueue_defs::PassengerIn>("ITQ_Carleton"),
	dynamic::translate::make_EIC<OTrain_defs::OT_PassengerIn,InTrainQueue_defs::PassengerIn>("ITQ_Confed"),
	dynamic::translate::make_EIC<OTrain_defs::OT_PassengerIn,InTrainQueue_defs::PassengerIn>("ITQ_Greenboro")
};
dynamic::modeling::EOCs eocs_OTrain = {
	dynamic::translate::make_EOC<StationArrivalGenerator_defs::StationNumber,OTrain_defs::OT_StationNumber>("SAG"),
	dynamic::translate::make_EOC<InTrainQueue_defs::PassengerOut,OTrain_defs::OT_PassengerOut>("ITQ_Bayview"),
	dynamic::translate::make_EOC<InTrainQueue_defs::PassengerOut,OTrain_defs::OT_PassengerOut>("ITQ_Carling"),
    dynamic::translate::make_EOC<InTrainQueue_defs::PassengerOut,OTrain_defs::OT_PassengerOut>("ITQ_Carleton"),
	dynamic::translate::make_EOC<InTrainQueue_defs::PassengerOut,OTrain_defs::OT_PassengerOut>("ITQ_Confed"),
	dynamic::translate::make_EOC<InTrainQueue_defs::PassengerOut,OTrain_defs::OT_PassengerOut>("ITQ_Greenboro")
};
dynamic::modeling::ICs ics_OTrain = {
    dynamic::translate::make_IC<StationArrivalGenerator_defs::StationNumber,InTrainQueue_defs::StationNumberIn>("SAG","ITQ_Bayview"),
	dynamic::translate::make_IC<StationArrivalGenerator_defs::StationNumber,InTrainQueue_defs::StationNumberIn>("SAG","ITQ_Carling"),
	dynamic::translate::make_IC<StationArrivalGenerator_defs::StationNumber,InTrainQueue_defs::StationNumberIn>("SAG","ITQ_Carleton"),
	dynamic::translate::make_IC<StationArrivalGenerator_defs::StationNumber,InTrainQueue_defs::StationNumberIn>("SAG","ITQ_Confed"),
	dynamic::translate::make_IC<StationArrivalGenerator_defs::StationNumber,InTrainQueue_defs::StationNumberIn>("SAG","ITQ_Greenboro")
};
