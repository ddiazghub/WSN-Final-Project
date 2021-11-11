/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 University of Padova
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
 * Authors: Davide Magrin <magrinda@dei.unipd.it>
 *          Martina Capuzzo <capuzzom@dei.unipd.it>
 */

#ifndef PEOPLE_COUNTER_ENTRY_H
#define PEOPLE_COUNTER_ENTRY_H

#include "ns3/object.h"
#include "ns3/lorawan-module.h"

namespace ns3 {
namespace lorawan {

class PeopleCounterEntry : public Object
{
public:
    static TypeId GetTypeId (void);
    PeopleCounterEntry();
    PeopleCounterEntry(uint32_t nodeNumber, uint32_t locationId);
    virtual ~PeopleCounterEntry();
    uint32_t GetNodeNumber ();
    uint32_t GetLocationId ();

  private:
    uint32_t nodeNumber;
    uint32_t locationId;
}; // namespace lorawan

}
} // namespace ns3
#endif /* PEOPLE_COUNTER_ENTRY_H */
