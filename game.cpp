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
    Vec2 pos,size,vel;
    float radius;
    Texture texture;
};
struct Pipe {
    Vec2 pos,vel;
    Vec2 size;
};

struct PipePair {
    Pipe top;
    Pipe bottom;
    bool passed;
};

Bird bird;
vector<PipePair> pipes;

float gravity;
float jumpForce;
float pipeSpeed;
int score;
bool gameOver;
// provides a random number between the two numbers provided
int random(int min, int max){
    int num = (rand()%(max-min+1))+min;
    return num;
}

// Resets the position and size of the pipes
void resetPipe(Pipe &top, Pipe &bottom, float x)
{
    int gap = 180;
    int topHeight = random(50, WINDOW_HEIGHT - gap - 50);
    
    top.pos = Vec2(x, 0);
    top.size = Vec2(50, topHeight);

    bottom.pos = Vec2(x, topHeight + gap);
    bottom.size = Vec2(50, WINDOW_HEIGHT - topHeight - gap);

    top.vel = Vec2(0,0);
    bottom.vel = Vec2(0,0);
}


bool isCollision(Vec2 Pos1, Vec2 Size1, Vec2 Pos2, Vec2 Size2)
{
    return (Pos1.x < Pos2.x + Size2.x && Pos1.x + Size1.x > Pos2.x &&
            Pos1.y < Pos2.y + Size2.y && Pos1.y + Size1.y > Pos2.y);
}

void init() {
    pipes.clear();

    bird.pos = Vec2(100, WINDOW_HEIGHT/2);
    bird.vel = Vec2(0, 0);
    bird.radius = 15;
    bird.size = Vec2(bird.radius * 2, bird.radius * 2);

    gravity = 400.0f;
    jumpForce = -250.0f;
    pipeSpeed = 200.0f;
    score = 0;
    gameOver = false;
    pipes.push_back(PipePair());
    pipes.push_back(PipePair());
    for (int i = 0; i < 5; i++) {
        pipes.push_back(PipePair());
        resetPipe(pipes[i].top, pipes[i].bottom, WINDOW_WIDTH + i * 300);
    }

}

// Update Game
void update(float dt) {
    if (gameOver) {
        if (keyIsPressed(KEY_R)) {
            init();
        }
        return;
    }


    if (keyIsPressed(KEY_SPACE)) {
        bird.vel.y = jumpForce;
    }
// apply gravity to the bird and move it down
    bird.vel.y = bird.vel.y + gravity * dt;
    bird.pos.y = bird.pos.y + bird.vel.y * dt;

    Vec2 forward = Vec2(-1,0);
// move the pipes towards left
    for (auto &pipePair : pipes) {
        pipePair.top.pos = pipePair.top.pos + forward * pipeSpeed * dt;
        pipePair.bottom.pos = pipePair.bottom.pos + forward * pipeSpeed * dt;
    }
// reset the pipes when they go off screen
    for (auto &pipePair : pipes) {
    if (pipePair.top.pos.x + pipePair.top.size.x <= 0)
    {
        float maxX = 0;
// find the maximum x position of the pipes to place the new pipe after the last one
        for (auto &p : pipes) {
            if (p.top.pos.x > maxX) {
                maxX = p.top.pos.x;
            }
        }

        resetPipe(pipePair.top, pipePair.bottom, maxX + 300);
        pipePair.passed = false;
    }
}

// check if the bird has passed the pipes and update the score    
    for (auto &pipePair : pipes) {
        if((pipePair.top.pos.x + pipePair.top.size.x < bird.pos.x) && !pipePair.passed){
            score++;
            pipePair.passed = true;
        }
    }

    Vec2 birdHitboxPos = Vec2(
    bird.pos.x - bird.radius,
    bird.pos.y - bird.radius
    );

    for (auto &pipePair : pipes) {
    if(isCollision(birdHitboxPos, bird.size, pipePair.top.pos, pipePair.top.size) ||
        isCollision(birdHitboxPos, bird.size, pipePair.bottom.pos, pipePair.bottom.size))
        {gameOver = true;}  

    }

    if (bird.pos.y - bird.radius < 0 ||
    bird.pos.y + bird.radius > WINDOW_HEIGHT){
        gameOver = true;}


}
// Render Game
void render(float lag) {
    // Clear Screen
    clear(0, 0, 0);

    drawCircle(bird.pos,bird.radius,Color::red);
    for (const auto &pipePair : pipes) {
        drawRect(pipePair.bottom.pos, pipePair.bottom.size, Color::green, 0);
        drawRect(pipePair.top.pos, pipePair.top.size, Color::green, 0);
    }

    drawText(20, 20, (char*)to_string(score).c_str(), 255, 255, 255, 255);


    if (gameOver) {
        drawText(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 20,
                 (char*)"GAME OVER", 255, 0, 0, 255);

        drawText(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 + 20,
                 (char*)"Press R to Restart", 255, 255, 255, 255);
    }
}

// Close the Game
void close() {

}