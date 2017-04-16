#include "ns3/lte-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include <ns3/lte-module.h>
#include <ns3/lte-spectrum-phy.h>
#include <ns3/lte-chunk-processor.h>
#include <ns3/multi-model-spectrum-channel.h>
#include <ns3/friis-spectrum-propagation-loss.h>

using namespace ns3;
  
   int main (int argc, char *argv[])
   {
	   // the rest of the simulation program follows
	Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
//This will instantiate some common objects (e.g., the Channel object) and provide the methods to add eNBs and UEs and configure them.
//Create Node objects for the eNB(s) and the UEs:

      CommandLine cmd;
     cmd.Parse (argc, argv);
   
    NodeContainer ueNodes;
    ueNodes.Create (5);
    // creat 1 enodeb 
    NodeContainer enbNodes;
	enbNodes.Create (1);
	
	 
	
     MobilityHelper mobility;
     mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (enbNodes);
     
	AnimationInterface::SetConstantPosition (enbNodes.Get(0), 0 , 0 );

     mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                    "MinX", DoubleValue (2.0),
                                    "MinY", DoubleValue (2.0),
                                    "DeltaX", DoubleValue (5.0),
                                    "DeltaY", DoubleValue (5.0),
                                  "GridWidth", UintegerValue (3),
                                   "LayoutType", StringValue ("RowFirst"));
      mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                               "Mode", StringValue ("Time"),
                                "Time", StringValue ("1s"),
                                "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=20.0]"),
                               "Bounds", RectangleValue (Rectangle (0.0, 20.0, 0.0, 20.0)));
    mobility.Install (ueNodes);
     // Set mobility random number streams to fixed values
     mobility.AssignStreams (ueNodes, 0);

   AsciiTraceHelper ascii1;
   AsciiTraceHelper ascii2;
     MobilityHelper::EnableAsciiAll (ascii1.CreateFileStream ("mobility-trace-example.mob"));
     //MobilityHelper::EnableAscii (ascii2.CreateFileStream ("ENB_Container-trace-example.mob"), enbNodes );

 NetDeviceContainer enbDevs;
enbDevs = lteHelper->InstallEnbDevice (enbNodes);

//Install an LTE protocol stack on the UEs:

NetDeviceContainer ueDevs;
ueDevs = lteHelper->InstallUeDevice (ueNodes);

//Attach the UEs to an eNB. This will configure each UE according to the eNB configuration, and create an RRC connection between them:
lteHelper->Attach (ueDevs, enbDevs.Get (0));
//Activate a data radio bearer between each UE and the eNB it is attached to:
/**********************************************************
enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
EpsBearer bearer (q);
lteHelper->ActivateDataRadioBearer (ueDevs, bearer);   
**********************************************************/
Config::SetDefault ("ns3::LteHelper::UsePdschForCqiGeneration", BooleanValue (true));
Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (false));

enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
EpsBearer bearer (q);
lteHelper->ActivateDataRadioBearer (ueDevs, bearer);


lteHelper->SetSchedulerType ("ns3::PssFfMacScheduler");     //PSS scheduler

lteHelper->SetSchedulerAttribute("nMux", UintegerValue(625000)); // the maximum number of UE selected by TD scheduler
lteHelper->SetSchedulerAttribute("PssFdSchedulerType", StringValue("CoItA")); // PF scheduler type in PSS


// configure all the simulation scenario here...

lteHelper->EnablePhyTraces ();
lteHelper->EnableMacTraces ();
lteHelper->EnableRlcTraces ();
lteHelper->EnablePdcpTraces ();

lteHelper->SetFadingModel("ns3::TraceFadingLossModel");

lteHelper->SetFadingModelAttribute ("TraceFilename", StringValue ("src/lte/model/fading-traces/fading_trace_EPA_3kmph.fad"));
lteHelper->SetFadingModelAttribute ("TraceLength", TimeValue (Seconds (10.0)));
lteHelper->SetFadingModelAttribute ("SamplesNum", UintegerValue (10000));
lteHelper->SetFadingModelAttribute ("WindowSize", TimeValue (Seconds (0.5)));
lteHelper->SetFadingModelAttribute ("RbNum", UintegerValue (100));


//this method will also activate two saturation traffic generators for that bearer, one in uplink and one in downlink.

//Set the stop time:
 
 //Run the simulation:
  AnimationInterface anim ("Mobility_test2.xml");
  uint32_t resourceUeId=anim.AddResource("/home/amr/Documents/ue.png");
  uint32_t resourceEnbId=anim.AddResource("/home/amr/Documents/enb.png");

  anim.SetMobilityPollInterval(Seconds(1.0));
   /* for enodeb */
   anim.UpdateNodeImage (5,resourceEnbId);
   anim.UpdateNodeSize(5,2,2);


   /* for user equ */
   for (uint8_t i=0;i<=4;i++)
   {
 	  anim.UpdateNodeImage (i,resourceUeId);
 	   anim.UpdateNodeSize(i,1,1);
   }

  anim.EnablePacketMetadata(true);
  
  
    Simulator::Stop (Seconds (10.0));
    //time of simulation
     Simulator::Run ();
     Simulator::Destroy ();
 }
