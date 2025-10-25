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
  randomSeed(analogRead(A0)); // ทำให้สุ่มไม่ซ้ำกัน
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
      lcd.print("Lives: 5");
      delay(2000);
      // เรียกฟังก์ชันเล่นเกมโดยส่งค่าจำนวนชีวิต
      playGame(30, 5);  // 👈 Easy มี 5 ชีวิต
      break;

    case 'B':
      lcd.print("Medium Mode 0-60");
      lcd.setCursor(0, 1);
      lcd.print("Lives: 7");
      delay(2000);
      playGame(60, 7);  // 👈 Medium มี 7 ชีวิต
      break;

    case 'C':
      lcd.print("Hard Mode 0-99");
      lcd.setCursor(0, 1);
      lcd.print("Lives: 10");
      delay(2000);
      playGame(99, 10); // 👈 Hard มี 10 ชีวิต
      break;

    default:
      lcd.print("Invalid choice!");
      break;
  }
}

void waitForKey() {
  while (!myKeypad.getKey()) {
    delay(100);
  }
}

void playGame(int maxNum, int lives) {
  int target = random(0, maxNum + 1);
  int guess = -1;
  String input = "";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Guess 0 - ");
  lcd.print(maxNum);

  showLives(lives);

  while (lives > 0) {
    char key = myKeypad.getKey();
    if (key) {
      if (key >= '0' && key <= '9') {
        input += key;
        lcd.setCursor(0, 1);
        lcd.print("Your guess: ");
        lcd.print(input);
      } 
      else if (key == '#') { // กด # เพื่อยืนยัน
        if (input.length() > 0) {
          guess = input.toInt();
          lcd.clear();

          if (guess == target) {
            lcd.print("Correct! ");
            lcd.print(target);
            lcd.setCursor(0, 1);
            lcd.print("Press any key...");
            waitForKey();
            loop(); // กลับหน้าแรก
            return;
          } else {
            lives--;
            if (guess < target) {
              lcd.print("Too Low!");
            } else {
              lcd.print("Too High!");
            }

            lcd.setCursor(0, 1);
            lcd.print("Lives left: ");
            lcd.print(lives);

            delay(1200);
            if (lives == 0) {
              lcd.clear();
              lcd.print("Out of lives!");
              lcd.setCursor(0, 1);
              lcd.print("Ans: ");
              lcd.print(target);
              delay(2000);
              lcd.clear();
              lcd.print("Press any key...");
              waitForKey();
              loop();
              return;
            }

            // เคลียร์และแสดงอีกครั้ง
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Guess 0 - ");
            lcd.print(maxNum);
            showLives(lives);
            input = "";
          }
        }
      } 
      else if (key == '*') { // ลบเลขตัวสุดท้าย
        if (input.length() > 0) {
          input.remove(input.length() - 1);
          lcd.setCursor(0, 1);
          lcd.print("Your guess: ");
          lcd.print(input);
          lcd.print(" "); // ลบตัวที่เหลือ
        }
      }
    }
  }
}

// ❤️ ฟังก์ชันแสดงจำนวนชีวิต
void showLives(int lives) {
  lcd.setCursor(0, 1);
  lcd.print("Lives: ");
  for (int i = 0; i < lives; i++) {
    lcd.print((char)3); // สัญลักษณ์หัวใจ (บาง LCD อาจแสดงเป็นกล่อง)
  }
}