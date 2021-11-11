#ifndef NETWORK_SETTINGS_H
#define NETWORK_SETTINGS_H

#include "ns3/vector.h"
#include "ns3/location.h"

#define NUMBER_OF_NODES 15
#define NUMBER_OF_GATEWAYS 2
#define NUMBER_OF_SERVERS 1
#define SIMULATION_TIME 3600

namespace ns3 {

Location NODE_POSITIONS[NUMBER_OF_NODES] = {
    Location(0, "Coliseo", 40, 340, 160, 0),
    Location(1, "Plaza", 40, 396, 325, 0),
    Location(2, "Biblioteca", 40, 365, 425, 0),
    Location(3, "Café", 40, 355, 240, 0),
    Location(4, "Bambú", 40, 230, 230, 0),
    Location(5, "BambúII", 40, 145, 125, 0),
    Location(6, "BloqueG", 40, 415, 230, 0),
    Location(7, "5K", 40, 250, 160, 0),
    Location(8, "Gimnasio", 40, 325, 185, 0),
    Location(9, "MesasE", 40, 362, 350, 0),
    Location(10, "MesasA", 40, 295, 370, 0),
    Location(11, "MesasB", 40, 300, 335, 0),
    Location(12, "MediaTorta", 40, 367, 198, 0),
    Location(13, "Express", 40, 335, 280, 0),
    Location(14, "Terrasse", 40, 317, 185, 0)
};

Vector GATEWAY_POSITIONS[NUMBER_OF_GATEWAYS] = {
    Vector (260, 150, 0),
    Vector (320, 370, 0)
};

}

#endif