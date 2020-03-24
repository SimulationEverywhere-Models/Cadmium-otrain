#ifndef BOOST_SIMULATION_PASSENGERID_HPP
#define BOOST_SIMULATION_PASSENGERID_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** PassengerID_t ****************/
/*******************************************/
struct PassengerID_t{
  PassengerID_t(){}
  PassengerID_t(int i_passengerID)
   :passengerID(i_passengerID){}

  	int   passengerID;
};

istream& operator>> (istream& is, PassengerID_t& msg);

ostream& operator<<(ostream& os, const PassengerID_t& msg);


#endif // BOOST_SIMULATION_PASSENGERID_HPP