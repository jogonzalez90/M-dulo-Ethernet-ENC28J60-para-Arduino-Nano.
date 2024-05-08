#include <EtherCard.h>

static byte mymac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};
static byte ip[] = {192,168,18,200}; //Romo
byte Ethernet::buffer[700];
char estado[] = "OFF";

void setup() {
  Serial.begin(9600);
  if (!ether.begin(sizeof Ethernet::buffer, mymac, 10))
    Serial.println("Error al iniciar ethernet");
  else
    Serial.println("Ethernet inicializado");
  if (!ether.staticSetup(ip))
    Serial.println("No se pudo obtener dirección IP");
}

void loop() {
  word rec = ether.packetReceive();
  word pack = ether.packetLoop(rec);
  if(pack) {
    if (strstr((char *)Ethernet::buffer + pack, "GET /estado") != 0) {
      ether.httpServerReply(homePage());
      return;
    }
    ether.httpServerReply(errorPage());
  }
}

static word homePage() {
  BufferFiller bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
"<html>"
"<head>"
"</head>"
"<body>"
"<h2>Estado del LED</h2>"
"<p>Estado actual: $S</p>"
"</body>"
"</html>"
), estado);
  return bfill.position();
}

static word errorPage() {
  BufferFiller bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
"<html>"
"<head>"
"</head>"
"<body>"
"<h2>Error</h2>"
"<p>Ocurrió un error al procesar la solicitud</p>"
"</body>"
"</html>"
));
  return bfill.position();
}
