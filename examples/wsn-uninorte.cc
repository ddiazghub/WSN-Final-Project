/*
 * This script simulates a simple network in which one end device sends one
 * packet to the gateway.
 */

/*
 * This script simulates a complex scenario with multiple gateways and end
 * devices. The metric of interest for this script is the throughput of the
 * network.
 */

#include "ns3/end-device-lora-phy.h"
#include "ns3/gateway-lora-phy.h"
#include "ns3/class-a-end-device-lorawan-mac.h"
#include "ns3/gateway-lorawan-mac.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/lora-helper.h"
#include "ns3/node-container.h"
#include "ns3/mobility-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/double.h"
#include "ns3/random-variable-stream.h"
#include "ns3/periodic-sender-helper.h"
#include "ns3/command-line.h"
#include "ns3/people-counter-helper.h"
#include "ns3/correlated-shadowing-propagation-loss-model.h"
#include "ns3/building-penetration-loss.h"
#include "ns3/building-allocator.h"
#include "ns3/buildings-helper.h"
#include "ns3/forwarder-helper.h"
#include "ns3/netanim-module.h"
#include "network-settings.h"
#include <algorithm>
#include <ctime>

using namespace ns3;
using namespace lorawan;

NS_LOG_COMPONENT_DEFINE ("ComplexLorawanNetworkExample");

#define NODE_ICON "/home/david/ns-3.35/src/wsn/res/node.png"
#define GW_ICON "/home/david/ns-3.35/src/wsn/res/gw.png"
#define MAP "/home/david/ns-3.35/src/wsn/res/map.png"

int appPeriodSeconds = 600;

// Output control
bool print = true;

