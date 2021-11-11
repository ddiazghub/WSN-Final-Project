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

#ifndef PEOPLE_COUNTER_NODE_HELPER_H
#define PEOPLE_COUNTER_NODE_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/address.h"
#include "ns3/attribute.h"
#include "ns3/net-device.h"
#include "ns3/node-container.h"
#include "ns3/application-container.h"
#include "ns3/location.h"
#include <vector>
#include <stdint.h>
#include <string>

namespace ns3 {
namespace lorawan {

/**
 * This class can be used to install PeriodicSender applications on a wide
 * range of nodes.
 */
class PeopleCounterNodeHelper
{
public:
  PeopleCounterNodeHelper ();

  ~PeopleCounterNodeHelper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c) const;

  ApplicationContainer Install (Ptr<Node> node) const;

private:
  Ptr<Application> InstallPriv (Ptr<Node> node) const;

  ObjectFactory m_factory;

    std::vector<Location> locations = {
        Location(0, "Coliseo", 300, 340, 160, 0),
        Location(1, "Plaza", 50, 396, 325, 0),
        Location(2, "Biblioteca", 100, 365, 425, 0),
        Location(3, "Café", 40, 300, 240, 0),
        Location(4, "Bambú", 25, 230, 230, 0),
        Location(5, "BambúII", 30, 145, 125, 0),
        Location(6, "BloqueG", 20, 415, 230, 0),
        Location(7, "5K", 45, 250, 160, 0),
        Location(8, "Gimnasio", 15, 325, 185, 0),
        Location(9, "MesasE", 15, 362, 350, 0),
        Location(10, "MesasA", 15, 295, 370, 0),
        Location(11, "MesasB", 15, 300, 335, 0),
        Location(12, "MediaTorta", 15, 367, 198, 0),
        Location(13, "Express", 20, 335, 280, 0),
        Location(14, "Terrasse", 10, 317, 185, 0)
    };

};

} // namespace ns3

}
#endif /* PERIODIC_SENDER_HELPER_H */
