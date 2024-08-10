#line 1 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8);
U8G2_ST7920_128X64_F_SW_SPI u8g2_1(U8G2_R0, /* clock=*/ 12, /* data=*/ 7, /* CS=*/ 9, /* reset=*/ 8);

byte incomingByte = 0;
byte state = 1;
byte i, j;
const byte xUnit = 16;
const byte yUnit = 8;
byte pozitieAtacX = 0;
byte pozitieAtacY = 0;
byte playerTurn = 1;
byte shown = 0;
const unsigned int duration = 20000;
unsigned long startTime;
unsigned long elapsedTime = 0;

byte tablePlayer1[8][8];
byte tablePlayer2[8][8];

#line 30 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
void u8g2_prepare(void);
#line 38 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
void u8g21_prepare(void);
#line 46 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
void setup(void);
#line 52 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
void initMessage();
#line 66 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
void populateMatrix(byte matrix[8][8]);
#line 103 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
void generateGame();
#line 138 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
void printMatrixes();
#line 163 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
int checkEnd(byte matrix[8][8]);
#line 174 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
int checkPosition(byte matrix[8][8]);
#line 181 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
byte unitsLeft(byte matrix[8][8]);
#line 193 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
void generateSmallGame();
#line 228 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
void loop(void);
#line 30 "C:\\Users\\andro\\AppData\\Local\\Temp\\.arduinoIDE-unsaved2023420-2232-yzcre7.17vh\\GraphicsTest\\GraphicsTest.ino"
void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}

void u8g21_prepare(void) {
  u8g2_1.setFont(u8g2_font_6x10_tf);
  u8g2_1.setFontRefHeightExtendedText();
  u8g2_1.setDrawColor(1);
  u8g2_1.setFontPosTop();
  u8g2_1.setFontDirection(0);
}

void setup(void) {
  Serial.begin(9600);
  u8g2.begin();
  u8g2_1.begin();
}

void initMessage(){
    u8g2.clearBuffer();
    u8g2_prepare();
    u8g2.drawStr( 13, 13, "Battleship Game!");
    u8g2.sendBuffer();

    delay(2000);

    u8g2_1.clearBuffer();
    u8g21_prepare();
    u8g2_1.drawStr( 13, 13, "Press New Game!");
    u8g2_1.sendBuffer();
}

void populateMatrix(byte matrix[8][8]) {
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            matrix[i][j] = 0;
        }
    }

    byte line4, line5;
    do {
        line4 = random(8);
        line5 = random(8);
    } while (line4 == line5);

    byte col2, col3_1, col3_2;
    do {
        col2 = random(8);
        col3_1 = random(8);
        col3_2 = random(8);
    } while (col2 == col3_1 || col2 == col3_2 || col3_1 == col3_2);

    for (j = 0; j < 8; j++) {
        if (j < 5)
            matrix[line4][j] = 1;
        if (j > 4 && j < 9)
            matrix[line5][j-4] = 1;
    }

    for (i = 0; i < 8; i++) {
        if (i < 2)
            matrix[i][col2] = 1;
        else if (i < 5) {
            matrix[i][col3_1] = 1;
            matrix[i][col3_2] = 1;
        }
    }
}

void generateGame(){
    for(i = 0; i < 8; i++){
      for(j = 0; j < 8; j++){
        tablePlayer1[i][j];
        tablePlayer2[i][j];
      }
    }
    populateMatrix(tablePlayer1);
    populateMatrix(tablePlayer2);
    
    u8g2_prepare();
    u8g2.clearBuffer();
    for(i = 0; i < 8; i++){
      for(j = 0; j < 8; j++){
        if(tablePlayer1[i][j] == 1){
          u8g2.drawBox(i * xUnit, j * yUnit, 16, 8);
        }
      }
    }
    u8g2.sendBuffer();
    delay(2000);

    u8g21_prepare();
    u8g2_1.clearBuffer();
    for(i = 0; i < 8; i++){
      for(j = 0; j < 8; j++){
        if(tablePlayer2[i][j] == 1){
          u8g2_1.drawBox(i * xUnit, j * yUnit, 16, 8);
          
        }
      }
    }
    u8g2_1.sendBuffer();
}

