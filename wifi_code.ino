
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <HardwareSerial.h>

const char* ssid = "Wifi ssid";
const char* password = "password";

WebServer server(80);

float voltage = 0;
float current = 0;
float power = 0;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    int comma1 = data.indexOf(',');
    int comma2 = data.indexOf(',', comma1 + 1);

    if (comma1 != -1 && comma2 != -1) {
      voltage = data.substring(0, comma1).toFloat();
      current = data.substring(comma1 + 1, comma2).toFloat();
      power = data.substring(comma2 + 1).toFloat();

      Serial.print("Voltage: ");
      Serial.println(voltage);
      Serial.print("Current: ");
      Serial.println(current);
      Serial.print("Power: ");
      Serial.println(power);
    }
  }

  delay(100);
}

void handleRoot() {
  String webpage = "<!DOCTYPE html>";
  webpage += "<html lang='en'>";
  webpage += "<head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  webpage += "<title>ESP32 Data</title><style>";
  webpage += "body { font-family: Arial; background-color: #f4f4f4; }";
  webpage += "header { background-color: #4CAF50; color: white; text-align: center; padding: 1em; }";
  webpage += "main { padding: 20px; } .data { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px #aaa; }";
  webpage += ".refresh-button { margin-top: 20px; padding: 10px 20px; background: #4CAF50; color: white; border: none; border-radius: 5px; }";
  webpage += ".refresh-button:hover { background: #45a049; }";
  webpage += "</style><script>function refreshPage() { location.reload(); }</script></head><body>";
  webpage += "<header><h1>Solar Monitoring System</h1></header><main><div class='data'>";
  webpage += "<p><strong>Voltage:</strong> " + String(voltage) + " V</p>";
  webpage += "<p><strong>Current:</strong> " + String(current) + " A</p>";
  webpage += "<p><strong>Power:</strong> " + String(power) + " W</p>";
  webpage += "<button class='refresh-button' onclick='refreshPage()'>Refresh</button>";
  webpage += "<p><strong>Max Voltage:</strong> 6.06 V</p>";
  webpage += "<p><strong>Max Current:</strong> 0.52 A</p>";
  webpage += "<p><strong>Max Power:</strong> 3.15 W</p>";
  webpage += "</div></main><footer><h2>Done By:</h2>";
  webpage += "<p>ARULSELVAN(7276722BEE104)</p><p>SASIDHARAN(7276722BEE020)</p><p>SAKTHI SRI(7276722BEE100)</p>";
  webpage += "</footer></body></html>";
  server.send(200, "text/html", webpage);
}
