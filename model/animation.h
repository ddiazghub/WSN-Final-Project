#ifndef ANIMATION_H
#define ANIMATION_H

#include "ns3/netanim-module.h"

using namespace ns3;

namespace wsn {

#define NODE_ICON "/home/david/ns-3.35/src/wsn/res/node.png"
#define GW_ICON "/home/david/ns-3.35/src/wsn/res/gw.png"
#define MAP "/home/david/ns-3.35/src/wsn/res/map.png"
#define NUMBER_OF_NODES 15

typedef struct {
  double x, y;
} Vector2;

class NetworkAnimation : public AnimationInterface {
    public:
        Vector2 nodePositions[NUMBER_OF_NODES];
        NetworkAnimation(const std::string filename);
};
} // namespace wsn

#endif