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

#include "ns3/pointer.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/lora-net-device.h"
#include "ns3/people-counter-node.h"
#include <iostream>
#include <math.h>

namespace ns3 {
namespace lorawan {

NS_LOG_COMPONENT_DEFINE ("PeopleCounterNode");

NS_OBJECT_ENSURE_REGISTERED (PeopleCounterNode);

TypeId
PeopleCounterNode::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PeopleCounterNode")
                          .SetParent<Application> ()
                          .AddConstructor<PeopleCounterNode> ()
                          .SetGroupName ("lorawan");

  return tid;
}

PeopleCounterNode::PeopleCounterNode ()
  : m_interval (Seconds (1)),
  m_initialDelay (Seconds (1)),
  m_basePktSize (4)
{
  this->rng = CreateObjectWithAttributes<UniformRandomVariable> (
      "Min", DoubleValue (0), "Max", DoubleValue (1));

  NS_LOG_FUNCTION_NOARGS ();
}

PeopleCounterNode::~PeopleCounterNode ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Location PeopleCounterNode::GetLocation()
{
  return this->location;
}

void PeopleCounterNode::SetLocation(Location location)
{
  this->location = location;
}

void
PeopleCounterNode::SendPacket (void)
{
    NS_LOG_FUNCTION (this);

    if (this->rng->GetValue () < 0.05)
      {
        double value = this->rng->GetValue ();
        int change;
        
        if (value < exp ((double) -this->location.ocupation / (double) this->location.capacity))
            change = 1;
        else
            change = -1;

        std::cout << this->m_node->GetId() << " (" << Simulator::Now().GetSeconds() << ")" << ": " << value << " " << exp (-4 * (double) this->location.ocupation / (double) this->location.capacity) << " " <<  this->location.ocupation << " " << change << std::endl;
        uint8_t *buffer = static_cast<uint8_t *> (static_cast<void *> (&change));

        // Create and send a new packet
        Ptr<Packet> packet;
        packet = Create<Packet> (buffer, m_basePktSize);
        m_mac->Send (packet);

        this->location.ocupation += change;

        NS_LOG_DEBUG ("Sent a packet of size " << packet->GetSize ());
    }

    // Schedule the next SendPacket event
    m_sendEvent = Simulator::Schedule (m_interval, &PeopleCounterNode::SendPacket, this);
}

void
PeopleCounterNode::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  // Make sure we have a MAC layer
  if (m_mac == 0)
    {
      // Assumes there's only one device
      Ptr<LoraNetDevice> loraNetDevice = m_node->GetDevice (0)->GetObject<LoraNetDevice> ();

      m_mac = loraNetDevice->GetMac ();
      NS_ASSERT (m_mac != 0);
    }

  // Schedule the next SendPacket event
  Simulator::Cancel (m_sendEvent);
  NS_LOG_DEBUG ("Starting up application with a first event with a " <<
                m_initialDelay.GetSeconds () << " seconds delay");
  m_sendEvent = Simulator::Schedule (MilliSeconds(m_initialDelay.GetMilliSeconds() + (this->m_node->GetId() * 10)),
                                     &PeopleCounterNode::SendPacket, this);
  NS_LOG_DEBUG ("Event Id: " << m_sendEvent.GetUid ());
}

void
PeopleCounterNode::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Simulator::Cancel (m_sendEvent);
}

}

}
