
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

void Send(Ptr<Socket> s, Address a) { s->SendTo(Create<Packet>(100), 0, a); }

int main()
{
  NodeContainer n(2);                    // n0 = sender, n1 = receiver
  InternetStackHelper().Install(n);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  p2p.SetChannelAttribute("Delay", StringValue("10ms"));

  Ipv4AddressHelper ip;
  ip.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer ifc = ip.Assign(p2p.Install(n));

  Ptr<Socket> s = Socket::CreateSocket(n.Get(0), UdpSocketFactory::GetTypeId());
  Ptr<Socket> r = Socket::CreateSocket(n.Get(1), UdpSocketFactory::GetTypeId());
  r->Bind(InetSocketAddress(Ipv4Address::GetAny(), 9000));
  Address dest = InetSocketAddress(ifc.GetAddress(1), 9000);

  for (int i = 0; i < 4; i++)            
  {
    if (i == 2) { std::cout << "Frame 2 LOST\n"; continue; }
    std::cout << "Frame " << i << " sent\n";
    Simulator::Schedule(Seconds(i + 1), &Send, s, dest);
  }
  std::cout << "Retransmitting Frame 2 and 3\n";
  Simulator::Schedule(Seconds(6), &Send, s, dest);   
  Simulator::Schedule(Seconds(7), &Send, s, dest);   

  AnimationInterface anim("gobackn.xml");
  Simulator::Run();
  Simulator::Destroy();
}
