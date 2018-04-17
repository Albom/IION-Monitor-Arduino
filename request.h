#ifndef REQUEST_H
#define REQUEST_H

#include <Ethernet.h>



// Обьявления
#define IP IPAddress(172, 17, 24, 131)
#define GATEWAY IPAddress(172, 17, 24, 1)
#define SUBNET IPAddress(255, 255, 255, 0)
extern char* getTemperature();
extern float getEnergy();


 
// Глобальные переменные:
EthernetServer server(80);



//! Ответить на запрос.
void requestResponse(EthernetClient& client) {
#if SERIAL_ENABLED
  Serial.println("Request");
#endif
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/plain"));
  client.println(F("Connection: close"));
  client.println();
  client.print(getTemperature());
  client.print("5000000000000001 ");
  client.println(getEnergy());
}



//! Проверить запрос.
void checkRequest() {
  for (uint8_t sock = 0; sock < MAX_SOCK_NUM; ++sock) {
    EthernetClient client = server.available_(sock);
    bool start = true;
    bool currentLineIsBlank = true;
    while (client.connected()) {
      char c = client.read();
      if (c == '\n' && currentLineIsBlank) {
        requestResponse(client);
        break;
      }

      if (c == '\n') {
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
    }
    client.stop();
  }
}



//! Настроить и запустить сервер.
void startServer() {
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  //IPAddress ip(192, 168, 1, 200);
//  IPAddress ip(172, 17, 24, 131);
//  IPAddress gateway(172, 17, 24, 1);
//  IPAddress subnet(255, 255, 255, 0);
  Ethernet.begin(mac, IP, GATEWAY, SUBNET);
  server.begin();
}

#endif // REQUEST_H
