#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

char getInputChar() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') return 0;
    return c;
  }
  return 0;
}

String getInputString() {
  String s = "";
  while (true) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '\n' || c == '\r') {
        if (s.length() > 0)
          return s;
      }
      else if (c >= '0' && c <= '9') {
        s += c;
        Serial.print(c);
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  randomSeed(analogRead(A0));
}

void loop() {
  lcd.clear();
  lcd.setCursor(16, 0);
  lcd.print("Number guess game!");
  lcd.setCursor(16, 1);
  lcd.print("press any key...");

  while (true) {
    char key = getInputChar();
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

  while (true) {
    char key = getInputChar();
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
      playGame(30, 5);
      break;

    case 'B':
      lcd.print("Medium Mode 0-60");
      lcd.setCursor(0, 1);
      lcd.print("Lives: 7");
      delay(2000);
      playGame(60, 7);
      break;

    case 'C':
      lcd.print("Hard Mode 0-99");
      lcd.setCursor(0, 1);
      lcd.print("Lives: 10");
      delay(2000);
      playGame(99, 10);
      break;

    default:
      lcd.print("Invalid choice!");
      delay(1200);
      loop();
      break;
  }
}

void waitForKey() {
  while (!getInputChar()) {
    delay(50);
  }
}

void playGame(int maxNum, int lives) {
  int target = random(0, maxNum + 1);
  int guess = -1;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Guess 0 - ");
  lcd.print(maxNum);

  showLives(lives);

  while (lives > 0) {

    Serial.println("\nEnter your guess (number only): ");
    String s = getInputString();
    guess = s.toInt();

    lcd.clear();
    if (guess == target) {
      lcd.print("Correct! ");
      lcd.print(target);
      lcd.setCursor(0, 1);
      lcd.print("Press any key...");
      waitForKey();
      loop();
      return;
    }

    lives--;
    if (guess < target) {
      lcd.print("Too Low!");
    } else {
      lcd.print("Too High!");
    }

    lcd.setCursor(0, 1);
    lcd.print("Lives left: ");
    lcd.print(lives);

    delay(1500);

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

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Guess 0 - ");
    lcd.print(maxNum);
    showLives(lives);
  }
}

void showLives(int lives) {
  lcd.setCursor(0, 1);
  lcd.print("Lives: ");
  for (int i = 0; i < lives; i++) {
    lcd.print((char)3);
  }
}