int main(int argc, char *argv[])
{
	// Set up logging
	LogComponentEnable ("ComplexLorawanNetworkExample", LOG_LEVEL_ALL);
	LogComponentEnable("LoraChannel", LOG_LEVEL_INFO);
	// LogComponentEnable("LoraPhy", LOG_LEVEL_ALL);
	// LogComponentEnable("EndDeviceLoraPhy", LOG_LEVEL_ALL);
	// LogComponentEnable("GatewayLoraPhy", LOG_LEVEL_ALL);
	// LogComponentEnable("LoraInterferenceHelper", LOG_LEVEL_ALL);
	// LogComponentEnable("LorawanMac", LOG_LEVEL_ALL);
	// LogComponentEnable("EndDeviceLorawanMac", LOG_LEVEL_ALL);
	// LogComponentEnable("ClassAEndDeviceLorawanMac", LOG_LEVEL_ALL);
	// LogComponentEnable("GatewayLorawanMac", LOG_LEVEL_ALL);
	// LogComponentEnable("LogicalLoraChannelHelper", LOG_LEVEL_ALL);
	// LogComponentEnable("LogicalLoraChannel", LOG_LEVEL_ALL);
	// LogComponentEnable("LoraHelper", LOG_LEVEL_ALL);
	// LogComponentEnable("LoraPhyHelper", LOG_LEVEL_ALL);
	// LogComponentEnable("LorawanMacHelper", LOG_LEVEL_ALL);
	// LogComponentEnable("PeriodicSenderHelper", LOG_LEVEL_ALL);
	// LogComponentEnable("PeriodicSender", LOG_LEVEL_ALL);
	// LogComponentEnable("LorawanMacHeader", LOG_LEVEL_ALL);
	// LogComponentEnable("LoraFrameHeader", LOG_LEVEL_ALL);
	// LogComponentEnable("NetworkScheduler", LOG_LEVEL_ALL);
	// LogComponentEnable("NetworkServer", LOG_LEVEL_ALL);
	// LogComponentEnable("NetworkStatus", LOG_LEVEL_ALL);
	// LogComponentEnable("NetworkController", LOG_LEVEL_ALL);

	/***********
	 *  Setup  *
	 ***********/

	// Create the time value from the period
	Time appPeriod = Seconds (appPeriodSeconds);

	// Mobility
	MobilityHelper mobility;
	Ptr<ListPositionAllocator> nodePositions = CreateObject<ListPositionAllocator> ();

	for (int i = 0; i < NUMBER_OF_NODES; i++)
		nodePositions->Add (NODE_POSITIONS[i]);

	mobility.SetPositionAllocator (nodePositions);
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

	/************************
	 *  Create the channel  *
	 ************************/

	// Create the lora channel object
	Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel> ();
	loss->SetPathLossExponent (3.76);
	loss->SetReference (1, 7.7);

	// Create the correlated shadowing component
	Ptr<CorrelatedShadowingPropagationLossModel> shadowing =
		CreateObject<CorrelatedShadowingPropagationLossModel> ();

	// Aggregate shadowing to the logdistance loss
	loss->SetNext (shadowing);

	// Add the effect to the channel propagation loss
	Ptr<BuildingPenetrationLoss> buildingLoss = CreateObject<BuildingPenetrationLoss> ();

	shadowing->SetNext (buildingLoss);

	Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();

	Ptr<LoraChannel> channel = CreateObject<LoraChannel> (loss, delay);

	/************************
	 *  Create the helpers  *
	 ************************/

	// Create the LoraPhyHelper
	LoraPhyHelper phyHelper = LoraPhyHelper ();
	phyHelper.SetChannel (channel);

	// Create the LorawanMacHelper
	LorawanMacHelper macHelper = LorawanMacHelper ();

	// Create the LoraHelper
	LoraHelper helper = LoraHelper ();
	helper.EnablePacketTracking (); // Output filename
	// helper.EnableSimulationTimePrinting ();

	//Create the NetworkServerHelper
	PeopleCounterHelper peopleCounterHelper = PeopleCounterHelper ();

	//Create the ForwarderHelper
	ForwarderHelper forHelper = ForwarderHelper ();

	/************************
	 *  Create End Devices  *
	 ************************/

	// Create a set of nodes
	NodeContainer loraNodes;
	loraNodes.Create (NUMBER_OF_NODES);

	// Assign a mobility model to each node
	mobility.Install (loraNodes);

	// Make it so that nodes are at a certain height > 0
	for (NodeContainer::Iterator j = loraNodes.Begin (); j != loraNodes.End (); ++j)
	{
		Ptr<MobilityModel> mobility = (*j)->GetObject<MobilityModel> ();
		Vector position = mobility->GetPosition ();
		position.z = 1.2;
		mobility->SetPosition (position);
	}

	// Create the LoraNetDevices of the end devices
	uint8_t nwkId = 54;
	uint32_t nwkAddr = 1864;
	Ptr<LoraDeviceAddressGenerator> addrGen = CreateObject<LoraDeviceAddressGenerator> (nwkId, nwkAddr);

	// Create the LoraNetDevices of the end devices
	macHelper.SetAddressGenerator (addrGen);
	phyHelper.SetDeviceType (LoraPhyHelper::ED);
	macHelper.SetDeviceType (LorawanMacHelper::ED_A);
	helper.Install (phyHelper, macHelper, loraNodes);

	// Now end devices are connected to the channel

	// Connect trace sources
	for (NodeContainer::Iterator j = loraNodes.Begin (); j != loraNodes.End (); ++j)
	{
		Ptr<Node> node = *j;
		Ptr<LoraNetDevice> loraNetDevice = node->GetDevice (0)->GetObject<LoraNetDevice> ();
		Ptr<LoraPhy> phy = loraNetDevice->GetPhy ();
	}

	/*********************
	 *  Create Gateways  *
	 *********************/

	// Create the gateway nodes (allocate them uniformely on the disc)
	NodeContainer gateways;
	gateways.Create (NUMBER_OF_GATEWAYS);

	MobilityHelper mobilityGW;
	Ptr<ListPositionAllocator> gwPosition = CreateObject<ListPositionAllocator> ();
	
	for (int i = 0; i < NUMBER_OF_GATEWAYS; i++)
		gwPosition->Add (GATEWAY_POSITIONS[i]);

	mobilityGW.SetPositionAllocator (gwPosition);
	mobilityGW.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobilityGW.Install (gateways);

	// Create a netdevice for each gateway
	phyHelper.SetDeviceType (LoraPhyHelper::GW);
	macHelper.SetDeviceType (LorawanMacHelper::GW);
	helper.Install (phyHelper, macHelper, gateways);

	/**********************
	 *  Handle buildings  *
	 **********************/

	double xLength = 130;
	double deltaX = 1;
	double yLength = 130;
	double deltaY = 1;
    int gridWidth = 4;
    int gridHeight = 4;

	Ptr<GridBuildingAllocator> gridBuildingAllocator;
	gridBuildingAllocator = CreateObject<GridBuildingAllocator> ();
	gridBuildingAllocator->SetAttribute ("GridWidth", UintegerValue (gridWidth));
	gridBuildingAllocator->SetAttribute ("LengthX", DoubleValue (xLength));
	gridBuildingAllocator->SetAttribute ("LengthY", DoubleValue (yLength));
	gridBuildingAllocator->SetAttribute ("DeltaX", DoubleValue (deltaX));
	gridBuildingAllocator->SetAttribute ("DeltaY", DoubleValue (deltaY));
	gridBuildingAllocator->SetAttribute ("Height", DoubleValue (10));
	gridBuildingAllocator->SetBuildingAttribute ("NRoomsX", UintegerValue (2));
	gridBuildingAllocator->SetBuildingAttribute ("NRoomsY", UintegerValue (2));
	gridBuildingAllocator->SetBuildingAttribute ("NFloors", UintegerValue (1));
	gridBuildingAllocator->SetAttribute (
		"MinX", DoubleValue (1));
	gridBuildingAllocator->SetAttribute (
		"MinY", DoubleValue (1));
	BuildingContainer bContainer = gridBuildingAllocator->Create (gridHeight * gridWidth);

	BuildingsHelper::Install (loraNodes);
	BuildingsHelper::Install (gateways);

	// Print the buildings
	if (print)
	{
		std::ofstream myfile;
		myfile.open ("buildings.txt");
		std::vector<Ptr<Building>>::const_iterator it;
		int j = 1;
		for (it = bContainer.Begin (); it != bContainer.End (); ++it, ++j)
		{
			Box boundaries = (*it)->GetBoundaries ();
			myfile << "set object " << j << " rect from " << boundaries.xMin << "," << boundaries.yMin
					<< " to " << boundaries.xMax << "," << boundaries.yMax << std::endl;
		}
		myfile.close ();
	}

	NS_LOG_DEBUG ("Completed configuration");

	/*********************************************
	 *  Install applications on the end devices  *
	 *********************************************/

	Time appStopTime = Seconds (SIMULATION_TIME);
	PeriodicSenderHelper appHelper = PeriodicSenderHelper ();
	appHelper.SetPeriod (Seconds (appPeriodSeconds));
	appHelper.SetPacketSize (23);
	Ptr<RandomVariableStream> rv = CreateObjectWithAttributes<UniformRandomVariable> (
		"Min", DoubleValue (0), "Max", DoubleValue (10));
	ApplicationContainer appContainer = appHelper.Install (loraNodes);

	appContainer.Start (Seconds (0));
	appContainer.Stop (appStopTime);

	/**************************
	 *  Create Network Server  *
	 ***************************/

	// Create the NS node
	NodeContainer server;
	server.Create (NUMBER_OF_SERVERS);

	// Create a NS for the network
	std::string adrType = "ns3::AdrComponent";

	peopleCounterHelper.SetEndDevices (loraNodes);
	peopleCounterHelper.SetGateways (gateways);
	peopleCounterHelper.Install (server);
	peopleCounterHelper.EnableAdr (true);
  	peopleCounterHelper.SetAdr (adrType);

	//Create a forwarder for each gateway
	forHelper.Install (gateways);

	////////////////
	// Simulation //
	////////////////

	Simulator::Stop (appStopTime + Hours (1));

	AnimationInterface animation("animation.xml");
	animation.SetBackgroundImage (MAP, 0, 0, 1, 1, 1);
	uint32_t nodeIconId = animation.AddResource (NODE_ICON);
    uint32_t gwIconId = animation.AddResource (GW_ICON);

    for (int i = 0; i < NUMBER_OF_NODES; i++) {
		animation.SetConstantPosition (loraNodes.Get (i), NODE_POSITIONS[i].x, NODE_POSITIONS[i].y);
		animation.UpdateNodeImage (i, nodeIconId);
	}

	for (int i = 0; i < NUMBER_OF_GATEWAYS; i++) {
		animation.SetConstantPosition (gateways.Get(i), GATEWAY_POSITIONS[i].x, GATEWAY_POSITIONS[i].y);
		animation.UpdateNodeImage(i, gwIconId);
	}

	NS_LOG_INFO ("Running simulation...");
	Simulator::Run ();

	Simulator::Destroy ();

	///////////////////////////
	// Print results to file //
	///////////////////////////
	NS_LOG_INFO ("Computing performance metrics...");

	LoraPacketTracker &tracker = helper.GetPacketTracker ();
	std::cout << tracker.CountMacPacketsGlobally (Seconds (0), appStopTime + Hours (1)) << std::endl;

	return 0;
}