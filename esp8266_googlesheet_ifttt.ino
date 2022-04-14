
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>
#define DHTPIN 0          //pin where the dht11 is connected
DHT dht(DHTPIN, DHT11);

WiFiClient wifiClient;
//ตั้งค่า wifi
const char* ssid = "your ssid"; // ชื่อ
const char* password = "your password"; // รหัสผ่าน

// ตั้งค่า IFTTT
String event = "demo"; //  event from ifttt
String key = "====iftt key====="; // copy from ifttt link

String ifttturl = "";
String Httpget="";

void setup () {
  //สำหรับ esp8266 url ตรง https ให้เปลี่ยนนเป็็น http
  ifttturl = "http://maker.ifttt.com/trigger/demo/with/key/+++key++++"; // url ที่ส่งค่าลง google sheet

  Serial.begin(115200);
  dht.begin();
  Serial.println(ifttturl);
  WiFi.begin(ssid, password);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
}

void loop() {
 int moisture=0;
  if (WiFi.status() == WL_CONNECTED) { //ถ้าเชื่อมต่อสำเร็จ
        for(int i=0;i<100;i++){
           moisture= moisture +analogRead(A0);
           delay (10);
        }
        moisture=moisture/100;
    float h = 191.11111; //dht.readHumidity();
    float t = 19.0;// dht.readTemperature();
    HTTPClient http;
    int Value1= moisture; 
    float Value2=h;
    float Value3=t;
    Httpget =ifttturl +"?" +"value1=" +Value1 + "&" +"value2=" +String(Value2,3) + "&" +"value3=" +String(Value3,3);
    Serial.println(Httpget);
    http.begin(wifiClient,Httpget); //กำหนด url เพื่อเซฟข้อมูลลง google sheets
 
    int httpCode = http.GET(); //ส่งค่า url
    if (httpCode > 0) { //  if success httpCode will greater than zero (1)
        String payload = http.getString();// อ่านค่าผลลัพธ์
        Serial.println(payload);
    }
    http.end(); //ปิดการเชื่อมต่อ

  }
  delay(30000); //หน่วงเวลา เซฟข้อมูลทุก 30 วินาที
}
