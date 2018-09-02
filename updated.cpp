/*
 author: Hannaneh Barahouei (barahoueipash@vcu.edu)
 file: updated.cpp
 */


using namespace std;
#include <iostream>
#include <fstream>
#include <deque> 
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <time.h>
#include <random>
#include <string>
#include <sstream>
#include <thread>   
#include <mutex>
#include "updated.h"

double next_slot =0.000;
double interval;
double prev_slot=0.000;
double total_t_=0;
int ret_threshold= 8;
int num_packets=0;
double slt_duration=0.00002;
ofstream myfile;
ofstream myfile2;
ofstream myfiledis;
ofstream nodefile;
ofstream nodefile1;
ofstream nodefile2;
ofstream nodefile3;
ofstream nodefile4;
ofstream nodefile5;
ifstream infile; 
ifstream infile1; 
ifstream infile2; 
ifstream infile3; 
ifstream infile4; 
int rew[200];

int num_nodes;
safeChannel channel;
std::mutex m;
int channelBusy[MAXNODES];
int accessCount;

long double prop[MAXNODES][MAXNODES];
long double dist0[MAXNODES][MAXNODES];
FILE * fEvent;                /* Pointer to file of events                            */

 	int 
       isDeferring[MAXNODES], /* is the node deferring?                               */
                              /* Is the channel busy?                                 */
       DEBUGGING, 
       done;                  /* Are we done with the simulation yet?                 */
      
   int numNodes,         /* number of nodes in the system                             */
       NUM_REARRIVAL=0,
       channelLength,    /* length of the channel                                     */
       bitsPerPac,       /* number of bits / packet                                   */
       bitsPerAck,       /* number of bits / ack                                      */
       buffPerNode,      /* Maximum size of buffer for each node                      */
       maxCWindow,       /* Maximum contention window size                            */
                         /* Minimum contention window size                            */
       curCWindow,       /* The current contention window size                        */
       numCollisions,    /* The total number of collisions                            */
       numPacketsLost,   /* the number of packets which overflow the nodes' buffers   */
       //numInQ[MAXNODES], /* The number of packets in queue for a node                 */
       numDeferring,     /* Number of nodes deferring                                 */
       totalPackets,     /* Total number of packets sent so far..                     */
       loopcounter,      /* Count the total number of events                          */
       iseed;            /* Seed of random number generator                           */
       
         
long double propDelay,        /* End-to-end propagation delay                              */   
       channelCapacity,  /* Capacity of the channel                                   */
       probError,        /* Probability[no error] when transmitting a packet          */
       SIFS,             /* SIFS interval                                             */
       DIFS,             /* DIFS interval                                             */
       T3,               /* Time out interval                                         */
       slotTime,         /* Approximation of the slot time                            */
       packetDelay,      /* The delay a packet has incurred in the system             */
       totalPacketDelay, /* The total delay of all the packets incur in the system    */
       timeNextEvent,    /* Time of next event occurance                              */
       lambdaN,          /* Node arrival rate                                         */ 
       lambdaS,          /* System arrival rate                                       */
       sysTime,          /* Global clock                                              */
       timeXAck,         /* time to transmit an ACK                                   */
       timeXPac,         /* time to transmit a packet                                 */
       I,                /* interrupt time + service delay                            */
       P1,
       minCWindow,
       P2;
//long double timeNextEvent;
       
bool isBusy[MAXNODES]; /* is the node busy                                          */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
int initParams(int argc, char * argv[])
   {
    /* This function read in all command line parameters and assigns
     * the proper values to the corresponding variables 
     */
    int counter;
   
    num_nodes=5;               /* set default values - in case not */
    channelLength=100;         /* all command line args are present */     
    propDelay=3.3e-07;
    if(trans_r){
     channelCapacity=5242880; 
    }
    else
    {
      channelCapacity=10242880; 
    }
    
    bitsPerPac=120000;
    bitsPerAck=3680;
    buffPerNode=30;
    maxCWindow=1024;
    SIFS=(2.5)*(3.3e-07);
    DIFS=(4.0)*(3.3e-07);
    T3=(3.0)*(3.3e-07);
    lambdaS=1.0;
    
    if(prob)
      probError=0.0;
    else
      probError=0.2;
      
    DEBUGGING=1;
    P1=0.1;
    P2=0.0;
    //interval = 2* propDelay;
    interval = 0.0001000;
    

    /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> command line options */
    if (argc==1) 
      {  printf ("\nUsing the default parameters...\n");  return (1);  }


   for (counter=1;  counter<argc;  counter++)   /* Process args */
     if (argv[counter][0] == '-')               /* If it's a flag */
       {
       switch (argv[counter][1])
          {
           case 'n': numNodes=atoi(argv[counter+1]);        break;
           case 'l': channelLength=atoi(argv[counter+1]);   break;
           case 'T': propDelay=atof(argv[counter+1]);       break;
           case 'c': channelCapacity=atof(argv[counter+1]); break;
           case 'x': bitsPerPac=atoi(argv[counter+1]);      break;
           case 'a': bitsPerAck=atoi(argv[counter+1]);      break; 
           case 'b': buffPerNode=atoi(argv[counter+1]);     break;
           case 'u': maxCWindow=atoi(argv[counter+1]);      break;
           case 'v': minCWindow=atoi(argv[counter+1]);      break;
           case 's': SIFS=atof(argv[counter+1]);            break;
           case 'd': DIFS=atof(argv[counter+1]);            break;
           case 't': T3=atof(argv[counter+1]);              break;
           case 'p': probError=atof(argv[counter+1]);       break;
           case 'S': lambdaS=atof(argv[counter+1]);         break;
           case 'D': DEBUGGING=atoi(argv[counter+1]);       break;
           default :
              { printf ("\nUnrecognized command line option...(%s)(%c).\n\n", 
                     argv[counter], argv[counter][1]);
                 exit(-1);  }
          }  /* End if switch() */
       }  /* End of for loop */

  } /* End if initParams() */


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void initAll(void)
  {
   /* This function goes through and calculates all of the necesary 
    * parameters based on the command line arguments and the default
    * settings - it also initializes all variables to NULL/zero  
    */
   int i;
   
   /*_______________General stuff to set to _________________*/
   timeNextEvent=0.00;    /* Reset time of next event           */
   sysTime=0;          /* Reset system timer                 */
   timeXPac=0;         /* Reset time to transmit a packet    */
   timeXAck=0;         /* Reset time to transmit an Ack      */ 
   iseed=1837;         /* Seed the random # generator        */
   loopcounter=0;      /* Number of events                   */
   done=FALSE;         /* Done with simulation               */
   totalPackets=0;     /* total packets sent is zero         */
   numCollisions=0;    /* number of collisions               */
   curCWindow=minCWindow;  /* Start with the smallest C win. */
   numPacketsLost=0;   /* No packets lost due to Q overflow  */
   

    for (i=0; i<num_nodes; i++){  /* None of the nodes is     */
       isDeferring[i]=FALSE;     /* deferring                */
       isBusy[i]=false;
       channelBusy[i]=0;
     }

    numDeferring=0;              /* nobody is deferring      */

    //channelBusy=FALSE;        /* the channel is assumed idle */

    packetDelay=0;               /* No packet delay          */
    totalPacketDelay=0;          /* no total packet delay    */
    //I= (double)(channelLength*bitsPerPac)/channelCapacity; 
     I=0.0001;

   /*_____________General stuff to calculate_______________*/
   lambdaN= lambdaS/((double)num_nodes);  /* Nodal arrival rate */
   timeXPac = (double)bitsPerPac/channelCapacity;
   timeXAck = (double)bitsPerAck/channelCapacity;
   slotTime=100000*(propDelay+timeXPac);

      

   /*____________________Open files_________________________*/          
   if (DEBUGGING) fEvent=fopen("logfile.txt", "w");
   if (DEBUGGING) if (!fEvent) fatal("initAll", "Can't open event file");

  }  /* End of initAll;



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void showParams(FILE * fp)
  {
   /* This function shows all of the values of the command line 
    * argument variables after they have been entered and processed
    */

  if (fp!=NULL)
    {
     /* end of going through all of the command line args */
      fprintf (fp, "\n\nNumber of Nodes............: %d",     num_nodes); 
      fprintf   (fp, "\nChannel length.............: %d ",    channelLength);
      fprintf   (fp, "\nPropagation Delay..........: %1.2e",  propDelay);
      fprintf   (fp, "\nChannel capacity...........: %1.3e",  channelCapacity);
      fprintf   (fp, "\nBits/packet................: %d",     bitsPerPac);
      fprintf   (fp, "\nBits/ACK...................: %d ",    bitsPerAck);
      fprintf   (fp, "\nMax number of buffers/node.: %d",     buffPerNode);
      fprintf   (fp, "\nMax contention window......: %d",     maxCWindow);   
      fprintf   (fp, "\nMin contention window......: %d",     minCWindow);
      fprintf   (fp, "\nSIFS.......................: %1.2e",  SIFS);
      fprintf   (fp, "\nDIFS.......................: %1.2e",  DIFS);
      fprintf   (fp, "\nT3.........................: %1.2e",  T3);
      fprintf   (fp, "\nProb[unsuccessful Packet]..: %1.1lf", probError);
      fprintf   (fp, "\nSystem packet arrival rate.: %lf",    lambdaS);
      fprintf   (fp, "\nNodal packet arrival rate..: %lf",    lambdaN);
      fprintf   (fp, "\nTime to transmit an ack....: %lf",    timeXAck);
      fprintf   (fp, "\nTime to transmit a packet..: %lf\n",  timeXPac);

    }
  } /* showParams */

//if (DEBUGGING) if (fEvent) fprintf (fEvent, "\nMedia free? %s", (channelBusy==FALSE)?"YES":"NO");

vector<Wireless_node> list_nodes;
//vector<Wireless_node> node1;
vector<Packet> buffer1;
vector<Packet> buffer_received;
Wireless_node node1[60];

void Packet::sent(long double current_time1)
{	
	//event *ev=NULL;
	if (first_send_attempt_ == -1)
	{
		first_send_attempt_ = current_time1;
	}
	num_retransmission_attempts_++;
	sent_time_ = current_time1;
	status_ = Packet::SENDING();
}

void Packet::received(long double current_time)
{	
	receive_time_ = current_time;
	status_ = Packet::RECEIVED();
}


long double Packet::get_effective_transmission_time() const {
	return receive_time_ - sent_time_;
}

long double Packet::get_total_transmission_time() const {
	return receive_time_ - first_send_attempt_;
}

long double Packet::get_total_delay() const {
	return receive_time_ - first_schedule_time_;
}
void Packet::output() const{
	cout << "PACKET: " << id_ << " from NODE: " << from_ << " [SCH]:" << schedule_time_ << " [FIRS]:" << first_send_attempt_ << " [SENT]:" << sent_time_
		<< " [XP_RCV]:" << expected_receive_time_ << " [RCV]:" << receive_time_ << " [ATMPT]" << num_retransmission_attempts_ <<endl;

}



