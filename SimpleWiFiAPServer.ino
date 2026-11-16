/*
 WiFi Access Point (SoftAP) Web Server LED Blink

 A simple web server hosted on the ESP32's own Wi-Fi network (Access Point mode)
 that lets you control an LED via a web browser without needing an external router.

 Steps to use:
 1. Upload this sketch to your ESP32.
 2. Connect your phone or laptop to the ESP32's Wi-Fi network:
    - SSID: "ESP32_LED_Server"
    - Password: "12345678"
 3. Open a web browser and go to the IP address printed in the Serial Monitor
    (default is http://192.168.4.1):
    - http://192.168.4.1/H turns the LED on
    - http://192.168.4.1/L turns the LED off

 Circuit:
 * ESP32 board
 * LED attached to pin 5
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>

const char *ssid = "ESP32_LED_Server";
const char *password = "12345678";  // Must be at least 8 characters (or "" for an open network)

NetworkServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);  // set the LED pin mode

  delay(10);

  Serial.println();
  Serial.println("Configuring access point...");

  // Start the ESP32 as a Wi-Fi Access Point (hotspot)
  if (!WiFi.softAP(ssid, password)) {
    Serial.println("Soft AP creation failed.");
    while (1);
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP Wi-Fi Network (SSID): ");
  Serial.println(ssid);
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.begin();
  Serial.println("Server started");
}

void loop() {
  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(5, HIGH);  // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(5, LOW);  // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
