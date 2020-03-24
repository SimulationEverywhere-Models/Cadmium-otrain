#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "passengerID.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const PassengerID_t& msg) {
  os << msg.passengerID;
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>> (istream& is, PassengerID_t& msg) {
  is >> msg.passengerID;
  return is;
}
