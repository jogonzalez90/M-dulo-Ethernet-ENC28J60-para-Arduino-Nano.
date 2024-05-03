#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
// ethernet interface ip address
static byte myip[] = { 192,168,18,200 };
// gateway ip address
static byte gwip[] = { 192,168,18,1 };
// remote website ip address and port

// remote website name
const char website[] = "192.168.18.1";

// static byte hisip[] = { 192,168,50,1 };



byte Ethernet::buffer[700];
static uint32_t timer;

// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
}

void setup () {
  Serial.begin(57600);
  Serial.println(F("\n[webClient]"));

  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));

  ether.staticSetup(myip, gwip);

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  char websiteIP[] = "192.168.18.1";
  ether.parseIp(ether.hisip, websiteIP);
}

void loop () {
  ether.packetLoop(ether.packetReceive());

  if (millis() > timer) {
    timer = millis() + 5000;
    Serial.println();
    Serial.print("<<< REQ ");
    ether.browseUrl(PSTR("/api?param1=23 HTTP/1.1\r\nHost: 192.168.18.1\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/111.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\nAccept-Language: es-EC,en-US;q=0.7,en;q=0.3\r\nAccept-Encoding: gzip, deflate\r\nUpgrade-Insecure-Requests: 1\r\nConnection: close\r\n\r\n"), "", ether.hisip, my_callback);
    //ether.browseUrl(PSTR("/api?param1=23"), "", ether.hisip, my_callback);
    //ether.browseUrl(PSTR("/foo/"), "bar", website, my_callback);
  }
}