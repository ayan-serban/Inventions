# ⏰ AYAN TimeMaster Pro

**ESP32-based circular clock with analog and digital display that never forgets the time!**

![Clock Display](https://img.shields.io/badge/Display-240x240%20Circular-blue)
![Platform](https://img.shields.io/badge/Platform-ESP32--S3-green)
![License](https://img.shields.io/badge/License-MIT-yellow)

## ✨ Features

- 🎯 **Analog Clock Face** - Beautiful moving hour, minute, and second hands
- 🔢 **Digital Time Display** - Large, centered time in HH:MM:SS format
- 📅 **Date & Day of Week** - Always know what day it is
- 🔋 **Battery Backup** - DS1302 RTC keeps time even when powered off
- 🔄 **Auto-Sync** - Automatically syncs to your laptop time on upload
- 💚 **Custom Name Display** - Shows "AYAN" at the bottom
- 🌈 **Colorful Interface** - Multi-color display (cyan, green, red, yellow, magenta)

## 🛠️ Hardware Required

| Component | Specification |
|-----------|--------------|
| Microcontroller | ESP32-S3 |
| Display | GC9A01A 1.28" Circular LCD (240x240, SPI) |
| RTC Module | DS1302 (with CR2032 battery) |
| Cables | Jumper wires |
| Power | USB-C cable |

**Total Cost: ~$15-20**

## 📌 Pin Connections

### GC9A01A Circular Display (SPI)
```
Display Pin  →  ESP32-S3 Pin
VCC          →  3.3V
GND          →  GND
SCL          →  GPIO 12
SDA          →  GPIO 11
RES/RST      →  GPIO 8
DC/RS        →  GPIO 9
CS           →  GPIO 10
```

### DS1302 RTC Module
```
RTC Pin  →  ESP32-S3 Pin
VCC      →  5V (or 3.3V, check your module)
GND      →  GND
CLK      →  GPIO 4
DAT      →  GPIO 5
RST      →  GPIO 6
```

## 📚 Libraries Required

Install these libraries in Arduino IDE (**Tools → Manage Libraries**):

1. **Adafruit GFX Library** by Adafruit
2. **Adafruit GC9A01A** by Adafruit
3. **RtcDS1302** by Makuna (search "Rtc")

## 🚀 Installation

1. **Clone or download this repository**
```bash
   git clone https://github.com/YourUsername/AYAN-TimeMaster-Pro.git
```

2. **Open Arduino IDE**
   - Install the required libraries (see above)
   - Open `centered_clock.ino`

3. **Select your board**
   - **Tools → Board → ESP32 Arduino → ESP32S3 Dev Module**
   - **Tools → Port → Select your ESP32 port**

4. **Upload the code**
   - Click the Upload button
   - The clock will automatically sync to your laptop's time!

5. **Done!**
   - Your AYAN TimeMaster Pro is ready! ⏰

## 🎨 Display Layout
```
          SUN          ← Day of week (top, magenta)
       27/10/2025      ← Date (cyan)
    
    [  Analog          ← Analog clock with hands
       Clock           • Hour hand (cyan)
       Face  ]         • Minute hand (green)
                       • Second hand (red)
    
         08:42         ← Digital time (center, yellow)
           :30         ← Seconds below
    
          AYAN         ← Custom name (bottom, green)
```

## ⚙️ How It Works

1. **Auto-Sync**: When you upload the code, it automatically grabs your laptop's current time and sets the DS1302 RTC module
2. **Battery Backup**: The CR2032 battery in the DS1302 keeps the time running even when the ESP32 is powered off
3. **Continuous Update**: The clock updates every second, reading time from the RTC module
4. **Accurate Timekeeping**: The DS1302 is more accurate than the ESP32's internal timer

## 🔧 Customization

### Change the Name
Edit line ~220 in `centered_clock.ino`:
```cpp
tft.print("AYAN");  // Change to your name!
```

### Change Colors
Modify these color constants:
- `GC9A01A_CYAN` - Hour hand
- `GC9A01A_GREEN` - Minute hand & name
- `GC9A01A_RED` - Second hand
- `GC9A01A_YELLOW` - Digital time
- `GC9A01A_MAGENTA` - Day of week

### Adjust Clock Size
Edit line 19:
```cpp
const int clockRadius = 85;  // Change this value (50-100)
```

## 🐛 Troubleshooting

| Problem | Solution |
|---------|----------|
| Display shows nothing | Check wiring, ensure CS pin is GPIO 10 |
| Wrong time | Upload code again to re-sync |
| Wrong day of week | Check that your laptop time/date is correct |
| RTC ERROR message | Check DS1302 wiring (CLK, DAT, RST pins) |
| Compile error | Install all required libraries |

## 📹 Demo

Coming soon to YouTube Shorts! 🎬

## 📄 License

MIT License - Feel free to modify and share!

## 👨‍💻 Author

**AYAN**
- YouTube: [Your Channel]
- Instagram: [Your Handle]

## ⭐ Show Your Support

Give a ⭐️ if this project helped you!

---

**Made with ❤️ and ESP32**AYAN-TimeMaster-Pro/README.md
