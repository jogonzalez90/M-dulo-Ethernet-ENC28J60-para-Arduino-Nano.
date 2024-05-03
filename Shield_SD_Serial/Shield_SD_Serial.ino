#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 }; // Dirección MAC del shield Ethernet
IPAddress ip(192,168,18,200); // Dirección IP del Arduino Nano
IPAddress dns(192,168,18,1);
IPAddress gateway(192,168,18,1);
IPAddress subnet(255,255,255,0);

EthernetServer server(80); // Puerto para el servidor web

void setup() {
  Ethernet.begin(mac, ip, dns, gateway, subnet); // Iniciar la conexión Ethernet
  server.begin(); // Iniciar el servidor web
}

void loop() {
  EthernetClient client = server.available(); // Esperar a que llegue una conexión
  
  if (client) { // Si hay una conexión
    while (client.connected()) { // Mientras la conexión esté activa
      client.println("HTTP/1.1 200 OK"); // Enviar el encabezado HTTP
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("<head><title>Ethernet Shield V1.0 Nano</title></head>");
      client.println("<body>");
      client.println("<h1>Hola desde Arduino Nano con Ethernet Shield V1.0</h1>");
      client.println("</body>");
      client.println("</html>");
      
      break; // Salir del bucle
    }
    client.stop(); // Cerrar la conexión
  }
}
