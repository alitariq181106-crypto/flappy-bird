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
    bool passed;
};

Bird bird;
Pipe pipe1_top;
Pipe pipe1_bottom;
Pipe pipe2_top;
Pipe pipe2_bottom;
vector<Pipe> pipes;

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
    bird.size = Vec2(10,10);

    gravity = 400.0f;
    jumpForce = -250.0f;
    pipeSpeed = 200.0f;
    score = 0;
    gameOver = false;
    resetPipe(pipe1_top, pipe1_bottom, WINDOW_WIDTH);
    resetPipe(pipe2_top, pipe2_bottom, WINDOW_WIDTH + 300);

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
    pipe1_top.pos = pipe1_top.pos + forward * pipeSpeed * dt;
    pipe1_bottom.pos = pipe1_bottom.pos + forward * pipeSpeed * dt;
    pipe2_top.pos = pipe2_top.pos + forward * pipeSpeed * dt;
    pipe2_bottom.pos = pipe2_bottom.pos + forward * pipeSpeed * dt;
// reset the pipes when they go off screen
    if((pipe1_bottom.pos.x <=0)&&(pipe1_top.pos.x <=0)){
        resetPipe(pipe1_top, pipe1_bottom, WINDOW_WIDTH);
    }
    if((pipe2_bottom.pos.x <=0)&&(pipe2_top.pos.x <=0)){
        resetPipe(pipe2_top, pipe2_bottom, WINDOW_WIDTH + 300);
    }


    if(isCollision(bird.pos, bird.size, pipe1_top.pos, pipe1_top.size) ||
    isCollision(bird.pos, bird.size, pipe1_bottom.pos, pipe1_bottom.size) ||
    isCollision(bird.pos, bird.size, pipe2_top.pos, pipe2_top.size) ||
    isCollision(bird.pos, bird.size, pipe2_bottom.pos, pipe2_bottom.size))
    {gameOver = true;}   

    if (bird.pos.y < 0 || bird.pos.y + bird.size.y > WINDOW_HEIGHT){
        gameOver = true;
    }


}
// Render Game
void render(float lag) {
    // Clear Screen
    clear(0, 0, 0);

    drawCircle(bird.pos,bird.radius,Color::red);
    drawRect(pipe1_bottom.pos,pipe1_bottom.size,Color::green,0);
    drawRect(pipe1_top.pos,pipe1_top.size,Color::green,0);
    drawRect(pipe2_bottom.pos,pipe2_bottom.size,Color::green,0);
    drawRect(pipe2_top.pos,pipe2_top.size,Color::green,0);


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