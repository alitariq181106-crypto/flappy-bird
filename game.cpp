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
    Texture texture;
};

struct PipePair {
    Pipe top;
    Pipe bottom;
    bool passed;
};

struct Heart {
    Vec2 pos;
    Vec2 size;
    bool active;
    Texture texture;
};

Bird bird;
vector<PipePair> pipes;
Heart heart;

int lives;
float gravity;
float jumpForce;
float pipeSpeed;
int score;
bool gameOver;

Texture birdTexture;
Texture pipeTopTexture;
Texture pipeBottomTexture;
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

    top.texture = pipeTopTexture;
    bottom.texture = pipeBottomTexture;
}


bool isCollision(Vec2 Pos1, Vec2 Size1, Vec2 Pos2, Vec2 Size2)
{
    return (Pos1.x < Pos2.x + Size2.x && Pos1.x + Size1.x > Pos2.x &&
            Pos1.y < Pos2.y + Size2.y && Pos1.y + Size1.y > Pos2.y);
}

void respawnBirdSafe(Bird &bird, const vector<PipePair> &pipes)
{
    bool safe = false;

    while (!safe) {
        safe = true;

        bird.pos = Vec2(100, random(100, WINDOW_HEIGHT - 100));
        bird.vel = Vec2(0, 0);

        Vec2 hitbox = Vec2(
            bird.pos.x - bird.radius,
            bird.pos.y - bird.radius
        );

        for (const auto &pipePair : pipes) {
            if (isCollision(hitbox, bird.size,
                            pipePair.top.pos, pipePair.top.size) ||
                isCollision(hitbox, bird.size,
                            pipePair.bottom.pos, pipePair.bottom.size)) {
                safe = false;
                break;
            }
        }
    }
}

void init() {
    pipes.clear();

    bird.pos = Vec2(100, WINDOW_HEIGHT/2);
    bird.vel = Vec2(0, 0);
    bird.radius = 15;
    bird.size = Vec2(bird.radius * 2, bird.radius * 2);

    heart.pos = Vec2(600, 200);
    heart.size = Vec2(30, 30);
    heart.active = true;

    gravity = 400.0f;
    jumpForce = -250.0f;
    pipeSpeed = 200.0f;
    score = 0;
    gameOver = false;
    lives = 3;
    bird.texture= loadTexture("./assets/images/bird.png", Vec2(10,10), Vec2(680,565));
    pipeTopTexture = loadTexture("./assets/images/pipes.png", Vec2(350,0), Vec2(325,600));
    pipeBottomTexture = loadTexture("./assets/images/pipes.png", Vec2(350,950), Vec2(325,1350));
    
    //pipeTopTexture = loadTexture("./assets/images/pipes.png", Vec2(325,470), Vec2(350,130));
    //pipeBottomTexture = loadTexture("./assets/images/pipes.png", Vec2(325,950), Vec2(350,13));

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
// move the heart towards left
    heart.pos = heart.pos + Vec2(-pipeSpeed * dt, 0);

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
        {lives--;
            if (lives <= 0) {
                gameOver = true;
                }else {respawnBirdSafe(bird, pipes);}
        }  

    }

    if (bird.pos.y - bird.radius < 0 ||
    bird.pos.y + bird.radius > WINDOW_HEIGHT){
        lives--;
        if (lives <= 0) {
            gameOver = true;
        }else {respawnBirdSafe(bird, pipes);}
}
// check if the bird collides with the heart and update lives
    if (heart.active && isCollision(birdHitboxPos, bird.size, heart.pos, heart.size)) {
        lives++;
        heart.active = false;
    }

    if(heart.pos.x + heart.size.x < 0){
        heart.active = true;
        bool isValidPosition = false;
// make sure the heart does not spawn on the pipes
        while (!isValidPosition) {
            isValidPosition = true;
            heart.pos = Vec2(WINDOW_WIDTH + random(200, 600), random(100, WINDOW_HEIGHT - (int)heart.size.y - 100));
// check for collision with pipes
            Vec2 testHeartPos = Vec2(heart.pos.x - 5, heart.pos.y - 5);
            Vec2 testHeartSize = Vec2(heart.size.x + 10, heart.size.y + 10);
            for (const auto &pipePair : pipes) {
                if (isCollision(testHeartPos, testHeartSize, pipePair.top.pos, pipePair.top.size) ||
                    isCollision(testHeartPos, testHeartSize, pipePair.bottom.pos, pipePair.bottom.size)) {
                    isValidPosition = false;
                    break;
                }
            }
        }
    }


}
// Render Game
void render(float lag) {
    // Clear Screen
    clear(0, 0, 0);

    drawTexture(bird.texture,bird.pos-bird.size/2,bird.size);
    drawCircle(bird.pos,bird.radius,Color::red);
 for (const auto &pipePair : pipes) {drawTexture(pipeTopTexture,pipePair.top.pos,pipePair.top.size);
    drawTexture(pipeBottomTexture,pipePair.bottom.pos,pipePair.bottom.size);
    drawRect(pipePair.bottom.pos, pipePair.bottom.size, Color::green, 0); 
    drawRect(pipePair.top.pos, pipePair.top.size, Color::green, 0); }

    
    if (heart.active) {
        drawRect(heart.pos, heart.size, Color::red, 0);
    }

    drawText(20, 20, (char*)("Score: " + to_string(score)).c_str(), 255,255,255,255);
    drawText(20, 60, (char*)("Lives: " + to_string(lives)).c_str(), 255,255,255,255);


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