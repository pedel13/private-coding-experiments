#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <AsyncTCP.h>
// Global Variables

// Hotspot password
const char *ssid = "Licht";
const char *password = "123456789";

boolean parola = false;

int rly1 = 5; // confirmed

String rly1State = "off";

// Server
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";

void setup()
{
  Serial.begin(115200);

  pinMode(rly1, OUTPUT);

  digitalWrite(rly1, LOW);

  IPAddress local_ip(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client)
  {                                // If a new client connects,
    Serial.println("New Client."); // print a message out in the serial port
    String currentLine = "";       // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off

            // Pumps
            if (header.indexOf("GET /" + String(rly1)) >= 0)
            {
              digitalWrite(rly1, !digitalRead(rly1));
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            ;

            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<head><script>function passwordControl(){let password = document.getElementById(\"passwort\").value;window.location.href=\"/\" + password;}</script>");
            client.println("<style> body {display: flex; align-items: center; justify-content: space-around; flex-direction: column; margin: 0px; background-color: rgba(0,0,255,0.4);}");
            client.println("h1 {font-size: 200px; color: aliceblue; padding-bottom: 26vw;}");
            client.println(".passworte{height: 5vh; width: 30vw;font-size: 80%;border: none;margin:0px;background-color:rgba(0,0,255,0);border-color:none;display:start}");
            client.println(".off{background-color: rgba(0,255,0,0.6);}");
            client.println(".on {background-color: rgba(255,0,0,0.6);}");
            client.println("button {font-size: 160px; color: white; height: 70vw; width: 70vw; border-radius: 50%;}</style></head>");
            client.println("<body> <br><input type=\"password\" class=\"passworte\" id=\"passwort\" maxlength=\"40\" required>");
            client.println("<button class=\"passworte\" onclick=\"passwordControl()\"></button>");
            client.println("<h1>LICHT</h1>");
            if (digitalRead(rly1))
            {
              client.println("<p><a href=\"/" + String(rly1) + "/on\"><button class=\"off\">AN</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/" + String(rly1) + "/off\"><button class=\"on\">AUS</button></a></p>");
            }
            if (header.indexOf("GET /fuck") >= 0)
            {
              parola = true;
            }
            if (parola == true)
            {
              client.println("<script>function passwordControl(){let password = document.getElementById(\"passwort\").value;window.location.href=\"/\" + password;}</script>");
              client.println("<style>.passworte{height: 5vh; width: 30vw;font-size: 80%;border: none;margin:0px;background-color:rgba(0,0,255,1);border-color:none;}</style>");
              client.println("<body> <br><input type=\"password\" class=\"passworte\" id=\"passwort\" maxlength=\"40\" required>");
              client.println("<h1>LICHT</h1>");
              client.println("<button class=\"passworte\" onclick=\"passwordControl()\"></button>");
              client.println("<p><a href=\"/slow\"><button class=\"off\">FlASH SLOW</button></a></p>");
              client.println("<p><a href=\"/fast\"><button class=\"off\">FLASH FAST</button></a></p>");
              if (header.indexOf("GET /slow") >= 0)
              {
                for (int i = 0; i < 100; i++)
                {
                  digitalWrite(rly1, !digitalRead(rly1));
                  delay(500);
                  if (header.indexOf("GET /stop") >= 0)
                  {
                    i = 100;
                  }
                }
              }

              if (header.indexOf("GET /fast") >= 0)
              {
                for (int i = 0; i < 100; i++)
                {
                  digitalWrite(rly1, !digitalRead(rly1));
                  delay(100);
                }
              }

              if (header.indexOf("GET /morse") >= 0)
              {
                char morsecod[] = {};
                String morse = header.substring(10, 50);
                morse.toLowerCase();
                morse.toCharArray(morsecod, 40);
                int j = 40;
                for (int i = 0; i < j; i++)
                {
                  switch (morsecod[i])
                  {
                  case 'a':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'b':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'c':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'd':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    break;
                  case 'e':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'f':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'g':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'h':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'i':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'j':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'k':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'l':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'm':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'n':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'o':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'p':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'q':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'r':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 's':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 't':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'u':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'v':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'w':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'x':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'y':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case 'z':
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(500);
                    digitalWrite(rly1, !digitalRead(rly1));
                    delay(1000);
                    break;
                  case ' ':
                    j = 0;
                    break;
                  default:
                    break;
                  }
                  delay(2500);
                }
              }
              if (header.indexOf("GET /end") >= 0)
              {
                parola = false;
              }
            }
            client.println("</body></html>");
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
  }
}