Packet Wireless_node::create_new_packet(char type, int from,  int to, double current_time)
{	

	Packet pkt;	
  Random_Number_Generator r;
	pkt.set_type(type);
	pkt.set_status(Packet::SCHEDULED());
	pkt.set_from(from);
	pkt.set_to(to);
	if(pkt.get_type()==Packet::ACK())
	{
		node1[from].set_generated_ack_packets(1);
		pkt.set_id(get_generated_ack_packets());
		
	}
	else
	{
    ++num_packets;
		node1[from].set_generated_packets(1);
		//pkt.set_id(get_generated_packets());
    pkt.set_id(num_packets);
		
		
	}

	cout << "Packet's type: " << pkt.get_type() << "with id: "<< pkt.get_id()<<endl;
  
  pkt.set_first_schedule_time(current_time);
  int id = pkt.get_id();
  //pkt.set_schedule_time(pkt.get_first_schedule_time());


   // cw=false;

  if (cw){
      minCWindow=rand_CW(7)* slt_duration;
      //cout <<"cw is"<< endl;
      
    }
    else{
      minCWindow=0;
    }
   // myfile2 <<"cw: "<<minCWindow<<'\n';

  Packet *pack= NULL;
  pack = &pkt;

  if(!Ack && pkt.get_type()== Packet::MSG()){

            cout << " Packet: "<< pack ->get_id() << " is from node: " << pack ->get_from() << " to node: "<< pack ->get_to()<<endl;
               node1[from].get_startTransmission_buff().insert(node1[from].get_startTransmission_buff().begin()+id, 
                pkt.get_first_schedule_time()+ minCWindow);
              }

  else if(Ack && pkt.get_type()== Packet::MSG()){
  if(pack->get_id()==1 ){
               node1[from].get_startTransmission_buff().insert(node1[from].get_startTransmission_buff().begin()+id, 
               pkt.get_first_schedule_time()+minCWindow);
              }
            else{

              cout << "This is after Packet's type: " <<endl;
               node1[from].get_startTransmission_buff().insert(node1[from].get_startTransmission_buff().begin()+id, 
               pkt.get_first_schedule_time()+minCWindow+ 2 * timeXPac);
              }

    } /*else if Ack and msg*/



    if ( pkt.get_type()== Packet::ACK())
        {
          cout << "This is if we  DO NOT have ack mechanism or its an Ack packet" << endl;
          //node1[j].set_start_transmission( current_time_ + pkt.get_schedule_time());
          node1[from].get_startTransmission_buff().insert(node1[from].get_startTransmission_buff().begin()+id,  
            pkt.get_first_schedule_time()+minCWindow);
        }



	long double thisSchedule = node1[from].start_transmission[id];

  

	cout << "packet with id: " << pkt.get_id() <<" is created from: " << from << " to: " << to << "at time: " << thisSchedule << '\n'; 
  if (protocol){

      pkt.set_schedule_time(thisSchedule);

      if (pkt.get_type()==Packet::ACK())
      {
        pkt.set_expected_receive_time(pkt.get_schedule_time() + timeXAck);
      }
      else
      {
        pkt.set_expected_receive_time(pkt.get_schedule_time() + timeXPac);
      }
      

      theSimulation.Event_Scheduler (new sendEvent (thisSchedule));
   }
   else if(!protocol)
   {  
      if (thisSchedule <= next_slot){
        cout<<"less than next slot"<<endl;

        pkt.set_schedule_time(next_slot);
        next_slot += interval;
      }
      else{
        
        next_slot += interval;
        pkt.set_schedule_time(thisSchedule);
      }

       if (pkt.get_type()==Packet::ACK())
      {
        pkt.set_expected_receive_time(pkt.get_schedule_time() + timeXAck + prop[from][to]);
      }
      else
      {
        pkt.set_expected_receive_time(pkt.get_schedule_time() + timeXPac + prop[from][to]);
      } 
      
      theSimulation.Event_Scheduler (new slotted_sendEvent (pkt.get_schedule_time()));
   }
	return pkt;
}

Packet Wireless_node::broadcast(char type, int from, int to, long double current_time){

}

Packet Wireless_node::create_new_msg_packet(int from, int to, double current_time)
{
	return create_new_packet(Packet::MSG(), from, to, current_time);
}

Packet Wireless_node::create_new_ack_packet(int from, int to,  double current_time, int pkt_id)
{
	Packet p = create_new_packet(Packet::ACK(), from, to, current_time);
	p.set_payload(pkt_id);
	return p;
}

/*
  Sets the seed of the random variables
*/
void Wireless_node::set_seed(long double trans)
{
     rnd_transmission.set_seed(trans);
}

/*
  Creates instances of the Random Variate Generator and
  also the first event...
*/
void Wireless_node::initialize()
{
	if (!initialized_) {
		//rnd_transmission.set_rate(transmission_rate_);
		initialized_ = true;
	}
}

void Wireless_node::output() const{
	for (int j=0; j< num_nodes; ++j)
	cout << "NODE: " << j << " [COMP]" << node1[j].get_completed_transmissions() << " [ATMPT]" 
	<< node1[j].get_attempted_transmissions()<< " [Total-GEN-MSG]" << node1[j].get_generated_packets() << " [GEN-ACK]" << node1[j].get_generated_ack_packets() -600 << " [Average Delay]" << abs (node1[j].get_average_delay()/ num_nodes)<< endl;
//		<< " [GEN]" << generated_packets_ << " [EFF]" << efficiency_ << "( " << transmission_rate_ * propagation_time_ << ") [DELAY]"  << delay_/generated_packets_ << endl;
	//cout << "NODE: " << id_ << " [Completed transmissions]" << completed_transmissions_ << " [# Transmission attempts]" << attempted_transmissions_
			//<< " [Throughput]" << efficiency_ << " [G]" << transmission_rate_ /* * propagation_time_ << " [Average Delay]" */ << delay_/generated_packets_ << endl;
}

void Wireless_node::attempt_transmission(Packet *p, long double current_time1) {
	attempted_transmissions_++;
	p->sent(current_time1);
}

void Wireless_node::completed_transmission(Packet *p, long double current_time) {
	completed_transmissions_++;
	p->received(current_time);
	efficiency_ += p->get_effective_transmission_time()/p->get_total_transmission_time();
	efficiency_ = efficiency_ / 2;
	delay_ += p->get_total_delay();
}

Packet Wireless_node::retransmit(Packet *pack, double current_time) {


  pack->set_first_schedule_time(current_time);
  int id = pack ->get_id();
  int from = pack -> get_from();
  int to = pack->get_to();


  if (cw){
      minCWindow=rand_CW(7)* slt_duration;
    
      cout <<"cw is"<< endl;
    }
    else{
      minCWindow=0;
    }


  if(!Ack && pack->get_type()== Packet::MSG()){

            cout << " Packet: "<< pack ->get_id() << " is from node: " << pack ->get_from() << " to node: "<< pack ->get_to()<<endl;
               node1[from].get_startTransmission_buff().insert(node1[from].get_startTransmission_buff().begin()+id, 
                pack->get_first_schedule_time()+ minCWindow);
              }

  else if(Ack && pack->get_type()== Packet::MSG()){
  if(pack->get_id()==1 ){
               node1[from].get_startTransmission_buff().insert(node1[from].get_startTransmission_buff().begin()+id, 
               pack->get_first_schedule_time()+minCWindow);
              }
            else{
               node1[from].get_startTransmission_buff().insert(node1[from].get_startTransmission_buff().begin()+id, 
               pack->get_first_schedule_time()+minCWindow+ 2 * timeXPac);
              }

    } /*else if Ack and msg*/


    if ( pack->get_type()== Packet::ACK())
        {
          cout << "This is if we  DO NOT have ack mechanism or its an Ack packet" << endl;
          //node1[j].set_start_transmission( current_time_ + pkt.get_schedule_time());
          node1[from].get_startTransmission_buff().insert(node1[from].get_startTransmission_buff().begin()+id,  
            pack->get_first_schedule_time()+minCWindow);
        }



  long double thisSchedule = node1[from].start_transmission[id];


  pack->set_schedule_time(thisSchedule);

  Packet pkt = *pack;
  cout << "packet with id: " <<pack->get_id() <<" is retransmitted from: " << from << " to: " << to << "at time: " << thisSchedule << '\n'; 
  if (protocol){
      theSimulation.Event_Scheduler (new sendEvent (thisSchedule));
   }
   else if(!protocol)
   {

     if (thisSchedule <= next_slot){
        cout<<"less than next slot"<<endl;

        pkt.set_schedule_time(next_slot);
        next_slot += interval;

      }
      else{
       
        pkt.set_schedule_time(thisSchedule);
        cout<<"NOw next slot"<<endl;
        next_slot += interval;
      }
      
      theSimulation.Event_Scheduler (new slotted_sendEvent (pkt.get_schedule_time()));
   }

   return pkt;

}

void Wireless_node::reschedule_packet(Packet *p, long double time1)
{
	p->set_status(Packet::RESCHEDULED());
	if (p->get_first_schedule_time()==-1)
	{
		p->set_first_schedule_time(p->get_schedule_time());
	}
	p->set_schedule_time(time1);
	p->set_expected_receive_time(p->get_schedule_time()+ prop [p->get_from()] [p->get_to()]);
	int from = p->get_from();
	Packet pk = *p;
	//cout << "packet: " <<pk.get_id() << " from node: "<< from << " is rescheduled" << endl;
	node1[from].getMyVec().push_back(pk);
}

void Wireless_node::collision(Packet *p, long double current) {
	Random_Number_Generator r;
	failed_transmissions_++;
	numCollisions ++;
	//completed_transmissions_ --;
	int from = p->get_from();
	int to = p->get_to();
  int id = p->get_id();
	//reschedule_packet(p, current + rnd_transmission.rand_exponential());
	if (backoff){
     if (p->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << p->get_retransmit_attempt() <<endl;
            p->set_retransmit_attempt(1);
            gen_rearrival(p,  (rand_CW(p->get_retransmit_attempt()))* slt_duration );
          }
		
	}
	else{
    if (p->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << p->get_retransmit_attempt() <<endl;
            p->set_retransmit_attempt(1);
            gen_rearrival(p , 0 );
          }
		
	}
}

/* this next method returns TRUE if the simulation is within the desired error range */
bool Wireless_node::is_within_error_range(long double err) const {
	return (efficiency_ > 0) && (abs(efficiency_ - get_expected_efficiency()) < err);
}

long double Wireless_node::get_expected_efficiency() const {
	long double G = transmission_rate_ ;//* propagation_time_;
	return G*exp((-1)*G);
}

long double Wireless_node::distance (long double x1, long double y1, long double x2, long double y2)
{
  
  	int distancex = abs(x2 - x1) * abs(x2 - x1);
  	int distancey = abs(y2 - y1) * abs(y2 - y1);

  	long double distance = sqrt(abs(distancex - distancey));

  	return distance;
}

long double Wireless_node::propagation_delay (long double dist)
{
  
	dist= dist/300000000;

  	return dist;
}

long double Wireless_node::range (int from, int to)
{	
	Wireless_node dis;
	long double distance_two;
	long double x1 = list_nodes[from].x;
	long double y1 = list_nodes[from].x;
	long double x2 = list_nodes[to].y;
	long double y2 = list_nodes[to].y;
	distance_two = dis.distance(x1,y1,x2,y2);
	if (distance_two - trans_range_ < 0)
		return true;
	else
		return false;
}



void simulator::run(){
	
	while (!eventQueue.empty()){
		event * nextEvent = eventQueue.top();
		eventQueue.pop();
		current_time = nextEvent ->current_time_;
		sysTime=current_time;

		if (current_time < 30){
		cout<< ">>>>>next event happens at: " << current_time << endl;
		nextEvent ->processEvent ();
		delete nextEvent;
		//nextEvent=NULL;
	}
}

}



