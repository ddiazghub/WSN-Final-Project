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

#ifndef PEOPLE_COUNTER_NODE_H
#define PEOPLE_COUNTER_NODE_H

#include "ns3/application.h"
#include "ns3/nstime.h"
#include "ns3/lorawan-mac.h"
#include "ns3/attribute.h"
#include "ns3/location.h"

namespace ns3 {
namespace lorawan {

class PeopleCounterNode : public Application
{
public:
	static TypeId GetTypeId (void);

	PeopleCounterNode ();
	~PeopleCounterNode ();

	/**
	 * Send a packet using the LoraNetDevice's Send method
	 */
	void SendPacket (void);

	/**
	 * Start the application by scheduling the first SendPacket event
	 */
	void StartApplication (void);

	/**
	 * Stop the application
	 */
	void StopApplication (void);

	Location GetLocation ();

	void SetLocation (Location location);


private:
	/**
	 * The interval between to consecutive send events
	 */
	Time m_interval;

	/**
	 * The initial delay of this application
	 */
	Time m_initialDelay;

	/**
	 * The sending event scheduled as next
	 */
	EventId m_sendEvent;

	/**
	 * The MAC layer of this node
	 */
	Ptr<LorawanMac> m_mac;

	/**
	 * The packet size.
	 */
	uint8_t m_basePktSize;

    Location location;

    Ptr<UniformRandomVariable> rng;
};

} //namespace ns3

}
#endif /* SENDER_APPLICATION */
