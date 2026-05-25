#include <engine.h>
#include <iostream>
#include <vector>

using namespace std;

// Initialise (called once at start)
// create a circle for the bird
// create 2 pillars with a space between them make it so they have random heights
// create 2 more pillars on top of the first pillars use random numbers to start from height on top of the first two pillars make this height between top and buttom pillar random
// make the pillars move towards left 
// allow the ball to move uo when press sapce button

struct Bird {
    Vec2 pos,size;
    Vec2 vel;
    float radius;
    Texture texture;
};

struct Pipe {
    Vec2 pos;
    Vec2 size;
    bool passed;
};

Bird bird;
vector<Pipe> pipes;

float gravity;
float jumpForce;
float pipeSpeed;
int score;


void init() {
    pipes.clear();

    bird.pos = Vec2(100, WINDOW_HEIGHT/2);
    bird.vel = Vec2(0, 0);
    bird.radius = 15;
    bird.size = Vec2(10,10);

    gravity = 400.0f;
    jumpForce = -250.0f;
    pipeSpeed = 200.0f;
    score = 0;


}

// Update Game
void update(float dt) {
    //if (isKeyPressed(KEY_SPACE)) {
        bird.vel.y = jumpForce;
    //}

    //bird.vel.y = bird.vel.y + gravity * dt;
    //bird.pos.y = bird.pos.y + bird.vel.y * dt;

}

// Render Game
void render(float lag) {
    // Clear Screen
    clear(0, 0, 0);

    drawCircle(bird.pos,bird.radius,Color::red);

}

// Close the Game
void close() {

}