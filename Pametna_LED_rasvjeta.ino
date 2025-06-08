#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int ledPin1 = 5;
const int ldrPin1 = 34;
const int ledPin2 = 18;
const int ldrPin2 = 35;

int led1Mode = 2;
int led2Mode = 2;

bool led1State = false;
bool led2State = false;

int thresholdLed1 = 2000;
int thresholdLed2 = 1000;

const char* ssid = "Ante";
const char* password = "stilud123";

WebServer server(80);

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display.clearDisplay();
  display.display();

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());

  setupServerRoutes();
  server.begin();
}

void loop() {
  server.handleClient();

  led1State = controlLED(led1Mode, ledPin1, ldrPin1, thresholdLed1);
  led2State = controlLED(led2Mode, ledPin2, ldrPin2, thresholdLed2);

  updateDisplay();
  delay(200);
}

bool controlLED(int mode, int ledPin, int ldrPin, int threshold) {
  if (mode == 0) {
    digitalWrite(ledPin, LOW);
    return false;
  } else if (mode == 1) {
    digitalWrite(ledPin, HIGH);
    return true;
  } else {
    int ldrVal = analogRead(ldrPin);
    bool ledOn = ldrVal <= threshold;
    digitalWrite(ledPin, ledOn ? HIGH : LOW);
    return ledOn;
  }
}

void updateDisplay() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("LED1: ");
  display.print(modeText(led1Mode, led1State));

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 16);
  display.print("LED2: ");
  display.print(modeText(led2Mode, led2State));

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 48);
  display.print("Connect to:");

  display.setCursor(0, 56);
  display.print(WiFi.localIP().toString());

  display.display();
}


String modeText(int mode, bool state) {
  if (mode == 0) return "OFF";
  if (mode == 1) return "ON";
  return "AUTO";
}

String getButtonStyle(bool isActive) {
  if (isActive) {
    return "background-color:#007BFF; color:white; border:none; border-radius:4px; padding:12px 20px; font-size:16px; margin:5px; cursor:pointer;";
  } else {
    return "background-color:white; color:#007BFF; border:2px solid #007BFF; border-radius:4px; padding:12px 20px; font-size:16px; margin:5px; cursor:pointer;";
  }
}

String getHTML() {
  String html = R"rawliteral(
    <!DOCTYPE html><html><head><meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin-top: 40px; }
      input[type="number"] {
        width: 80px; padding: 8px; font-size: 14px; margin: 5px; border-radius: 4px; border: 1px solid #ccc; text-align: center;
      }
      form { margin-bottom: 20px; }
    </style>
    </head><body>
  )rawliteral";

  html += "<h2>LED1 - RED (Mode: " + modeText(led1Mode, led1State) + ")</h2>\n";
  html += "<button style='" + getButtonStyle(led1Mode == 0) + "' onclick=\"location.href='/led1/off'\">OFF</button>\n";
  html += "<button style='" + getButtonStyle(led1Mode == 1) + "' onclick=\"location.href='/led1/on'\">ON</button>\n";
  html += "<button style='" + getButtonStyle(led1Mode == 2) + "' onclick=\"location.href='/led1/auto'\">AUTO</button>\n";
  html += R"rawliteral(
    <form action="/setThreshold" method="POST">
      <p>Current threshold: )rawliteral" + String(thresholdLed1) + R"rawliteral(</p>
      <label>Set threshold:
        <input type="number" name="threshold" value=)" + String(thresholdLed1) + R"rawliteral( min="0" max="4095" required>
      </label>
      <input type="hidden" name="ledNum" value="1">
      <button type="submit">Set Threshold</button>
    </form>
  )rawliteral";

  html += "<h2>LED2 - YELLOW (Mode: " + modeText(led2Mode, led2State) + ")</h2>\n";
  html += "<button style='" + getButtonStyle(led2Mode == 0) + "' onclick=\"location.href='/led2/off'\">OFF</button>\n";
  html += "<button style='" + getButtonStyle(led2Mode == 1) + "' onclick=\"location.href='/led2/on'\">ON</button>\n";
  html += "<button style='" + getButtonStyle(led2Mode == 2) + "' onclick=\"location.href='/led2/auto'\">AUTO</button>\n";
  html += R"rawliteral(
    <form action="/setThreshold" method="POST">
      <p>Current threshold: )rawliteral" + String(thresholdLed2) + R"rawliteral(</p>
      <label>Set threshold:
        <input type="number" name="threshold" value=)" + String(thresholdLed2) + R"rawliteral( min="0" max="4095" required>
      </label>
      <input type="hidden" name="ledNum" value="2">
      <button type="submit">Set Threshold</button>
    </form>
  )rawliteral";

  html += "</body></html>";

  return html;
}

void setupServerRoutes() {
  server.on("/", []() {
    server.send(200, "text/html", getHTML());
  });

  server.on("/led1/off", []() { led1Mode = 0; server.send(200, "text/html", getHTML()); });
  server.on("/led1/on",  []() { led1Mode = 1; server.send(200, "text/html", getHTML()); });
  server.on("/led1/auto",[]() { led1Mode = 2; server.send(200, "text/html", getHTML()); });

  server.on("/led2/off", []() { led2Mode = 0; server.send(200, "text/html", getHTML()); });
  server.on("/led2/on",  []() { led2Mode = 1; server.send(200, "text/html", getHTML()); });
  server.on("/led2/auto",[]() { led2Mode = 2; server.send(200, "text/html", getHTML()); });

  server.on("/setThreshold", HTTP_POST, []() {
    if (server.hasArg("ledNum") && server.hasArg("threshold")) {
      int ledNum = server.arg("ledNum").toInt();
      int newThreshold = server.arg("threshold").toInt();
      if (newThreshold >= 0 && newThreshold <= 4095) {
        if (ledNum == 1) thresholdLed1 = newThreshold;
        else if (ledNum == 2) thresholdLed2 = newThreshold;
      }
    }
    server.sendHeader("Location", "/");
    server.send(303);
  });
}
