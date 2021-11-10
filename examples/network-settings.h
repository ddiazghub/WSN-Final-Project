#ifndef NETWORK_SETTINGS_H
#define NETWORK_SETTINGS_H

#include "ns3/vector.h"

#define NUMBER_OF_NODES 15
#define NUMBER_OF_GATEWAYS 2
#define NUMBER_OF_SERVERS 1
#define SIMULATION_TIME 600

namespace ns3 {

Vector NODE_POSITIONS[NUMBER_OF_NODES] = {
    Vector(340, 160, 0),
    Vector(396, 325, 0),
    Vector(365, 425, 0),
    Vector(355, 240, 0),
    Vector(230, 230, 0),
    Vector(145, 125, 0),
    Vector(415, 230, 0),
    Vector(250, 160, 0),
    Vector(325, 185, 0),
    Vector(362, 350, 0),
    Vector(295, 370, 0),
    Vector(300, 335, 0),
    Vector(367, 198, 0),
    Vector(335, 280, 0),
    Vector(317, 185, 0)
};

Vector GATEWAY_POSITIONS[NUMBER_OF_GATEWAYS] = {
    Vector (260, 150, 0),
    Vector (320, 370, 0)
};

}

#endif