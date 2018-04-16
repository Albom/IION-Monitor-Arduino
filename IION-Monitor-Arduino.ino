#include <OneWire.h>
//#include <SPI.h>
#include <Ethernet.h>

#define SERIAL_ENABLED 0

EthernetServer server(80);
OneWire  ds(9);  // DS18b20 on pin 9 (a 4.7K resistor is necessary)
const byte sensorsCount = 5;
char output_buffer[128];
char one_sensor_data[sensorsCount][32];
byte addr[sensorsCount][8];
byte data[12];
byte addrCount = 0;


//! Настройка при включении
void setup() {
  //! Адресс
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  //IPAddress ip(192, 168, 1, 200);
  IPAddress ip(172, 17, 24, 131);
  IPAddress gateway(172, 17, 24, 1);
  IPAddress subnet(255, 255, 255, 0);

#if SERIAL_ENABLED
  Serial.begin(9600); // Можно больше???
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif

  delay(250);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();

  addrCount = 0;
  while (ds.search(addr[addrCount]) && (addrCount < sensorsCount)) {
    addr2str(one_sensor_data[addrCount], addr[addrCount]);
    one_sensor_data[addrCount][16] = ' ';
    ++addrCount;
  }
  for (byte i = 0; i < addrCount; ++i) {
    one_sensor_data[i][16] = ' ';
  }
}



void eth(EthernetClient client) {
  bool start = true;
  bool currentLineIsBlank = true;
  while (client.connected()) {
    if (start) {
      start = false;
      output_buffer[0] = '\0';
      for (byte i = 0; i < addrCount; ++i) {
        strcat(output_buffer, one_sensor_data[i]);
        strcat(output_buffer, "\n");
      }
    }
    //if (client.available()) {
    char c = client.read();

    if (c == '\n' && currentLineIsBlank) {
      client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Content-Type: text/plain"));
      client.println(F("Connection: close"));
      client.println();
      client.print((char *)&output_buffer[0]);
      break;
    }

    if (c == '\n') {
      currentLineIsBlank = true;
    } else if (c != '\r') {
      currentLineIsBlank = false;
    }
    //}
  }
  delay(1); // ???
  client.stop();
}



//! Преобразование восьмибитного адреса в шестнадцатеричную строку
void addr2str(char *str, const byte *addr) {
  const char d[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  for (byte i = 0; i < 8; i++) {
    str[i * 2] = d[addr[i] / 16];
    str[i * 2 + 1] = d[addr[i] % 16];
  }
}


//! Считывание температуры в буфер
void temperature() {
  static byte index = 0;
  if (index == addrCount) {
    index = 0;
  }
  byte i;
  //  output_buffer[0] = '\0';
  //while (ds.search(addr)) {
  addr2str(one_sensor_data[index], addr[index]);

  if (OneWire::crc8(addr[index], 7) == addr[index][7]) {
    ds.reset();
    ds.select(addr[index]);
    ds.write(0x44, 1);

    //! Задержка 750+ милисекунд
    delay(1000);

    ds.reset();
    ds.select(addr[index]);
    ds.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data[i] = ds.read();
    }

    int16_t raw = (data[1] << 8) | data[0];
    itoa(raw / 16, one_sensor_data[index] + 17, 10);
    //strcat(output_buffer, one_sensor_data);
    //strcat(output_buffer, "\n");
    delay(250); // ?
  }
  ++index;
}



void loop() {
  temperature();
  for (int sock = 0; sock < MAX_SOCK_NUM; sock++) {
    EthernetClient client = server.available_(sock);
    eth(client);
  }
}

