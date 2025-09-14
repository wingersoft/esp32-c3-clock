# ESP32-C3 OLED NTP Clock Architecture
## System Architecture
The system consists of these main components:

1. **WiFi Manager** - Handles connection to the WiFi network using credentials stored in config.h
2. **NTP Client** - Synchronizes time with NTP servers to maintain accurate timekeeping
3. **DST Manager** - Automatically handles Daylight Saving Time transitions for Europe/Amsterdam timezone
4. **OLED Display Driver** - Controls the SSD1306 OLED display to show the current time
5. **LED Controller** - Manages the blue LED status indicator
6. **Main Loop Controller** - Coordinates the timing of updates and display refreshes
## Source Code Structure
- `src/main.cpp` - Main application code containing setup and loop functions
- `src/config.h` - Configuration file storing WiFi credentials

## Key Technical Decisions
- Using U8g2 library for OLED display control due to its comprehensive support for SSD1306
- Using NTPClient library for simplified NTP synchronization
- Hardware I2C implementation for better performance with OLED display
- Direct pin control for LED status indicator
- Hardcoded DST transition dates for enhanced accuracy (2025-2030)
- Dynamic timezone offset management for automatic DST transitions
- Non-blocking loop implementation using millis() for better responsiveness

## Component Relationships
1. Setup initializes WiFi, OLED display, and NTP client
2. Main loop continuously updates time from NTP client
3. DST Manager checks and adjusts timezone offset based on current date
4. Time is formatted and sent to OLED display
5. LED toggles every second to indicate operation

## Critical Implementation Paths
- WiFi connection with blocking retry mechanism
- NTP time synchronization with 60-second update interval
- DST status checking every 10 minutes with automatic offset adjustment
- OLED display refresh every second with formatted time string