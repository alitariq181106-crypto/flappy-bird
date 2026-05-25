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
Pipe pipe1;
Pipe pipe2;
Pipe pipe2_top;
vector<Pipe> pipes;

float gravity;
float jumpForce;
float pipeSpeed;
int score;
// provides a random number between the two numbers provided
int random(int num1, int num2){
    int num = (rand()%num2)+num1;
    return num;
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

    pipe1.pos = Vec2 (WINDOW_WIDTH/1.3,0);
    pipe1.vel = Vec2(0,0);
    pipe1_top.pos = Vec2(WINDOW_WIDTH/1.3,WINDOW_HEIGHT);
    pipe2.pos = Vec2 (WINDOW_WIDTH/0.35,0);
    pipe2_top.pos = Vec2(WINDOW_WIDTH/0.35,WINDOW_HEIGHT);
    pipe1.size= Vec2 (50,100);
    pipe2.size= Vec2 (50,500);
    pipe2_top.size= Vec2 (50,800);
    pipe1_top.size= Vec2 (50,600);



}

// Update Game
void update(float dt) {
    //if (isKeyPressed(KEY_SPACE)) {
        //bird.vel.y = jumpForce;
    //}
    Vec2 forward = Vec2(-1,0);
    pipe1.vel = Vec2(forward*pipeSpeed);
    pipe1_top.pos += pipe1.vel *dt;
    pipe2.pos +=pipe1.vel *dt;
    pipe2_top.pos += pipe1.vel *dt;
    pipe1.pos += pipe1.vel *dt;

    //bird.vel.y = bird.vel.y + gravity * dt;
    //bird.pos.y = bird.pos.y + bird.vel.y * dt;
    if ((pipe1.pos.x <=0)&&(pipe1_top.pos.x <=0)){
        pipe1.size = Vec2(50,random(0,WINDOW_HEIGHT));
        pipe1_top.size = Vec2(50,random(pipe1.size.y+50,WINDOW_HEIGHT));// this fifty can alo be a random number that will be the distance between two pillars
        pipe1_top.pos = Vec2(WINDOW_WIDTH,WINDOW_HEIGHT);
        pipe1.pos = Vec2 (WINDOW_WIDTH,0);
    }
    if ((pipe2.pos.x <=0)&&(pipe2_top.pos.x <=0)){
        pipe2.size = Vec2(50,random(0,WINDOW_HEIGHT));
        pipe2_top.size = Vec2(50,random(pipe2.size.y+50,WINDOW_HEIGHT));// this fifty can alo be a random number that will be the distance between two pillars
        pipe1_top.pos = Vec2(WINDOW_WIDTH,WINDOW_HEIGHT);
        pipe2.pos = Vec2 (WINDOW_WIDTH,0);
    }
}

// Render Game
void render(float lag) {
    // Clear Screen
    clear(0, 0, 0);

    drawCircle(bird.pos,bird.radius,Color::red);
    drawRect(pipe1.pos,pipe1.size,Color::green,0);
    drawRect(pipe1_top.pos,pipe1_top.size,Color::green,0);
    drawRect(pipe2.pos,pipe2.size,Color::green,0);
    drawRect(pipe2_top.pos,pipe2_top.size,Color::green,0);

}

// Close the Game
void close() {

}