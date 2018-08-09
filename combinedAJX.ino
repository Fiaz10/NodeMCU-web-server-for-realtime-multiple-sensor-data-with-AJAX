/* Receiver code for NodeMCU
 * Example of sending data from an Ardunio UNO to NodeMCU through UART
 * Using this code you can send multiple data packets 
 * for instance from different sensors to a NodeMCU from an Arduino UNO
 * The data received in displayed in the serial monitor
 * Additionaly this code creates an WiFi access point in the NodeMcu 
 * Which shows the data in a webpage accessible by pc or mobile phone
 * the data also updates in real time with the help of AJAX, without the need for the page to be refreshed
 * it also allows u to control a LED connected to pin D1 of NodeMCU
 * This code was written for the Telemedicine project of Dhaka University BMPT Department
 * Author: MD Fiaz Islam Bhuiyan, The University of Texas at Dallas
 * Date: 16th July 2018
 * 
   -----------------------------------------------------------------------*/


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include<SoftwareSerial.h>


SoftwareSerial com(D1,D2); // RX, TX

enum {getLdrSensor, getRandomData1, getRandomData2, getRandomData3, getRandomData4, getRandomData5, getRandomData6, getRandomData7 };

int whichNumber = getLdrSensor;

int r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, brightness = 0;

String toPrint = "No Value";

const char endOfNumberDelimiter   = '|';




IPAddress apIP(42, 42, 42, 42);

// Replace with your network credentials
const char* ssid = "ESP-AP";
const char* password = "Fall2017?";
ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "";
String text = "";
//double data;





void setup(void){
 //pinMode(A0, INPUT);
 delay(1000);
 Serial.begin(115200);
 com.begin(115200);

 //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("HTTP server started");

 Serial.println("");

 // Wait for connection
 

 Serial.println("");
 Serial.print("Connected to ");
 Serial.println(ssid);
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
 server.on("/data.txt", [](){
   text = "Data 1: " + (String)brightness + "<br \>Data 2: " +(String)r1 + "<br \>Data 3: " +(String)r2 + "<br \>Data 4: " +(String)r3 + "<br \>Data 5: " +(String)r4 + "<br \>Data 6: " +(String)r5 + "<br \>Data 7: " +(String)r6 + "<br \>Data 8: " +(String)r7;
   server.send(200, "text/html", text);
 });
 server.on("/", [](){
   page = "<h1>DU BMPT Telemedicine Server</h1><h1>Real Time Diagnostic Data:</h1> <h1 id=\"data\">""</h1>\r\n";
   page += "<script>\r\n";
   page += "var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 100);\r\n";
   page += "function loadData(url, callback){\r\n";
   page += "var xhttp = new XMLHttpRequest();\r\n";
   page += "xhttp.onreadystatechange = function(){\r\n";
   page += " if(this.readyState == 4 && this.status == 200){\r\n";
   page += " callback.apply(xhttp);\r\n";
   page += " }\r\n";
   page += "};\r\n";
   page += "xhttp.open(\"GET\", url, true);\r\n";
   page += "xhttp.send();\r\n";
   page += "}\r\n";
   page += "function updateData(){\r\n";
   page += " document.getElementById(\"data\").innerHTML = this.responseText;\r\n";
   page += "}\r\n";
   page += "</script>\r\n";
   server.send(200, "text/html", page);
  });

 server.begin();
 Serial.println("Web server started!");
}





// UART DATA receiving code
void processNumber (const long n)
 {
  
  switch (whichNumber)
    {
    case getLdrSensor: 
      Serial.print ("LDR Value = ");
      toPrint = "LDR Value = ";
      brightness = n;
      break;
      
    case getRandomData1: 
      Serial.print ("Random Data1 = ");
      toPrint = "Random Data1 = ";
      r1 = n;
      break;

    case getRandomData2: 
      Serial.print ("Random Data2 = ");
      toPrint = "Random Data2 = ";
      r2 = n;
      break;

    case getRandomData3: 
      Serial.print ("Random Data3 = ");
      toPrint = "Random Data3 = ";
      r3 = n;
      break;

    case getRandomData4: 
      Serial.print ("Random Data4 = ");
      toPrint = "Random Data4 = ";
      r4 = n;
      break;

    case getRandomData5: 
      Serial.print ("Random Data5 = ");
      toPrint = "Random Data5 = ";
      r5 = n;
      break;

    case getRandomData6: 
      Serial.print ("Random Data6 = ");
      toPrint = "Random Data6 = ";
      r6 = n;
      break;

    case getRandomData7: 
      Serial.print ("Random Data7 = ");
      toPrint = "Random Data7 = ";
      r7 = n;
      break;
    }

  Serial.println (n);
 }  // end of processNumber



void processInput ()
 {
 static long receivedNumber = 0;
 static boolean negative = false;
 
 byte c = com.read ();
 
 switch (c)
   {
     
   case endOfNumberDelimiter:  
     if (negative) 
       processNumber (- receivedNumber); 
     else
       processNumber (receivedNumber); 

   // fall through to start a new number
   case 'A': 
     receivedNumber = 0; 
     negative = false;
     whichNumber = getLdrSensor;
     break;
   case 'B': 
     receivedNumber = 0; 
     negative = false;
     whichNumber = getRandomData1;
     break;
   case 'C': 
     receivedNumber = 0; 
     negative = false;
     whichNumber = getRandomData2;
     break;
   case 'D': 
     receivedNumber = 0; 
     negative = false;
     whichNumber = getRandomData3;
     break;
   case 'E': 
     receivedNumber = 0; 
     negative = false;
     whichNumber = getRandomData4;
     break;
   case 'F': 
     receivedNumber = 0; 
     negative = false;
     whichNumber = getRandomData5;
     break;
   case 'G': 
     receivedNumber = 0; 
     negative = false;
     whichNumber = getRandomData6;
     break;
   case 'H': 
     receivedNumber = 0; 
     negative = false;
     whichNumber = getRandomData7;
     break;
     
   case '0' ... '9': 
     receivedNumber *= 10;
     receivedNumber += c - '0';
     break;
     
   case '-':
     negative = true;
     break;
     
   } // end of switch  
 }  // end of processInput



void loop(void){
  if (com.available())
  {
    processInput();
  }
 server.handleClient();
}

 
