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

#ifndef PEOPLE_COUNTER_HEADER_H
#define PEOPLE_COUNTER_HEADER_H

#include "ns3/header.h"

namespace ns3 {
namespace lorawan {

/**
 * This class represents the Frame header (FHDR) used in a LoraWAN network.
 *
 * Although the specification divides the FHDR from the FPort field, this
 * implementation considers them as a unique entity (i.e., FPort is treated as
 * if it were a part of FHDR).
 *
 * \remark Prior to using it, this class needs to be informed of whether the
 * header is for an uplink or downlink message. This is necessary due to the
 * fact that UL and DL messages have subtly different structure and, hence,
 * serialization and deserialization schemes.
 */
class PeopleCounterHeader : public Header
{
public:
  PeopleCounterHeader ();
  ~PeopleCounterHeader ();

  // Methods inherited from Header
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Return the size required for serialization of this header
   *
   * \return The serialized size in bytes
   */
  virtual uint32_t GetSerializedSize (void) const;

  /**
   * Serialize the header.
   *
   * See Page 15 of LoraWAN specification for a representation of fields.
   *
   * \param start A pointer to the buffer that will be filled with the
   * serialization.
   */
  virtual void Serialize (Buffer::Iterator start) const;

  /**
   * Deserialize the contents of the buffer into a PeopleCounterHeader object.
   *
   * \param start A pointer to the buffer we need to deserialize.
   * \return The number of consumed bytes.
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

  /**
   * Print the header in a human-readable format.
   *
   * \param os The std::ostream on which to print the header.
   */
  virtual void Print (std::ostream &os) const;

    int32_t GetOcupationChange ();
    uint32_t GetDeviceId ();

    void SetOcupationChange (int32_t change);
    void SetDeviceId (uint32_t id);

  private:
    int32_t ocupationChange;
    uint32_t deviceId;
};
}
}
#endif