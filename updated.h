/*
 author: Hannaneh Barahouei (barahoueipash@vcu.edu)
 file: updated.h
 */


#include <stdio.h>
#include <stdlib.h>

extern int          iseed;     /* Seed for the random # genertor (main.c) */
extern long double       lambdaN;   /* Node arrival rate (main.c)              */
extern long double       timeNextEvent;   /* Time of next event                */
extern long double       sysTime;         /* Global clock time                 */

bool Ack;
bool backoff;
bool csma;
bool prob;
bool protocol;
bool cw;
bool wire = true;
bool trans_r;
bool noise=false;
//double probError;

struct Coordinates
{
     int value1;
     int value2;
};
double rand_gen();

#define FrameArrival 0
#define AckArrival 1
#define X_SUCCESSFUL 2
#define X_unSUCCESSFUL 3
#define RE_ARRIVAL 4
#define Departure 5
#define TimeOut 6
#define NodeFree 7
#define ARRIVAL 8
#define Aloha 10;
#define Slotted-Aloha 11;

bool String2Convert(string var);
//#define CSMA 12;




void map_to_state(int value){

	switch(value){
		case 0:
			Ack=true; backoff=true; prob=false; cw=true; csma=true; protocol=true;trans_r=true;
		break;
		case 1:
			Ack=true; backoff=true; prob=false; cw=true; csma=true; protocol=true;trans_r=false;
		break;
		case 2:
			Ack=false; backoff=true; prob=false; cw=true; csma=true; protocol=true;trans_r=true;
		break;
		case 3:
			Ack=false; backoff=true; prob=false; cw=true; csma=true; protocol=true;trans_r=false;
		break;
		case 4:
			Ack=true; backoff=false; prob=false; cw=true; csma=true; protocol=true;trans_r=true;
		break;
		case 5:
			Ack=true; backoff=false; prob=false; cw=true; csma=true; protocol=true;trans_r=false;
		break;
		case 6:
			Ack=true; backoff=true; prob=false; cw=false; csma=true; protocol=true;trans_r=true;
		break;
		case 7:
			Ack=true; backoff=true; prob=false; cw=false; csma=true; protocol=true;trans_r=false;
		break;
		case 8:
			Ack=true; backoff=true; prob=false; cw=true; csma=false; protocol=true;trans_r=true;
		break;
		case 9:
			Ack=true; backoff=true; prob=false; cw=true; csma=false; protocol=true;trans_r=false;
		break;
		case 10:
			Ack=false; backoff=false; prob=false; cw=true; csma=true; protocol=true;trans_r=true;
		break;
		case 11:
			Ack=false; backoff=false; prob=false; cw=true; csma=true; protocol=true;trans_r=false;
		break;
		case 12:
			Ack=true; backoff=false; prob=false; cw=false; csma=true; protocol=true;trans_r=true;
		break;
		case 13:
			Ack=true; backoff=false; prob=false; cw=false; csma=true; protocol=true;trans_r=false;
		break;
		case 14:
			Ack=true; backoff=true; prob=false; cw=false; csma=false; protocol=true;trans_r=true;
		break;
		case 15:
			Ack=true; backoff=true; prob=false; cw=false; csma=false; protocol=true;trans_r=false;
		break;
		case 16:
			Ack=false; backoff=true; prob=false; cw=false; csma=true; protocol=true;trans_r=true;
		break;
		case 17:
			Ack=false; backoff=true; prob=false; cw=false; csma=true; protocol=true;trans_r=false;
		break;
		case 18:
			Ack=false; backoff=true; prob=false; cw=true; csma=false; protocol=true;trans_r=true;
		break;
		case 19:
			Ack=false; backoff=true; prob=false; cw=true; csma=false; protocol=true;trans_r=false;
		break;
		case 20:
			Ack=true; backoff=false; prob=false; cw=true; csma=false; protocol=true;trans_r=true;
		break;
		case 21:
			Ack=true; backoff=false; prob=false; cw=true; csma=false; protocol=true;trans_r=false;
		break;
		case 22:
			Ack=true; backoff=false; prob=false; cw=false; csma=true; protocol=true;trans_r=true;
		break;
		case 23:
			Ack=true; backoff=false; prob=false; cw=false; csma=true; protocol=true;trans_r=false;
		break;
		case 24:
			Ack=false; backoff=false; prob=false; cw=false; csma=true; protocol=true;trans_r=true;
		break;
		case 25:
			Ack=false; backoff=false; prob=false; cw=false; csma=true; protocol=true;trans_r=false;
		break;
		case 26:
			Ack=false; backoff=false; prob=false; cw=false; csma=false; protocol=true;trans_r=true;
		break;
		case 27:
			Ack=false; backoff=false; prob=false; cw=false; csma=false; protocol=true;trans_r=false;
		break;
		case 28:
			Ack=false; backoff=false; prob=false; cw=true; csma=false; protocol=true;trans_r=true;
		break;
		case 29:
			Ack=false; backoff=false; prob=false; cw=true; csma=false; protocol=true;trans_r=false;
		break;
		case 30:
			Ack=true; backoff=true; prob=true; cw=true; csma=false; protocol=false;trans_r=true;
		break;
		case 31:
			Ack=true; backoff=true; prob=true; cw=true; csma=false; protocol=false;trans_r=false;
		break;
		case 32:
			Ack=false; backoff=true; prob=true; cw=true; csma=false; protocol=false;trans_r=true;
		break;
		case 33:
			Ack=false; backoff=true; prob=true; cw=true; csma=false; protocol=false;trans_r=false;
		break;
		case 34:
			Ack=true; backoff=false; prob=true; cw=true; csma=false; protocol=false;trans_r=true;
		break;
		case 35:
			Ack=true; backoff=false; prob=true; cw=true; csma=false; protocol=false;trans_r=false;
		break;
		case 36:
			Ack=true; backoff=true; prob=false; cw=true; csma=false; protocol=false;trans_r=true;
		break;
		case 37:
			Ack=true; backoff=true; prob=false; cw=true; csma=false; protocol=false;trans_r=false;
		break;
		case 38:
			Ack=true; backoff=true; prob=true; cw=false; csma=false; protocol=false;trans_r=true;
		break;
		case 39:
			Ack=true; backoff=true; prob=true; cw=false; csma=false; protocol=false;trans_r=false;
		break;
		case 40:
			Ack=false; backoff=false; prob=true; cw=true; csma=false; protocol=false;trans_r=true;
		break;
		case 41:
			Ack=false; backoff=false; prob=true; cw=true; csma=false; protocol=false;trans_r=false;
		break;
		case 42:
			Ack=true; backoff=false; prob=false; cw=true; csma=false; protocol=false;trans_r=true;
		break;
		case 43:
			Ack=true; backoff=false; prob=false; cw=true; csma=false; protocol=false;trans_r=false;
		break;
		case 44:
			Ack=true; backoff=true; prob=false; cw=false; csma=false; protocol=false;trans_r=true;
		break;
		case 45:
			Ack=true; backoff=true; prob=false; cw=false; csma=false; protocol=false;trans_r=false;
		break;
		case 46:
			Ack=true; backoff=false; prob=false; cw=true; csma=false; protocol=false;trans_r=true;
		break;
		case 47:
			Ack=true; backoff=false; prob=false; cw=true; csma=false; protocol=false;trans_r=false;
		break;
		case 48:
			Ack=false; backoff=true; prob=false; cw=true; csma=false; protocol=false;trans_r=true;
		break;
		case 49:
			Ack=false; backoff=true; prob=false; cw=true; csma=false; protocol=false;trans_r=false;
		break;
		case 50:
			Ack=false; backoff=true; prob=true; cw=false; csma=false; protocol=false;trans_r=true;
		break;
		case 51:
			Ack=false; backoff=true; prob=true; cw=false; csma=false; protocol=false;trans_r=false;
		break;
		case 52:
			Ack=false; backoff=false; prob=false; cw=true; csma=false; protocol=false;trans_r=true;
		break;
		case 53:
			Ack=false; backoff=false; prob=false; cw=true; csma=false; protocol=false;trans_r=false;
		break;
		case 54:
			Ack=true; backoff=false; prob=false; cw=false; csma=false; protocol=false;trans_r=true;
		break;
		case 55:
			Ack=true; backoff=false; prob=false; cw=false; csma=false; protocol=false;trans_r=false;
		break;
		case 56:
			Ack=true; backoff=false; prob=true; cw=false; csma=false; protocol=false;trans_r=true;
		break;
		case 57:
			Ack=true; backoff=false; prob=true; cw=false; csma=false; protocol=false;trans_r=false;
		break;
		case 58:
			Ack=false; backoff=false; prob=false; cw=false; csma=false; protocol=false;trans_r=true;
		break;
		case 59:
			Ack=false; backoff=false; prob=false; cw=false; csma=false; protocol=false;trans_r=false;
		break;

	}
}