void printMatrixes(){
    u8g2.clearBuffer();
    u8g2_prepare();
    for(i = 0; i < 8; i++){
      for(j = 0; j < 8; j++){
        if(tablePlayer1[i][j] == 1){
          u8g2.drawBox(i * xUnit, j * yUnit, 16, 8);
        }
      }
    }
    u8g2.sendBuffer();

    u8g2_1.clearBuffer();
    u8g21_prepare();
    for(i = 0; i < 8; i++){
      for(j = 0; j < 8; j++){
        if(tablePlayer2[i][j] == 1){
          u8g2_1.drawBox(i * xUnit, j * yUnit, 16, 8);
          
        }
      }
    }
    u8g2_1.sendBuffer();
}

int checkEnd(byte matrix[8][8]){
  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      if(matrix[i][j] == 1){
        return 0;
      }
    }
  }
  return 1;
}

int checkPosition(byte matrix[8][8]){
  if(matrix[pozitieAtacY][pozitieAtacX] == 1){
    return 1;
  }
  return 0;
}

byte unitsLeft(byte matrix[8][8]){
  byte count = 0;
  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      if(matrix[i][j] == 1){
        count++;
      }
    }
  }
  return count;
}

void generateSmallGame(){
  tablePlayer1[0][0] = 1;
  tablePlayer1[1][1] = 1;

  tablePlayer2[3][2] = 1;
  tablePlayer2[2][3] = 1;

    u8g2.clearBuffer();
    u8g2_prepare();
    for(i = 0; i < 8; i++){
      for(j = 0; j < 8; j++){
        if(tablePlayer1[i][j] == 1){
          u8g2.drawBox(i * xUnit, j * yUnit, 16, 8);
        }
      }
    }

    u8g2.sendBuffer();
    delay(2000);

    u8g2_1.clearBuffer();
    u8g21_prepare();
    for(i = 0; i < 8; i++){
      for(j = 0; j < 8; j++){
        if(tablePlayer2[i][j] == 1){
          u8g2_1.drawBox(i * xUnit, j * yUnit, 16, 8);
          
        }
      }
    }
    u8g2_1.sendBuffer();
}



