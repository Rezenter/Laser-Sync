#include <UIPEthernet.h>
//#include <EthernetUdp.h>
const byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
const IPAddress ip(192, 168, 10, 48);
const IPAddress remoteIP(192, 168, 10, 255);
const unsigned int port = 8888;      // local port to listen on
EthernetUDP Udp;

void setup() {
  delay(4000);
  Ethernet.init(10);  // slave select pin

  Ethernet.begin(mac, ip);

  
  Udp.begin(port);
  delay(4000);
  
  if(Udp.beginPacket(remoteIP, port) == 1){
    Udp.write(255);
    Udp.endPacket();
  }
}

void loop() {
}