struct Channelst {

    bool channelstate;

    Channelst() : channelstate(false){}

    void busy(){
       channelstate=true;
    }
    void free(){
       channelstate=false;
    }

}status;

struct safeChannel {
    std::mutex mutex;
    Channelst channelBiz;

    void busy(){
        std::lock_guard<std::mutex> guard(mutex);
        channelBiz.busy();
    }

    void free(){
        std::lock_guard<std::mutex> guard(mutex);
        channelBiz.free();
    }
};

struct Counter {
    std::mutex mutex;
    bool channelstate;

    Counter() : channelstate(false) {}

    void busy(){
        mutex.lock();
        channelstate=true;
        mutex.unlock();
    }
    void free(){
        mutex.lock();
        channelstate=false;
        mutex.unlock();
    }
};

class Random_Number_Generator {
	long double seed_;
	long double rate_; 
	long double previous_number_;
	/* LCG values */
    int M; // 2^31 -1 = 2147483647
    int a; // 8*8192+3 = 65539

public:
	Random_Number_Generator() : seed_(1234.0), rate_(4.0), previous_number_(1),
	                            M(2147483648), a(65539)
    {
        /* initialize random seed: */
        srand ((int)seed_);
    };

   	/* getters and setters */
   	void set_seed(long double d) { seed_ = d; srand ((int)seed_); previous_number_ = seed_; };
   	long double get_seed() const { return seed_; };
   	void set_rate(long double d) { rate_ = d; };
   	long double get_rate() const { return rate_; };
   	
   	
   	void output()
   	{
   	     cout << "*** Random_Number_Generator ***" << endl
                 << "seed " << seed_ << endl
                 << "rate " << rate_ << endl;	     
   	};

