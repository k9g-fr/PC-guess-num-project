#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte numRows = 4;
const byte numCols = 4;

char keymap[numRows][numCols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[numRows] = {9, 8, 7, 6};
byte colPins[numCols] = {5, 4, 3, 2};

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setBacklight(HIGH);
}

void loop() {
  lcd.clear();
  lcd.setCursor(16, 0);
  lcd.print("Number guess game!");
  lcd.setCursor(16, 1);
  lcd.print("press any key...");

  // 🔁 ทำให้ข้อความเลื่อนวนไปเรื่อยๆ จนกว่าจะมีการกดปุ่ม
  while (true) {
    char key = myKeypad.getKey();
    if (key) {
      showDifficultyScreen();
      return;
    }

    lcd.scrollDisplayLeft();
    delay(200);
  }
}

void showDifficultyScreen() {
  lcd.clear();
  lcd.setCursor(16, 0);
  lcd.print("Select Difficulty");
  lcd.setCursor(16, 1);
  lcd.print("A.Easy B.Med C.Hard");

  // 🔁 ให้ข้อความเลื่อนไปเรื่อย ๆ จนกว่าจะมีการกดปุ่ม
  while (true) {
    char key = myKeypad.getKey();
    if (key) {
      handleDifficultySelection(key);
      return;
    }

    lcd.scrollDisplayLeft();
    delay(200);
  }
}

void handleDifficultySelection(char key) {
  lcd.clear();
  lcd.setCursor(0, 0);

  switch (key) {
    case 'A':
      lcd.print("Easy Mode 0-30");
      lcd.setCursor(0, 1);
      lcd.print("press to start...");
      break;
    case 'B':
      lcd.print("Medium Mode 0-60");
      lcd.setCursor(0, 1);
      lcd.print("press to start...");
      break;
    case 'C':
      lcd.print("Hard Mode 0-99");
      lcd.setCursor(0, 1);
      lcd.print("press to start...");
      break;
    default:
      lcd.print("Invalid choice!");
      break;
  }

  // รอให้ผู้ใช้กดปุ่มใด ๆ เพื่อกลับหน้าหลัก
  while (!myKeypad.getKey()) {
    delay(100);
  }

  // กลับไปหน้าหลัก
  loop();
}
