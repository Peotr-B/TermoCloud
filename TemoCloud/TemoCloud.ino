/* 26апр25
https://microkontroller.ru/esp8266-projects/podklyuchenie-datchika-temperatury-ds18b20-k-esp8266/
Подключение датчика температуры DS18B20 к NodeMCU ESP8266 и Google Firebase
*/

#include <ESP8266WiFi.h>                 
#include <FirebaseArduino.h>  

#include <OneWire.h>
#include <DallasTemperature.h>

#define FIREBASE_HOST "temocloud-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "***" // ваш секретный ключ из firebase
#define WIFI_SSID "***" // Change the name of your WIFI
#define WIFI_PASSWORD "***" // Change the password of your WIFI
 
// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  delay(1000);
  sensors.begin();
  delay(1000);
               
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                  //печатаем локальный IP адрес
  delay(5000);
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                    // соединяемся с firebase
  Firebase.setString("TempºC", "TempºF");                         //передаем начальную строку состояния светодиода
}

// https://microkontroller.ru/esp8266-projects/avtomatizacziya-doma-s-pomoshhyu-google-firebase-i-nodemcu-esp8266/?ysclid=m9wgwbv7v1992380719
void firebasereconnect()
{
  Serial.println("Trying to reconnect");
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  }

void loop() {

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
    
 if (Firebase.failed())
      {
      Serial.print("setting number failed:");
      Serial.println(Firebase.error());
      firebasereconnect();
      return;
      }

  Firebase.setFloat ("TempºC",temperatureC);
  Firebase.setFloat ("TempºF",temperatureF);

  delay(3000);
}
