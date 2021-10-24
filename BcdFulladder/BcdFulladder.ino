/**
        @file   BcdFulladder.ino
        @brief  Bcdで足し算をする
        @author caseyNelseon314
        @date   2021/9/15
*/
bool OR  [2][2];
bool NOR [2][2];
bool AND [2][2];
bool NAND[2][2];
bool XOR [2][2];
//#############################################
bool fulladderinput [2][4];//2桁、4桁2進数入力
bool fulladderoutput[4];   //1桁、4桁2進数出力
bool BCDinput [2][4];
bool BCDoutput[4];
bool memory;
bool carryoutmemory;
bool carryout;
void setup() {
  Serial.begin(115200);
  //OR
  OR  [0][0] = 0;
  OR  [0][1] = 1;
  OR  [1][0] = 1;
  OR  [1][1] = 1;

  //NOR
  NOR [0][0] = 1;
  NOR [0][1] = 0;
  NOR [1][0] = 0;
  NOR [1][1] = 0;

  //AND
  AND [0][0] = 0;
  AND [0][1] = 0;
  AND [1][0] = 0;
  AND [1][1] = 1;

  //NAND
  NAND[0][0] = 1;
  NAND[0][1] = 1;
  NAND[1][0] = 1;
  NAND[1][1] = 0;

  //XOR
  XOR [0][0] = 0;
  XOR [0][1] = 1;
  XOR [1][0] = 1;
  XOR [1][1] = 0;
}

void loop() {
  //1001下から～
  BCDinput[0][0] = 1;
  BCDinput[0][1] = 1;
  BCDinput[0][2] = 0;
  BCDinput[0][3] = 0;
  //0001下から～
  BCDinput[1][0] = 1;
  BCDinput[1][1] = 0;
  BCDinput[1][2] = 0;
  BCDinput[1][3] = 1;
  BCDfulladder(0);
  Serial.print("000");
  Serial.print(carryout);
  Serial.print(" ");
  for (int i = 3; i >= 0; i--) {
    Serial.print(BCDoutput[i]);
  }
  Serial.println();
}





//############################################BCD全加算器####################################################
void BCDfulladder(bool BCDcarryin) {
  carryout = 0;
  bool input6[4] = {0, 1, 1, 0};
  bool AAA;
  //第一演算
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 4; j++) {
      fulladderinput[i][j] = BCDinput[i][j];
    }
  }
  fulladder4digit();
  AAA = carryoutmemory;
  //第二演算
  for (int i = 0; i < 4; i++) {
    BCDoutput[i]         = fulladderoutput[i];//6を足す前に結果を記憶
    fulladderinput[0][i] = fulladderoutput[i];
    fulladderinput[1][i] = input6[i];         //6を足す
  }

  fulladder4digit();

  if (carryoutmemory || AAA) {                //第一演算か第二演算で繰り上がりがあったとき
    carryout = 1;                             //6を足して繰り上がりがあるときcarryoutを1
    for (int i = 0; i < 4; i++) {
      BCDoutput[i] = fulladderoutput[i];            //繰り上がりがあるとき上書き
    }
  }
  if (BCDcarryin) {                           //繰り上がり信号あるとき
    for (int i = 0; i < 4; i++) {
      fulladderinput[0][i] = fulladderoutput[i];
    }

    for (int i = 0; i < 3; i++) {
      fulladderinput[1][i + 1] = 0;
    }
    fulladderinput[1][0] = BCDcarryin;

    fulladder4digit();

    for (int i = 0; i < 4; i++) {
      BCDoutput[i] = fulladderoutput[i];
    }
  }
}


void fulladder4digit() {
  bool carryoutA = 0;
  for (int i = 0; i < 4; i++) {
    fulladderoutput[i] = fulladder(fulladderinput[0][i], fulladderinput[1][i], carryoutA, 0);
    carryoutA          = fulladder(fulladderinput[0][i], fulladderinput[1][i], carryoutA, 1);
  }
  carryoutmemory = carryoutA;
}


bool fulladder(bool inputfulladderA, bool inputfulladderB, bool inputfulladderC, bool inputfulladder_digit) {
  //input1 , input2 , carryin ,  0:BCDoutput 1:carryout

  bool aa = 0, bb = 0, cc = 0, dd = 0, ee = 0, ff = 0, gg = 0, hh = 0;
  bool fulladder_output = 0;
  aa = inputfulladderA;
  bb = inputfulladderB;
  cc = inputfulladderC;
  //////////////演算部//////////////
  //halfadder
  dd = XOR[aa][bb];
  ee = AND[aa][bb];

  //halfadder
  ff = XOR[dd][cc];//BCDoutput
  gg = AND[dd][cc];

  hh = OR [ee][gg];//carryout
  /////////////////////////////////

  fulladder_output = inputfulladder_digit ? hh : ff;  //出力切り替え

  return fulladder_output;
}
//###########################################################################################################
