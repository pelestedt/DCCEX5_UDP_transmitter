// Example user configuration to use with the DCCEXProtocol examples
// Copy this file and rename to config.h
// Replace the variables below with the appropriate values for your environment
#define ssid  "NTJ01"          // WiFi SSID name here
#define password   "NTJNTJ01" // WiFi password here
IPAddress serverAddress(192,168,143,197);         // IP address of your EX-CommandStation
int serverPort = 2560;                        // Network port of your EX-CommandStation

unsigned long vxlastTime = 0;
unsigned long locolastTime = 0;
unsigned long Chkrepeat = 0;

bool doneTurnouts = false;
bool doneLocos = false;
bool doneRoutes = false;

byte vxisthrown=0;
int  vxthrown=0;
byte vxposition=0;
//Turnout* turnout1=nullptr;
//Turnout* turnout2=nullptr;
Turnout* vx[49]={nullptr};
int vxstatus[49]={2};
int vxtimer=500;

Loco* locomotive[21]{nullptr};
int locoaddress[21];
int speed[21];
int direction[21];
int functions[21];
int locotimer=1000;
byte rosterlength=4;
byte antalvx=48;
String sendmsgq[31]={"1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111",
                    "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111",
                    "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111",
                    "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111",
                    "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111",
                    "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111"
                   };
byte sendmsgqnumber=0;


//Config från originalprogrammet

unsigned int localPort = 8888;      // local port to listen on
long last = 0;
byte ack = 0;
byte ackcounter = 0;
byte resent[31];
int Sequence[31];
unsigned long Seq = 1;
#define maxretry 10 //antalet omsändningar innan meddelandet tas bort ur kön
String Broadcastraddress = "255.255.255.255";
String broadCast[3]; //broadcastadressen lagrad som vektor
String sendmsg;
unsigned long UDPseq=0;
unsigned long stime=0;
byte found=0;
byte sluta=0;
byte msginq=0;
// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
//char  ReplyBuffer[31] = "acknowledged\r\n";       // a string to send back
String repeatQ[31] = {"1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111",
                    "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111",
                    "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111",
                    "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111",
                    "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111",
                    "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111", "1111111111111"
                   };
String UDPincoming = "2222222222222222222"; //I12345H123D10Q12345
byte restart=0;
int wait = 200;
byte Q = 0;
WiFiUDP Udp;
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
byte r = 0;
unsigned long sent = 0;