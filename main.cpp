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

// 조건
// 뱀의 몸통 및 움직임 구현
// 뱀이 진행방향으로 자동 전진 구현 / 구현 중
// 벽 또는 몸통에 부딪히면 패배 판정 구현 / 벽만 구현
// 뱀이 맵을 다 채우면 승리 판정 구현
// 사과를 먹으면 뱀의 길이 증가 구현 / 완료
// 점수를 맵 바로 아래 중앙 배치 구현
// 패배시 게임 정지 구현 / 완료
// 엔터 누르면 재시작 구현 / 미구현

// 구현한 것
// 사과 랜덤 출현

int applex, appley, score, tail; // 사과의 x, y좌표, 점수, 꼬리의 수
int x = BOARD_SIZE / 2; // 뱀의 시작 x좌표
int y = BOARD_SIZE / 2; // 뱀의 시작 y좌표
int tailx[BOARD_SIZE]; // 뱀의 꼬리의 x좌표를 저장하기 위한 배열
int taily[BOARD_SIZE]; // 뱀의 꼬리의 y좌표를 저장하기 위한 배열
bool gameOver = false; // 게임 오버인지 판단하기 위한 bool타입 값
bool gamePlay = true; // 게임을 플레이할 것인지 결정하는 bool타입 값
int snakeState; // 뱀의 방향을 결정할 값

void moveDelay() {
  // MOVE_DELAY로 정의된 프레임수 만큼 딜레이된다.
  for (int i = 0; i < MOVE_DELAY; i++) {
    console::wait();
  }
}

void handleInput() {
  // 방향키를 입력받아 뱀의 방향을 설정한다.
  if (console::key(console::K_LEFT)) {
    x--;
    //moveDelay();
    //snakeState = 1;
  }
  if (console::key(console::K_RIGHT)) {
    x++;
    //moveDelay();
    //snakeState = 2;
  }
  if (console::key(console::K_UP)) {
    y--;
    //moveDelay();
    //snakeState = 3;
  }
  if (console::key(console::K_DOWN)) {
    y++;
    //moveDelay();
    //snakeState = 4;
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

void drawBoard() {
  // 맵을 그린다.
  console::draw(0, 0, WALL_LEFT_TOP_STRING);
  for (int i = 1; i < BOARD_SIZE - 1; i++) {
    console::draw(i, 0, WALL_HORIZONTAL_STRING);
  }
  console::draw(BOARD_SIZE - 1, 0, WALL_RIGHT_TOP_STRING);

  for (int i = 1; i < BOARD_SIZE - 1; i++) {
    console::draw(0, i, WALL_VERTICAL_STRING);
  }
  for (int i = 1; i < BOARD_SIZE - 1; i++) {
    console::draw(BOARD_SIZE - 1, i, WALL_VERTICAL_STRING);
  }

  console::draw(0, BOARD_SIZE - 1, WALL_LEFT_BOTTOM_STRING);
  for (int i = 1; i < BOARD_SIZE - 1; i++) {
    console::draw(i, BOARD_SIZE - 1, WALL_HORIZONTAL_STRING);
  }
  console::draw(BOARD_SIZE - 1, BOARD_SIZE - 1, WALL_RIGHT_BOTTOM_STRING);
}

void xyApple() {
  // applex, appley를 구한다.
  applex = (rand() % (BOARD_SIZE - 2)) + 1;
  appley = (rand() % (BOARD_SIZE - 2)) + 1;
}

void draw() {
  // 맵과 사과, 뱀을 그린다.
  drawBoard();
  console::draw(applex, appley, APPLE_STRING);

  console::draw(x, y, SNAKE_STRING);
  
  for (int i = 0; i < tail; i++) {
    console::draw(tailx[i], taily[i], SNAKE_BODY_STRING);
    
  }
}

void detection() { 
  // 뱀의 방향, 뱀의 꼬리 저장, 사과와 벽과의 충돌 처리를 한다.
  restrictInScreen();
  /*
  if (snakeState == 1) {
    x--;
    moveDelay();
  }else if (snakeState == 2) { 
    x++;
    moveDelay();
  }else if (snakeState == 3) {
    y--;
    moveDelay();
  }else if (snakeState == 4) {
    y++;
    moveDelay();
  }
  */
  int tailX1 = tailx[0];
  int tailY1 = taily[0];
  int tailX2, tailY2;
  tailx[0] = x;
  taily[0] = y;

  for (int i = 1; i < tail; i++) {
    tailX2 = tailx[i];
    tailY2 = taily[i];
    tailx[i] = tailX1;
    taily[i] = tailY1;
    tailX1 = tailX2;
    tailY1 = tailY2;
  }

  if (x == applex && y == appley) {
    score += 10;
    xyApple();
    tail += 1;
    tailx[tail - 1] = tailx[tail - 2];
    taily[tail - 1] = taily[tail - 2];
  }

  if (x == 0 || x == BOARD_SIZE - 1 || y == 0 || y == BOARD_SIZE - 1){
    gameOver = true;
  }
}

void setting() {
  // 게임을 시작하기 전에 필요한 기본 세팅을 한다.
  snakeState = 1;
  bool gameOver = false;
  bool gamePlay = true;
  x = BOARD_SIZE / 2;
  y = BOARD_SIZE / 2;
  srand(time(0));
  xyApple();
}

void game() {
  // 콘솔 라이브러리를 초기화한다.
  console::init();

  while (true) {
    // 화면을 지운다.
    console::clear();

    handleInput();
    draw();
    detection();
    // GAMEOVER의 조건이 충족되면 종료한다.
    if (gameOver == true) {
      gamePlay = false;
      break;
    }
    // ESC를 누르면 종료한다.
    if (console::key(console::K_ESC)) {
      break;
    }

    // 화면을 갱신하고 다음 프레임까지 대기한다.
    console::wait();
  }
}

void rePlay() {
  // 게임을 재시작한다.
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