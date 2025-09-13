# ESP32-C3 OLED NTP Clock

A digital clock project built on the ESP32-C3 microcontroller that displays accurate time on an OLED screen through WiFi connectivity and NTP synchronization.

## Features

- **WiFi Connectivity**: Connects to your WiFi network for internet access
- **NTP Time Synchronization**: Retrieves accurate time from NTP servers
- **OLED Display**: Shows time in HH:MM:SS format on a 128x64 SSD1306 OLED display
- **LED Status Indicator**: Blue LED blinks every second to indicate operation
- **USB-CDC Serial**: Debug output via USB serial connection
- **PlatformIO Development**: Built using PlatformIO IDE with Arduino framework

## Hardware Requirements

- ESP32-C3 microcontroller
- SSD1306 128x64 OLED display (I2C interface)
- Blue LED (connected to pin 8)
- USB-C cable for programming and power

### Pin Connections

- OLED SDA: GPIO 5
- OLED SCL: GPIO 6
- LED: GPIO 8

## Software Requirements

- PlatformIO IDE
- ESP-IDF toolchain
- Arduino framework for ESP32

### Dependencies

- U8g2 (OLED display library)
- NTPClient (NTP synchronization)
- WiFi (ESP32 WiFi library)
- WiFiUdp (UDP communication)

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/wingersoft/esp32-c3-clock.git
   cd esp32-c3-clock
   ```

2. Open the project in PlatformIO IDE

3. Configure your WiFi credentials in `src/config.h`:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

4. Build and upload to your ESP32-C3:
   ```bash
   pio run --target upload
   ```

## Usage

1. Power on the ESP32-C3
2. The device will connect to WiFi and synchronize time with NTP servers
3. Current time will be displayed on the OLED screen
4. The blue LED will blink every second to indicate operation

## Configuration

### WiFi Settings

Edit `src/config.h` to configure your WiFi network:

```cpp
const char* ssid = "YourNetworkName";
const char* password = "YourNetworkPassword";
```

### NTP Settings

The NTP client is configured in `src/main.cpp` with the following defaults:
- NTP Server: `pool.ntp.org`
- Timezone Offset: UTC (adjust as needed)
- Update Interval: 60 seconds

## Project Structure

```
esp32-c3-clock/
├── .gitignore
├── platformio.ini          # PlatformIO configuration
├── src/
│   ├── main.cpp           # Main application code
│   └── config.h           # Configuration file
└── .kilocode/             # Development documentation
    └── rules/
        └── memory-bank/   # Project documentation
```

## Development

### Building

```bash
pio run
```

### Uploading

```bash
pio run --target upload
```

### Serial Monitor

```bash
pio device monitor
```

## Troubleshooting

- **WiFi Connection Issues**: Check your SSID and password in `config.h`
- **OLED Display Not Working**: Verify I2C connections (SDA: GPIO 5, SCL: GPIO 6)
- **Time Not Syncing**: Ensure internet connectivity and NTP server availability
- **LED Not Blinking**: Check LED connection to GPIO 8

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is open source. Feel free to use and modify as needed.

## Future Enhancements

- Timezone support
- Display brightness control
- Battery-powered operation
- Multiple display formats
- Web-based configuration interface