/* 
 * ESP8266 NodeMCU DHT11 - Humidity Temperature Data logging Example
 * https://circuits4you.com
 * 
 * Referances
 * https://circuits4you.com/2017/12/31/nodemcu-pinout/
 * 
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "index.h" //Our HTML webpage contents with javascripts

#define LED 2 //On board LED

//SSID and Password of your WiFi router
const char *ssid = "ATT7RT97i2";
const char *password = "f3y436pwzu6y";

ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void sendHTML()
{
  String s = MAIN_page;             //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}
void handleData()
{
  int analog_data = analogRead(A0);
  int random_number1 = random(1, 20);
  int random_number2 = random(20, 50);
  //Ref 1: https://circuits4you.com/2019/01/11/nodemcu-esp8266-arduino-json-parsing-example/
  //Ref 2: https://circuits4you.com/2019/01/25/arduino-how-to-put-quotation-marks-in-analog_data-string/
  String data = "{\"ADC\":\"" + String(analog_data) + "\", \"value1\":\"" +
                String(random_number1) + "\", \"value2\":\"" + String(random_number2) + "\"}";

  digitalWrite(LED, !digitalRead(LED)); //Toggle LED on data request ajax
  server.send(200, "text/plane", data); //Send ADC value, temperature and humidity JSON to client ajax request

  //Get Humidity temperatue data after request is complete
  //Give enough time to handle client to avoid problems
  delay(2000);

  Serial.print(random_number1);
  Serial.print(random_number2);
}

//==============================================================
//                  SETUP
//==============================================================
void setup()
{
  Serial.begin(115200);
  Serial.println();

  //Ref 3: https://circuits4you.com/2019/01/25/interfacing-dht11-with-nodemcu-example/
  // Autodetect is not working reliable, don't use the following line
  // dht.setup(17);

  // use this instead:
  //dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17
  //dht.setup(DHTpin, DHTesp::DHT22); //for DHT22 Connect DHT sensor to GPIO 17

  WiFi.begin(ssid, password); //Connect to your WiFi router
  Serial.println("");

  //Onboard LED port Direction output
  pinMode(LED, OUTPUT);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //IP address assigned to your ESP

  server.on("/", sendHTML);             //Which routine to handle at root location. This is display page
  server.on("/handleData", handleData); //This page is called by java Script AJAX

  server.begin(); //Start server
  Serial.println("HTTP server started");
}

//==============================================================
//                     LOOP
//==============================================================
void loop()
{
  server.handleClient(); //Handle client requests
}