    long double next()
    {
      previous_number_ = fmod((a * previous_number_), M);
      return previous_number_;
    }

    long double next_normalized()
    {
      previous_number_ = fmod((a * previous_number_), M);
      return -previous_number_/M;
    }

    long double exponential(long double n)
    {
      // calculating F'(x) - using inverse transformation of exponential pdf
      return (-1/rate_) * log(1-n);
    }
	
    long double exponential()
    {
      // calculating F'(x) - using inverse transformation of exponential pdf
      return (-1/rate_) * log(1-next_normalized());
    }


    long double rand_exponential()
    {
		random_device rd;  //Will be used to obtain a seed for the random number engine
    	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    	uniform_real_distribution<> dis(0.00, 2.00);
    	return dis(gen);
    }

    void test()
    {
        ofstream outFile;

        outFile.open("rnd_01.txt", ios::out);
        outFile.precision(6);
        set_seed(1);
        for (int i = 0; i < 10000; i++) 
        {
            outFile << fixed << exponential() << endl;
        }
        outFile << fixed << exponential();
        outFile.close();

        outFile.open("rnd_02.txt", ios::out);
        outFile.precision(6);
        set_seed(7.51391e+008);
        for (int i = 0; i < 10000; i++) 
        {
            outFile << fixed << exponential() << endl;
        }
        outFile << fixed << exponential();
        outFile.close();

        outFile.open("rnd_03.txt", ios::out);
        outFile.precision(6);
        set_seed(5.648e+008);
        for (int i = 0; i < 10000; i++) 
        {
            outFile << fixed << exponential() << endl;
        }
        outFile << fixed << exponential();
        outFile.close();
        
        outFile.open("rnd_04.txt", ios::out);
        outFile.precision(6);
        set_seed(1234);
        for (int i = 0; i < 10000; i++) 
        {
            outFile << fixed << rand_exponential() << endl;
        }
        outFile << fixed << rand_exponential();
        outFile.close();
        
    }
};




 // event 
class event {

	
public:
	const double current_time_;
	event( double current_time): current_time_(current_time)
	{}

	virtual void processEvent()=0;


};




