#include "config.h"
#include <NTPClient.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <time.h>

// Define the reset pin for the OLED display
#define OLED_RESET U8X8_PIN_NONE // Reset pin

// Define the I2C pins for the OLED display
#define OLED_SDA 5
#define OLED_SCL 6

// Define the blue LED pin
#define BLUE_LED 8

// NTP settings
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // Start with UTC, update every 60s

// DST tracking
int currentDSTOffset = 0; // Track current DST offset

// DST helper functions
int getLastSundayOfMonth(int year, int month);
bool isDST(int year, int month, int day, int hour);
void updateDST();

// Initialize the U8g2 library for the SSD1306 OLED display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RESET, OLED_SCL, OLED_SDA);

// Define the dimensions and offset for the display content
int width = 72;
int height = 40;
int xOffset = 30; // = (132 - width) / 2
int yOffset = 12; // = (64 - height) / 2

//
// Function to get the last Sunday of a given month and year for DST transitions
// Uses hardcoded values for Europe/Amsterdam timezone DST transition dates
//
int getLastSundayOfMonth(int year, int month)
{
    // DST transition dates for Europe/Amsterdam (last Sunday of March and October)
    if (month == 3) { // March
        switch (year) {
            case 2025: return 30;
            case 2026: return 29;
            case 2027: return 28;
            case 2028: return 26;
            case 2029: return 25;
            case 2030: return 31;
            default: return 31; // fallback
        }
    } else if (month == 10) { // October
        switch (year) {
            case 2025: return 26;
            case 2026: return 25;
            case 2027: return 31;
            case 2028: return 29;
            case 2029: return 28;
            case 2030: return 27;
            default: return 27; // fallback
        }
    }

    return 31; // fallback for other months
}

//
// Function to determine if the current date/time is within the DST period
//
bool isDST(int year, int month, int day, int hour)
{
    // Months from April to September are always DST
    if (month > 3 && month < 10)
    {
        return true;
    }

    // Months of January, February, November, December are never DST
    if (month < 3 || month > 10)
    {
        return false;
    }

    // March - check if we're past the last Sunday
    if (month == 3)
    {
        int lastSunday = getLastSundayOfMonth(year, 3);
        if (day < lastSunday)
        {
            return false; // Before last Sunday of March
        }
        else if (day > lastSunday)
        {
            return true; // After last Sunday of March
        }
        else
        {
            // On the last Sunday of March
            // DST starts at 02:00 local time (which is 01:00 UTC)
            return hour >= 2;
        }
    }

    // October - check if we're before the last Sunday
    if (month == 10)
    {
        int lastSunday = getLastSundayOfMonth(year, 10);
        if (day < lastSunday)
        {
            return true; // Before last Sunday of October
        }
        else if (day > lastSunday)
        {
            return false; // After last Sunday of October
        }
        else
        {
            // On the last Sunday of October
            // DST ends at 03:00 local time (which is 01:00 UTC)
            return hour < 3;
        }
    }

    // Should never reach here
    return false;
}

//
// Function to check and update the DST status
//
void updateDST()
{
    // Get current time
    time_t now = timeClient.getEpochTime();

    // Convert to local time structure
    struct tm *timeinfo = localtime(&now);

    // Extract components
    int year = timeinfo->tm_year + 1900; // tm_year is years since 1900
    int month = timeinfo->tm_mon + 1;    // tm_mon is 0-11
    int day = timeinfo->tm_mday;
    int hour = timeinfo->tm_hour;

    // Determine the correct offset based on DST rules
    int correctOffset;
    if (isDST(year, month, day, hour))
    {
        correctOffset = 7200; // CEST (UTC+2)
    }
    else
    {
        correctOffset = 3600; // CET (UTC+1)
    }

    // Check if the current offset matches the correct offset
    if (currentDSTOffset != correctOffset)
    {
        // Print debug information
        Serial.print("DST transition detected. Changing offset from ");
        Serial.print(currentDSTOffset);
        Serial.print(" to ");
        Serial.println(correctOffset);

        // Update the time offset
        timeClient.setTimeOffset(correctOffset);
        currentDSTOffset = correctOffset;
    }
}

//
// Initialize the system
//
void setup()
{
    // Set the blue LED pin as an output
    pinMode(BLUE_LED, OUTPUT);
    Serial.begin(115200);
    // Initialize OLED
    u8g2.begin();
    u8g2.setContrast(255);
    // Set the I2C bus clock speed
    u8g2.setBusClock(400000);        // 400kHz I2C
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
    // Set initial DST offset
    updateDST();
}

//
// Loop forever
//
void loop()
{
    // Update NTP time every second (request every 60s, but update display every 1s)
    timeClient.update();

    // Check DST status periodically (e.g., every 10 minutes)
    static unsigned long lastDSTCheck = 0;

    if (millis() - lastDSTCheck > 60000)
    { // 10 minutes
        updateDST();
        lastDSTCheck = millis();
    }

    // Update display every second using millis()
    static unsigned long lastDisplayUpdate = 0;
    if (millis() - lastDisplayUpdate >= 1000)
    { // 1 second
        // Get formatted time string: HH:MM:SS
        String timeStr = timeClient.getFormattedTime();
        // Ensure it's always 8 characters: "HH:MM:SS"
        if (timeStr.length() == 8)
        {
            // Clear buffer and draw
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_8x13_mn);
            u8g2.drawStr(xOffset + 3, yOffset + 35, timeStr.c_str());
            // Send the buffer to the display
            u8g2.sendBuffer(); // Send buffer to display
        }

        // Toggle the blue LED
        digitalWrite(BLUE_LED, !digitalRead(BLUE_LED));
        lastDisplayUpdate = millis();
    }
}
