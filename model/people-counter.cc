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

#include "ns3/network-server.h"
#include "ns3/net-device.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/packet.h"
#include "ns3/lorawan-mac-header.h"
#include "ns3/lora-frame-header.h"
#include "ns3/lora-device-address.h"
#include "ns3/network-status.h"
#include "ns3/lora-frame-header.h"
#include "ns3/node-container.h"
#include "ns3/class-a-end-device-lorawan-mac.h"
#include "ns3/people-counter-node.h"
#include "ns3/mac-command.h"
#include "ns3/people-counter.h"
#include "ns3/people-counter-entry.h"
#include "ns3/people-counter-header.h"
#include "ns3/location.h"
#include <iostream>

namespace ns3 {
namespace lorawan {

NS_LOG_COMPONENT_DEFINE ("PeopleCounter");

NS_OBJECT_ENSURE_REGISTERED (PeopleCounter);

TypeId
PeopleCounter::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PeopleCounter")
    .SetParent<NetworkServer> ()
    .AddConstructor<PeopleCounter> ()
    .SetGroupName ("lorawan");
  return tid;
}

PeopleCounter::PeopleCounter () : NetworkServer() {}

PeopleCounter::~PeopleCounter () {}

void PeopleCounter::StartApplication (void)
{
  for (auto it = this->locations.cbegin ();  it != this->locations.cend (); it++)
  {
    char buffer[25];
    sprintf (buffer, "log/%s.txt", it->second.name.c_str());
    std::ofstream file (buffer);
    file.close ();
  }
  
  NetworkServer::StartApplication ();
  this->m_logEvent = Simulator::Schedule (Minutes(1), &PeopleCounter::LogToFile, this);
}

void PeopleCounter::LogToFile ()
{
  for (auto it = this->locations.cbegin ();  it != this->locations.cend (); it++)
  {
    char buffer[40];
    sprintf (buffer, "log/siteOcupations/%s.txt", it->second.name.c_str());
    std::fstream file (buffer, std::ios::app);
    file << Simulator::Now () << " " << it->second.ocupation << std::endl;
    file.close ();
  }

  this->m_logEvent = Simulator::Schedule (Minutes(1), &PeopleCounter::LogToFile, this);
}
/**
   * Stop the NS application.
   */
void PeopleCounter::StopApplication (void)
{
  NetworkServer::StopApplication ();
  Simulator::Cancel (this->m_logEvent);
}

void PeopleCounter::AddGateway (Ptr<Node> gateway, Ptr<NetDevice> netDevice)
{
  NS_LOG_FUNCTION (this << gateway);

  // Get the PointToPointNetDevice
  Ptr<PointToPointNetDevice> p2pNetDevice;
  for (uint32_t i = 0; i < gateway->GetNDevices (); i++)
    {
      p2pNetDevice = gateway->GetDevice (i)->GetObject<PointToPointNetDevice> ();
      if (p2pNetDevice != 0)
        {
          // We found a p2pNetDevice on the gateway
          break;
        }
    }

  // Get the gateway's LoRa MAC layer (assumes gateway's MAC is configured as first device)
  Ptr<GatewayLorawanMac> gwMac = gateway->GetDevice (0)->GetObject<LoraNetDevice> ()->
    GetMac ()->GetObject<GatewayLorawanMac> ();
  NS_ASSERT (gwMac != 0);

  // Get the Address
  Address gatewayAddress = p2pNetDevice->GetAddress ();

  // Create new gatewayStatus
  Ptr<GatewayStatus> gwStatus = Create<GatewayStatus> (gatewayAddress,
                                                       netDevice,
                                                       gwMac);

  m_status->AddGateway (gatewayAddress, gwStatus);
}

void
PeopleCounter::AddNodes (NodeContainer nodes)
{
  NS_LOG_FUNCTION_NOARGS ();

  // For each node in the container, call the function to add that single node
  NodeContainer::Iterator it;
  for (it = nodes.Begin (); it != nodes.End (); it++)
    {
      AddNode (*it);
    }
}

void
PeopleCounter::AddNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);

  // Get the LoraNetDevice
  Ptr<LoraNetDevice> loraNetDevice;
  for (uint32_t i = 0; i < node->GetNDevices (); i++)
    {
      loraNetDevice = node->GetDevice (i)->GetObject<LoraNetDevice> ();
      if (loraNetDevice != 0)
        {
          // We found a LoraNetDevice on the node
          break;
        }
    }

  // Get the MAC
  Ptr<ClassAEndDeviceLorawanMac> edLorawanMac =
    loraNetDevice->GetMac ()->GetObject<ClassAEndDeviceLorawanMac> ();

  // Update the NetworkStatus about the existence of this node
  m_status->AddNode (edLorawanMac);

  int nodeNumber = m_status->CountEndDevices ();
  
  this->nodes.insert (std::pair<LoraDeviceAddress, Ptr<PeopleCounterEntry>> (
      edLorawanMac->GetDeviceAddress (), Create<PeopleCounterEntry> (nodeNumber, nodeNumber)));
}

bool
PeopleCounter::Receive (Ptr<NetDevice> device, Ptr<const Packet> packet,
                        uint16_t protocol, const Address& address)
{
  NS_LOG_FUNCTION (this << packet << protocol << address);


  // Fire the trace source
  m_receivedPacket (packet);

  // Inform the scheduler of the newly arrived packet
  m_scheduler->OnReceivedPacket (packet);

  // Inform the status of the newly arrived packet
  m_status->OnReceivedPacket (packet, address);

  // Inform the controller of the newly arrived packet
  m_controller->OnNewPacket (packet);

  /*
  PeopleCounterHeader header;
  Ptr<Packet> myPacket = packet->CreateFragment (30, 8);
  myPacket->PeekHeader (header);
  change = header.GetOcupationChange ();
  nodeId = header.GetDeviceId ();
  */

  for (auto it = this->m_status->m_endDeviceStatuses.cbegin ();  it != this->m_status->m_endDeviceStatuses.cend (); it++)
      this->locations[it->second->GetMac ()->GetDevice ()->GetNode ()->GetId ()].ocupation = Ptr<PeopleCounterNode> (dynamic_cast<PeopleCounterNode *> (PeekPointer (it->second->GetMac ()->GetDevice ()->GetNode ()->GetApplication (0))))->GetLocation().ocupation;

  return true;
}

}
}
