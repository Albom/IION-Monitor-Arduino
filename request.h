#ifndef REQUEST_H
#define REQUEST_H

#include <Ethernet.h>



//! Адрес устройства.
#define IP IPAddress(172, 17, 24, 131)
#define GATEWAY IPAddress(172, 17, 24, 1)
#define SUBNET IPAddress(255, 255, 255, 0)
byte MAC[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };


 
//! Глобальный сервер.
EthernetServer server(80);



//! Ответить на запрос. Определение в файле IION-Monitor-Arduino.
extern void requestResponse(EthernetClient& client);



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
  Ethernet.begin(MAC, IP, GATEWAY, SUBNET);
  server.begin();
}

#endif // REQUEST_H
