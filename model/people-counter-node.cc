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
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/lora-net-device.h"
#include "ns3/people-counter-node.h"
#include "ns3/people-counter-header.h"
#include "ns3/end-device-lorawan-mac.h"
#include "ns3/class-a-end-device-lorawan-mac.h"
#include "ns3/end-device-lora-phy.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include <iostream>
#include <fstream>
#include <string>
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
  m_basePktSize (2 * sizeof(int))
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
    if (this->rng->GetValue () < 0.05 * CAPACITY_MULTIPLIER)
      {
        double value = this->rng->GetValue ();
        int change;
        int id = this->m_node->GetId ();

        if (value < exp ((double) -this->location.ocupation / (double) this->location.capacity))
        {
          change = 1;
          this->entered += 1;
        }
        else
        {
          this->exited += 1;
          change = -1;
        }
    
        // Create and send a new packet
        Ptr<Packet> packet;
        packet = Create<Packet> (0);
        m_mac->Send (packet);
        this->location.ocupation += change;
        this->ocupationChange += change;

        NS_LOG_DEBUG ("Sent a packet of size " << packet->GetSize () << " with data: Node= " << id
                                               << " Change= " << change);
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

  std::string str = std::to_string (CAPACITY_MULTIPLIER);
  str.erase (str.find_last_not_of ('0') + 1, std::string::npos);
  std::ofstream logFile (std::string("log/ocupationChange/").append(str).append(".txt"));
  logFile << "NodeID Entered Exited" << std::endl;
  logFile.close ();
}

void
PeopleCounterNode::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  std::string str = std::to_string (CAPACITY_MULTIPLIER);
  str.erase (str.find_last_not_of ('0') + 1, std::string::npos);
  std::fstream logFile (std::string("log/ocupationChange/").append(str).append(".txt"), std::ios::app);
  logFile << this->m_node->GetId () << " " << this->entered << " " << this->exited << std::endl;
  logFile.close ();
  Simulator::Cancel (m_sendEvent);
}

void PeopleCounterNode::SetOcupationChange (int32_t change)
{
  this->ocupationChange = change;
}

int32_t PeopleCounterNode::GetOcupationChange ()
{
  return this->ocupationChange;
}

/*
void
EndDeviceLorawanMac::DoSend (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this);
  // Checking if this is the transmission of a new packet
  if (packet != m_retxParams.packet)
    {
      NS_LOG_DEBUG ("Received a new packet from application. Resetting retransmission parameters.");
      m_currentFCnt++;
      NS_LOG_DEBUG ("APP packet: " << packet << ".");

      // Add the Lora Frame Header to the packet
      LoraFrameHeader frameHdr;
      ApplyNecessaryOptions (frameHdr);
      packet->AddHeader (frameHdr);

      NS_LOG_INFO ("Added frame header of size " << frameHdr.GetSerializedSize () <<
                   " bytes.");

      // Check that MACPayload length is below the allowed maximum
      if (packet->GetSize () > m_maxAppPayloadForDataRate.at (m_dataRate))
        {
          NS_LOG_WARN ("Attempting to send a packet larger than the maximum allowed"
                       << " size at this DataRate (DR" << unsigned(m_dataRate) <<
                       "). Transmission canceled.");
          return;
        }


      // Add the Lora Mac header to the packet
      LorawanMacHeader macHdr;
      ApplyNecessaryOptions (macHdr);
      packet->AddHeader (macHdr);

      // Reset MAC command list
      m_macCommandList.clear ();

      if (m_retxParams.waitingAck)
        {
          // Call the callback to notify about the failure
          uint8_t txs = m_maxNumbTx - (m_retxParams.retxLeft);
          m_requiredTxCallback (txs, false, m_retxParams.firstAttempt, m_retxParams.packet);
          NS_LOG_DEBUG (" Received new packet from the application layer: stopping retransmission procedure. Used " <<
                        unsigned(txs) << " transmissions out of a maximum of " << unsigned(m_maxNumbTx) << ".");
        }

      // Reset retransmission parameters
      resetRetransmissionParameters ();

        PeopleCounterNode *app = dynamic_cast<PeopleCounterNode *> (PeekPointer (this->m_device->GetNode ()->GetApplication (0)));
      if (app != nullptr)
      {
        Ptr<PeopleCounterNode> peopleCounter = Ptr<PeopleCounterNode> (app);
        PeopleCounterHeader header;
        header.SetDeviceId (peopleCounter->GetNode ()->GetId());
        header.SetOcupationChange (peopleCounter->GetOcupationChange());
        peopleCounter->SetOcupationChange (0);
        packet->AddHeader (header);
      }

      // If this is the first transmission of a confirmed packet, save parameters for the (possible) next retransmissions.
      if (m_mType == LorawanMacHeader::CONFIRMED_DATA_UP)
        {
          m_retxParams.packet = packet->Copy ();
          m_retxParams.retxLeft = m_maxNumbTx;
          m_retxParams.waitingAck = true;
          m_retxParams.firstAttempt = Simulator::Now ();
          m_retxParams.retxLeft = m_retxParams.retxLeft - 1;       // decreasing the number of retransmissions

          NS_LOG_DEBUG ("Message type is " << m_mType);
          NS_LOG_DEBUG ("It is a confirmed packet. Setting retransmission parameters and decreasing the number of transmissions left.");

          NS_LOG_INFO ("Added MAC header of size " << macHdr.GetSerializedSize () <<
                       " bytes.");

          // Sent a new packet
          NS_LOG_DEBUG ("Copied packet: " << m_retxParams.packet);
          m_sentNewPacket (m_retxParams.packet);

          // static_cast<ClassAEndDeviceLorawanMac*>(this)->SendToPhy (m_retxParams.packet);
          SendToPhy (m_retxParams.packet);
        }
      else
        {
          m_sentNewPacket (packet);
          // static_cast<ClassAEndDeviceLorawanMac*>(this)->SendToPhy (packet);
          SendToPhy (packet);
        }
    }
  // this is a retransmission
  else
    {
      if (m_retxParams.waitingAck)
        {

          // Remove the headers
          LorawanMacHeader macHdr;
          LoraFrameHeader frameHdr;
          packet->RemoveHeader(macHdr);
          packet->RemoveHeader(frameHdr);

          // Add the Lora Frame Header to the packet
          frameHdr = LoraFrameHeader ();
          ApplyNecessaryOptions (frameHdr);
          packet->AddHeader (frameHdr);

          NS_LOG_INFO ("Added frame header of size " << frameHdr.GetSerializedSize () <<
                       " bytes.");

          // Add the Lorawan Mac header to the packet
          macHdr = LorawanMacHeader ();
          ApplyNecessaryOptions (macHdr);
          packet->AddHeader (macHdr);
          m_retxParams.retxLeft = m_retxParams.retxLeft - 1;           // decreasing the number of retransmissions
          NS_LOG_DEBUG ("Retransmitting an old packet.");

          // static_cast<ClassAEndDeviceLorawanMac*>(this)->SendToPhy (m_retxParams.packet);
          SendToPhy (m_retxParams.packet);
        }
    }

}*/

}

}
