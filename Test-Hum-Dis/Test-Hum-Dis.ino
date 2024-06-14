#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <DHT.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

int trigPin = 14; // TRIG pin for ultrasonic sensor
int echoPin = 12; // ECHO pin for ultrasonic sensor
#define DHTPIN 2 // DHT11 data pin
#define DHTTYPE DHT11

// Replace with your network credentials
const char* ssid     = "AIMANBT2";
const char* password = "mariaahmad@123";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://esp8266-midterm.000webhostapp.com/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "fdc7d98d-ef4c-433f-bcc4-3f86ae34a96f";

String sensorName = "ESP8266";
String sensorLocation = "Home";

DHT dht(DHTPIN, DHTTYPE);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() 
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
    dht.begin();

    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) 
    { 
      
      delay(500);
      Serial.print(".");
    }
    
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    //LED DETECT
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
    { 
        Serial.println("SSD1306 allocation failed");
        for(;;);
    }
    
    display.clearDisplay();
    display.display();

    // Set custom font
    display.setFont(&FreeSerif9pt7b);

    // Set custom text color
    display.setTextColor(WHITE); 

    delay(2000);
}

void loop() 
{
    // Ultrasonic sensor
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    float duration_us = pulseIn(echoPin, HIGH);
    float distance_cm = 0.017 * duration_us;

    // DHT11 sensor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    String combinedText = "Distance: " + String(distance_cm, 2) + " cm | Humidity: " + String(humidity, 2) + "% | Temperature: " + String(temperature, 2) + "°C";
    int textLength;
    int xPos;

    // Calculate the width of the text in pixels
    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    display.getTextBounds(combinedText, 0, 0, &x1, &y1, &textWidth, &textHeight);

    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED)
    {
        WiFiClientSecure *client = new WiFiClientSecure;
        client->setInsecure(); //don't use SSL certificate
        HTTPClient https;
        https.setTimeout(4500);
        
        // Your Domain name with URL path or IP address with path
        https.begin(*client, serverName);
        
        // Specify content-type header
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");
        
        // Prepare your HTTP POST request data
        String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
                              + "&location=" + sensorLocation + "&humvalue=" + String(humidity, 2)
                              + "&temvalue=" + String(temperature, 2) + "&disvalue=" + String(distance_cm, 2) + "";
        Serial.print("httpRequestData: ");
        Serial.println(httpRequestData);

        
        // You can comment the httpRequestData variable above
        // then, use the httpRequestData variable below (for testing purposes without the sensor)
        //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=ESP8266&location=HOME&humvalue=60.00&temvalue=33.00&disvalue=100.00";

        // Send HTTP POST request
        int httpResponseCode = https.POST(httpRequestData);

        if (httpResponseCode>0) 
        {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          Serial.println("Successfully data input.");
        }
        else 
        
        {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode); 
        }
        // Free resources
        https.end();
    }

    else 
    {
        Serial.println("WiFi Disconnected");
    }

    // Check for errors
    if (isnan(humidity) || isnan(temperature) || isnan(distance_cm)) 
    {
        display.setFont(&FreeSerif9pt7b);
        display.clearDisplay();
        display.setTextSize(1);             
        display.setTextColor(WHITE);        
        display.setCursor(0,20);             
        display.println("Sensor error!");
        display.display();
        delay(2000);  
          
        Serial.println("Sensor error!");
    } 
      
    else 
    {
        // Display on LCD
        textLength = display.width() * combinedText.length() / SCREEN_WIDTH;
        xPos = SCREEN_WIDTH;

        display.clearDisplay();
          
        // Draw the text at the current position
        display.setCursor(xPos, 0); // Adjust y position as needed for your display
        // Move text position to the left
        xPos--;

        // If the text has completely moved off to the left, reset its position
        if (xPos < -textLength) 
        {
            xPos = SCREEN_WIDTH;
        }

        // Set custom font
        display.setFont(&FreeSerif9pt7b);

        // Set custom text color
        display.setTextColor(WHITE);
        display.print(combinedText);

        // Update the display
        display.display();

        // Delay for smooth scrolling
        delay(50);

        // Print to serial monitor
        Serial.print("Distance: ");
        Serial.print(distance_cm);
        Serial.print(" cm | Humidity: ");
        Serial.print(humidity);
        Serial.print("% | Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
    }

    delay(5000);
}

