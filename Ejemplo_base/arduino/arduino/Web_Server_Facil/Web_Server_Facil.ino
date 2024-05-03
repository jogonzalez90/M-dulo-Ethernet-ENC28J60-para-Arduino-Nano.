#include <EtherCard.h>

static byte mymac[] = {0xDD,0xDD,0xDD,0x00,0x01,0x05};
static byte myip[] = {192,168,18,200};
byte Ethernet::buffer[700];

const int ledPin = 2;

char* EstadoLed="OFF";

void setup () {
 
  Serial.begin(9600);
  Serial.println("Test del Modulo  ENC28J60");
 
  if (!ether.begin(sizeof Ethernet::buffer, mymac, 10))
    Serial.println( "No se ha podido acceder a la controlador Ethernet");
 else
   Serial.println("Controlador Ethernet inicializado");
 
  if (!ether.staticSetup(myip))
    Serial.println("No se pudo establecer la dirección IP");

  Serial.println();
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

static word homePage() {
  
 BufferFiller bfill = ether.tcpOffset();
 bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
      "Content-Type: text/htmlrnPragma: no-cachernRefresh: 5\r\n\r\n"
      "<html><head><title>Naylamp Mechatronics</title></head>"
      "<body>"
      "<div style='text-align:center;'>"
      "<h1>Test del Módulo  ENC28J60</h1>"      
      "Tiempo transcurrido : $L segundos"
      "<br /><br />Estado del LED: $S<br />"      
      "<a href=\"/?status=ON\"><input type=\"button\" value=\"ON\"></a>"
      "<a href=\"/?status=OFF\"><input type=\"button\" value=\"OFF\"></a>"
      "<br /><br />Potenciómetro: $D (resolución de 1024)"
      "<br /><br />"
      "<a href='https://naylampmechatronics.com/'>naylampmechatronics.com</a>"
      "</body></html>"      
      ),millis()/1000,EstadoLed,analogRead(0));
     
  return bfill.position();
}
  
void loop() {
 
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  
  if(pos) {
    
    if(strstr((char *)Ethernet::buffer + pos, "GET /?status=ON") != 0) {
      Serial.println("Comando ON recivido");
      digitalWrite(ledPin, HIGH);
      EstadoLed = "ON";
    }

    if(strstr((char *)Ethernet::buffer + pos, "GET /?status=OFF") != 0) {
      Serial.println("Comando OFF recivido");
      digitalWrite(ledPin, LOW);
       EstadoLed= "OFF";
    }        
    ether.httpServerReply(homePage()); // se envia página Web
  }
}