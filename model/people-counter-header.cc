/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 University of Padova
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Davide Magrin <magrinda@dei.unipd.it>
 */

#include "ns3/people-counter-header.h"
#include "ns3/log.h"
#include "ns3/end-device-lorawan-mac.h"
#include "ns3/class-a-end-device-lorawan-mac.h"
#include "ns3/end-device-lora-phy.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include <bitset>

namespace ns3 {
namespace lorawan {

NS_LOG_COMPONENT_DEFINE ("PeopleCounterHeader");

// Initialization list
PeopleCounterHeader::PeopleCounterHeader ()
    : ocupationChange(0), deviceId (0)
{
}

PeopleCounterHeader::~PeopleCounterHeader ()
{
}

TypeId
PeopleCounterHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("PeopleCounterHeader")
    .SetParent<Header> ()
    .AddConstructor<PeopleCounterHeader> ()
  ;
  return tid;
}

TypeId
PeopleCounterHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
PeopleCounterHeader::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION_NOARGS ();

  // Sizes in bytes:
  // 4 for DevAddr + 1 for FCtrl + 2 for FCnt + 1 for FPort + 0-15 for FOpts
  uint32_t size = sizeof(int32_t) + sizeof(uint32_t);

  NS_LOG_INFO ("PeopleCounterHeader serialized size: " << size);

  return size;
}

void
PeopleCounterHeader::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION_NOARGS ();
  start.WriteU32 (this->deviceId);
  start.WriteU32 (this->ocupationChange);

  NS_LOG_DEBUG ("Serializing the following data: ");
  NS_LOG_DEBUG ("Device: " << this->deviceId);
  NS_LOG_DEBUG ("Ocupation Change: " << this->ocupationChange);
}

uint32_t
PeopleCounterHeader::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION_NOARGS ();
  this->deviceId = start.ReadU32 ();
  this->ocupationChange = (int32_t) start.ReadU32 ();

  NS_LOG_DEBUG ("Deserialized data: ");
  NS_LOG_DEBUG ("Device: " << this->deviceId);
  NS_LOG_DEBUG ("Ocupation Change: " << this->ocupationChange);

  return sizeof(uint32_t) + sizeof(int32_t);       // the number of bytes consumed.
}

void
PeopleCounterHeader::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION_NOARGS ();

  os << "Device=" << this->deviceId << std::endl;
  os << "OcupationChange=" << this->ocupationChange << std::endl;
}

int32_t PeopleCounterHeader::GetOcupationChange ()
{
  return this->ocupationChange;
}

uint32_t PeopleCounterHeader::GetDeviceId ()
{
  return this->deviceId;
}

void PeopleCounterHeader::SetOcupationChange (int32_t change)
{
    this->ocupationChange = change;
}

void PeopleCounterHeader::SetDeviceId (uint32_t id)
{
  this->deviceId = id;
}

}
}
