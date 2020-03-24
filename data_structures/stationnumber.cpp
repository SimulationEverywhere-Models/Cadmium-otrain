#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "stationnumber.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const StationNumber_t& msg) {
  os << msg.direction << " " << msg.station;
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>> (istream& is, StationNumber_t& msg) {
  is >> msg.direction;
  is >> msg.station;
  return is;
}
