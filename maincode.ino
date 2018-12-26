/*
 Web based IOT Home Automation Project
 Using ESP8266 with Arduino IDE
 The circuit:
  ESP8266   ->   Relay Driver Board-4Channel
    D1      ->      IN1
    D2      ->      IN2
    D3      ->      IN3
    D4      ->      IN4
   VIN      ->      VCC
   GND      ->      GND
   
                                               by Laukik T.
*/


#include<ESP8266WiFi.h>

// Pins definition
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define SD3 10
#define SD2 9

// Relay Driver Board specifications
const int ACTIVE = LOW;     // Board is Active LOW. Change to HIGH if your board is active HIGH
const int INACTIVE = !ACTIVE;


const int out1 = D1;
const int out2 = D2;
const int out3 = D5;
const int out4 = D6;

int outstate[4] = {INACTIVE,INACTIVE,INACTIVE,INACTIVE};  //Disable all pins

const char* ssid = "wlan_ssid_here";      //Your WiFi SSID here
const char* pass = "wlan_password_here";  //Your WiFi Password here

WiFiServer server(80);                    //Initialize server at port 80

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  
  // Wait till WiFi gets connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);                                //Wait 
  }
  
  server.begin();  // start server
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Get your IP Address from Serial Port. Put this IP into your browser to access it.
  
  
  // Pins direction Assignment
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);
  
  // Pins state assignment
  digitalWrite(out1, INACTIVE);
  digitalWrite(out2, INACTIVE);
  digitalWrite(out3, INACTIVE);
  digitalWrite(out4, INACTIVE);
}

void loop()
{
  String header = "", currLine = "";
  WiFiClient client = server.available(); // check whether client connected
  if (client)
  {
    while (client.connected()) // not to waste packets over wifi
    {
      if (client.available())
      {
        char c = client.read();
        header += c;
        if (c == '\n')
        {
          if (currLine.length() == 0)
          {
            // http header response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            // gpio state assignment
            if (header.indexOf("GET /out1/INACTIVE") >= 0)
            {
              outstate[0] = INACTIVE;
            } else if (header.indexOf("GET /out1/ACTIVE") >= 0)
            {
              outstate[0] = ACTIVE;
            } else if (header.indexOf("GET /out2/INACTIVE") >= 0)
            {
              outstate[1] = INACTIVE;
            } else if (header.indexOf("GET /out2/ACTIVE") >= 0)
            {
              outstate[1] = ACTIVE;
            } else if (header.indexOf("GET /out3/INACTIVE") >= 0)
            {
              outstate[2] = INACTIVE;
            } else if (header.indexOf("GET /out3/ACTIVE") >= 0)
            {
              outstate[2] = ACTIVE;
            } else if (header.indexOf("GET /out4/INACTIVE") >= 0)
            {
              outstate[3] = INACTIVE;
            } else if (header.indexOf("GET /out4/ACTIVE") >= 0)
            {
              outstate[3] = ACTIVE;
            }



            //html webpage styling
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button1 { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            
            // Button manager
            client.println("<p>Home Automation Server</p>");
            String temp = outstate[0]==ACTIVE?"ON":"OFF";
            client.println("<p>Light1 is"+temp+"\n</p>");
            outstate[0]==INACTIVE?client.println("<p><a href=\"/out1/ACTIVE\"><button class=\"button\">OFF</button></a></p>"):client.println("<p><a href=\"/out1/INACTIVE\"><button class=\"button button2\">ON</button></a></p>");
            
            temp = outstate[1]==ACTIVE?"ON":"OFF";
            client.println("<p>Light2 is"+temp+"\n</p>");
            outstate[1]==INACTIVE?client.println("<p><a href=\"/out2/ACTIVE\"><button class=\"button\">OFF</button></a></p>"):client.println("<p><a href=\"/out2/INACTIVE\"><button class=\"button button2\">ON</button></a></p>");
            
            temp = outstate[2]==ACTIVE?"ON":"OFF";
            client.println("<p>Light3 is"+temp+"\n</p>");
            outstate[2]==INACTIVE?client.println("<p><a href=\"/out3/ACTIVE\"><button class=\"button\">OFF</button></a></p>"):client.println("<p><a href=\"/out3/INACTIVE\"><button class=\"button button2\">ON</button></a></p>");
            
            temp = outstate[3]==ACTIVE?"ON":"OFF";
            client.println("<p>Light4 is"+temp+"\n</p>");
            outstate[3]==INACTIVE?client.println("<p><a href=\"/out4/ACTIVE\"><button class=\"button\">OFF</button></a></p>"):client.println("<p><a href=\"/out4/INACTIVE\"><button class=\"button button2\">ON</button></a></p>");
            
            client.println("</body></html>");
            client.println();




// send data to pins
            digitalWrite(out1,outstate[0]);
            digitalWrite(out2,outstate[1]);
            digitalWrite(out3,outstate[2]);
            digitalWrite(out4,outstate[3]);

            
            break;
          } else
          {
            currLine = "";
          }
        } else if(c!='\r')
        {
          currLine += c;
        }
      }
    }

    client.stop(); // client disconnect
  }

}
