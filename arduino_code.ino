#include <SPI.h>
#include <Ethernet.h>
#include <Ultrasonic.h>
#include <NewPing.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;

Ultrasonic ultrasonic(8, 9);
NewPing sonar(8, 9, 450);

void setup() {
  Serial.begin(9600);

  Serial.begin(9600);
  while (!Serial) {
  }

  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    while (true) {
      delay(1);
    }
  }

  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void sendData(String data) {
  if (client.connect("192.168.1.10", 3000)) {
    Serial.println("Connected to server");
    client.println("POST /api/test HTTP/1.1");
    client.println("Host: 192.168.1.10");
    client.println("Connection: close");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
    client.println();    

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  } else {
    Serial.println("connection failed");
  }
}

void loop() {
  delay(1000);
  sendData(String(0.1715 * sonar.ping_median(60)));
}
