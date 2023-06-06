#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"
#include "string.h"
 
const int LED_DO1 = D0;
const int LED_XANH1 = D1;
const int LED_VANG1 = D2;
const int LED_DO2 = D3;
const int LED_XANH2 = D4;
const int LED_VANG2 = D5;
int s = 30000;
int sensor,j,k,m,d,x,y;
float h ;
float t;
unsigned long lastMillis = 0;
unsigned long previousMillis = 0;
const long interval = 1000;
char a[50], b[50], c[50];
int dem, chukiden =0;
 
#define AWS_IOT_PUBLISH_TOPIC   "esp8266/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp8266/sub" 
 
WiFiClientSecure net;
 
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);
 
PubSubClient client(net);
 
time_t now;
time_t nowish = 1510592825;
 
 
void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}
 
 
void messageReceived(char *topic, byte *payload, unsigned int length)
{
  int check = 1;
 j = 0;  k= 0 ; d = 1;
 // nhan chuoi 
  for (int i = 0; i < length; i++)
  {
    a[i] = (char)payload[i];
  //  Serial.print(a[i]);
  }
  // tach chuoi
  for (int i=0 ; i<length ; i++)
  {
    if (a[i] >='a' && a[i]<= 'z')
    {
        b[j] = a[i];
  //      Serial.print(b[j]);
        j++;
    }
    if (a[i] >='0' && a[i]<= '9' && a[i-1] == '"')
    {
        k = 0;
        m = i;
        while (a[m] != '"')
        {
          k = k*10 + (a[m] - '0');
          m++;
        }
        if (i<10) x = k;
        else y=k;
    }
  }
  
  Serial.print(dem);
  Serial.print(x);
  Serial.print(y);
  if (chukiden % 4 == 0)
    {
      Serial.println("do1 xanh2");
    }
    if (chukiden % 4 == 1)
    {
      Serial.println("do1 vang2");
    }
    if (chukiden % 4 == 2)
    {
      Serial.println("xanh1 do2");
    }
    if (chukiden % 4 == 3)
    {
      Serial.println("vang1 do2");
    }
  if (chukiden % 2 == 0)
  {
    dem++;
    if (dem < 4)
      {
        delay(5000);
      }
    if (dem >= 4)
    {
      if (chukiden % 4 == 0 && y>x)
      {
        delay(5000);
      }
      else if ((chukiden-2) % 4 == 0 && x>y)
      {
        delay(5000);
      }
      else
      {
        check = 0;
      }
    } 
      if (check == 0 || dem == 10) 
      {
        dem = 0;
        chukiden++;
      }
  }
  else 
  {
    delay(5000);
    chukiden++;
  }
  
  
  // so sánh để tăng time
  // if (k>h)
  //   s = s + 5000;
  // else 
  //   s = s - 5000;
  //Serial.print(k);
  //Serial.println();
}
 
 
void connectAWS()
{
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
 
  NTPConnect();
 
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);
 
  client.setServer(MQTT_HOST, 8883);
  client.setCallback(messageReceived);
 
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(1000);
  }
 
  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
 
void publishMessage(int sensor)
{
  StaticJsonDocument<200> doc;
  doc["id"] = String(1);
  doc["time"] = String(millis());
  doc["sensor"] = String(sensor);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
 
void setup()
{
  Serial.begin(115200);
  pinMode(LED_DO1 , OUTPUT);
  pinMode(LED_VANG1 , OUTPUT);
  pinMode(LED_XANH1 , OUTPUT);
  pinMode(LED_DO2 , OUTPUT);
  pinMode(LED_XANH2 , OUTPUT);
  pinMode(LED_VANG2 , OUTPUT);
  digitalWrite(LED_DO1, LOW);
  digitalWrite(LED_VANG1, LOW);
  digitalWrite(LED_XANH1, LOW);
  digitalWrite(LED_DO2, LOW);
  digitalWrite(LED_VANG2, LOW);
  digitalWrite(LED_XANH2, LOW);
  connectAWS();
}
 
void led()
{
  
}

void loop()
{
  h = 5;
  t = 10;
  //int sensor = digitalRead(PIND0);
  if (isnan(h) || isnan(t) )  // Check if any reads failed and exit early (to try again).
  {
    Serial.println(F("Failed to read from sensor!"));
    return;
  }
 
  //Serial.print(F("sensor : "));
  //Serial.println(sensor);
  delay(50);
 
  now = time(nullptr);
 
  if (!client.connected())
  {
    connectAWS();
  }
  else
  {
    if (chukiden % 4 == 0)
    {
      digitalWrite(LED_VANG1, LOW);
      digitalWrite(LED_DO2, LOW);
      digitalWrite(LED_DO1, HIGH);
      digitalWrite(LED_XANH2, HIGH);
    }
    if (chukiden % 4 == 1)
    {
      digitalWrite(LED_XANH2, LOW);
      digitalWrite(LED_VANG2, HIGH);
    }
    if (chukiden % 4 == 2)
    {
      digitalWrite(LED_VANG2, LOW);
      digitalWrite(LED_DO1, LOW);
      digitalWrite(LED_DO2, HIGH);
      digitalWrite(LED_XANH1, HIGH);
    }
    if (chukiden % 4 == 3)
    {
      digitalWrite(LED_VANG1, HIGH);
      digitalWrite(LED_XANH1, LOW);
    }
    client.loop();
  }
  
}

