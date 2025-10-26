#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>

// ===== GC9A01A Circular Display Pins =====
#define TFT_DC    9    // DC/RS pin
#define TFT_CS    10   // CS pin  
#define TFT_MOSI  11   // SDA pin on display
#define TFT_SCLK  12   // SCL pin on display
#define TFT_RST   8    // RES/RST pin

// ===== DS1302 RTC Pins =====
#define RTC_CLK 4   // CLK pin
#define RTC_DAT 5   // DAT pin
#define RTC_RST 6   // RST pin

// Create display object
Adafruit_GC9A01A tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Create RTC object for DS1302
ThreeWire myWire(RTC_DAT, RTC_CLK, RTC_RST);
RtcDS1302<ThreeWire> Rtc(myWire);

// Clock center and radius
const int centerX = 120;
const int centerY = 120;
const int clockRadius = 85;  // Bigger clock!

// Variables for display updates
unsigned long previousMillis = 0;
const long interval = 1000;  // Update every 1 second

// Store previous hand positions to erase them
int lastHourX = 0, lastHourY = 0;
int lastMinuteX = 0, lastMinuteY = 0;
int lastSecondX = 0, lastSecondY = 0;
bool firstDraw = true;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Initializing circular display and DS1302 RTC...");
  
  // ===== Initialize Circular Display =====
  tft.begin();  
  tft.setRotation(0);
  tft.fillScreen(GC9A01A_BLACK);
  
  // ===== Initialize DS1302 RTC =====
  Serial.println("Initializing DS1302...");
  Rtc.Begin();
  
  // ========== AUTO-SYNC TO LAPTOP TIME ==========
  // This automatically sets the RTC to your laptop's time when you upload!
  
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Rtc.SetDateTime(compiled);
  
  Serial.print("Time synced to laptop: ");
  Serial.print(compiled.Day());
  Serial.print("/");
  Serial.print(compiled.Month());
  Serial.print("/");
  Serial.print(compiled.Year());
  Serial.print(" ");
  Serial.print(compiled.Hour());
  Serial.print(":");
  Serial.print(compiled.Minute());
  Serial.println();
  // ==============================================
  
  if (!Rtc.GetIsRunning()) {
    Serial.println("Starting RTC...");
    Rtc.SetIsRunning(true);
  }
  
  Rtc.SetIsWriteProtected(false);
  
  Serial.println("DS1302 RTC initialized successfully!");
  
  // Draw initial clock face
  drawClockFace();
  
  Serial.println("Setup complete!");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Update display every second
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Get current time from RTC
    RtcDateTime now = Rtc.GetDateTime();
    
    // Check if time is valid
    if (!now.IsValid()) {
      Serial.println("RTC time is not valid!");
      tft.fillScreen(GC9A01A_RED);
      tft.setCursor(40, 100);
      tft.setTextColor(GC9A01A_WHITE);
      tft.setTextSize(2);
      tft.println("RTC ERROR!");
      tft.println(" Check wiring!");
      return;
    }
    
    // Update clock
    updateClock(now);
    
    // Print to Serial for debugging
    Serial.print(now.Year());
    Serial.print('/');
    Serial.print(now.Month());
    Serial.print('/');
    Serial.print(now.Day());
    Serial.print(" ");
    Serial.print(now.Hour());
    Serial.print(':');
    Serial.print(now.Minute());
    Serial.print(':');
    Serial.println(now.Second());
  }
}

void drawClockFace() {
  // Draw outer circle (smaller)
  tft.drawCircle(centerX, centerY, clockRadius, GC9A01A_WHITE);
  
  // Draw hour markers
  for (int i = 0; i < 12; i++) {
    float angle = (i * 30 - 90) * PI / 180;  // -90 to start at 12 o'clock
    
    // Outer point
    int x1 = centerX + (clockRadius - 3) * cos(angle);
    int y1 = centerY + (clockRadius - 3) * sin(angle);
    
    // Inner point
    int x2 = centerX + (clockRadius - 10) * cos(angle);
    int y2 = centerY + (clockRadius - 10) * sin(angle);
    
    // Draw thicker lines for 12, 3, 6, 9
    if (i % 3 == 0) {
      tft.drawLine(x1, y1, x2, y2, GC9A01A_WHITE);
      tft.drawLine(x1 + 1, y1, x2 + 1, y2, GC9A01A_WHITE);
    } else {
      tft.drawLine(x1, y1, x2, y2, GC9A01A_WHITE);
    }
  }
  
  // Draw center dot
  tft.fillCircle(centerX, centerY, 3, GC9A01A_WHITE);
}