void Wireless_node::BubbleSort(vector <Packet> num)
{
      int i, j, flag = 1;    // set flag to 1 to start first pass
      Packet temp;            // holding variable
      Wireless_node node;
      Packet pkt;
      double numLength = node.buff.size(); 
      for(i = 1; (i <= numLength) && flag; i++)
     {
          flag = 0;
          for (j=0; j < (numLength -1); j++)
         {
               if (node.buff[j+1].get_schedule_time() < node.buff[j].get_schedule_time())      // ascending order simply changes to <
              { 
                    temp = node.buff[j];             // swap elements
                    node.buff[j] = node.buff[j+1];
                    node.buff[j+1] = temp;
                    flag = 1;               // indicates that a swap occurred.
               }
          }
     }
     return;   //arrays are passed to functions by address; nothing is returned
}

void transmitEvent::processEvent() {

	Wireless_node node_;
	Packet *pack=NULL;
	Packet pkt;

	cout << "- - - - - - - transmitEvent Happened- - - - - - - - "<<endl;
	long double b;


	for (int j = 0; j < num_nodes; ++j)
	{
		//for (int j = i; node1[j].getMyVec().size(); ++j)
		if(!(node1[j].getMyVec().empty()))
		{

			//node1[j].BubbleSort(node1[j].buff);

		int i, x, flag = 1;    // set flag to 1 to start first pass
     	 Packet temp;            // holding variable
      	Packet pkt,pkt2,pkt3;
      	double numLength = node1[j].buff.size(); 
      	for(i = 1; (i <= numLength) && flag; i++)
     	{
          flag = 0;
          for (x=0; x < (numLength -1); x++)
         {
               if (node1[j].buff[x+1].get_schedule_time() < node1[j].buff[x].get_schedule_time())      // ascending order simply changes to <
              { 
                    temp = node1[j].buff[x];             // swap elements
                    node1[j].buff[x] = node1[j].buff[x+1];
                    node1[j].buff[x+1] = temp;
                    flag = 1;               // indicates that a swap occurred.
               }
          }
     }

		//for (int n1=0; n1<node1[j].buff.size(); ++n1)
		//{
				pkt= node1[0].buff[0];
				pkt2=node1[j].buff[0];
				pkt3=node1[j].buff[0];
				pack= &pkt;
				b= current_time_ + pkt.get_schedule_time();
				//cout << "min(buff[0],buff[1])==" << std::min(pkt.get_schedule_time(), pkt3.get_schedule_time()) <<endl;
				

			//cout << "transmitEvent time is: " << T->current_time << endl;

			//int t = pack ->get_to();
			//node1[t].set_start_reception( pack->get_expected_receive_time());
			//t.current_time= pkt.get_schedule_time();
			cout << "sendEvent happens at time: " << b + pkt.get_schedule_time() << endl;
			cout << "current_time_ is: " << current_time_ << endl;
			//current_time =current_time + pkt.get_schedule_time();
			
		//}

	}
}
	theSimulation.Event_Scheduler (new sendEvent (b));
}

	