struct eventCompare {
	bool operator () (const event * one, const event * two) const {
		return one->current_time_ > two->current_time_;
	}
};

//class simulator
class simulator {
public:
	simulator(): current_time(), eventQueue()
	{}
	void run ();
	void Event_Scheduler (event * newEvent) {
		eventQueue.push(newEvent);
	}


	 double current_time ;
protected:                        //making a priority queue of pointers to event using vector container
	std::priority_queue<event*,
                      std::vector<event *, std::allocator<event*> >,
                      eventCompare> eventQueue;
};

class AlohaSimulator : public simulator
 {
	public:
	AlohaSimulator (): simulator()
	{}
	void transmit(long double t);
	void send();
	void endsend(long double t);
	void reception(long double t);
	void endreception(long double t);
	void check_transmit(long double t);

} theSimulation;

class transmitEvent : public event {

public:
	transmitEvent ( double current_time) : event (current_time)
	{}
virtual void processEvent ();

};

class sendEvent : public event {

public:
	sendEvent ( double current_time) : event (current_time)
	{}
	~sendEvent ()
	{}
virtual void processEvent ();

};

class slotted_sendEvent : public event {

public:
	slotted_sendEvent ( double current_time) : event (current_time)
	{}
	~slotted_sendEvent ()
	{}
virtual void processEvent ();

};

class end_sendEvent : public event {

public:
	end_sendEvent ( double current_time) : event (current_time)
	{}
	~end_sendEvent ()
	{}
virtual void processEvent ();

};

class receptionEvent : public event {

public:
	receptionEvent ( double current_time) : event (current_time)
	{}
	~receptionEvent ()
	{}
virtual void processEvent ();

};

class end_receptionEvent : public event {

public:
	end_receptionEvent ( double current_time) : event (current_time)
	{}
	~end_receptionEvent ()
	{}
virtual void processEvent ();

};

class collisionEvent : public event {

public:
	collisionEvent ( double current_time) : event (current_time)
	{}
virtual void processEvent ();

};


// class packet

class Packet {
	int id_;

	char type_;
	char status_;

	 double schedule_time_;
	 double first_schedule_time_;
	 double first_send_attempt_;
	 double sent_time_;
	 double expected_receive_time_;
	 double receive_time_;
	 long double timer;

	int num_retransmission_attempts_;
	int retransmit_attempt;

	int from_;
	int to_;
	int payload_;


public:
	Packet() :
				id_(-1),
				type_(Packet::UNDEFINED()),
				status_(Packet::UNDEFINED()),
				schedule_time_(-1),
				first_schedule_time_(-1),
				first_send_attempt_(-1),
				sent_time_(-1),
				expected_receive_time_(-1),
				receive_time_(-1),
				num_retransmission_attempts_(0),
				retransmit_attempt(0),
				from_(-1),
				to_(-1),
				timer(-1),
				payload_(-1){};

	/* pseudo-constants */
	static char MSG() 					{ char m='M' ; return m; }; /* 0000 0000 */
	static char ACK() 					{ char a='A'; return a; }; /* 0001 0000 */
	static char UNDEFINED() 			{ char u='U'; return u; }; /* 0010 0000 */
	static char SENDING() 				{ char m='S' ; return m; }; /* 0011 0000 */
	static char RECEIVED() 				{ char m='R' ; return m; }; /* 0100 0000 */
	static char SCHEDULED() 			{ char m='C' ; return m; }; /* 0101 0000 */
	static char RESCHEDULED() 			{ char m='H' ; return m; }; /* 0101 0000 */
	static char ACKNOWLEDGED() 			{ char m='K' ; return m; }; /* 0110 0000 */


	vector <int> to_others;
	/* getters and setters */
	void set_id(int i) { id_ = i; }
	int get_id()  const { return id_; }

	void set_type(char c) { type_ = c; }
	char get_type()  const { return type_; }

	void set_status(char c) { status_ = c; }
	char get_status()  const { return status_; }

	void set_schedule_time( double t) { schedule_time_ = t; }
	 double get_schedule_time()  const { return schedule_time_; }

	void set_first_schedule_time( double t) { first_schedule_time_ = t; }
	double get_first_schedule_time()  const { return first_schedule_time_; }

	void set_transmission_time(){}

