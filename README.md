# ESP32 Wi-Fi Web Server LED Control Tutorial

A beginner-friendly Arduino tutorial demonstrating how to control an LED connected to an **ESP32** over Wi-Fi using a web browser.

This repository covers **both ways** to host a web server on the ESP32:
1. **Station Mode (`SimpleWiFiServer.ino`):** The ESP32 connects to your existing home/office Wi-Fi router.
2. **Access Point Mode (`SimpleWiFiAPServer.ino`):** The ESP32 creates its own Wi-Fi hotspot that your phone or laptop connects to directly (no router or internet needed).

---

## Project Demo Videos

| Demo | Description | Video File |
| :--- | :--- | :--- |
| **Mobile Browser Control** | Controlling the ESP32 LED (`/H` & `/L`) from a smartphone browser on the local network | [Watch Mobile Demo (`media/mobile-control-demo.mp4`)](media/mobile-control-demo.mp4) |
| **Laptop Browser Control** | Controlling the ESP32 LED (`/H` & `/L`) from a laptop web browser on the local network | [Watch Laptop Demo (`media/laptop-control-demo.mp4`)](media/laptop-control-demo.mp4) |

---

## Hardware Required

- **ESP32 Development Board**
- **1x LED** (or use the onboard LED if connected to GPIO 5)
- **1x 220$\Omega$ – 330$\Omega$ Resistor**
- **Breadboard & Jumper Wires**
- **USB Cable** (for programming and power)

### Circuit & Wiring

| ESP32 Pin | Component Connection |
| :--- | :--- |
| **GPIO 5** | LED Anode (`+`, longer leg) |
| **GND** | LED Cathode (`-`, shorter leg) via a 220$\Omega$ resistor |

---

## Understanding the Two Modes

| Feature | 1. Station Mode (`SimpleWiFiServer.ino`) | 2. Access Point Mode (`SimpleWiFiAPServer.ino`) |
| :--- | :--- | :--- |
| **Who hosts the Wi-Fi?** | Your home Wi-Fi router | The ESP32 itself (Hotspot) |
| **Who hosts the webpage?** | The ESP32 | The ESP32 |
| **Key Arduino function** | `WiFi.begin(ssid, password)` | `WiFi.softAP(ssid, password)` |
| **Internet stays active on phone/PC?** | Yes (connected to your normal Wi-Fi) | No (connected directly to ESP32's Wi-Fi) |
| **Works outdoors / without a router?** | No | Yes |

---

## Tutorial 1: Station Mode (`SimpleWiFiServer.ino`)

In this mode, the ESP32 joins your existing Wi-Fi network and gets an IP address from your router.

### Steps:
1. Open `SimpleWiFiServer.ino` in the **Arduino IDE**.
2. Enter your Wi-Fi router credentials at the top of the sketch:
   ```cpp
   const char *ssid = "YOUR_WIFI_SSID";
   const char *password = "YOUR_WIFI_PASSWORD";
   ```
3. Select your ESP32 board (**Tools > Board**) and COM port (**Tools > Port**), then click **Upload**.
4. Open the **Serial Monitor** (**Tools > Serial Monitor**) and set the baud rate to **`115200`**.
5. Press the **EN / RST** button on your ESP32. Once connected, you will see output like:
   ```text
   Connecting to YOUR_WIFI_SSID
   ....
   WiFi connected.
   IP address:
   192.168.1.45
   ```
6. Make sure your phone or computer is connected to the **same Wi-Fi router**, open a browser, and visit the IP address shown in the Serial Monitor (e.g., `http://192.168.1.45`).
7. Click the links on the page (or visit `/H` and `/L`) to turn the LED on and off!

---

## Tutorial 2: Access Point / Hotspot Mode (`SimpleWiFiAPServer.ino`)

In this mode, the ESP32 broadcasts its own Wi-Fi network so you can connect directly to it anywhere.

### Steps:
1. Open `SimpleWiFiAPServer.ino` in the **Arduino IDE**.
2. *(Optional)* Customize the Wi-Fi hotspot name and password in the code:
   ```cpp
   const char *ssid = "ESP32_LED_Server";
   const char *password = "12345678"; // Must be at least 8 characters
   ```
3. Upload the sketch to your ESP32 and open the **Serial Monitor** at **`115200`** baud:
   ```text
   Configuring access point...
   AP Wi-Fi Network (SSID): ESP32_LED_Server
   AP IP address: 192.168.4.1
   Server started
   ```
4. On your phone or laptop, open Wi-Fi settings and connect to **`ESP32_LED_Server`** using password **`12345678`**.
5. Open a web browser and go to:
   ```text
   http://192.168.4.1
   ```
6. Click the links on the webpage to turn the LED on (**`/H`**) and off (**`/L`**).

---

## How the Code Works

1. **Starting the Web Server:**
   ```cpp
   NetworkServer server(80);
   ```
   Creates an HTTP server listening on port `80` (the standard port for HTTP web traffic).

2. **Listening for Browser Connections:**
   Inside `loop()`, `NetworkClient client = server.accept();` checks if a web browser is requesting a page.

3. **Reading HTTP Requests & Controlling the LED:**
   When you click a link in the browser, your browser sends an HTTP `GET` request:
   - `GET /H` $\rightarrow$ triggers `digitalWrite(5, HIGH)` to turn the LED **ON**.
   - `GET /L` $\rightarrow$ triggers `digitalWrite(5, LOW)` to turn the LED **OFF**.

4. **Sending the HTML Response:**
   When the request header ends with a blank line, the ESP32 responds with `HTTP/1.1 200 OK` and sends back the HTML links so your browser renders the control page.

---

## Note When Compiling in Arduino IDE

Because the Arduino IDE automatically merges all `.ino` files inside the same folder, keep only **one** `.ino` file per sketch folder when compiling (or move `SimpleWiFiAPServer.ino` into its own `SimpleWiFiAPServer/` folder before clicking Upload).
