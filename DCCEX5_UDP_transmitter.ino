// WiThrottleProtocol library: Roster and other objects example
//
// Shows how to retrieve the Roster, Turnouts/Points, Routes, Turntables
// Tested with ESP32-WROOM board
//
// Peter Akers (Flash62au), Peter Cole (PeteGSX) and Chris Harlow (UKBloke), 2023
// Luca Dentella, 2020


//#include <WiFi.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <DCCEXProtocol.h>
#include <DCCEXLoco.h>
#include "config.h"
#include <DCCEXTurnouts.h>

void printRoster();
void printTurnouts();
void printRoutes();
void printTurntables();

// Delegate class
class MyDelegate : public DCCEXProtocolDelegate {

public:

 void receivedLocoUpdate(Loco* loco) {
      Serial.print("Received Loco update for DCC address: ");
      Serial.println(loco->getAddress());
    }

 void receivedTurnoutAction(int turnoutId, bool thrown) {
      Serial.print("Received Turnout update for DCC address: ");
      Serial.print(turnoutId);
      Serial.print("  Position; ");
      Serial.println(thrown);
      vxisthrown=1;
      vxthrown=turnoutId;
      vxposition=thrown;
    }



  void receivedServerVersion(int major, int minor, int patch) {
    Serial.print("\n\nReceived version: ");
    Serial.print(major);
    Serial.print(".");
    Serial.print(minor);
    Serial.print(".");
    Serial.println(patch);
  }

  void receivedTrackPower(TrackPower state) {
    Serial.print("\n\nReceived Track Power: ");
    Serial.println(state);
    Serial.println("\n\n");
  }

  void receivedRosterList() {
    Serial.println("\n\nReceived Roster");
    //printRoster();
  }
  void receivedTurnoutList() {
    Serial.println("\n\nReceived Turnouts/Points list");
    //printTurnouts();
    Serial.println("\n\n");
  }
  void receivedRouteList() {
    Serial.println("\n\nReceived Routes List");
    printRoutes();
    Serial.println("\n\n");
  }
  void receivedTurntableList() {
    Serial.println("\n\nReceived Turntables list");
    printTurntables();
    Serial.println("\n\n");
  }
};

//bool done = false;

// Global objects
WiFiClient client;
DCCEXProtocol dccexProtocol;
MyDelegate myDelegate;

void printRoster() {
  for (Loco* loco = dccexProtocol.roster->getFirst(); loco; loco = loco->getNext()) {
    int id = loco->getAddress();
    char* name = loco->getName();
    Serial.print(id);
    Serial.print(" ~");
    Serial.print(name);
    Serial.println("~");
  }
  Serial.println("\n");
}

void printTurnouts() {
  for (Turnout* turnout = dccexProtocol.turnouts->getFirst(); turnout; turnout = turnout->getNext()) {
    int id = turnout->getId();
    char* name = turnout->getName();
    Serial.print(id);
    Serial.print(" ~");
    Serial.print(name);
    Serial.println("~");
  }
  Serial.println("\n");
}

void printRoutes() {
  for (Route* route = dccexProtocol.routes->getFirst(); route; route = route->getNext()) {
    int id = route->getId();
    char* name = route->getName();
    Serial.print(id);
    Serial.print(" ~");
    Serial.print(name);
    Serial.println("~");
  }
  Serial.println("\n");
}

void printTurntables() {
  for (Turntable* turntable = dccexProtocol.turntables->getFirst(); turntable; turntable = turntable->getNext()) {
    int id = turntable->getId();
    char* name = turntable->getName();
    Serial.print(id);
    Serial.print(" ~");
    Serial.print(name);
    Serial.println("~");

    int j = 0;
    for (TurntableIndex* turntableIndex = turntable->getFirstIndex(); turntableIndex; turntableIndex = turntableIndex->getNextIndex()) {
      char* indexName = turntableIndex->getName();
      Serial.print("  index");
      Serial.print(j);
      Serial.print(" ~");
      Serial.print(indexName);
      Serial.println("~");
      j++;
    }
  }
  Serial.println("\n");
}

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println("");
  Serial.println("DCCEX5 UDP transmitter for NTJ");
  Serial.println();

  // Connect to WiFi network
  Serial.println("Connecting to WiFi..");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    restart++;
    if (100 == restart) ESP.restart();
    delay(100);
  }

  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  // Connect to the server
  Serial.println("Connecting server...");
  while (!client.connect(serverAddress, serverPort)) {
    Serial.print(".");
    restart++;
    if (250 == restart) ESP.restart();
    delay(100);
  }


  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("connection failed, restart");
    ESP.restart();
  }
  Serial.println("Connected to server");

  // Enable logging on Serial
  dccexProtocol.setLogStream(&Serial);

  // Pass the delegate instance to wiThrottleProtocol
  dccexProtocol.setDelegate(&myDelegate);

  // Pass the communication to wiThrottleProtocol
  dccexProtocol.connect(&client);
  Serial.println("DCC-EX connected");

  // dccexProtocol.requestServerVersion();
  // dccexProtocol.powerOn();


  Udp.begin(localPort);

  for (int a = 0; a < 49; a++) {
    vxstatus[a] = 2;
  }
  restart = 1;
}