	void set_expected_receive_time( double t) { expected_receive_time_ = t; }
	double get_expected_receive_time()  const { return expected_receive_time_; }

	void set_to(int i) { to_ = i; }
	int get_to()  const { return to_; }

	void set_from(int i) { from_ = i; }
	int get_from()  const { return from_; }

	void set_payload(int i) { payload_ = i; }
	int get_payload() const { return payload_; }

	void set_retransmit_attempt(int n)  {
		retransmit_attempt=retransmit_attempt+n;
	};

	double get_retransmit_attempt()  {
		return retransmit_attempt;
	};

	void set_timer(long double d) {
		timer = d;
	};

	long double get_timer() const {
		return timer;
	};

	/* Method declaration */
	void sent(long double time);
	void received(long double time);
	void output() const;
	long double get_effective_transmission_time() const;
	long double get_total_transmission_time() const;
	long double get_total_delay() const;

	//virtual void processEvent();
};


class Wireless_node {
	int id_;
	/*
	 These variables are used to evaluate the performance of
	 the node during or at the end of the simulation
	 */
	int completed_transmissions_; // total number of transmissions completed
	int attempted_transmissions_; // total number of attempted transmissions
	int failed_transmissions_; // total number of failed transmission attempts
	int generated_packets_; // total number of generated packets
	int generated_ack_packets_;
	int num_receiving_packets;
	int total_generated;
	//vector<Packet> buff;
	
	//vector<long double> propagation_time_;


	long double efficiency_;
	long double delay_;

	/*
	 These variables are used for the simulation of the node
	 */
	Random_Number_Generator rnd_transmission;
	long double transmission_rate_;
	//long double propagation_time_;
	bool initialized_;
	int x,y; //coordinates
	long double trans_range_;
	long double carrier_range_;
	char status_;

	
	

private:
	Packet create_new_packet(char type, int from, int to,  double current_time);

public:
	

	static char SENDING() 				{ return 0x30; }; /* 0011 0000 */
	static char RECEIVING() 			{ return 0x40; }; /* 0100 0000 */
	static char IDLE()      			{ return 0x50; }; /* 0101 0000 */
	static char COLLIDED()				{ return 0x60; }; /* 0101 0000 */

	vector<Packet> buff;
	vector<Packet> trans_buff;
	vector<Packet> send_buff;
	vector<Packet> receipt_buff;
	vector<Packet> collidedReceipt_buff;
	vector<Packet> endreceipt_buff;
	vector<Packet> gen_buff;
	vector< double> dist;
	vector < double> start_transmission;
	vector < double> end_transmission;
	vector < double> start_reception;
	vector < double> end_reception;

	Wireless_node() :
		completed_transmissions_(0), attempted_transmissions_(0),
				failed_transmissions_(0), generated_packets_(0), generated_ack_packets_(0) , status_(Wireless_node::IDLE()),
				efficiency_(1), delay_(0),trans_range_ (70), carrier_range_(100), x(0),y(0), num_receiving_packets(0), total_generated(0),
				transmission_rate_(1), initialized_(false)/*, buff(0),trans_buff(0), send_buff(0),receipt_buff(0),collidedReceipt_buff(0),gen_buff(0)*/,
				start_transmission(10000),end_transmission(10000),start_reception(10000),end_reception(10000) {};


	/* Getters and Setters */
	long double get_completed_transmissions() const {
		return completed_transmissions_;
	};

	long double set_num_receiving_packs(int n)  {
		num_receiving_packets=num_receiving_packets+n;
	};

	long double get_num_receiving_packs()  {
		return num_receiving_packets;
	};

	long double get_attempted_transmissions() const {
		return attempted_transmissions_;
	};

	long double set_generated_packets(int n ){
		generated_packets_ += n;
	};

	long double get_generated_packets() const {
		return generated_packets_;
	};

	long double set_generated_ack_packets(int n ) {
		generated_ack_packets_ += n;
	};
	long double get_generated_ack_packets() const {
		return generated_ack_packets_;
	};
	long double get_efficiency() const {
		return efficiency_;
	};

	long double get_average_delay() const {
		return delay_/generated_packets_;
	};

	void set_id(int i) {
		id_ = i;
	};

	int get_id() const {
		return id_;
	};

	void set_status(char c) { status_ = c; }
	char get_status()  const { return status_;}