void loop(void) {
  switch(state){
    case 1: {
      initMessage();
      delay(1000);
      while(1){
        if (Serial.available() > 0) {
          incomingByte = Serial.read();
          if(incomingByte == 1){
            incomingByte = 0;
            break;
          }
        }
      }
      delay(5000);
      generateSmallGame();
      delay(5000);
      state = 2;
    }
    case 2:{
      incomingByte = 0;
      if (Serial.available() > 0) {
        incomingByte = Serial.read();
      }
      if(playerTurn == 1){
        if(shown == 0){
          u8g2.clearBuffer();
          u8g2_prepare();
          u8g2.drawStr( 0, 0, "Player 1 turn!");
          u8g2.sendBuffer();
          startTime = millis();
          u8g2_1.clearBuffer();
          u8g2_1.sendBuffer();
          shown = 1;
        }
        elapsedTime = millis() - startTime;
        if (elapsedTime >= duration) {
          startTime = millis();
          pozitieAtacY = 0;
          pozitieAtacX = 0;
          playerTurn = 2;
          shown = 0;
        }
        switch(incomingByte){
          case 1:{
            u8g2.clearBuffer();
            u8g2_prepare();
            u8g2.drawStr( 0, 0, "Don't press New Game!");
            u8g2.sendBuffer();
            break;
          }
          case 2:{
            if(checkPosition(tablePlayer2)){
              u8g2.clearBuffer();
              u8g2_prepare();
              u8g2.drawStr( 0, 0, "HIT!");
              u8g2.sendBuffer();
              delay(2000);

              tablePlayer2[pozitieAtacY][pozitieAtacX] = 0;
              if(checkEnd(tablePlayer2)){
                u8g2.clearBuffer();
                u8g2_prepare();
                u8g2.drawStr( 0, 0, "Player 1 WINS!");
                u8g2.sendBuffer();

                delay(2000);

                u8g2_1.clearBuffer();
                u8g21_prepare();
                u8g2_1.drawStr( 0, 0, "Player 1 WINS!");
                u8g2_1.sendBuffer();
                delay(5000);
                state = 1;
              }
            }
            else{
              u8g2.clearBuffer();
              u8g2_prepare();
              u8g2.drawStr( 0, 0, "MISS!");
              u8g2.sendBuffer();
              delay(2000);
            }
            pozitieAtacY = 0;
            pozitieAtacX = 0;
            playerTurn = 2;
            shown = 0;
            break;
          }
          case 3:{
            pozitieAtacY--;
            pozitieAtacY %= 8;
            u8g2.clearBuffer();
            u8g2_prepare();
            u8g2.drawBox(pozitieAtacX * xUnit, pozitieAtacY * yUnit, 16, 8);
            u8g2.sendBuffer();
            break;
          }
          case 4:{
            pozitieAtacY++;
            pozitieAtacY %= 8;
            u8g2.clearBuffer();
            u8g2_prepare();
            u8g2.drawBox(pozitieAtacX * xUnit, pozitieAtacY * yUnit, 16, 8);
            u8g2.sendBuffer();
            break;
          }
          case 5:{
            pozitieAtacX--;
            pozitieAtacX %= 8;
            u8g2.clearBuffer();
            u8g2_prepare();
            u8g2.drawBox(pozitieAtacX * xUnit, pozitieAtacY * yUnit, 16, 8);
            u8g2.sendBuffer();
            break;
          }
          case 6:{
            pozitieAtacX++;
            pozitieAtacX %= 8;
            u8g2.clearBuffer();
            u8g2_prepare();
            u8g2.drawBox(pozitieAtacX * xUnit, pozitieAtacY * yUnit, 16, 8);
            u8g2.sendBuffer();
            break;
          }
        }
      }
      else if(playerTurn == 2){
        if(shown == 0){
          u8g2_1.clearBuffer();
          u8g21_prepare();
          u8g2_1.drawStr( 0, 0, "Player 2 turn!");
          u8g2_1.sendBuffer();
          startTime = millis();
          u8g2.clearBuffer();
          u8g2.sendBuffer();
          shown = 1;
        }
        elapsedTime = millis() - startTime;
        if (elapsedTime >= duration) {
          startTime = millis();
          pozitieAtacY = 0;
          pozitieAtacX = 0;
          playerTurn = 1;
          shown = 0;
        }
        switch(incomingByte){
          case 1:{
            u8g2_1.clearBuffer();
            u8g21_prepare();
            u8g2_1.drawStr( 0, 0, "Don't press New Game!");
            u8g2_1.sendBuffer();
            break;
          }
          case 2:{
            if(checkPosition(tablePlayer1)){
              u8g2_1.clearBuffer();
              u8g21_prepare();
              u8g2_1.drawStr( 0, 0, "HIT!");
              u8g2_1.sendBuffer();
              delay(2000);

              tablePlayer1[pozitieAtacY][pozitieAtacX] = 0;
              if(checkEnd(tablePlayer1)){
                u8g2.clearBuffer();
                u8g2_prepare();
                u8g2.drawStr( 0, 0, "Player 2 WINS!");
                u8g2.sendBuffer();

                delay(2000);

                u8g2_1.clearBuffer();
                u8g21_prepare();
                u8g2_1.drawStr( 0, 0, "Player 2 WINS!");
                u8g2_1.sendBuffer();
                delay(5000);
                state = 1;
              }
            }
            else{
              u8g2_1.clearBuffer();
              u8g21_prepare();
              u8g2_1.drawStr( 0, 0, "MISS!");
              u8g2_1.sendBuffer();
              delay(2000);
            }
            pozitieAtacY = 0;
            pozitieAtacX = 0;
            playerTurn = 1;
            shown = 0;
            break;
          }
          case 3:{
            pozitieAtacY--;
            pozitieAtacY %= 8;
            u8g2_1.clearBuffer();
            u8g21_prepare();
            u8g2_1.drawBox(pozitieAtacX * xUnit, pozitieAtacY * yUnit, 16, 8);
            u8g2_1.sendBuffer();
            break;
          }
          case 4:{
            pozitieAtacY++;
            pozitieAtacY %= 8;
            u8g2_1.clearBuffer();
            u8g21_prepare();
            u8g2_1.drawBox(pozitieAtacX * xUnit, pozitieAtacY * yUnit, 16, 8);
            u8g2_1.sendBuffer();
            break;
          }
          case 5:{
            pozitieAtacX--;
            pozitieAtacX %= 8;
            u8g2_1.clearBuffer();
            u8g21_prepare();
            u8g2_1.drawBox(pozitieAtacX * xUnit, pozitieAtacY * yUnit, 16, 8);
            u8g2_1.sendBuffer();
            break;
          }
          case 6:{
            pozitieAtacX++;
            pozitieAtacX %= 8;
            u8g2_1.clearBuffer();
            u8g21_prepare();
            u8g2_1.drawBox(pozitieAtacX * xUnit, pozitieAtacY * yUnit, 16, 8);
            u8g2_1.sendBuffer();
            break;
          }
        }
      }
    }

  }
}
