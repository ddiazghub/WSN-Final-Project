#ifndef NETWORK_SETTINGS_H
#define NETWORK_SETTINGS_H

#include "ns3/vector.h"
#include "ns3/location.h"
#include "ns3/end-device-lorawan-mac.h"
#include <vector>

#define NUMBER_OF_NODES 15
#define NUMBER_OF_GATEWAYS 2
#define NUMBER_OF_SERVERS 1
#define SIMULATION_TIME 3600

namespace ns3 {

std::vector<Location> NODE_POSITIONS = {
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

Vector GATEWAY_POSITIONS[NUMBER_OF_GATEWAYS] = {
    Vector (260, 150, 0),
    Vector (320, 370, 0)
};

}

#endif