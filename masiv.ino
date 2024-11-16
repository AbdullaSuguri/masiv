#include <Arduino_LED_Matrix.h>
struct Figures {
  byte w, h;
  byte matrix[4][2];
};
ArduinoLEDMatrix ser;
Figures g[7] = {
  { 2, 3, { { 1, 0 }, { 1, 0 }, { 1, 1 }, { 0, 0 } } },
  { 2, 3, { { 0, 1 }, { 0, 1 }, { 1, 1 }, { 0, 0 } } },
  { 2, 3, { { 1, 0 }, { 1, 1 }, { 1, 0 }, { 0, 0 } } },
  { 1, 4, { { 1, 0 }, { 1, 0 }, { 1, 0 }, { 1, 0 } } },
  { 2, 3, { { 1, 0 }, { 1, 1 }, { 0, 1 }, { 0, 0 } } },
  { 2, 3, { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 } } },
  { 2, 2, { { 1, 1 }, { 1, 1 }, { 0, 0 }, { 0, 0 } } }
};

byte fn = random(6);

int y = 4;
int x = 9;
int left;
int right;
int up;
int down;
int i;
int j;
int score = 0;
int scorast;
int povorot;
int maxY;

int angle;
byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

byte frame_bd[8][12] = {
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }
};
void clear_line() {
  for (int a = 0; a < 12; a++) {
    bool full = true;
    for (int i = 0; i < 8; i++)
      if (frame[i][a] == 0) {
        full = false;
        break;
      }
    if (full) {
      for (int i = 0; i < 8; i++) {
        for (int j = a; j < 11; j++)

          frame[i][j] = frame[i][j + 1];
        frame[i][11] = 0;
      }
      score += 1;
      a--;
    }
  }

  Serial.print("Score: ");
  Serial.println(score);
}

void game_over() {
  for (int i = 0; i < 8; i++) {
    bool full = true;
    for (int j = 0; j < 11; j++)
      if (frame[i][j] == 0) {
        full = false;
        break;
      }
    if (full) {
      ser.renderBitmap(frame_bd, 8, 12);
      Serial.println("Game over:");
      delay(10000);
      break;
    }
  }
}

bool kn(int data) {
  if (data == 0)
    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 12; j++)
        frame_bd[i][j] = frame[i][j];

  for (int i = 0; i < g[fn].h; i++) 
    for (int j = 0; j < g[fn].w; j++) 
      switch (angle) {
        case 0:
          {
            if ((g[fn].matrix[i][j] == 1) && ((y + i) < 8) && ((x + j) < 12)) 
              frame[y + i][x + j] = data;
            break;
          }
        case 1:
          {
            if ((g[fn].matrix[g[fn].h - 1 - i][j] == 1) && ((y + j) < 8) && ((x + i) < 12)) 
              frame[y + j][x + i] = data;

            break;
          }
        case 2:
          {
            if ((g[fn].matrix[g[fn].h - 1 - i][g[fn].w - 1 - j] == 1) && ((y + i) < 8) && ((x + j) < 12)) 
              frame[y + i][x + j] = data;

            break;
          }
        case 3:
          {
            if ((g[fn].matrix[i][g[fn].w - 1 - j] == 1) && ((y + j) < 8) && ((x + i) < 12)) 
              frame[y + j][x + i] = data;
            break;
          }
        default:
          {
            break;
          }
      }
  if (data == 1) {
    int cnt_f = 0;
    int cnt_f_bd = 0;
    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 12; j++) {
        if (frame[i][j] == 1) cnt_f++;
        if (frame_bd[i][j] == 1) cnt_f_bd++;
      }
    if (cnt_f_bd > cnt_f) {
      for (int i = 0; i < 8; i++)
        for (int j = 0; j < 12; j++)
          frame[i][j] = frame_bd[i][j];
      return false;
    } else return true;
  }
}

void setup() {

  Figures gg;
  gg.w = 2;
  gg.h = 3;
  gg.matrix[0][0] = 1;
  gg.matrix[0][1] = 0;
  gg.matrix[1][0] = 1;
  gg.matrix[1][1] = 0;
  gg.matrix[2][0] = 1;
  gg.matrix[2][1] = 1;
  Serial.begin(9600);
  ser.begin();
  ser.renderBitmap(frame, 8, 12);
  ser.renderBitmap(frame_bd, 8, 12);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
}

void loop() {
  kn(0);
  if (angle % 2 == 0) maxY = 5;
  else maxY = 6;
  i++;
  int axel;
  if (digitalRead(2) == 1) axel = 1;
  else axel = 0;
  if (i >= (10 + axel * 90)) {

    x--;
    i = 0;
  }

  //up
  if (digitalRead(1) == 0) {
    if (up == 0)
      if ((y > 0) && (frame[y - 1][x] == 0))
        y--;

    up = 1;

  } else up = 0;

  //down
  if (digitalRead(0) == 0) {
    if (down == 0)
      if ((y < maxY) && (frame[y + 1][x] == 0))
        y++;
    down = 1;
  } else down = 0;
  if (digitalRead(3) == 0) {
    if (povorot == 0) {
      if (y == 6)
        y = 5;
      angle++;
      angle %= 4;
      povorot = 1;
    }
  } else povorot = 0;
  bool stat = kn(1);
  delay(10);
  if ((x <= 0) || !stat) {
    clear_line();
    game_over();
    x = 11;
    y = 1;
    fn=random(6);
    kn(0);
    kn(1);
  }



  ser.renderBitmap(frame, 8, 12);
}
