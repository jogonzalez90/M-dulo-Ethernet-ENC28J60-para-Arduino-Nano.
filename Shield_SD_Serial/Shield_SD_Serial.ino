#include <EtherCard.h>

static byte mymac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};
static byte ip[] = {192,168,1,200}; //Cangrejo
byte Ethernet::buffer[700];
char EstadoP7 = "OFF";

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
    Serial.println(EstadoP7);
    if (strstr((char *)Ethernet::buffer + pack, "POST /?estado=") != 0) {
      Serial.println(EstadoP7);
      char *ptr = (char *)Ethernet::buffer + pack;
      ptr = strstr(ptr, "estado=");
      Serial.println("Dato recibido= ");
      //Serial.println(ptr);
      if (ptr != 0) {
        //ptr += 7; // saltar "estado="
        //EstadoP7[0] = ptr[0];
        ether.httpServerReply(homePage());
        return;
      }
    }
    ether.httpServerReply(homePage());
  }
}

static word homePage() {
  BufferFiller bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
"<html>"
"<head>"
"<script>"
"function enviarEstado() {"
"  var estado = document.getElementById('estado').value;"
"  var xhttp = new XMLHttpRequest();"
"  xhttp.open('POST', '/?estado=' + estado, true);"
"  xhttp.send();"
"}"
"</script>"
"</head>"
"<body>"
"<h2>Control de LED</h2>"
"<input type='text' id='estado' name='estado' placeholder='ON/OFF' />"
"<button onclick='enviarEstado()'>Enviar</button>"
"<p>Estado del LED: $S</p>"
"</body>"
"</html>"
), EstadoP7);
  return bfill.position();
}
