#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MQUnifiedsensor.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define Board ("ESP-32")
#define Pin (36)
#define Threshold (23)
#define Type ("MQ-2")
#define Voltage_Resolution (3.3)
#define ADC_Bit_Resolution (12)
#define RatioMQ2CleanAir (9.83)
#define LED_PIN 2

// WiFi Configuration
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Server Configuration
const char* serverURL = "http://httpbin.org/post";

// MQ2 Library Instance Setup
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables
int gas_value = 0;
bool lastUnsafeState = false;
unsigned long lastPostTime = 0;
const unsigned long POST_COOLDOWN = 10000;

// Function declarations
void setupWiFi();
void sendUnsafeAlert(int gasPercentage, float ppmValue);

void setup() {
  // LCD Init
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.print("Gas Sensor");
  delay(1000);

  // Serial Port Communication Setup
  Serial.begin(9600);

  // WiFi Setup
  setupWiFi();

  // Set Parameters to detect PPM concentration for LPG
  MQ2.setRegressionMethod(1);
  MQ2.setA(574.25);
  MQ2.setB(-2.222);

  // MQ2 Init
  MQ2.init();

  // Calibrate Sensor
  Serial.print("Calibrating Sensor, please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ2.update();
    calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    Serial.print(".");
  }
  MQ2.setR0(calcR0/10);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ2 m= ");
  lcd.print(calcR0);
  delay(500);

  // Exception handling
  // Connection Issue Exceptions
  if(isinf(calcR0)) { Serial.println("Warning: R0 value is infinite. Please check your wiring."); while(1); }
  if(calcR0 == 0) { Serial.println("Warning: R0 value is zero. Please check your wiring."); while(1); }

  // Show MQ2 Debug Log
  MQ2.serialDebug(true);

  // Threshold Pin Setup
  pinMode(Threshold, INPUT);

  // LED Pin Setup
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // Update reading
  MQ2.update();
  MQ2.readSensor();
  MQ2.serialDebug();
  // Read values
  gas_value = analogRead(Pin);
  gas_value = map(gas_value, 0, 4095, 0, 100);
  Serial.println(gas_value);

  //Show value on Display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ2 = ");
  lcd.print(gas_value);
  lcd.print(" %");

  // Determine unsafe state based on gas percentage threshold
  bool currentUnsafeState = (digitalRead(Threshold) == HIGH);

  // Show Threshold Indicator and Update LED
  lcd.setCursor(0, 1);
  if (digitalRead(Threshold) == HIGH) {
    lcd.print("Unsafe");
    digitalWrite(LED_PIN, HIGH);
  } else {
    lcd.print("Safe");
    digitalWrite(LED_PIN, LOW);
  }

   // Show status on second line of LCD
  lcd.setCursor(0, 1);
  if (currentUnsafeState) {
    // Send POST if changed to unsafe and cooldown has passed
    if (!lastUnsafeState && (millis() - lastPostTime > POST_COOLDOWN)) {
      Serial.println("Enviando alerta POST...");
      sendUnsafeAlert(gas_value, (gas_value * 10));
      lastPostTime = millis();
    }
  }
  lastUnsafeState = currentUnsafeState;

  delay(500); // this speeds up the simulation
}

void setupWiFi() {
  WiFi.begin(ssid, password);
  
  Serial.print("Conectando a WiFi");
  lcd.clear();
  lcd.print("Conectando WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    if (attempts < 16) {
      lcd.setCursor(attempts, 1);
      lcd.print(".");
    }
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    
    lcd.clear();
    lcd.print("WiFi OK");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("Error: No se pudo conectar WiFi");
    lcd.clear();
    lcd.print("WiFi Error");
  }
  
  delay(2000);
}

void sendUnsafeAlert(int gasPercentage, float ppmValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");
    http.setTimeout(10000);
    
    int rawADC = analogRead(Pin);
    
    // Create JSON payload
    String jsonString = "{";
    jsonString += "\"timestamp\":" + String(millis()) + ",";
    jsonString += "\"sensor_id\":\"MQ2_ESP32_001\",";
    jsonString += "\"gas_level_percent\":" + String(gasPercentage) + ",";
    jsonString += "\"gas_level_ppm\":" + String(ppmValue, 2) + ",";
    jsonString += "\"raw_adc\":" + String(rawADC) + ",";
    jsonString += "\"status\":\"UNSAFE\",";
    jsonString += "\"threshold_digital\":" + String(digitalRead(Threshold)) + ",";
    jsonString += "\"r0_value\":" + String(MQ2.getR0(), 2) + ",";
    jsonString += "\"location\":\"Lab_Room_A\",";
    jsonString += "\"alert_type\":\"GAS_LEAK_DETECTED\",";
    jsonString += "\"device_ip\":\"" + WiFi.localIP().toString() + "\",";
    jsonString += "\"sensor_type\":\"MQ2_Unified\"";
    jsonString += "}";
    
    Serial.println("Enviando POST...");
    Serial.println("Payload: " + jsonString);
    
    int httpResponseCode = http.POST(jsonString);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
      
      // Show success briefly
      lcd.clear();
      lcd.print("POST OK!");
      lcd.setCursor(0, 1);
      lcd.print("Code: " + String(httpResponseCode));
      delay(1500);
      
    } else {
      Serial.println("POST Error: " + String(httpResponseCode));
      Serial.println("Error detail: " + http.errorToString(httpResponseCode));
      
      // Show error briefly
      lcd.clear();
      lcd.print("POST Error");
      lcd.setCursor(0, 1);
      lcd.print("Code: " + String(httpResponseCode));
      delay(1500);
    }
    
    http.end();
  } else {
    Serial.println("WiFi desconectado - no se puede enviar POST");
    lcd.clear();
    lcd.print("WiFi Error");
    delay(1000);
  }
}