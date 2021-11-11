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

#ifndef PEOPLE_COUNTER_H
#define PEOPLE_COUNTER_H

#include "ns3/object.h"
#include "ns3/application.h"
#include "ns3/net-device.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/packet.h"
#include "ns3/lora-device-address.h"
#include "ns3/gateway-status.h"
#include "ns3/network-status.h"
#include "ns3/network-scheduler.h"
#include "ns3/network-controller.h"
#include "ns3/node-container.h"
#include "ns3/log.h"
#include "ns3/class-a-end-device-lorawan-mac.h"
#include "ns3/network-server.h"
#include "ns3/people-counter-entry.h"
#include "ns3/location.h"
#include <map>

namespace ns3 {
namespace lorawan {

/**
 * The PeopleCounter is an application standing on top of a node equipped with
 * links that connect it with the gateways.
 *
 * This version of the PeopleCounter attempts to closely mimic an actual
 * Network Server, by providing as much functionality as possible.
 */
class PeopleCounter : public NetworkServer
{
public:
    static TypeId GetTypeId (void);

    PeopleCounter ();
    virtual ~PeopleCounter ();

    /**
   * Start the NS application.
   */
  void StartApplication (void);

  /**
   * Stop the NS application.
   */
  void StopApplication (void);

    void AddNodes (NodeContainer nodes);

    /**
     * Inform the PeopleCounter that this node is connected to the network.
     * This method will create a DeviceStatus object for the new node (if it
     * doesn't already exist).
     */
    void AddNode (Ptr<Node> node);
    
    /**
     * Add this gateway to the list of gateways connected to this NS.
     * Each GW is identified by its Address in the NS-GWs network.
     */
    void AddGateway (Ptr<Node> gateway, Ptr<NetDevice> netDevice);

    /**
     * Receive a packet from a gateway.
     * \param packet the received packet
     */
    bool Receive (Ptr<NetDevice> device, Ptr<const Packet> packet, uint16_t protocol,
                const Address &address);

    void LogToFile ();

  public:
    std::map<LoraDeviceAddress, Ptr<PeopleCounterEntry>> nodes;
    std::map<int, Location> locations = {
      {0, Location(0, "Coliseo", 300, 340, 160, 0)},
      {1, Location(1, "Plaza", 50, 396, 325, 0)},
      {2, Location(2, "Biblioteca", 100, 365, 425, 0)},
      {3, Location(3, "Café", 40, 355, 240, 0)},
      {4, Location(4, "Bambú", 25, 230, 230, 0)},
      {5, Location(5, "BambúII", 30, 145, 125, 0)},
      {6, Location(6, "BloqueG", 20, 415, 230, 0)},
      {7, Location(7, "5K", 45, 250, 160, 0)},
      {8, Location(8, "Gimnasio", 15, 325, 185, 0)},
      {9, Location(9, "MesasE", 15, 362, 350, 0)},
      {10, Location(10, "MesasA", 15, 295, 370, 0)},
      {11, Location(11, "MesasB", 15, 300, 335, 0)},
      {12, Location(12, "MediaTorta", 15, 367, 198, 0)},
      {13, Location(13, "Express", 20, 335, 280, 0)},
      {14, Location(14, "Terrasse", 10, 317, 185, 0)}
    };

  private:
    EventId m_logEvent;
};

} // namespace lorawan

} // namespace ns3
#endif /* NETWORK_SERVER_H */
