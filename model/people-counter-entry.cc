#include "ns3/people-counter-entry.h"
#include "ns3/log.h"

namespace ns3 {
namespace lorawan {

NS_LOG_COMPONENT_DEFINE ("PeopleCounterEntry");
NS_OBJECT_ENSURE_REGISTERED (PeopleCounterEntry);

TypeId PeopleCounterEntry::GetTypeId(void)
{
  static TypeId tid = TypeId ("ns3::PeopleCounterEntry")
    .AddConstructor<PeopleCounterEntry> ()
    .SetGroupName ("lorawan");
  return tid;
}

PeopleCounterEntry::PeopleCounterEntry()
{
  this->nodeNumber = -1;
  this->locationId = -1;
}

PeopleCounterEntry::PeopleCounterEntry(uint32_t nodeNumber, uint32_t locationId)
{
  this->nodeNumber = nodeNumber;
  this->locationId = locationId;
}

PeopleCounterEntry::~PeopleCounterEntry() {}

uint32_t PeopleCounterEntry::GetNodeNumber()
{
  return this->nodeNumber;
}

uint32_t PeopleCounterEntry::GetLocationId()
{
  return this->locationId;
}
}
}