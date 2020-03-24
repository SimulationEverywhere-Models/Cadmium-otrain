#ifndef BOOST_SIMULATION_STATIONNUMBER_HPP
#define BOOST_SIMULATION_STATIONNUMBER_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** StationNumber_t ****************/
/*******************************************/
struct StationNumber_t{
  StationNumber_t(){}
  StationNumber_t(int i_direction, int i_station)
   :direction(i_direction), station(i_station){}

  	int   direction;
  	int   station;
};

istream& operator>> (istream& is, StationNumber_t& msg);

ostream& operator<<(ostream& os, const StationNumber_t& msg);


#endif // BOOST_SIMULATION_STATIONNUMBER_HPP