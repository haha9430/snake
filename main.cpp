#include "console.h"
#include <cstdlib>
#include <string>
#include <cstring>
#include <ctime>

#define BOARD_SIZE 20
#define MOVE_DELAY 15
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

int applex, appley, score, tail;
int x = BOARD_SIZE / 2;
int y = BOARD_SIZE / 2;
int tailx[BOARD_SIZE];
int taily[BOARD_SIZE];
bool gameOver = false;
bool gamePlay = true;
int snakeState;

void moveDelay() {
  for (int i = 0; i < MOVE_DELAY; i++) {
    console::wait();
  }
}

void handleInput() {
  if (console::key(console::K_LEFT)) {
    if (tail == 0) {
      snakeState = 1;
    }else if (snakeState != 2) {
      snakeState = 1;
    }
  }
  if (console::key(console::K_RIGHT)) {
    if (tail == 0) {
      snakeState = 2;
    }else if (snakeState != 1) {
      snakeState = 2;
    }
  }
  if (console::key(console::K_UP)) {
    if (tail == 0) {
      snakeState = 3;
    }else if (snakeState != 4) {
      snakeState = 3;
    }
  }
  if (console::key(console::K_DOWN)) {
    if (tail == 0) {
      snakeState = 4;
    }else if (snakeState != 3) {
      snakeState = 4;
    }
  }
}

void restrictInScreen() {
  // x, y 위치를 화면의 최대 크기에서 벗어나지 않게 한다.
  if (x < 0)
    x = 0;
  if (x >= console::SCREEN_WIDTH)
    x = console::SCREEN_WIDTH - 1;
  if (y < 0)
    y = 0;
  if (y >= console::SCREEN_HEIGHT)
    y = console::SCREEN_HEIGHT - 1;
}

void draw() {
  // x, y 위치에 *을 그린다.
  console::draw(x, y, SNAKE_STRING);
  console::draw(applex, appley, APPLE_STRING);
  for (int i = 0; i < tail; i++) {
    console::draw(tailx[i], taily[i], SNAKE_BODY_STRING);
  }
  
  char scoreSentence[10] = "SCORE: ";
  char scoreString[5];
  sprintf(scoreString, "%d", score);
  strcat(scoreSentence, scoreString);
  int scoreLocation = (BOARD_SIZE / 2) - 4;
  if (score > 99) {
    scoreLocation = (BOARD_SIZE / 2) - 5;
  }
  console::draw(scoreLocation, BOARD_SIZE, scoreSentence);
}

void drawBoard() {
  console::draw(0, 0, WALL_LEFT_TOP_STRING);
  for (int i = 1; i < BOARD_SIZE - 1; i++) {
    console::draw(i, 0, WALL_HORIZONTAL_STRING);
  }
  console::draw(19, 0, WALL_RIGHT_TOP_STRING);

  for (int i = 1; i < BOARD_SIZE - 1; i++) {
    console::draw(0, i, WALL_VERTICAL_STRING);
  }
  for (int i = 1; i < BOARD_SIZE - 1; i++) {
    console::draw(19, i, WALL_VERTICAL_STRING);
  }
  
  console::draw(0, 19, WALL_LEFT_BOTTOM_STRING);
  for (int i = 1; i < BOARD_SIZE - 1; i++) {
    console::draw(i, 19, WALL_HORIZONTAL_STRING);
  }
  console::draw(19, 19, WALL_RIGHT_BOTTOM_STRING);
}

void drawApple() {
  // applex, appley 위치에 ●을 그린다.
  applex = (rand() % (BOARD_SIZE - 2)) + 1;
  appley = (rand() % (BOARD_SIZE - 2)) + 1;
}

void detection() {
  int prevX = tailx[0];
	int prevY = taily[0];
	int prev2X, prev2Y;
	tailx[0] = x;
	taily[0] = y;

	for (int i = 1; i < tail; i++) {
		prev2X = tailx[i];
		prev2Y = taily[i];
		tailx[i] = prevX;
		taily[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

  if (snakeState == 1) {
    x--;
  }else if (snakeState == 2) { 
    x++;
  }else if (snakeState == 3) {
    y--;
  }else if (snakeState == 4) {
    y++;
  }

  if (x == 0 || x == BOARD_SIZE - 1 || y == 0 || y == BOARD_SIZE - 1){
    gameOver = true;
  }

  for (int i = 0; i < tail; ++i) {
    if (tailx[i] == x && taily[i] == y) {
      gameOver = true;
      break;
    }
  }

  if (x == applex && y == appley) {
		score += 10;
		drawApple();
    tail += 1;
    tailx[tail - 1] = tailx[tail - 2];
    taily[tail - 1] = taily[tail - 2];
	}

}

void setting() {
  snakeState = 1;
  bool gameOver = false;
  bool gamePlay = true;
  x = BOARD_SIZE / 2;
  y = BOARD_SIZE / 2;
  srand(time(0));
  drawApple();
}

void game() {
  // 콘솔 라이브러리를 초기화한다.
  console::init();
  
  while (true) {
    // 화면을 지운다.
    console::clear();

    handleInput();
    drawBoard();
    draw();
    restrictInScreen();
    detection();
    
    if (gameOver == true) {
      gamePlay = false;
      break;
    }
    
    if (console::key(console::K_ESC)) {
      break;
    }

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    moveDelay();
  }
}

void rePlay() {
  gamePlay = true;
}

int main() {
  setting();
  game();

  /*
  while(true) {
    setting();
    game();
    if (console::key(console::K_ENTER)) {
      continue;
    }
    if (console::key(console::K_ESC)) {
      break;
    }
  }
  */
 
  return 0;
}
