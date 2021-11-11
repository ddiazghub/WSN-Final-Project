#include "ns3/location.h"
#include "ns3/log.h"

namespace ns3 {

Location::Location (int id, std::string name, int capacity, double _x, double _y, double _z)
    : Vector (_x, _y, _z)
{
  this->id = id;
  this->name = name;
  this->ocupation = 0;
  this->capacity = capacity;
}

Location::Location() : Vector(0, 0, 0)
{
  this->id = -1;
  this->name = "";
  this->ocupation = 0;
  this->capacity = 0;
}
}