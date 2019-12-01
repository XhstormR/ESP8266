#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

DNSServer dnsServer;
ESP8266WebServer webServer(80);

void handleRoot() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  String payload = "Connected devices = ";
  payload += WiFi.softAPgetStationNum();
  payload += "\n";

  int n = WiFi.scanNetworks();
  if (n == 0) {
    payload += "no networks found";
  } else {
    payload += n;
    payload += " networks found";
    payload += "\n";
    for (int i = 0; i < n; i++) {
      payload += i + 1;
      payload += ": ";
      payload += WiFi.SSID(i);
      payload += " (";
      payload += WiFi.RSSI(i);
      payload += ")";
      payload += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
      payload += "\n";
    }
  }

  webServer.send(200, "text/plain", payload);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);

  /*
  String ssid = "free";
  String password = "free";
  Serial.printf("Connecting to %s", ssid.c_str());
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  Serial.printf("Station IP address = %s\n", WiFi.localIP().toString().c_str());
  */

  WiFi.mode(WIFI_AP);
  WiFi.softAP("LEO", "12345678");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  webServer.on("/", handleRoot);
  webServer.begin();

  dnsServer.start(53, "qq.com", IPAddress(192, 168, 4, 1));
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
