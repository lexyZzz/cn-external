
#include "ns3/point-to-point-layout-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"

using namespace ns3;

int main()
{
    // Configure point-to-point links
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("2ms"));

    // Create star topology with 5 end devices
    PointToPointStarHelper star(5, p2p);

    // Install Internet stack
    star.InstallStack(InternetStackHelper());

    // Assign IP addresses
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.1.1.0", "255.255.255.0");
    star.AssignIpv4Addresses(ipv4);

    // Create UDP Echo Server on fifth spoke
    UdpEchoServerHelper server(9);
    ApplicationContainer serverApp =
        server.Install(star.GetSpokeNode(4));

    serverApp.Start(Seconds(1));
    serverApp.Stop(Seconds(10));

    // Create UDP Echo Client on first spoke
    UdpEchoClientHelper client(star.GetSpokeIpv4Address(4), 9);

    client.SetAttribute("MaxPackets", UintegerValue(4));
    client.SetAttribute("Interval", TimeValue(Seconds(1)));
    client.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApp =
        client.Install(star.GetSpokeNode(0));

    clientApp.Start(Seconds(2));
    clientApp.Stop(Seconds(10));

    // Populate routing tables
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Generate NetAnim XML file
    star.BoundingBox(0, 0, 100, 100);
    AnimationInterface anim("star-topology.xml");

    // Run simulation
    Simulator::Stop(Seconds(10));
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
