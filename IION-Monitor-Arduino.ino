#include <OneWire.h>
#include <SPI.h>
#include <Ethernet.h>

#define SERIAL_ENABLED 0

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(172, 17, 24, 131);
IPAddress gateway(172, 17, 24, 1);
IPAddress subnet(255, 255, 255, 0);


EthernetServer server(80);
OneWire  ds(9);  // DS18b20 on pin 9 (a 4.7K resistor is necessary)


char output_buffer[128];
char one_sensor_data[32];


void setup() {

#if SERIAL_ENABLED
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif


  delay(250);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();


#if SERIAL_ENABLED
  Serial.print(F("server is at "));
  Serial.println(Ethernet.localIP());
#endif

  one_sensor_data[16] = ' ';

}


void eth(EthernetClient client) {


#if SERIAL_ENABLED
  Serial.println(F("new client"));
#endif

  boolean currentLineIsBlank = true;
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
#if SERIAL_ENABLED
      Serial.write(c);
#endif

      if (c == '\n' && currentLineIsBlank) {
        client.println(F("HTTP/1.1 200 OK"));
        client.println(F("Content-Type: text/plain"));
        client.println(F("Connection: close"));
        client.println();


//        temperature();

        client.print((char *)&output_buffer[0]);

        break;

      }


      if (c == '\n') {
        currentLineIsBlank = true;
      } else if (c != '\r') {
        currentLineIsBlank = false;
      }
    }
  }
  delay(1);
  client.stop();

#if SERIAL_ENABLED
  Serial.println(F("client disconnected"));
#endif

}

void addr2str(char *str, const byte *addr) {

  const char d[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

  for (byte i = 0; i < 8; i++) {
    str[i * 2] = d[addr[i] / 16];
    str[i * 2 + 1] = d[addr[i] % 16];
  }
}

void temperature() {

  byte i;
  byte data[12];
  byte addr[8];



  output_buffer[0] = '\0';

  while (ds.search(addr)) {

    addr2str(one_sensor_data, addr);

#if SERIAL_ENABLED
    for ( i = 0; i < 16; i++) {
      Serial.write(one_sensor_data[i]);
    }
    Serial.write(' ');
#endif

    if (OneWire::crc8(addr, 7) != addr[7]) {
      continue;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);

    delay(1000);

    ds.reset();
    ds.select(addr);
    ds.write(0xBE);

    for ( i = 0; i < 9; i++) {
      data[i] = ds.read();
    }

    int16_t raw = (data[1] << 8) | data[0];

    itoa(raw / 16, one_sensor_data + 17, 10);
    strcat(output_buffer, "\n");
    strcat(output_buffer, one_sensor_data);

#if SERIAL_ENABLED
    Serial.println(raw / 16);
#endif

    delay(250); // ?

  }

  ds.reset_search();

}

void loop() {

  EthernetClient client = server.available();
  if (client) {
    eth(client);
  }

  temperature();

}