void sendEvent::processEvent()
{	
	Wireless_node node_;
	Packet *pack=NULL;
	Packet pkt;

	cout << "- - - - - - - sendEvent Happened- - - - - - - - "<<endl;

for (int j = 0; j < num_nodes; ++j)
	{ 
		/*check the buffer of each node*/

		if (! node1[j].getMyVec().empty())
		{
      long double b;
     int i, x, flag = 1;    //set flag to 1 to start first pass
      Packet temp;            //holding variable
      double temp_id;
        Packet pkt,pkt2,pkt3;
        double numLength = node1[j].getMyVec().size();
        cout<< "node buff size: "<< numLength << endl; 
        for(i = 1; (i <= numLength) && flag; i++)
      {
          flag = 0;
          for (x=0; x < (numLength -1); x++)
         {
               if (node1[j].buff[x+1].get_schedule_time() < node1[j].buff[x].get_schedule_time()) 
              { 
                    temp_id = node1[j].buff[x].get_schedule_time();             //swap elements
                    node1[j].buff[x].set_schedule_time(node1[j].buff[x+1].get_schedule_time());
                    node1[j].buff[x+1].set_schedule_time(temp_id);
                    flag = 1;               //indicates that a swap occurred.
               }
          }
     }

      for(int k= 0 ; k < node1[j].buff.size() ; ++k )
      {
			
			cout<< "node " << j << "size: " << node1[j].buff.size()<< endl;
			pkt= node1[j].buff[k];
			pack= &pkt;
			int to = pack->get_to();
			int from= pack->get_from();
			int id =pack->get_id();
			
			cout<< "packet with id:  "<< id << "schedule time is "<<pack->get_schedule_time()<< endl;
			cout <<"current_time_ is "<< current_time_ <<endl;

      /*This is only to test*/
      if(current_time_ == pack->get_schedule_time())
        cout <<"its equal"<<endl;

				if((node1[j].get_status()==Wireless_node::IDLE())  && channelBusy[j]==0 && current_time_ == pack->get_schedule_time())
				{
          cout<< "i entered" <<endl;

					if (Ack && pkt.get_type()== Packet::MSG())
					{
						cout << " Packet: "<< pack ->get_id() << " is from node: " << pack ->get_from() << " to node: "<< pack ->get_to()<<endl;


							cout << "\n With start transmission time of the packet: "<< id << "		"<< node1[j].start_transmission[id] << endl;
							node1[j].attempt_transmission(pack, node1[j].start_transmission[id]);
              //node1[j].completed_transmission(pack, current_time_);
							node1[j].set_status(Wireless_node::SENDING());
							isBusy[from]=true;

              /*Check if its wired/wireless and assign channel as busy accordingly */

							for(int i=0; i< num_nodes; ++i){

                if (wire){

                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
								else{
                 if (dist0[from][i]<70){
									//cout << "dist from "<< from <<" to node "<< i << dist0[from][i]<<endl;
									m.lock();
									++channelBusy[i];
									cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
									accessCount++;
									m.unlock();
								}
							}
            }
							

							node1[from].get_send_buff().push_back(pkt);
							cout<< "node "<< from<< "buffer send size: "<< node1[j].get_send_buff().size()<<endl;
							node1[to].get_receipt_buff().push_back(pkt);
							node1[j].get_endTransmission_buff().insert(node1[j].get_endTransmission_buff().begin()+id,
							node1[j].start_transmission[id]+ timeXPac);
							cout << "\n With end transmission time of first packet: "<< node1[j].end_transmission[id] << endl;
							//node1[to].get_startReception_buff().insert(node1[to].get_startReception_buff().begin()+id, node1[j].start_transmission[id] + timeXPac + I );
							node1[to].get_startReception_buff().insert(node1[to].get_startReception_buff().begin()+id, node1[j].end_transmission[id]+ I);
							//cout << "node: " << to << " start_reception is: " << node1[j].end_transmission[id] <<endl;
							//pack->set_timer(pack->get_expected_receive_time() * 2);
              //cout <<"\n  timer is set to " << pack->get_timer() << endl;
							theSimulation.Event_Scheduler (new end_sendEvent ( node1[j].end_transmission[id]));
							cout << "with ReceptionEvent at node: "<< to << "at time: "<< node1[to].start_reception[id] << endl;
							theSimulation.Event_Scheduler (new receptionEvent (node1[to].start_reception[id]));
							node1[j].getMyVec().erase(node1[j].getMyVec().begin());

					}//if msg
					else if(!Ack && pkt.get_type()== Packet::MSG()){

						cout << " Packet: "<< pack ->get_id() << " is from node: " << pack ->get_from() << " to node: "<< pack ->get_to()<<endl;
							
	
							cout << "\n With start transmission time of the packet: "<< id << "		"<< node1[j].start_transmission[id] << endl;
							node1[j].attempt_transmission(pack, node1[j].start_transmission[id]);
              //node1[j].completed_transmission(pack, current_time_);

							//pack->set_timer(pack->get_expected_receive_time() * 2);
              //cout <<"\n  timer is set to " << pack->get_timer() << endl;
							node1[j].set_status(Wireless_node::SENDING());
							isBusy[from]=true;
							
               /*Check if its wired/wireless and assign channel as busy accordingly */

              for(int i=0; i< num_nodes; ++i){

                if (wire){

                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
                else{
                 if (dist0[from][i]<70){
                  //cout << "dist from "<< from <<" to node "<< i << dist0[from][i]<<endl;
                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
              }
            }


							node1[from].get_send_buff().push_back(pkt);
							cout<< "node "<< from<< "buffer send size: "<< node1[j].get_send_buff().size()<<endl;
							node1[to].get_receipt_buff().push_back(pkt);
							node1[j].get_endTransmission_buff().insert(node1[j].get_endTransmission_buff().begin()+id,
							node1[j].start_transmission[id]+ timeXPac);
							cout << "\n With end transmission time of first packet: "<< node1[j].end_transmission[id] << endl;
							cout<< "the I value is: "<< I <<endl;
							//node1[to].get_startReception_buff().insert(node1[to].get_startReception_buff().begin()+id, node1[j].start_transmission[id] + timeXPac + I );
							node1[to].get_startReception_buff().insert(node1[to].get_startReception_buff().begin()+id, node1[j].end_transmission[id] + I );
							cout << "node: " << to << " start_reception is: " << node1[to].start_reception[id] <<endl;
							
							theSimulation.Event_Scheduler (new end_sendEvent ( node1[j].end_transmission[id]));
							cout << "with ReceptionEvent at node: "<< to << "at time: "<< node1[to].start_reception[id] << endl;
							theSimulation.Event_Scheduler (new receptionEvent (node1[to].start_reception[id]));
							node1[j].getMyVec().erase(node1[j].getMyVec().begin());


					}
				if ( pkt.get_type()== Packet::ACK() && current_time_ == pack->get_schedule_time())
				{
					cout << "This is if we  DO NOT have ack mechanism or its an Ack packet" << endl;
				
					node1[j].attempt_transmission(pack,node1[j].start_transmission[id]);
					node1[j].get_send_buff().push_back(pkt);
					node1[j].set_status(Wireless_node::SENDING());
					isBusy[from]=true;
					
           /*Check if its wired/wireless and assign channel as busy accordingly */

              for(int i=0; i< num_nodes; ++i){

                if (wire){

                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
                else{
                 if (dist0[from][i]<70){
                  //cout << "dist from "<< from <<" to node "<< i << dist0[from][i]<<endl;
                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
              }
            }
				
					node1[j].get_endTransmission_buff().insert(node1[j].get_endTransmission_buff().begin()+id,
							node1[j].start_transmission[id]+ timeXAck );
					node1[to].get_startReception_buff().insert(node1[to].get_startReception_buff().begin()+id, node1[j].start_transmission[id] + timeXAck + I );
					cout << "End_sendEvent happens at time: " << node1[j].start_transmission[id]+ timeXAck << endl;
					theSimulation.Event_Scheduler (new end_sendEvent ( node1[j].start_transmission[id]+ timeXAck));
					//node1[to].set_start_reception(node1[j].get_start_transmission() + pack->get_expected_receive_time());
					
					cout << "with ReceptionEvent at node: "<< to << "at time: "<< node1[to].start_reception[id] << endl;
					pack->set_timer(pack->get_expected_receive_time() + I);
          cout <<"\n  timer is set to " << pack->get_timer() << endl;

					node1[to].get_receipt_buff().push_back(pkt);
					theSimulation.Event_Scheduler (new receptionEvent (node1[to].start_reception[id]));
					node1[j].getMyVec().erase(node1[j].getMyVec().begin());

				}
				

			} /*This is if the channel is busy and we try to send*/

			else if (node1[j].get_status() ==Wireless_node::IDLE() && channelBusy[j]!= 0 && current_time_ == pack->get_schedule_time())
			{
				
        if (csma && !backoff){
          cout<< "Channel Busy in csma" <<endl;
          isDeferring[from]=true;
          node1[j].getMyVec().erase(node1[j].getMyVec().begin());
          if (pack->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
            pack->set_retransmit_attempt(1);
            gen_rearrival(pack,  timeXPac);
          }
          else if (pack->get_retransmit_attempt() >= ret_threshold){
            gen_arrival(from,to, 5);
          }
          
        }

       if (csma && backoff){
          cout<< "Channel Busy in csma" <<endl;
          isDeferring[from]=true;
          node1[j].getMyVec().erase(node1[j].getMyVec().begin());
          if (pack->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
            pack->set_retransmit_attempt(1);
            gen_rearrival(pack, (rand_CW(pack->get_retransmit_attempt()))* slt_duration );
          }
          else if (pack->get_retransmit_attempt() >= ret_threshold){
            gen_arrival(from,to, 5);
          }
          
        } 

        else if (!csma){
				cout<< " Channel Busy" <<endl;
				Random_Number_Generator r;
				++numCollisions;

        if(from==0){
          nodefile1.open("reward1.txt");
          nodefile1 << -1 <<'\n';
          
    }
      else if(from==1){
        nodefile2.open("reward2.txt");
        nodefile2 << -1 <<'\n';
    }

    else if (from==2){
        nodefile3.open("reward3.txt");
        nodefile3 << -1 <<'\n';
    }
    else if (from==3){
       nodefile4.open("reward4.txt");
       nodefile4 << -1 <<'\n';
      }

     else if (from==4){
      nodefile5.open("reward5.txt");
      nodefile5 << -1 <<'\n';
      }
        nodefile<<"node"<<j<<" :"<< -1 <<"for packet: " << pack->get_id()<<'\n';
				node1[j].getMyVec().erase(node1[j].getMyVec().begin());

				if (backoff){

           if (pack->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
            pack->set_retransmit_attempt(1);
            gen_rearrival(pack, (rand_CW(pack->get_retransmit_attempt()))* slt_duration);
          }
          else if (pack->get_retransmit_attempt() >= ret_threshold){
            //gen_arrival(from,to, 5);
          }
				}

				else if (! backoff){
          cout<< " before condition retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
           if (pack->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
            pack->set_retransmit_attempt(1);
            gen_rearrival(pack,  0);
          }
          else if (pack->get_retransmit_attempt() >= ret_threshold){
            //gen_arrival(from,to, 5);
          }
				}

      }/*else !csma*/
			}
			else if ( node1[j].get_status() !=Wireless_node::IDLE()
			 && current_time_ == pack->get_schedule_time())
			{
				Random_Number_Generator r;

				cout<< "I'm Busy" <<endl;
				node1[j].getMyVec().erase(node1[j].getMyVec().begin());
        if (pack->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
            pack->set_retransmit_attempt(1);
            gen_rearrival(pack,    slt_duration*(rand_CW(pack->get_retransmit_attempt())));
          }
			}

      else if(node1[j].get_status() ==Wireless_node::IDLE() && current_time_ == pack->get_schedule_time())
    {
      cout<< "My schedule match but not other tests" <<endl;
    }

    }//for packets in buffer
		}/*if buffer empty*/
	}//for nodes
}


void slotted_sendEvent::processEvent()
{	
	Wireless_node node_;
	Packet *pack=NULL;
	Packet pkt;

	cout << "- - - - - - - slotted sendEvent Happened- - - - - - - - "<<endl;


for (int j = 0; j < num_nodes; ++j)
	{ 
		/*check the buffer of each node*/

		if (! node1[j].buff.empty())
		{

		long double b;
		int i, x, flag = 1;    /*set flag to 1 to start first pass*/
     	Packet temp;            /*holding variable*/
     	double temp_id;
      	Packet pkt,pkt2,pkt3;
      	double numLength = node1[j].buff.size(); 
      	for(i = 1; (i <= numLength) && flag; i++)
     	{
          flag = 0;
          for (x=0; x < (numLength -1); x++)
         {
               if (node1[j].buff[x+1].get_schedule_time() < node1[j].buff[x].get_schedule_time()) 
              { 
                    temp_id = node1[j].buff[x].get_schedule_time();             /*swap elements*/
              		//cout<< "temp id"<< temp_id<<endl;
                    //node1[j].buff[x] = node1[j].buff[x+1];
                    node1[j].buff[x].set_schedule_time(node1[j].buff[x+1].get_schedule_time());
                    node1[j].buff[x+1].set_schedule_time(temp_id);
                    flag = 1;               /*indicates that a swap occurred.*/
               }
          }
     }

     for(int k= 0 ; k < node1[j].buff.size() ; ++k )
      {
			
			cout<< "node " << j << "size: " << node1[j].getMyVec().size()<< endl;
			pkt= node1[j].buff[k];
			pack= &pkt;
			int to = pack->get_to();
			int from= pack->get_from();
			int id =pack->get_id();
			
			cout<< "packet "<< id << "schedule_time_ is  "<< pack->get_schedule_time()<< endl;
			cout <<"current_time_ is "<< current_time_ <<endl;

      
        /*while(! inRange(prev_slot,next_slot,current_time_))
        {
          prev_slot=next_slot;
          next_slot += interval;
          cout <<" stuck in while loop! " <<endl;
        }
        
      }
			
			if (inRange(prev_slot,next_slot,current_time_))  {
				cout<< "prev_slot is : "<< prev_slot<<endl;
				cout<< "current_time_ is : "<< current_time_<<endl;
				cout<< "next_slot is : "<< next_slot<<endl;*/

			if (pack->get_schedule_time() <= current_time_)
      {	
				if (Prob())
				{


        if((node1[j].get_status()==Wireless_node::IDLE())  && channelBusy[j]==0 && current_time_ == pack->get_schedule_time())
        {
          cout<< "i entered" <<endl;

          if (Ack && pkt.get_type()== Packet::MSG())
          {
            cout << " Packet: "<< pack ->get_id() << " is from node: " << pack ->get_from() << " to node: "<< pack ->get_to()<<endl;

              cout << "\n With start transmission time of the packet: "<< id << "   "<< node1[j].start_transmission[id] << endl;
              node1[j].attempt_transmission(pack, node1[j].start_transmission[id]);
              //node1[j].completed_transmission(pack, current_time_);
              node1[j].set_status(Wireless_node::SENDING());
              isBusy[from]=true;

              /*Check if its wired/wireless and assign channel as busy accordingly */

              for(int i=0; i< num_nodes; ++i){

                if (wire){

                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
                else{
                 if (dist0[from][i]<70){
                  //cout << "dist from "<< from <<" to node "<< i << dist0[from][i]<<endl;
                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
              }
            }
              

              node1[from].get_send_buff().push_back(pkt);
              cout<< "node "<< from<< "buffer send size: "<< node1[j].get_send_buff().size()<<endl;
              node1[to].get_receipt_buff().push_back(pkt);
              node1[j].get_endTransmission_buff().insert(node1[j].get_endTransmission_buff().begin()+id,
              node1[j].start_transmission[id]+ timeXPac);
              cout << "\n With end transmission time of first packet: "<< node1[j].end_transmission[id] << endl;
              node1[to].get_startReception_buff().insert(node1[to].get_startReception_buff().begin()+id, node1[j].end_transmission[id]+ I);
            
              //pack->set_timer(pack->get_expected_receive_time() * 2);
              //cout <<"\n  timer is set to " << pack->get_timer() << endl;
              theSimulation.Event_Scheduler (new end_sendEvent ( node1[j].end_transmission[id]));
              cout << "with ReceptionEvent at node: "<< to << "at time: "<< node1[to].start_reception[id] << endl;
              theSimulation.Event_Scheduler (new receptionEvent (node1[to].start_reception[id]));
              node1[j].getMyVec().erase(node1[j].getMyVec().begin());

          }//if msg
          else if(!Ack && pkt.get_type()== Packet::MSG()){

            cout << " Packet: "<< pack ->get_id() << " is from node: " << pack ->get_from() << " to node: "<< pack ->get_to()<<endl;
               /*node1[j].get_startTransmission_buff().insert(node1[j].get_startTransmission_buff().begin()+id, 
               node1[j].buff[0].get_schedule_time()+minCWindow);*/
  
              cout << "\n With start transmission time of the packet: "<< id << "   "<< node1[j].start_transmission[id] << endl;
              node1[j].attempt_transmission(pack, node1[j].start_transmission[id]);
              //node1[j].completed_transmission(pack, current_time_);
              
              node1[j].set_status(Wireless_node::SENDING());
              isBusy[from]=true;
              
               /*Check if its wired/wireless and assign channel as busy accordingly */

              for(int i=0; i< num_nodes; ++i){

                if (wire){

                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
                else{
                 if (dist0[from][i]<70){
                  //cout << "dist from "<< from <<" to node "<< i << dist0[from][i]<<endl;
                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
              }
            }


              node1[from].get_send_buff().push_back(pkt);
              cout<< "node "<< from<< "buffer send size: "<< node1[j].get_send_buff().size()<<endl;
              node1[to].get_receipt_buff().push_back(pkt);
              node1[j].get_endTransmission_buff().insert(node1[j].get_endTransmission_buff().begin()+id,
              node1[j].start_transmission[id]+ timeXPac);
              cout << "\n With end transmission time of first packet: "<< node1[j].end_transmission[id] << endl;
              cout<< "the I value is: "<< I <<endl;
              //node1[to].get_startReception_buff().insert(node1[to].get_startReception_buff().begin()+id, node1[j].start_transmission[id] + timeXPac + I );
              node1[to].get_startReception_buff().insert(node1[to].get_startReception_buff().begin()+id, node1[j].end_transmission[id] + I );
              cout << "node: " << to << " start_reception is: " << node1[to].start_reception[id] <<endl;
              //pack->set_timer(pack->get_expected_receive_time() * 2);
              //cout <<"\n  timer is set to " << pack->get_timer() << endl;
              theSimulation.Event_Scheduler (new end_sendEvent ( node1[j].end_transmission[id]));
              cout << "with ReceptionEvent at node: "<< to << "at time: "<< node1[to].start_reception[id] << endl;
              theSimulation.Event_Scheduler (new receptionEvent (node1[to].start_reception[id]));
              node1[j].getMyVec().erase(node1[j].getMyVec().begin());


          }
        if ( pkt.get_type()== Packet::ACK() && current_time_ == pack->get_schedule_time())
        {
          cout << "This is if we  DO NOT have ack mechanism or its an Ack packet" << endl;
          //node1[j].set_start_transmission( current_time_ + pkt.get_schedule_time());
          /*node1[j].get_startTransmission_buff().insert(node1[j].get_startTransmission_buff().begin()+id,  
            pkt.get_schedule_time());*/
          node1[j].attempt_transmission(pack,node1[j].start_transmission[id]);
          node1[j].get_send_buff().push_back(pkt);
          node1[j].set_status(Wireless_node::SENDING());
          isBusy[from]=true;
          
           /*Check if its wired/wireless and assign channel as busy accordingly */

              for(int i=0; i< num_nodes; ++i){

                if (wire){

                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
                else{
                 if (dist0[from][i]<70){
                  //cout << "dist from "<< from <<" to node "<< i << dist0[from][i]<<endl;
                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
              }
            }
          //node1[j].set_end_transmission((node1[j].start_transmission[id])+ 0.000008);
          node1[j].get_endTransmission_buff().insert(node1[j].get_endTransmission_buff().begin()+id,
              node1[j].start_transmission[id]+ timeXAck );
          node1[to].get_startReception_buff().insert(node1[to].get_startReception_buff().begin()+id, node1[j].start_transmission[id] + timeXAck + I );
          cout << "End_sendEvent happens at time: " << node1[j].start_transmission[id]+ timeXAck << endl;
          theSimulation.Event_Scheduler (new end_sendEvent ( node1[j].start_transmission[id]+ timeXAck));
          //node1[to].set_start_reception(node1[j].get_start_transmission() + pack->get_expected_receive_time());
          
          cout << "with ReceptionEvent at node: "<< to << "at time: "<< node1[to].start_reception[id] << endl;
          pack->set_timer(pack->get_expected_receive_time()+ I);
          cout <<"\n  timer is set to " << pack->get_timer() << endl;
          node1[to].get_receipt_buff().push_back(pkt);
          theSimulation.Event_Scheduler (new receptionEvent (node1[to].start_reception[id]));
          node1[j].getMyVec().erase(node1[j].getMyVec().begin());

        }
        

      } /*This is if the channel is busy and we try to send*/
      else if (node1[j].get_status() ==Wireless_node::IDLE()  && channelBusy[j] !=0 && current_time_ == pack->get_schedule_time())
      {
        
        cout<< "before Channel Busy" <<endl;
        Random_Number_Generator r;
        ++numCollisions;
        if(from==0){
          nodefile1.open("reward1.txt");
          nodefile1 << -1 <<'\n';
          
    }
      else if(from==1){
        nodefile2.open("reward2.txt");
        nodefile2 << -1 <<'\n';
    }

    else if (from==2){
        nodefile3.open("reward3.txt");
        nodefile3 << -1 <<'\n';
    }
    else if (from==3){
       nodefile4.open("reward4.txt");
       nodefile4 << -1 <<'\n';
      }

     else if (from==4){
      nodefile5.open("reward5.txt");
      nodefile5 << -1 <<'\n';
      }
        nodefile<<"node"<<j<<" :"<< -1 <<"for packet: " << pack->get_id()<<'\n';
        node1[j].getMyVec().erase(node1[j].getMyVec().begin());

        if (backoff){

           if (pack->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
            pack->set_retransmit_attempt(1);
            gen_rearrival(pack,  1+timeXPac);
          }
          else if (pack->get_retransmit_attempt() >= ret_threshold){
            //gen_arrival(from,to, 5);
          }
        }
        else{

            cout<< "retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
            if (pack->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
            pack->set_retransmit_attempt(1);
            gen_rearrival(pack,  timeXPac);
          }
          else if (pack->get_retransmit_attempt() >= ret_threshold){
            //gen_arrival(from,to, 5);
          }
        }

    
      }
      else if ( node1[j].get_status() !=Wireless_node::IDLE()
       && current_time_ == pack->get_schedule_time())
      {
        Random_Number_Generator r;

        cout<< "I'm Busy" <<endl;
        node1[j].getMyVec().erase(node1[j].getMyVec().begin());
        if (pack->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
            pack->set_retransmit_attempt(1);
            gen_rearrival(pack,  1+timeXPac);
          }
      }




		}/* if prob() end*/
		else
		{
      nodefile << 0 << '\n';
      if(from==0){
          nodefile1.open("reward1.txt");
          nodefile1 << 0 <<'\n';
          
    }
      else if(from==1){
        nodefile2.open("reward2.txt");
        nodefile2 << 0 <<'\n';
    }

    else if (from==2){
        nodefile3.open("reward3.txt");
        nodefile3 << 0 <<'\n';
    }
    else if (from==3){
       nodefile4.open("reward4.txt");
       nodefile4 << 0 <<'\n';
      }

     else if (from==4){
      nodefile5.open("reward5.txt");
      nodefile5 << 0 <<'\n';
      }
      nodefile<<"node"<<j<<" :"<< 0 <<"for packet: " << pack->get_id()<<'\n';
			node1[j].getMyVec().erase(node1[j].getMyVec().begin());
       if (pack->get_retransmit_attempt() < 24){
            cout<< "retransmit attempt of packet "<< id << "is " << pack->get_retransmit_attempt() <<endl;
            pack->set_retransmit_attempt(1);
            gen_rearrival(pack, slt_duration*(rand_CW(pack->get_retransmit_attempt())));
          }
		}
  }// if packet schedule == current

		/*} else when it is in range*/
  }//for the packets in buffer
		}/*if buffer empty*/
	}//for nodes
}



void end_sendEvent::processEvent()
{
	cout << "- - - - - - - end_sendEvent Happened- - - - - - - - "<<endl;
	Packet pkt;
	Packet *pack;
	for (int j = 0; j<num_nodes; ++j)
	{
		int count=0;
		cout << "node "<< j << " buffer size : " << node1[j].get_send_buff().size()<<endl;
		if (!node1[j].send_buff.empty())
		{
			
		for (int n1=0; n1< node1[j].send_buff.size(); ++n1)
		{	
			pkt= node1[j].send_buff[n1];
			pack= &pkt;
			int to = pack->get_to();
			int from = pack->get_from();
			int id = pack->get_id();
			//cout<< "pack id " << id << " node1[j].end_transmission[id] "<< node1[j].end_transmission[id]<<endl;

			if (node1[j].end_transmission[id]== current_time_)
			{

			
			node1[j].get_send_buff().erase(node1[j].get_send_buff().begin());

	
		if(node1[j].get_status()== Wireless_node::SENDING() || node1[j].get_status()== Wireless_node::IDLE())
			{

			node1[j].set_status(Wireless_node::IDLE());
			isBusy[j]=false;
		
       /*Check if its wired/wireless and assign channel as busy accordingly */

              for(int i=0; i< num_nodes; ++i){

                if (wire){

                  m.lock();
                  --channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
                else{
                 if (dist0[from][i]<70){
                  //cout << "dist from "<< from <<" to node "<< i << dist0[from][i]<<endl;
                  m.lock();
                  --channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
              }
            }

			cout << "Packet with type: " << pkt.get_type() << " is sent from: "<< pkt.get_from() << " to: " << pkt.get_to() <<endl;
			long double event_time = node1[j].start_transmission[id];

			cout << "node "<< j << " status changes from SENDING to IDLE"<<endl;
			cout << "node "<< from << " buffer size is: " << node1[j].getMyVec().size()<<endl;
			if (! node1[from].getMyVec().empty())
			{
				cout << "node "<< from << "Buffer has: "<< node1[from].buff.size()<<" more packet(s) to send\n So call SendEvent at "<<node1[to].start_reception[id]+ 0.0000008 + pack->get_expected_receive_time() <<endl;
				//theSimulation.Event_Scheduler (new sendEvent (node1[to].start_reception[id]+ 0.000008 + pack->get_expected_receive_time()));
			}
			if ( ! node1[to].getMyVec().empty())
			{	
				cout << "node "<< to << "Buffer has: "<< node1[to].buff.size()<<" more packet(s) to send\n So call SendEvent at "<< node1[from].start_reception[id]+ 0.0000008 + pack->get_expected_receive_time()<<endl;
				//theSimulation.Event_Scheduler (new sendEvent (node1[from].start_reception[id]+ 0.000008 + pack->get_expected_receive_time()));
			}
		}
		} /*if transmission is current*/
		}/*for buff size*/
}//if ! empty
}//for nodes

}//void

void receptionEvent::processEvent()
{	
	Wireless_node node_;
	Packet *pack=NULL;
	Packet pkt;
	cout << "- - - - - - - receptionEvent Happened- - - - - - - - "<<endl;

	for (int j = 0; j < num_nodes; ++j)
	{

		cout<< "node "<< j<< " buffer size is "<< node1[j].receipt_buff.size()<< endl;
	
		if( !node1[j].receipt_buff.empty() && node1[j].get_status() != Wireless_node::COLLIDED())
		{
			

			for (int n = 0; n < node1[j].receipt_buff.size(); ++n)
			{
			
			pkt= node1[j].receipt_buff[n];
			pack= &pkt;
			int t = pack-> get_to();
			int from =pack->get_from();
			int id = pack->get_id();
			cout<<"packet with id "<< pack->get_id() <<endl;
      cout<<"reception is "<< node1[t].start_reception[id]<<endl;
      cout<<"current_time_ is "<< current_time_ <<endl;

      if (pack->get_type()==Packet::ACK() && node1[t].start_reception[id] == current_time_){
        cout <<"timer is: " << pack->get_timer() << endl;
        cout<<"node reception is "<< node1[t].start_reception[id]<<endl;
      }

      if (!wire){
			if (dist0[from][t] > 70)
			{
				cout << "Node: "<< t << "out of range, retransmit" <<endl;

			}
			if (from != j && t!=j && dist0[from][j] < 70)
			{
        if (node1[j].get_status()== Wireless_node::RECEIVING() || node1[j].get_status()== Wireless_node::SENDING())
        {
          ++numCollisions;
          node1[j].set_status(Wireless_node::RECEIVING());
        }
        else
				  node1[j].set_status(Wireless_node::RECEIVING());
			}

    } /*if !wire*/

      if (node1[t].start_reception[id] == current_time_ /*&& pack->get_timer() <= current_time_*/){
        cout << "equal! can enter"<<endl;
      }
			if( /*dist0[from][t] < 70 && */ channelBusy[from]==0 && node1[t].start_reception[id] == current_time_
			 && (node1[t].get_status()== Wireless_node::IDLE())){
				
         /*Check if its wired/wireless and assign channel as busy accordingly */

              for(int i=0; i< num_nodes; ++i){

                if (wire){

                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
                else{
                 if (dist0[from][i]<70){
                  //cout << "dist from "<< from <<" to node "<< i << dist0[from][i]<<endl;
                  m.lock();
                  ++channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
              }
            }
			
				if(pack->get_type()==Packet::ACK())
				{
					cout << "get_start_reception of node:" << t <<" from: " << from << " is: "<< node1[t].start_reception[id] <<endl;
					/*cout <<"get_timer: "<< node1[t].get_timer() *10000<<endl;
					if (node1[t].start_reception[id] <= node1[t].get_timer() * 10000)
					{*/
						node1[t].set_status(Wireless_node::RECEIVING());
						cout << "node: "<< t << " receives an Ack before timeout: status changes From IDLE to receiving" <<endl;
						cout << "at time: "<< node1[t].start_reception[id] <<endl;

						long double eventtime= node1[t].start_reception[id];
						node1[t].set_num_receiving_packs(1);
						node1[t].get_endReception_buff().insert(node1[t].get_endReception_buff().begin()+id, eventtime + timeXAck );
						//node1[t].set_end_reception (eventtime + 0.000008);
						cout << "end_receptionEvent happens at time: "<< node1[t].end_reception[id] << endl;
						theSimulation.Event_Scheduler (new end_receptionEvent (node1[t].end_reception[id]));
					//}

					/*else
					{
						cout << "timeout"<<endl;
						//node1[t].getMyVec().push_back (node1[t].create_new_msg_packet(t, from, current_time_));
					}*/
				}
				if(pack->get_type()==Packet::MSG())
				{
					node1[t].set_status(Wireless_node::RECEIVING());
					cout << "node: "<< t << " status changes From IDLE to receiving" <<endl;
					cout << "at time: "<< node1[t].start_reception[id] <<endl;

					long double eventtime= node1[t].start_reception[id];
					node1[t].set_num_receiving_packs(1);
					//node1[t].set_end_reception (eventtime + 0.000008);
					node1[t].get_endReception_buff().insert(node1[t].get_endReception_buff().begin()+id, eventtime + timeXPac );
					cout << "end_receptionEvent happens at time: "<< node1[t].end_reception[id] << endl;
					theSimulation.Event_Scheduler (new end_receptionEvent (node1[t].end_reception[id]));
				}


			} /*if is busy*/

			else if ( node1[t].start_reception[id] == current_time_ /*&& dist0[from][t] < 70 */ && channelBusy[j]>0)
		    {
			++numCollisions;
      if(from==0){
          nodefile1.open("reward1.txt");
          nodefile1 << -1 <<'\n';
          
    }
      else if(from==1){
        nodefile2.open("reward2.txt");
        nodefile2 << -1 <<'\n';
    }

    else if (from==2){
        nodefile3.open("reward3.txt");
        nodefile3 << -1 <<'\n';
    }
    else if (from==3){
       nodefile4.open("reward4.txt");
       nodefile4 << -1 <<'\n';
      }

     else if (from==4){
      nodefile5.open("reward5.txt");
      nodefile5 << -1 <<'\n';
      }
      nodefile<<"node"<<t<<" :"<< -1 <<"for packet: " << pkt.get_id()<<'\n';
			node1[t].get_collidedReceipt_buff().push_back(pkt);
			//node1[j].receipt_buff.erase(node1[j].receipt_buff.begin());
			//node1[j].receipt_buff.erase(node1[j].receipt_buff.begin());
		    }


			}
		}
		
	}/*if ! empty*/


}

void end_receptionEvent::processEvent()
{	
  
  int cnt;
	Packet pkt;
	Packet *pack;
	Packet *packt;
	cout << "- - - - - - - end_receptionEvent Happened- - - - - - - - "<<endl;

double end_time = current_time_;
for (int j = 0; j < num_nodes; ++j)
{

	if (! node1[j].receipt_buff.empty() && node1[j].get_status() != Wireless_node::COLLIDED())
	{
		for (int n = 0; n < node1[j].receipt_buff.size(); ++n)
			{

				pkt= node1[j].receipt_buff[n];
				pack= &pkt;
				int t= pack-> get_to();
				int from =pack->get_from();
				int id = pack->get_id();

        if(!wire){
				if (from != j && t!=j && dist0[from][j] < 70 )
				{
					node1[j].set_status(Wireless_node::IDLE());
				}
      }
      else
      {
        node1[j].set_status(Wireless_node::IDLE());
      }

				if(node1[t].get_status()== Wireless_node::RECEIVING() || node1[t].get_status()== Wireless_node::IDLE()
					&& node1[t].end_reception[id] == current_time_)
				{
					cout << "node: "<< t << " status changes From RECEIVING to IDLE" <<endl;
					
           /*Check if its wired/wireless and assign channel as busy accordingly */

              for(int i=0; i< num_nodes; ++i){

                if (wire){

                  m.lock();
                  --channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
                else{
                 if (dist0[from][i]<70){
                  //cout << "dist from "<< from <<" to node "<< i << dist0[from][i]<<endl;
                  m.lock();
                  --channelBusy[i];
                  cout<< "send ev: channel busy for node "<<i<<"with value "<< channelBusy[i]<<endl;
                  accessCount++;
                  m.unlock();
                }
              }
            }

					node1[t].set_status(Wireless_node::IDLE());
					node1[j].get_receipt_buff().erase(node1[j].get_receipt_buff().begin());

					if (pack->get_type()==Packet::MSG())
					{
            if (!noise){

              node1[j].completed_transmission(pack, node1[t].end_reception[id]);
              
              rew[cnt++]=1;
              if(from==0){
          nodefile1.open("reward1.txt");
          nodefile1 << 1 <<'\n';
          
    }
      else if(from==1){
        nodefile2.open("reward2.txt");
        nodefile2 << 1 <<'\n';
    }

    else if (from==2){
        nodefile3.open("reward3.txt");
        nodefile3 << 1 <<'\n';
    }
    else if (from==3){
       nodefile4.open("reward4.txt");
       nodefile4 << 1 <<'\n';
      }

     else if (from==4){
      nodefile5.open("reward5.txt");
      nodefile5 << 1 <<'\n';
      }
              nodefile<<"node"<<j<<" :"<< 1 <<"for packet: " << pack->get_id()<<'\n';
              //nodefile << j <<'\n';
            }
            if (noise && trans_r){
              if (uni() <= 0.8){
                  node1[j].completed_transmission(pack, node1[t].end_reception[id]);
                  if(from==0){
                    nodefile1.open("reward1.txt");
                    nodefile1 << 1 <<'\n';
                      
                }
                  else if(from==1){
                    nodefile2.open("reward2.txt");
                    nodefile2 << 1 <<'\n';
                }

                else if (from==2){
                    nodefile3.open("reward3.txt");
                    nodefile3 << 1 <<'\n';
                }
                else if (from==3){
                   nodefile4.open("reward4.txt");
                   nodefile4 << 1 <<'\n';
                  }

                 else if (from==4){
                  nodefile5.open("reward5.txt");
                  nodefile5 << 1 <<'\n';
                  }
                  rew[cnt++]=1;
                  nodefile<<"node"<<j<<" :"<< 1 <<"for packet: " << pack->get_id()<<'\n';
                  //nodefile << 1 <<'\n';
                  //nodefile << j <<'\n';
              }
              else{
                //nodefile.open ("reward_node.txt",fstream::app);
                  rew[cnt++]=0;
                  if(from==0){
                    nodefile1.open("reward1.txt");
                    nodefile1 << -1 <<'\n';
                      
                }
                  else if(from==1){
                    nodefile2.open("reward2.txt");
                    nodefile2 << -1 <<'\n';
                }

                else if (from==2){
                    nodefile3.open("reward3.txt");
                    nodefile3 << -1 <<'\n';
                }
                else if (from==3){
                   nodefile4.open("reward4.txt");
                   nodefile4 << -1 <<'\n';
                  }

                 else if (from==4){
                  nodefile5.open("reward5.txt");
                  nodefile5 << -1 <<'\n';
                  }
                  nodefile<<"node"<<j<<" :"<< -1 <<"for packet: " << pack->get_id()<<'\n';
                  //nodefile << -1 <<'\n';
                  //nodefile << j <<'\n';
              }


            }
            else if (noise && !trans_r){
              if (uni() <= 0.4){
                  node1[j].completed_transmission(pack, node1[t].end_reception[id]);
                  //nodefile.open ("reward_node.txt",fstream::app);
                  rew[cnt++]=1;
                  if(from==0){
                    nodefile1.open("reward1.txt");
                    nodefile1 << 1 <<'\n';
                      
                }
                  else if(from==1){
                    nodefile2.open("reward2.txt");
                    nodefile2 << 1 <<'\n';
                }

                else if (from==2){
                    nodefile3.open("reward3.txt");
                    nodefile3 << 1 <<'\n';
                }
                else if (from==3){
                   nodefile4.open("reward4.txt");
                   nodefile4 << 1 <<'\n';
                  }

                 else if (from==4){
                  nodefile5.open("reward5.txt");
                  nodefile5 << 1 <<'\n';
                  }
                  nodefile<<"node"<<j<<" :"<< 1 <<"for packet: " << pack->get_id()<<'\n';
                  //nodefile << j <<'\n';
              }
            else{
                //nodefile.open ("reward_node.txt",fstream::app);
                  rew[cnt++]=0;
                  if(from==0){
                    nodefile1.open("reward1.txt");
                    nodefile1 << -1 <<'\n';
                      
                }
                  else if(from==1){
                    nodefile2.open("reward2.txt");
                    nodefile2 << -1 <<'\n';
                }

                else if (from==2){
                    nodefile3.open("reward3.txt");
                    nodefile3 << -1 <<'\n';
                }
                else if (from==3){
                   nodefile4.open("reward4.txt");
                   nodefile4 << -1 <<'\n';
                  }

                 else if (from==4){
                  nodefile5.open("reward5.txt");
                  nodefile5 << -1 <<'\n';
                  }
                  //nodefile <<-1 <<'\n';
                  nodefile<<"node"<<j<<" :"<< -1 <<"for packet: " << pack->get_id()<<'\n';
              }

            }
						cout<< "This is a MSG Packet that needs an ack\n";
						cout<< "This packet is from: "<< pack->get_from()<< " to: "<< pack->get_to() <<endl;

						if (Ack)
						{
						
						node1[t].getMyVec().push_back(node1[t].create_new_ack_packet(pack->get_to(), pack->get_from(), end_time, pack->get_id()));
						//pkt.set_type(Packet::ACK());
						//packt= &node1[t].getMyVec().front();
				
						cout<< "node1[t].end_reception[id]: "<< node1[t].end_reception[id] <<endl;
						if(protocol){

							//theSimulation.Event_Scheduler (new sendEvent ( end_time ));
              gen_arrival(from,t,end_time);
						}
						else if (!protocol){

              gen_arrival(from,t,end_time);
							//theSimulation.Event_Scheduler (new slotted_sendEvent ( end_time ));
						}
						}
						else if (!Ack)
						{
							if(protocol){

								//theSimulation.Event_Scheduler (new sendEvent ( end_time));
                gen_arrival(from,t,end_time);
							}
							else if (!protocol){

                gen_arrival(from,t,end_time);
								//theSimulation.Event_Scheduler (new slotted_sendEvent ( end_time));
							}

						}
				}

				else if (pack->get_type()==Packet::ACK())
				{

					for(int i=0; i< num_nodes ; ++i){
						if (!node1[j].getMyVec().empty()){
							cout<< "dont forget me ! call send !"<< endl;
              gen_arrival(t,from,end_time);

						}
           
					}
				}
		}

	}/*for loop*/

}/*if !empty*/
else if ( !node1[j].get_collidedReceipt_buff().empty())
{

  //nodefile.open ("reward_node.txt",fstream::app);
  rew[cnt++]=0;
  //nodefile << -1 <<'\n';
  nodefile<<"node"<<j<<" :"<< -1 <<"for packet: " << pack->get_id()<<'\n';

	Packet *pkt;
	pkt= &node1[j].get_collidedReceipt_buff().front();
  int id = pkt->get_id();
	cout <<"some packet at collided buffer"<<endl;
   if (pkt->get_retransmit_attempt() < ret_threshold){
            cout<< "retransmit attempt of packet "<< id << "is " << pkt->get_retransmit_attempt() <<endl;
            pkt->set_retransmit_attempt(1);
            gen_rearrival(pkt, slt_duration*(rand_CW(pack->get_retransmit_attempt())));
          }
}
}
total_t_=current_time_; /*get total simulation time*/
}
void statistics()
{
cout << "Total Simulation Time " << total_t_ << endl;
	double current_time_=total_t_;
	double t = 0.0, a = 0.0, th = 0.0, d = 0.0;
	for (int i=0; i < num_nodes; i++)
	{
		Wireless_node *n = &node1[i];
		t += n->get_completed_transmissions();
		a += n->get_attempted_transmissions();
		//th += n->get_efficiency();
		d += abs (n->get_average_delay());
	}
  th= (t) /30;
  //ofstream myfile;
  
  
  myfile2 << th <<'\n';
  //myfile2.close();

  //myfile.open ("reward.txt",fstream::app);
  //myfile << th/num_nodes<<'\n';
  //myfile.close();
  myfile << 0.25 * (-numCollisions) + t * 0.75<<","<<'\n';

	cout << " [Completed transmissions]" << t << " [# Transmission attempts]" << a
			<< " [Throughput]" << /*(t)*timeXPac / 50.0 */ th/num_nodes <<  " [Average Delay]"  << d/num_nodes << "\n number of collisions:"
			<< numCollisions <<endl;

}

void collisionEvent::processEvent()
{

	Packet pkt;
	Packet *pack;
	//long double next_time=10000;
	cout << "- - - - - - - collisionEvent Happened- - - - - - - - "<<endl;
	for (int j = 0; j<3 ; ++j)
	{
	while (!(node1[j].get_gen_buff().empty())){	
		//if(node1[j].get_status()== Wireless_node::SENDING() && node1[j].start_reception[j] < node1[j].get_end_transmission() )
			{	
				pkt=(node1[j].getMyVec().front());
				pack= &pkt;
				node1[j].collision(pack, node1[j].start_transmission[0]);
			}
		if( node1[j].get_status()== Wireless_node::RECEIVING() && node1[j].get_num_receiving_packs() >1 )
			{	
				pkt=(node1[j].getMyVec().front());
				pack= &pkt;
				//node1[j].collision(pack, node1[j].get_start_reception());
			}	
	}
}
}

void AlohaSimulator::send(){

    std::cout << " [[SEND]] At Time: " << current_time << '\n';
}



Coordinates rand_coordinates(int N)
{	

    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(0, 100);
    int a= dis(gen);
    int b= dis(gen);
    Coordinates result= {a,b};
    return result;
}

void Wireless_node::init_simulator()
{	
	
	Random_Number_Generator r;
	Wireless_node *node=NULL;
	Coordinates input;
	list_nodes.clear();
	Packet pack;

	node = new Wireless_node;

	for (int i = 0; i < num_nodes; ++i)
	{	
		input = rand_coordinates(130);
		node1[i].x = input.value1;
		node1[i].y = input.value2;
		node1[i].set_id(i);
		node1[i].set_transmission_rate(r.get_rate());
		node1[i].initialize();

		//cout <<"node with id: " <<	node1[i].id_ << "\t Coordinates are:" << node1[i].x <<"," << node1[i].y <<"\n";
	}
	//long double dist0[num_nodes][num_nodes];


	               /*Distance Calculation*/
	/*for (int k = 0; k < num_nodes; ++k)

	{
		for (int i = 0; i < num_nodes; ++i)
		{
			dist0[k][i]= node1[k].distance(node1[k].x, node1[k].y, node1[i].x, node1[i].y);
			//cout<< "we are calculating the distance of dist0[k][i]: " << dist0[k][i] <<endl;
			int disThis = dist0[k][i];
      myfiledis <<"dist0["<<k<<"]["<<i<<"]="<<disThis<<";"<<'\n';
			node1[k].getMyVecDist().push_back(disThis);
		}
		
		
	}
		dist0[0][0]=0;
		dist0[1][1]=0;
		dist0[2][2]=0;
		dist0[3][3]=0;
		dist0[4][4]=0;
		dist0[0][1]=50;
		dist0[1][0]=50;
		dist0[0][2]=60;
		dist0[2][0]=60;
		dist0[0][3]=110;
		dist0[3][0]=110;
		dist0[0][4]=120;
		dist0[4][0]=120;
		dist0[1][2]=40;
		dist0[2][1]=40;
		dist0[1][3]=110;
		dist0[3][1]=110;
		dist0[1][4]=120;
		dist0[4][1]=120;
		dist0[2][3]=110;
		dist0[3][2]=110;
		dist0[3][4]=50;
		dist0[4][3]=50;
		dist0[4][2]=110;
		dist0[2][4]=110;*/


    dist0[0][0]=0;
dist0[0][1]=50;
dist0[0][2]=60;
dist0[0][3]=110;
dist0[0][4]=120;
dist0[0][5]=59;
dist0[0][6]=18;
dist0[0][7]=39;
dist0[0][8]=59;
dist0[0][9]=35;
dist0[0][10]=7;
dist0[0][11]=41;
dist0[0][12]=48;
dist0[0][13]=40;
dist0[0][14]=21;
dist0[0][15]=28;
dist0[0][16]=55;
dist0[0][17]=64;
dist0[0][18]=78;
dist0[0][19]=40;
dist0[1][0]=50;
dist0[1][1]=0;
dist0[1][2]=50;
dist0[1][3]=110;
dist0[1][4]=120;
dist0[1][5]=71;
dist0[1][6]=26;
dist0[1][7]=13;
dist0[1][8]=35;
dist0[1][9]=23;
dist0[1][10]=24;
dist0[1][11]=3;
dist0[1][12]=32;
dist0[1][13]=74;
dist0[1][14]=16;
dist0[1][15]=50;
dist0[1][16]=57;
dist0[1][17]=62;
dist0[1][18]=47;
dist0[1][19]=25;
dist0[2][0]=60;
dist0[2][1]=50;
dist0[2][2]=0;
dist0[2][3]=110;
dist0[2][4]=110;
dist0[2][5]=16;
dist0[2][6]=18;
dist0[2][7]=64;
dist0[2][8]=47;
dist0[2][9]=61;
dist0[2][10]=49;
dist0[2][11]=56;
dist0[2][12]=58;
dist0[2][13]=23;
dist0[2][14]=64;
dist0[2][15]=20;
dist0[2][16]=0;
dist0[2][17]=9;
dist0[2][18]=29;
dist0[2][19]=38;
dist0[3][0]=110;
dist0[3][1]=110;
dist0[3][2]=110;
dist0[3][3]=0;
dist0[3][4]=27;
dist0[3][5]=57;
dist0[3][6]=11;
dist0[3][7]=54;
dist0[3][8]=67;
dist0[3][9]=22;
dist0[3][10]=14;
dist0[3][11]=54;
dist0[3][12]=22;
dist0[3][13]=39;
dist0[3][14]=35;
dist0[3][15]=22;
dist0[3][16]=51;
dist0[3][17]=60;
dist0[3][18]=74;
dist0[3][19]=51;
dist0[4][0]=120;
dist0[4][1]=120;
dist0[4][2]=110;
dist0[4][3]=27;
dist0[4][4]=0;
dist0[4][5]=28;
dist0[4][6]=23;
dist0[4][7]=71;
dist0[4][8]=66;
dist0[4][9]=63;
dist0[4][10]=29;
dist0[4][11]=66;
dist0[4][12]=58;
dist0[4][13]=0;
dist0[4][14]=67;
dist0[4][15]=4;
dist0[4][16]=23;
dist0[4][17]=32;
dist0[4][18]=46;
dist0[4][19]=54;
dist0[5][0]=59;
dist0[5][1]=71;
dist0[5][2]=16;
dist0[5][3]=57;
dist0[5][4]=28;
dist0[5][5]=0;
dist0[5][6]=11;
dist0[5][7]=85;
dist0[5][8]=63;
dist0[5][9]=89;
dist0[5][10]=58;
dist0[5][11]=76;
dist0[5][12]=89;
dist0[5][13]=16;
dist0[5][14]=91;
dist0[5][15]=30;
dist0[5][16]=12;
dist0[5][17]=7;
dist0[5][18]=5;
dist0[5][19]=54;
dist0[6][0]=18;
dist0[6][1]=26;
dist0[6][2]=18;
dist0[6][3]=11;
dist0[6][4]=23;
dist0[6][5]=11;
dist0[6][6]=0;
dist0[6][7]=40;
dist0[6][8]=43;
dist0[6][9]=0;
dist0[6][10]=25;
dist0[6][11]=39;
dist0[6][12]=27;
dist0[6][13]=31;
dist0[6][14]=23;
dist0[6][15]=17;
dist0[6][16]=22;
dist0[6][17]=29;
dist0[6][18]=50;
dist0[6][19]=30;
dist0[7][0]=39;
dist0[7][1]=13;
dist0[7][2]=64;
dist0[7][3]=54;
dist0[7][4]=71;
dist0[7][5]=85;
dist0[7][6]=40;
dist0[7][7]=0;
dist0[7][8]=40;
dist0[7][9]=25;
dist0[7][10]=19;
dist0[7][11]=8;
dist0[7][12]=33;
dist0[7][13]=88;
dist0[7][14]=20;
dist0[7][15]=63;
dist0[7][16]=71;
dist0[7][17]=77;
dist0[7][18]=64;
dist0[7][19]=22;
dist0[8][0]=59;
dist0[8][1]=35;
dist0[8][2]=47;
dist0[8][3]=67;
dist0[8][4]=66;
dist0[8][5]=63;
dist0[8][6]=43;
dist0[8][7]=40;
dist0[8][8]=0;
dist0[8][9]=65;
dist0[8][10]=50;
dist0[8][11]=29;
dist0[8][12]=73;
dist0[8][13]=76;
dist0[8][14]=61;
dist0[8][15]=61;
dist0[8][16]=50;
dist0[8][17]=50;
dist0[8][18]=15;
dist0[8][19]=8;
dist0[9][0]=35;
dist0[9][1]=23;
dist0[9][2]=61;
dist0[9][3]=22;
dist0[9][4]=63;
dist0[9][5]=89;
dist0[9][6]=0;
dist0[9][7]=25;
dist0[9][8]=65;
dist0[9][9]=0;
dist0[9][10]=51;
dist0[9][11]=35;
dist0[9][12]=7;
dist0[9][13]=86;
dist0[9][14]=3;
dist0[9][15]=51;
dist0[9][16]=72;
dist0[9][17]=81;
dist0[9][18]=73;
dist0[9][19]=50;
dist0[10][0]=7;
dist0[10][1]=24;
dist0[10][2]=49;
dist0[10][3]=14;
dist0[10][4]=29;
dist0[10][5]=58;
dist0[10][6]=25;
dist0[10][7]=19;
dist0[10][8]=50;
dist0[10][9]=51;
dist0[10][10]=0;
dist0[10][11]=26;
dist0[10][12]=61;
dist0[10][13]=37;
dist0[10][14]=42;
dist0[10][15]=27;
dist0[10][16]=56;
dist0[10][17]=64;
dist0[10][18]=79;
dist0[10][19]=29;
dist0[11][0]=41;
dist0[11][1]=3;
dist0[11][2]=56;
dist0[11][3]=54;
dist0[11][4]=66;
dist0[11][5]=76;
dist0[11][6]=39;
dist0[11][7]=8;
dist0[11][8]=29;
dist0[11][9]=35;
dist0[11][10]=26;
dist0[11][11]=0;
dist0[11][12]=43;
dist0[11][13]=80;
dist0[11][14]=30;
dist0[11][15]=59;
dist0[11][16]=63;
dist0[11][17]=67;
dist0[11][18]=50;
dist0[11][19]=13;
dist0[12][0]=48;
dist0[12][1]=32;
dist0[12][2]=58;
dist0[12][3]=22;
dist0[12][4]=58;
dist0[12][5]=89;
dist0[12][6]=27;
dist0[12][7]=33;
dist0[12][8]=73;
dist0[12][9]=7;
dist0[12][10]=61;
dist0[12][11]=43;
dist0[12][12]=0;
dist0[12][13]=84;
dist0[12][14]=11;
dist0[12][15]=44;
dist0[12][16]=71;
dist0[12][17]=81;
dist0[12][18]=74;
dist0[12][19]=58;
dist0[13][0]=40;
dist0[13][1]=74;
dist0[13][2]=23;
dist0[13][3]=39;
dist0[13][4]=0;
dist0[13][5]=16;
dist0[13][6]=31;
dist0[13][7]=88;
dist0[13][8]=76;
dist0[13][9]=86;
dist0[13][10]=37;
dist0[13][11]=80;
dist0[13][12]=84;
dist0[13][13]=0;
dist0[13][14]=89;
dist0[13][15]=8;
dist0[13][16]=12;
dist0[13][17]=16;
dist0[13][18]=26;
dist0[13][19]=65;
dist0[14][0]=21;
dist0[14][1]=16;
dist0[14][2]=64;
dist0[14][3]=35;
dist0[14][4]=67;
dist0[14][5]=91;
dist0[14][6]=23;
dist0[14][7]=20;
dist0[14][8]=61;
dist0[14][9]=3;
dist0[14][10]=42;
dist0[14][11]=30;
dist0[14][12]=11;
dist0[14][13]=89;
dist0[14][14]=0;
dist0[14][15]=56;
dist0[14][16]=75;
dist0[14][17]=83;
dist0[14][18]=74;
dist0[14][19]=45;
dist0[15][0]=28;
dist0[15][1]=50;
dist0[15][2]=20;
dist0[15][3]=22;
dist0[15][4]=4;
dist0[15][5]=30;
dist0[15][6]=17;
dist0[15][7]=63;
dist0[15][8]=61;
dist0[15][9]=51;
dist0[15][10]=27;
dist0[15][11]=59;
dist0[15][12]=44;
dist0[15][13]=8;
dist0[15][14]=56;
dist0[15][15]=0;
dist0[15][16]=27;
dist0[15][17]=36;
dist0[15][18]=50;
dist0[15][19]=48;
dist0[16][0]=55;
dist0[16][1]=57;
dist0[16][2]=0;
dist0[16][3]=51;
dist0[16][4]=23;
dist0[16][5]=12;
dist0[16][6]=22;
dist0[16][7]=71;
dist0[16][8]=50;
dist0[16][9]=72;
dist0[16][10]=56;
dist0[16][11]=63;
dist0[16][12]=71;
dist0[16][13]=12;
dist0[16][14]=75;
dist0[16][15]=27;
dist0[16][16]=0;
dist0[16][17]=6;
dist0[16][18]=22;
dist0[16][19]=41;
dist0[17][0]=64;
dist0[17][1]=62;
dist0[17][2]=9;
dist0[17][3]=60;
dist0[17][4]=32;
dist0[17][5]=7;
dist0[17][6]=29;
dist0[17][7]=77;
dist0[17][8]=50;
dist0[17][9]=81;
dist0[17][10]=64;
dist0[17][11]=67;
dist0[17][12]=81;
dist0[17][13]=16;
dist0[17][14]=83;
dist0[17][15]=36;
dist0[17][16]=6;
dist0[17][17]=0;
dist0[17][18]=9;
dist0[17][19]=42;
dist0[18][0]=78;
dist0[18][1]=47;
dist0[18][2]=29;
dist0[18][3]=74;
dist0[18][4]=46;
dist0[18][5]=5;
dist0[18][6]=50;
dist0[18][7]=64;
dist0[18][8]=15;
dist0[18][9]=73;
dist0[18][10]=79;
dist0[18][11]=50;
dist0[18][12]=74;
dist0[18][13]=26;
dist0[18][14]=74;
dist0[18][15]=50;
dist0[18][16]=22;
dist0[18][17]=9;
dist0[18][18]=0;
dist0[18][19]=22;
dist0[19][0]=40;
dist0[19][1]=25;
dist0[19][2]=38;
dist0[19][3]=51;
dist0[19][4]=54;
dist0[19][5]=54;
dist0[19][6]=30;
dist0[19][7]=22;
dist0[19][8]=8;
dist0[19][9]=50;
dist0[19][10]=29;
dist0[19][11]=13;
dist0[19][12]=58;
dist0[19][13]=65;
dist0[19][14]=45;
dist0[19][15]=48;
dist0[19][16]=41;
dist0[19][17]=42;
dist0[19][18]=22;
dist0[19][19]=0;

    


				  /*Propagation Calculation*/
	 //prop[num_nodes][num_nodes];
	for (int k = 0; k < num_nodes; ++k)

	{
		for (int i = 0; i < num_nodes; ++i)
		{
			prop[k][i]=dist0[k][i]/propDelay;
			//cout<< "we are calculating distance: " << "["<< k<< "]"<< "["<<i<<"] "<< prop[k][i] <<endl;
		}
		
		
	}
	for (int b = 0; b<200; b++){
    Random_Number_Generator r; 
	 gen_arrival(0,1,0);
	 gen_arrival(1,0,0);
	 gen_arrival(2,3,0);
	 /*gen_arrival(3,4,0);
	 gen_arrival(4,0,0);
	/*gen_arrival(2,3,0);
	gen_arrival(3,1,0);
	gen_arrival(4,5,0);
	gen_arrival(5,6,0);
	gen_arrival(6,7,0);
	gen_arrival(7,8,0);
	gen_arrival(8,9,0);
	gen_arrival(9,10,0);
	gen_arrival(10,11,0);
	gen_arrival(11,9,0);
	gen_arrival(12,10,0);
	gen_arrival(13,8,0);
	gen_arrival(14,2,0);
	gen_arrival(15,3,0);
	gen_arrival(16,7,0);
	gen_arrival(17,1,0);
	gen_arrival(18,5,0);
	//gen_arrival(0,1);*/
    }
}
void gen_arrival(int from, int to, long double current)
  {
   /* This function assigns a time to an arrival based on the nodal 
    * arrival rate and the poisson assumption.  It also fills the 
    * required fields of the structure  
    */

    long double timeArrival;


          /*select a random value for prob*/
          random_device rd;  //Will be used to obtain a seed for the random number engine
          mt19937 gen(rd());
          uniform_real_distribution<> dis(0,4);
          int y= dis(gen);
          //map_to_prob(y);
          nextarrival();                     /* Calculate a poisson arrival time */
          timeArrival = timeNextEvent;       /* Assign arrival time (poisson)    */
          cout<< "time Next Event Arrival is: "<< timeArrival<<endl;
          cout<< "prob error is: "<< probError<<endl;
          node1[from].getMyVec().push_back(node1[from].create_new_msg_packet(from,to,timeArrival));
   
       
  } /* End of gen_arrival() */

void gen_rearrival (Packet *pack, double current)
  {
   /* This function assigns a time to an rearrival based on the nodal 
    * arrival rate and the poisson assumption.  It also fills the 
    * required fields of the structure  
    */

   long double timeArrival;
   Random_Number_Generator r;
   int from = pack->get_from();
   int to = pack->get_to();
   int id = pack->get_id();
   nextarrival();                               /* Calculate a poisson arrival time */
  
   timeArrival = timeNextEvent + current;       /* Assign arrival time (poisson)    */

   //cout<< "packet with id " << id << "time in Re-arrival is: "<< timeArrival<<endl;
   cout<< "packet with id " << id << "schedule time before retransmitting in Re-arrival is: "<< pack->get_schedule_time()<<endl;
   Packet pkt= node1[from].retransmit(pack, timeArrival);
   cout<< "packet with id " << id << "schedule time after retransmitting in Re-arrival is: "<< pkt.get_schedule_time()<<endl;
   node1[from].getMyVec().push_back(pkt);

   
       
  } /* End of gen_arrival() */

void nextarrival( void )
 /* Compute the poisson distribution of the next arrival according to 
  * the arrival rate of lambda and set the global variable next to be 
  * the time at which the next event occurs*/
  
{
 long double deltat, ln;
 Random_Number_Generator r;
 ln = log( 1-uni()); 
 deltat = -1 * (( ln  ) / r.get_rate());
 //deltat = -1 * (( ln  ) / lambdaN);
 /*deltat =rnd_transmission.exponential();
 cout<< "exponential is: " << rnd_transmission.exponential()<<endl;*/
 timeNextEvent = sysTime + deltat;
}

double uni(void)
/* Generate a random number from Unif(0,1) */
{
int a, b;
   
 a = (int) (iseed / 16384);
 b = (int) (iseed % 16384);
 		   
 iseed = (((13205*a + 74505*b) % 16384)*16384 + 13205*b) %
	 268435456;

 return( (double) iseed / 268435456) ;
}

bool inRange(double low, double high, double x)
{
    return (low <= x  && x <= high);
}

int rand_CW(int n)
    {
		random_device rd;  //Will be used to obtain a seed for the random number engine
    	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    	uniform_real_distribution<> dis(0, pow(2,n)-1);
      //uniform_real_distribution<> dis(0, 4);
    	return dis(gen);
    }

double rand_gen()
    {
    random_device rd;  //Will be used to obtain a seed for the random number engine
      mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
      uniform_real_distribution<> dis(3.0*timeXPac,2.0);
      return dis(gen);
    }

bool Prob( void )
  {
   /* This function will return a "1" if the transmission is going to happen at the same slot
   /*  and  "0" if not 
    */
   bool x;
 
   x =  ( (uni() ) < probError )?false:true;
    
   return(x);

   } /* End of Prob */
bool String2Convert(string var)
{
   if(var == "true" )
      return true;
   else if(var == "false")
      return false;
   //throw exception("bad string");
   
}

int main(int argc,  char * argv[])
{	

	Wireless_node node;
  Random_Number_Generator r;

	
  bool values[7];
  bool pb;  
  myfile2.open ("allreward.txt",fstream::app);
  myfiledis.open ("dist.txt",fstream::app);
  /*infile.open("input.txt");

  if(infile.fail()) 
  { 
    cout << "error" << endl; 
    return 1;
  }
  string line;
  int num_elements=0;
  while(getline(infile,line))
 {
    stringstream   linestream(line);
    string         value;
    std::string::size_type sz;
    
    while(getline(linestream,value,','))
    {
        std::cout << "Value(" << value << ")\n";
        values[num_elements]= String2Convert(value);
        ++num_elements;
        //infile >> Ack >> backoff >>  pb >> cw >> csma >> protocol;
    }
    cout << "Line Finished" << endl;

}
infile.close();
Ack=values[0]; backoff=values[1]; prob=values[2]; cw=values[3]; csma=values[4]; protocol=values[5];
 */ 
infile.open("action.txt");
nodefile.open ("reward_node.txt",fstream::app);
  if(infile.fail()) 
  { 
    cout << "error" << endl; 
    return 1;
  }
  string line;
  int num_elements=0;
  string value;
  while(getline(infile,line))
 {
    stringstream   linestream(line);
    std::string::size_type sz;
    
    while(getline(linestream,value,'\n'))
    {
        //std::cout << "Value(" << value << ")\n";
    }
  }  
  infile.close();

  //map_to_state(std::stoi(value));
  for (int x=0; x<1; ++x){
    for(int y=0; y<1; ++y)
    {
    int z;
    //z = atof(argv[1]);
    map_to_state(1);
    myfile.open ("reward.txt",fstream::app);
    myfile << z<<':';
   // backoff=false; Ack=false; cw=false;
    myfile2 <<z<<':';
  	initParams(argc, argv);   /* Process command line arguments    */       
    initAll();                /* Set system parameters             */

    node.init_simulator();

    theSimulation.run ();
    statistics();
    node.output();
    cout << "number of collisions "<<numCollisions<< endl;
    cout << "number of access "<<accessCount<< endl;
    
    cout<<"backoff is:"<< backoff<<endl;
    /*for(int i = 0; i < num_elements; ++i){
      cout<< values[i]<<endl;
    }*/

    cout << " [Ack:] " << Ack << " [backoff:] " << backoff << " [CW:] "<< cw << " [prob:] "<< prob  << " [csma:] "<< csma << " [prot:] "<< protocol  <<endl;
     cout << "End of Simulation \n";
     cout<< "the rate is: "<< r.get_rate() << endl;
   }
 }
    if (!noise){
              
              cout<< "no noise!"<<endl;
            }
    return 0;
}

