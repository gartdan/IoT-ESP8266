#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

const char* ssid     = "<YOURSSID>";
const char* password = "<YOURPASSWORD>";

String KEY = "<YOUTTHINGSPEAKKEY>";
const char* THINGSPEAK_HOST = "api.thingspeak.com";
const char* GATEWAY_HOST= "esp8266gateway.local"; //address of the gateway
const String DEVICE_ID = "esp8266_huzzah_001";


DHT dht(DHTPIN, DHTTYPE, 11);

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



int value = 0;

void loop() {
  delay(10000);
  ++value;
  
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);
  float hi = dht.computeHeatIndex(f, h);
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.println(" *F");
  
  int light = analogRead(A0);
  Serial.println("Light: "+ String(light));

  Serial.print("sending the data ");
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  // We now create a URI for the request
  String thingspeakUrl = "/update?key="+ KEY + "&field1="+String((int)f) + "&field2="+String((int)t) + "&field3="+String((int)h) + "&field4="+String(light);
  SendHttpGet(client, thingspeakUrl, THINGSPEAK_HOST);
  
  String azureUrl = "/sensorData?temp="+String((int)f) + "&humidity=" + String((int)h) +"&light="+ String(light) +"&deviceId=" + DEVICE_ID;
  SendHttpGet(client, azureUrl, GATEWAY_HOST);


}


void SendHttpGet(WiFiClient client, String url, const char* host)
{
  
  Serial.println("attempting connection to host: "+ String(host));
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

