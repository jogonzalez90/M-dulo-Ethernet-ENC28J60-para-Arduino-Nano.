#include <EtherCard.h>
#include <OneWire.h>

//Pines lectura de chips DS1990 4 unidades
OneWire  iB1(11);                            // Cara a corresponde a D11
OneWire  iB2(12);                            // Cara b corresponde a D12
OneWire  iB3(2);                             // Cara c corresponde a D2
OneWire  iB4(3);                             // Cara d corresponde a D3

static byte mymac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};
//static byte ip[] = {192,168,1,200}; //Cangrejo
static byte ip[] = {192,168,18,200}; //Romo
byte Ethernet::buffer[700];
char EstadoP7 []= "OFF";

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
    //if (strstr((char *)Ethernet::buffer + pack, "POST /?estado=") != 0) {
      Serial.println("Mensaje recibido= ");
      char *ptr = (char *)Ethernet::buffer + pack;
      ptr = strstr(ptr, "estado=");
      //if (ptr != 0) {
        ptr += 7; // saltar "estado="
        EstadoP7[0] = ptr[0];
        EstadoP7[1] = ptr[1];
        if (EstadoP7[0] == 'l'){
        Serial.println("Prueba l");
        ether.httpServerReply(ipPuertos());
        }
        else if (EstadoP7[0] == 'o'){
          Serial.println("Prueba o");
          ether.httpServerReply(errorPage());
        }
        else {
          Serial.println("Pagina principal");
          ether.httpServerReply(homePage());
        }
        //Serial.println(EstadoP7[0]);
        //Serial.println(EstadoP7[1]);
        //ether.httpServerReply(homePage());
        //return;
      //}
    //}
    //else {
    //  ether.httpServerReply(homePage());
    //}
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
"<input type='text' id='estado' name='estado' placeholder='ON/OFF' />"
"<button onclick='enviarEstado()'>Enviar</button>"
"</body>"
"</html>"
));
  return bfill.position();
}

static word ipPuertos() {
  BufferFiller bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
 "<html>"
"<head>"
"<script>"
"<h2>Estado del LED</h2>"
"function enviarEstado() {"
"  var estado = document.getElementById('estado').value;"
"  var xhttp = new XMLHttpRequest();"
"  xhttp.open('POST', '/?estado=' + estado, true);"
"  xhttp.send();"
"}"
"</script>"
"</head>"
"<body>"
"<input type='text' id='estado' name='estado' placeholder='ON/OFF' />"
"<button onclick='enviarEstado()'>Enviar</button>"
"</body>"
"</html>"
));
  return bfill.position();
}

static word errorPage() {
  BufferFiller bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
    "HTTP/1.0 404 Not Found\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"
    "\r\n"
    "<!DOCTYPE html>"
    "<html>"
    "<head><title>Error 404</title></head>"
    "<body>"
    "<h1>Error 404</h1>"
    "<p>La página solicitada no se encontró.</p>"
    "</body>"
    "</html>"
  ));
  return bfill.position();
}