void loop() {
  // parse incoming messages
  dccexProtocol.check();

  // sequentially request and get the required lists. To avoid overloading the buffer
  //getLists(bool rosterRequired, bool turnoutListRequired, bool routeListRequired, bool turntableListRequired)
  if (!doneTurnouts && !doneLocos) {
    //int antalvx = dccexProtocol.getTurnoutCount();
    dccexProtocol.getLists(true, true, false, false);
  }
  if (dccexProtocol.receivedLists() && !doneTurnouts) {
    if (dccexProtocol.getTurnoutCount() >= 2) {
      vx[1] = dccexProtocol.turnouts->getFirst();
      for (int a = 2; a < antalvx + 1; a++) {
        vx[a] = vx[a - 1]->getNext();
        delay(10);
      }
    }
    doneTurnouts = true;
  }
  if (doneTurnouts && !doneLocos) {
    locomotive[1] = dccexProtocol.roster->getFirst();
    for (int a = 2; a < rosterlength + 1; a++) {
      locomotive[a] = locomotive[a - 1]->getNext();
      delay(10);
    }
    doneLocos = true;
  }
  //if(1==restart){Serial.println("Väntar på start"); delay(3000); restart=0; }
/*  if (millis() > vxlastTime + vxtimer && doneTurnouts) {
       for (int a = 1; a < antalvx + 1; a++) {
      Chek4UDP();
      int vxaddress = vx[a]->getId();
      int stat = vx[a]->getThrown();
      if (vxstatus[a] != stat) {
        vxstatus[a] = stat;
        sendmsgqnumber++;
        if (31 == sendmsgqnumber) sendmsgqnumber = 0;
        sendmsgq[sendmsgqnumber] = "T" + String(vxaddress) + "P" + String(stat);
        resent[sendmsgqnumber] = 1;
        goto bailout;
      }
    }
bailout:
    vxlastTime = millis();
    vxtimer = 500;
  }
*/
if(1==vxisthrown){
  




}//End handling of thrown vx










  if (millis() > locolastTime + locotimer && doneLocos) {
    for (int a = 1; a < rosterlength + 1; a++) {
      void Chek4UDP();
      int locoaddress = locomotive[a]->getAddress();
      byte locospeed = locomotive[a]->getSpeed();
      byte locodirection = locomotive[a]->getDirection();
      int locofunctions = locomotive[a]->getFunctionStates();
      if (locospeed != speed[a] || locodirection != direction[a]) {
        sendmsgqnumber++;
        if (31 == sendmsgqnumber) sendmsgqnumber = 0;
        sendmsgq[sendmsgqnumber] = "I" + String(locoaddress) + "H" + String(locospeed) + "R" + String(locodirection);
        speed[a] = locospeed;
        direction[a] = locodirection;
        resent[sendmsgqnumber] = 1;
        goto done;
      }
      if (locofunctions != functions[a]) {
        sendmsgqnumber++;
        if (31 == sendmsgqnumber) sendmsgqnumber = 0;
        sendmsgq[sendmsgqnumber] = "I" + String(locoaddress) + "F" + String(locofunctions);
        functions[a] = locofunctions;
        resent[sendmsgqnumber] = 1;
        goto done;
      }
    }
    locotimer = 500;
done:
    locolastTime = millis();
  }
  //Härnedan skall utsändning och omsändning av meddelandekön läggas in
  //Beta av sändkön
  for (int k = 0; k < 31; k++) {
    Chek4UDP();
    if (sendmsgq[k].startsWith("I") || sendmsgq[k].startsWith("T")) {
      inputString = sendmsgq[k];
      //Serial.print("Nytt utgående meddelande ");
      //Serial.println(inputString);
      sendmsgq[k] = "&";
      goto next;
    }
  }
next:
  inputString.replace("\n", "");  //rensa onödiga tecken
  //Lägg till hantering enbart av meddelanden som börjar med "I" eller "T"
  if (inputString.startsWith("I") || inputString.startsWith("T")) {
    repeatQ[Q] = inputString;
    Seq++;  //incrementera sekvensnumret dvs serienummer på inkommet meddelande från DCC-EX
    Sequence[Q] = Seq;
    //Skicka inkommet meddelande direkt
    sendmsg = repeatQ[Q] + "Q" + Sequence[Q];
    IPAddress broadCast = WiFi.localIP();
    broadCast[3] = 255;
    Udp.beginPacket(broadCast, 8888);
    Udp.print(sendmsg);
    Udp.endPacket();
    Serial.print(" 1:a sändning ");
    Serial.println(sendmsg);
    unsigned long start = millis();
    Q++;                 //inkrementera köpekaren
    if (31 == Q) Q = 0;  //sätt kön till 0 om den nått taket på max antal meddelanden i kö
  }
  msginq = 0;
  for (int d = 0; d < 31; d++) {
    if (Sequence[d] > 0) msginq++;
  }
  inputString = "";

  //sänd om meddelanden som inte ackats över UDP periodiskt
  Chek4UDP();
  wait = 800 / (msginq + 1);

  if (millis() > last + wait) {
    for (int r = 0; r < 30; r++) {
      if (Sequence[Q] > 0) {
        String resendmsg = repeatQ[Q] + "Q" + Sequence[Q];
        IPAddress broadCast = WiFi.localIP();
        broadCast[3] = 255;
        Udp.beginPacket(broadCast, 8888);
        Udp.print(resendmsg);
        Udp.endPacket();
        sent = millis();
        resent[Q] = resent[Q] + 1;
        Serial.print(" Omsänt ");
        Serial.print(resent[Q]);
        Serial.print(" ggr msg ");
        Serial.println(resendmsg);

        if (resent[Q] > maxretry) {
          resent[Q] = 0;
          repeatQ[Q] = "1111111111111";
          Sequence[Q] = 0;
        }
        goto resent;
      }
      Q++;
      if (Q > 30) Q = 0;
    }
resent:
    last = millis();
  }  //slut på omsändning
}  //slut loop