	void set_transmission_rate(long double d) {
		transmission_rate_ = d;
	};

	long double get_transmission_rate() const {
		return transmission_rate_;
	};

	

	/*void set_start_transmission(long double d) {
		start_transmission = d;
	};

	long double get_start_transmission() const {
		return start_transmission;
	};


		void set_end_transmission(long double d) {
		end_transmission = d;
	};

	long double get_end_transmission() const {
		return end_transmission;
	};*/

	/*void set_start_reception(long double d) {
		start_reception = d;
	};

	long double get_start_reception() const {
		return start_reception;
	};*/

	void set_end_reception(long double d) {
		//end_reception = d;
	};
	long double get_end_reception() const {
		//return end_reception;
	};

/*	void set_propagation_time (long double d) {
		propagation_time_ = d;
	};

	long double get_propagation_time() const {
		return propagation_time_;
	};*/

	vector< double> propagation_time_;
	vector<Packet>& getMyVec() { return buff; }
	vector<Packet>& get_trans_buff() { return trans_buff; }
	vector<Packet>& get_send_buff() { return send_buff; }
	vector<Packet>& get_receipt_buff() { return receipt_buff; }
	vector<Packet>& get_collidedReceipt_buff() { return collidedReceipt_buff; }
	vector<Packet>& get_gen_buff() { return gen_buff; }
	vector<Packet>& get_endreceipt_buff() { return endreceipt_buff; }
	vector< double>& getMyVecDist() { return dist; }
	vector< double>& get_propagation_buff() { return propagation_time_; }
	vector< double>& get_startReception_buff() { return start_reception; }
	vector< double>& get_endReception_buff() { return end_reception; }
	vector< double>& get_startTransmission_buff() { return start_transmission; }
	vector< double>& get_endTransmission_buff() { return end_transmission; }

	/* definition of remaining public functions */
	void initialize();
	Packet create_new_msg_packet(int from, int to, double current_time);
	Packet create_new_ack_packet(int from, int to, double current_time, int pkt_id);
	Packet retransmit (Packet *p, double current_time);
	void attempt_transmission(Packet *p, long double current_time);
	void completed_transmission(Packet *p, long double current_time);
	void collision(Packet *p, long double current_time);
	void reschedule_packet(Packet *p, long double exact_time);
	void set_seed(long double transmission);
	void output() const;
	bool is_within_error_range(long double err) const;
	long double get_expected_efficiency() const;
	long double distance (long double x1, long double y1, long double x2, long double y2);
	long double propagation_delay (long double dist);
	long double range(int from, int to);
	void init_simulator();
	Packet broadcast(char type, int from, int to, long double current_time);
	void BubbleSort(vector <Packet> num);
	//Coordinates set_rand_coordinates(int N);

};



#define fatal(F, M)  { printf ("\nFatal error!");         \
                       printf ("\nProcedure:  %s", F);    \
                       printf ("\nMessage  : %s\n\n", M); \
                       printf ("\n\n");                   \
                       exit(-1);   }

#define TRUE 1
#define FALSE 0

#define YES 1
#define NO  0

#define OLD 0
#define NEW 1
#define MAXNODES 30

             /* initialize stuff */
int rand_CW(int n);
int initParams(int argc, char * argv[]);
void initAll(void);

            /* display information */
void showParams(FILE * fp);
void showCurrentEvent(FILE * fp);
void showEventList(FILE * fp);
void showReport(FILE * fp);


          /* Generate event functions */
void gen_arrival( int from, int to, long double time );
void gen_re_arrival( int node );
void gen_x_successful( int node );
void gen_x_unsuccessful( void );
void gen_ackarrival(int node);
void gen_depart(int node);
void gen_nodefree(int node);
void gen_timeout(int node);
void gen_rearrival(Packet *pack, double current);

            /* Process EVENTS */
short processArrival( void );
short processXSuccessful( void );
short processXunSuccessful( void );
short processReArrival( void );
void processFrameArrival( void );
void processAckArrival( void );
void processDeparture( void );
void processNodeFree( void );
void processTimeOut( void );
void testi (Packet *pack);


          /* Utility functions */
void queueItUp( int node );
int getNumCompeting( void );
bool Prob( void ); 
bool inRange(double low, double high, double x);
void nextarrival( void );
double uni(void);
void statistics();

