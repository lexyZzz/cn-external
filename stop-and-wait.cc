
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

Ptr<Socket> s, r;  Address sA, rA;  EventId timer;  uint32_t seq = 0;

void Send()
{
  if (seq == 5) return;
  s->SendTo(Create<Packet>((uint8_t *)&seq, 4), 0, rA);
  std::cout << "Sent frame " << seq << "\n";
  timer = Simulator::Schedule(Seconds(0.1), &Send);
}

void OnData(Ptr<Socket> x) { r->SendTo(x->Recv(), 0, sA); }

void OnAck(Ptr<Socket> x)
{
  uint32_t a;
  x->Recv()->CopyData((uint8_t *)&a, 4);
  if (a != seq) return;                              
  Simulator::Cancel(timer);  seq++;  Send();         
}

int main()
{
  NodeContainer n(2);                
  InternetStackHelper().Install(n);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  p2p.SetChannelAttribute("Delay", StringValue("10ms"));
  NetDeviceContainer d = p2p.Install(n);

  Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
  em->SetAttribute("ErrorRate", DoubleValue(0.3));
  em->SetAttribute("ErrorUnit", StringValue("ERROR_UNIT_PACKET"));
  d.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));

  Ipv4AddressHelper ip;
  ip.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer ifc = ip.Assign(d);

  s = Socket::CreateSocket(n.Get(0), UdpSocketFactory::GetTypeId());
  r = Socket::CreateSocket(n.Get(1), UdpSocketFactory::GetTypeId());
  s->Bind(InetSocketAddress(Ipv4Address::GetAny(), 9001));
  r->Bind(InetSocketAddress(Ipv4Address::GetAny(), 9000));
  sA = InetSocketAddress(ifc.GetAddress(0), 9001);
  rA = InetSocketAddress(ifc.GetAddress(1), 9000);
  r->SetRecvCallback(MakeCallback(&OnData));
  s->SetRecvCallback(MakeCallback(&OnAck));

  AnimationInterface anim("stopwait.xml");
  Simulator::Schedule(Seconds(1), &Send);
  Simulator::Run();
  Simulator::Destroy();
}