void updateClock(RtcDateTime now) {
  // Erase old hands (draw in black) - except on first draw
  if (!firstDraw) {
    drawHourHand(lastHourX, lastHourY, GC9A01A_BLACK);
    drawMinuteHand(lastMinuteX, lastMinuteY, GC9A01A_BLACK);
    drawSecondHand(lastSecondX, lastSecondY, GC9A01A_BLACK);
  }
  firstDraw = false;
  
  // Calculate angles (adjust for 12 o'clock at top)
  int hour = now.Hour() % 12;
  int minute = now.Minute();
  int second = now.Second();
  
  float hourAngle = ((hour * 30) + (minute * 0.5) - 90) * PI / 180;
  float minuteAngle = ((minute * 6) - 90) * PI / 180;
  float secondAngle = ((second * 6) - 90) * PI / 180;
  
  // Calculate hand end points (adjusted for bigger clock)
  int hourX = centerX + (clockRadius - 40) * cos(hourAngle);
  int hourY = centerY + (clockRadius - 40) * sin(hourAngle);
  
  int minuteX = centerX + (clockRadius - 20) * cos(minuteAngle);
  int minuteY = centerY + (clockRadius - 20) * sin(minuteAngle);
  
  int secondX = centerX + (clockRadius - 15) * cos(secondAngle);
  int secondY = centerY + (clockRadius - 15) * sin(secondAngle);
  
  // Draw new hands
  drawHourHand(hourX, hourY, GC9A01A_CYAN);
  drawMinuteHand(minuteX, minuteY, GC9A01A_GREEN);
  drawSecondHand(secondX, secondY, GC9A01A_RED);
  
  // Store positions for next erase
  lastHourX = hourX;
  lastHourY = hourY;
  lastMinuteX = minuteX;
  lastMinuteY = minuteY;
  lastSecondX = secondX;
  lastSecondY = secondY;
  
  // Redraw center dot over the hands
  tft.fillCircle(centerX, centerY, 3, GC9A01A_WHITE);
  
  // Display digital time IN CENTER
  displayDigitalTime(now);
  
  // Display date at top
  displayDate(now);
  
  // Display AYAN at bottom
  displayAyan();
}

void drawHourHand(int x, int y, uint16_t color) {
  // Thick hour hand
  tft.drawLine(centerX, centerY, x, y, color);
  tft.drawLine(centerX + 1, centerY, x + 1, y, color);
  tft.drawLine(centerX, centerY + 1, x, y + 1, color);
}

void drawMinuteHand(int x, int y, uint16_t color) {
  // Medium minute hand
  tft.drawLine(centerX, centerY, x, y, color);
  tft.drawLine(centerX + 1, centerY, x + 1, y, color);
}

void drawSecondHand(int x, int y, uint16_t color) {
  // Thin second hand
  tft.drawLine(centerX, centerY, x, y, color);
}

void displayDigitalTime(RtcDateTime now) {
  // Clear area for digital time (CENTER OF DISPLAY)
  tft.fillRect(40, 105, 160, 35, GC9A01A_BLACK);
  
  // Display digital time - LARGE AND CENTERED
  tft.setTextSize(4);
  tft.setTextColor(GC9A01A_YELLOW, GC9A01A_BLACK);
  tft.setCursor(45, 108);
  
  if (now.Hour() < 10) tft.print("0");
  tft.print(now.Hour());
  tft.print(":");
  if (now.Minute() < 10) tft.print("0");
  tft.print(now.Minute());
  
  // Seconds below, smaller
  tft.setTextSize(2);
  tft.setCursor(100, 135);
  if (now.Second() < 10) tft.print("0");
  tft.print(now.Second());
}

void displayDate(RtcDateTime now) {
  // Clear area for date (TOP OF SCREEN)
  tft.fillRect(20, 20, 200, 35, GC9A01A_BLACK);
  
  // Display day of week - CENTERED AT TOP
  const char* days[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  tft.setTextSize(2);
  tft.setTextColor(GC9A01A_MAGENTA, GC9A01A_BLACK);
  
  // Center the day name
  tft.setCursor(90, 22);
  tft.print(days[now.DayOfWeek()]);
  
  // Display date - CENTERED BELOW DAY
  tft.setTextSize(2);
  tft.setTextColor(GC9A01A_CYAN, GC9A01A_BLACK);
  tft.setCursor(55, 40);
  
  if (now.Day() < 10) tft.print("0");
  tft.print(now.Day());
  tft.print("/");
  if (now.Month() < 10) tft.print("0");
  tft.print(now.Month());
  tft.print("/");
  tft.print(now.Year());
}

void displayAyan() {
  // Display AYAN at bottom of screen
  tft.setTextSize(3);
  tft.setTextColor(GC9A01A_GREEN, GC9A01A_BLACK);
  tft.setCursor(75, 210);
  tft.print("AYAN");
}
