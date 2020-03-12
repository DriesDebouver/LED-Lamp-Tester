#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <BH1750.h>
#include <Adafruit_SSD1306.h>
Adafruit_BMP280 bmp;
BH1750 lightMeter;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char auth[] = "nYqsKGPM3tYvzfpVRkfGj_wbrzsRlI5r";
char ssid[] = "WiFi-Debouver-2.4";
char pass[] = "UzpSk6g9D3E3";

float bmpTemp;
float bmpPress;
float lux;

void setup() {
  Serial.begin(115200);

  //BMP280 Sensor:
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

  //SSD1306 OLED:
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  //BH1750 Sensor:
  lightMeter.begin();

  //WiFi & Blynk:
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);
}

void loop() {
  delay(1000);
  //BMP280:
  bmpTemp = bmp.readTemperature();

  //BH1750:
  lux = lightMeter.readLightLevel();

  //Serial monitor print:
  Serial.println("--------------------------------");
  Serial.println("BMP280 Sensor:");
  Serial.print("Temperature = ");
  Serial.print(bmpTemp);
  Serial.println(" °C");
  Serial.println("BH1750 Sensor:");
  Serial.print("Light = ");
  Serial.print(lux);
  Serial.println(" lux");

  //OLED print:
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  display.println("BMP280:");
  display.setCursor(0, 10);
  display.setTextColor(BLACK, WHITE);
  display.print((char)247); // ' ° ' graden symbool
  display.print("C: ");
  display.print(bmpTemp, 1);
  
  display.setCursor(0, 30);
  display.setTextColor(WHITE);
  display.println("BH1750:");
  display.setCursor(0, 40);
  display.setTextColor(BLACK, WHITE);
  display.print("Light = ");
  display.print(lux);
  display.print(" lux");
  display.display();
  
  //Blynk:
  Blynk.run();
  Blynk.virtualWrite(V1, bmpTemp);
  Blynk.virtualWrite(V2, lux);
}
