#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>             // библиотека расширенной работы со строками
#include <Wire.h>               // библиотека для соединения
/*------------------------------------------------*/
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
/*------------------------------------------------*/
unsigned long err;
int barHeight = 68;
int PCdata[7];
int printErr;
String perc;

void setup() {
  Serial.begin(9600);
  // инициализация дисплея
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // инициализация дисплея по интерфейсу I2C, адрес 0x3C)
  display.clearDisplay(); // очистка дисплея
  display.setTextSize(1); // установка размера шрифта
  display.setTextColor(WHITE); // установка цвета текста
}

// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------
void loop() {
    if (! Serial.available() > 0) {
    if (err == 0){
      err = millis();
    }
    if (millis() - err > (unsigned long) 10*1000 && printErr != 1) {
      display.clearDisplay();
      printErr = 1;
      display.setCursor(39, 25);
      display.print("CONNECTION");
      display.setCursor(51, 33);
      display.print("FAILED");
      display.display(); 
    }
  }

  while (Serial.available() == 7) {
    err = 0;
    printErr = 0;
    for (int i = 0; i < 7; i++) { 
      char aChar = Serial.read();
      PCdata[i] = aChar;
    }
  }
  if (printErr != 1){
    display.clearDisplay();
    static_info();
    printData();
    printTable();
    display.display();  
  }
}

void static_info() {
  display.setCursor(0, 0);
  display.print("CPU:");
  display.setCursor(0, 9);
  display.print("GPU:");
  display.setCursor(0, 18);
  display.print("GPUmem:");
  display.setCursor(0, 27);
  display.print("RAMuse:");
  display.setCursor(0, 36);
  display.print("SWAP:");
}

void printData() {
  // CPU temp
  display.setCursor(22, 0); display.print(PCdata[5]);
  if (PCdata[5] < 100) display.write(247);
  // CPU load
  display.setCursor(110, 0); display.print(PCdata[0]);
  if (PCdata[0] < 100) perc = "%";
    else perc = "";  display.print(perc);
  // GPU temp
  display.setCursor(22, 9); display.print(PCdata[6]);
  if (PCdata[6] < 100) display.write(247);
  // GPU load
  display.setCursor(110, 9); display.print(PCdata[1]); 
    if (PCdata[1] < 100) perc = "%";
      else perc = "";  display.print(perc);
  // GPU memory usage
  display.setCursor(110, 18); display.print(PCdata[2]);
  if (PCdata[2] < 100) perc = "%";
    else perc = "";  display.print(perc);
  // RAM usage
  display.setCursor(110, 27); display.print(PCdata[3]);
  if (PCdata[3] < 100) perc = "%";
    else perc = "";  display.print(perc);
  // SWAP usage
  display.setCursor(110, 36); display.print(PCdata[4]);
  if (PCdata[4] < 100) perc = "%";
    else perc = "";  display.print(perc);
}
//     
void printTable() {
  int k = 0;
  for (int16_t i = 1; i<display.height()-18; i+=9) {
    int line = ceil(PCdata[k] * barHeight / 100);
    display.drawRect(40, i, barHeight, 6, SSD1306_WHITE);
    display.fillRect(40, i, line, 6, SSD1306_WHITE);
    k++;
  }
}
