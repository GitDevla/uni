#include <unistd.h>

#include <cmath>
#include <iostream>
using namespace std;

#pragma region Constants
// nincs kedvem 60 soros ui-t írni
const int FPS = 30;
// képfrissítési frekvencia, fizikát nem befolyásolja (kivéve
// pattanásnál, ott egy kicsi pontosság elszál <= ~10 fps)
const float GAMESPEED = 1; // játék sebessége

const int AREA_WIDTH = 40;  // pálya szélessége
const int AREA_HEIGHT = 20; // pálya magassága

const float GRAVITY = 9.8;  // gravitáció (block/s^2)
const float FRICTION = 0.8; // % energia megtartása falhoz érésnél
const float BOUNCINESS = 0.8; // % energia megtartása ütközéss írányával szemben

const int INITIAL_X = 1;   // labda kezdőpozíciója (x) balról mérve
const int INITIAL_Y = 15;  // labda kezdőpozíciója (y) alulról mérve
const int INITIAL_DX = 30; // labda kezdősebessége (x) (block/s) jobbra
const int INITIAL_DY = 10; // labda kezdősebessége (y) (block/s) felfele

const char *BALL_TEXTURE = "⬤";  // labda textúrája
const char *TRAIL_TEXTURE = "·"; // labda útjának textúrája

const int SECOND = 1000000 / GAMESPEED;

#pragma endregion

struct Vec2 {
    double x;
    double y;
    Vec2(double x, double y) : x(x), y(y) {}
    Vec2() : x(0), y(0) {}
    Vec2 operator+(Vec2 other) { return Vec2(x + other.x, y + other.y); }
    Vec2 operator+=(Vec2 other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vec2 operator*=(float other) {
        x *= other;
        y *= other;
        return *this;
    }
};

struct Ball {
    Vec2 pos;
    Vec2 dir;
    Ball(Vec2 pos, Vec2 dir) : pos(pos), dir(dir) {}
    Ball() : pos(Vec2(0, 0)), dir(Vec2(0, 0)) {}
};
Ball ball;
float frame_length;
float gravity_per_um;
char *screenBuffer[AREA_WIDTH * AREA_HEIGHT];
Vec2 prev_pos;
#pragma region UI
string ilovecpp(float x) {
    auto temp = to_string(round(x * 100) / 100);
    return temp.substr(0, temp.find('.') + 5);
}

void clearScreen() { cout << "\033[2J\033[1;1H"; }

void drawBorder(char *buffer[]) {
    for (size_t i = 0; i < AREA_HEIGHT; i++) {
        buffer[i * AREA_WIDTH] = "║";
        buffer[i * AREA_WIDTH + AREA_WIDTH - 1] = "║";
    }
    for (size_t i = 0; i < AREA_WIDTH; i++) {
        buffer[i] = "═";
        buffer[(AREA_HEIGHT - 1) * AREA_WIDTH + i] = "═";
    }
    buffer[0] = "╔";
    buffer[AREA_WIDTH - 1] = "╗";
    buffer[(AREA_HEIGHT - 1) * AREA_WIDTH] = "╚";
    buffer[AREA_HEIGHT * AREA_WIDTH - 1] = "╝";
}

void appendGameStats(string &buffer) {
    buffer += "FPS: " + to_string(FPS) + "\n";
    buffer += "Pálya: (x: " + to_string(AREA_WIDTH) +
              " y: " + to_string(AREA_HEIGHT) + ")\n";
    buffer += "Labda pozíció: (x: " + ilovecpp(ball.pos.x) +
              " y: " + ilovecpp(AREA_HEIGHT - ball.pos.y) + ")\n";
    buffer += "Sebesség: (dx: " + ilovecpp(ball.dir.x * SECOND) +
              " dy: " + to_string(ball.dir.y * SECOND) + ")\n";
    buffer += "Gravitáció: " + ilovecpp(GRAVITY) + " block/s^2\n";
    buffer += "Siklás: " + ilovecpp(FRICTION * 100) + "%\n";
    buffer += "Rugalmasság: " + ilovecpp(BOUNCINESS * 100) + "%\n";
}

void prepareScreen() {
    for (size_t i = 0; i < AREA_WIDTH * AREA_HEIGHT; i++)
        screenBuffer[i] = " ";

    drawBorder(screenBuffer);
}

void updateScreen() {
    screenBuffer[(int)round(prev_pos.y) * AREA_WIDTH + (int)round(prev_pos.x)] =
        (char *)TRAIL_TEXTURE;
    screenBuffer[(int)round(ball.pos.y) * AREA_WIDTH + (int)round(ball.pos.x)] =
        (char *)BALL_TEXTURE;
    string raw;
    for (size_t i = 0; i < AREA_HEIGHT; i++) {
        for (size_t j = 0; j < AREA_WIDTH; j++)
            raw += screenBuffer[i * AREA_WIDTH + j];
        raw += '\n';
    }
    appendGameStats(raw);
    clearScreen();
    cout << raw;
}

#pragma endregion

#pragma region GameLogic
void prepare() {
    frame_length = (double)SECOND / (double)FPS;
    gravity_per_um = GRAVITY / ((double)SECOND * (double)SECOND);
    ball = Ball(Vec2(INITIAL_X, AREA_HEIGHT - INITIAL_Y),
                Vec2((double)INITIAL_DX / (double)SECOND,
                     (double)-INITIAL_DY / (double)SECOND));
}

void gameLogic() {
    prev_pos = ball.pos;
    ball.pos.y +=
        ball.dir.y * frame_length +
        0.5 * gravity_per_um * frame_length * frame_length; // s = v0t + 1/2at^2
    ball.pos.x += ball.dir.x * frame_length;
    if (ball.pos.x < 1 || ball.pos.x > AREA_WIDTH - 2) {
        ball.dir.x = -ball.dir.x * BOUNCINESS;
        // snapback
        double offset = ball.pos.x < 1 ? abs(ball.pos.x - 1)
                                       : abs(ball.pos.x - (AREA_WIDTH - 2));
        ball.pos.x = ball.pos.x <= 1 ? 1 + offset : AREA_WIDTH - 2 - offset;
        ball.dir.y *= FRICTION;
    }
    if (ball.pos.y < 1 || ball.pos.y > AREA_HEIGHT - 2) {
        ball.dir.y = -ball.dir.y * BOUNCINESS;
        // snapback
        double offset = ball.pos.y < 1 ? abs(ball.pos.y - 1)
                                       : abs(ball.pos.y - (AREA_HEIGHT - 2));
        ball.pos.y = ball.pos.y <= 1 ? 1 + offset : AREA_HEIGHT - 2 - offset;
        ball.dir.x *= FRICTION;
    }
    ball.dir.y += gravity_per_um * frame_length;
}
#pragma endregion

int main() {
    prepare();
    prepareScreen();
    while (true) {
        gameLogic();
        updateScreen();
        usleep(frame_length);
    }
    return 0;
}