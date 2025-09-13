#include "config.h"
#include <NTPClient.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Define the reset pin for the OLED display
#define OLED_RESET U8X8_PIN_NONE // Reset pin

// Define the I2C pins for the OLED display
#define OLED_SDA 5
#define OLED_SCL 6

// Define the blue LED pin
#define BLUE_LED 8

// NTP settings
const long utcOffsetInSeconds = 7200; // Adjust for your timezone (e.g., +3600 for CET)
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, 60000); // Update every 60s

// Initialize the U8g2 library for the SSD1306 OLED display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RESET, OLED_SCL, OLED_SDA);

// Define the dimensions and offset for the display content
int width = 72;
int height = 40;
int xOffset = 30; // = (132-w)/2
int yOffset = 12; // = (64-h)/2

void setup()
{
    // Set the blue LED pin as an output
    pinMode(BLUE_LED, OUTPUT);
    Serial.begin(115200);
    // Initialize OLED
    u8g2.begin();
    u8g2.setContrast(255);
    // Set the I2C bus clock speed
    u8g2.setBusClock(400000);  // 400kHz I2C
    u8g2.clearBuffer();              // Clear internal memory
    u8g2.setFont(u8g2_font_8x13_mn); // Good readable font for digits (12px height)
    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
    // Start NTP client
    timeClient.begin();
}

void loop()
{
    // Update NTP time every second (request every 60s, but update display every 1s)
    timeClient.update();
    // Get formatted time string: HH:MM:SS
    String timeStr = timeClient.getFormattedTime();
    // Ensure it's always 8 characters: "HH:MM:SS"
    if (timeStr.length() == 8)
    {
        // Clear buffer and draw
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_8x13_mn);
        u8g2.drawStr(xOffset - 0, yOffset + 30, timeStr.c_str());
        // Send the buffer to the display
        u8g2.sendBuffer(); // Send buffer to display
    }
    // Toggle the blue LED
    digitalWrite(BLUE_LED, !digitalRead(BLUE_LED));
    // Wait 1 second before next update
    delay(1000);
}