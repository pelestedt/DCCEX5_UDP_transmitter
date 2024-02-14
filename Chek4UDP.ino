void Chek4UDP() {
  if(millis()-50>Chkrepeat){
  Chkrepeat=millis();
  //Serial.print(".");
  //kolla efter inkommande meddelanden på UDP och buffra
  int packetSize = Udp.parsePacket();
  if (packetSize) {  //meddelande har kommit in och ligger i packetBuffer
    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, 20);
    packetBuffer[n] = 0;
    UDPincoming = packetBuffer;
    UDPincoming.replace("\n", "");
    UDPseq = UDPincoming.toInt();  //Detta förutsätter att noden ackar med endast sekvensnumret
    Serial.print("  Inkommet svar ");
    Serial.println(UDPseq);
    found=1;
  }
  for (int l = 0; l < 31; l++) {
    //delay(1);
    if (UDPseq==Sequence[l] && Sequence[l]>0) {
      repeatQ[l] = "1111111111111";  //om meddelandet finns i repetitionskön, ta bort det
      Sequence[l] = 0;
      resent[l] = 0;
      UDPseq = 0;
      sendmsg = "";
      
            goto exit1;
      //      Serial.println("Hittat i kön, raderat");
    }
  }
  exit1:
  byte a = 0;

}}