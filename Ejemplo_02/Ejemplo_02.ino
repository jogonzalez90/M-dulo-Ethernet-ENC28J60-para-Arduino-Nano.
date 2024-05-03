/*Ejemplo 02: Ethernet Hanrun HR911105A*/
 
#include <EtherCard.h>


// Si colocamos 1 deshabilitamos el DHCP
// Si colocamos 0 colocamos una IP estática
#define ESTATICA 1

#if ESTATICA
  // Colocamos la dirección IP al dispositivo
  static byte myip[] = { 192,168,18,200 };
  // Colocamos la dirección IP de la puerta de enlace de nuestro router
  static byte gwip[] = { 192,168,2,1 };
#endif

// Colocamos la dirección MAC, que sera único en nuestra red
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

// Enviar y recibir buffer (TCP/IP)
byte Ethernet::buffer[500]; 

const char page[] PROGMEM =
"HTTP/1.0 503 Ethernet Hanrun HR911105A - \r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "Ejemplo #2: Ethernet Hanrun HR911105A"
  "</title></head>"
  "<body>"
    "<h3>Hola Mundo</h3>"
    "<p><em>"
      "Esto es una prueba.<br />"
      ":)."
    "</em></p>"
  "</body>"
"</html>"
;

void setup(){
  Serial.begin(19200);
  Serial.println("\n[Vuelva pronto]");
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Error al acceder al controlador Ethernet");
#if ESTATICA
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP falló");
#endif

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  
}

void loop(){
  //Se espera un paquete TCP entrante, luego se ignora el contenido
  if (ether.packetLoop(ether.packetReceive())) {
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  }
}
