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
    vector <Texture> frames;
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
vector <Texture>frames1;
Heart heart;
int lives,score;
float gravity,jumpForce,pipeSpeed,some,backgroundX,tutorialStart,start;
bool gameOver,last1,showTutorial,animation;
Texture bird1,over,pipeTopTexture,pipeBottomTexture,background2,restart,heart1,texture1;
AudioClip background, jump, death;
static bool spacePressedLastFrame;
// provides a random number between the two numbers provided
int random(int min, int max){
    int num = (rand()%(max-min+1))+min;
    return num;
}

// Resets the position and size of the pipes
void resetPipe(Pipe &top, Pipe &bottom, float x, int gap)
{
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
    animation =true;
    bird.pos = Vec2(100, WINDOW_HEIGHT/2);
    bird.vel = Vec2(0, 0);
    bird.radius = 20;
    bird.size = Vec2(bird.radius * 2, bird.radius * 2);

    heart.pos = Vec2(600, 200);
    heart.size = Vec2(30, 30);
    heart.active = true;

    gravity = 400.0f;
    jumpForce = -290.0f;
    pipeSpeed = 200.0f;
    score = 0;
    gameOver = false;
    lives = 3;
    last1 =true;
    tutorialStart = getTimeInSeconds();
    showTutorial = true;

    pipeTopTexture = loadTexture("./assets/images/pipes.png", Vec2(350,0), Vec2(325,600));
    pipeBottomTexture = loadTexture("./assets/images/pipes.png", Vec2(350,950), Vec2(325,1350));
    background2 = loadTexture("./assets/images/background.png", Vec2(0,0), Vec2(1020,1530));
    restart = loadTexture("./assets/images/restart.png", Vec2(355,230), Vec2(950,400));
    heart1 = loadTexture("./assets/images/heart.png", Vec2(155,410), Vec2(710,600));
    texture1= loadTexture("./assets/images/bird1.png");
    over= loadTexture("./assets/images/over.png");
    for (float iy =0;iy<2;iy++){
        for (float ix = 0;ix <4;ix++){
            bird1 = subTexture(texture1,Rect{ix *512,iy*417.5f,512,417.5f});
            bird.frames.push_back(bird1);
            frames1.push_back(subTexture(over,ix *512,iy*417.5f,512,417.5f));
        }
    }
    start = getTimeInSeconds();
    spacePressedLastFrame = false;
    background = loadAudioClip("./assets/audio/background.mp3");
    jump = loadAudioClip("./assets/audio/pop.mp3");
    death = loadAudioClip ("./assets/audio/game-over.mp3");
    playAudio(background,1.0f,true);
    for (int i = 0; i < 5; i++) {
        pipes.push_back(PipePair());
        resetPipe(pipes[i].top, pipes[i].bottom, WINDOW_WIDTH + i * 300, 180);
    }
}

// Update Game

void update(float dt) {
    if(showTutorial && getTimeInSeconds() - tutorialStart >2.0f){
    showTutorial = false;
    }

    if (gameOver) {
        if (getTimeInSeconds() - some > 0.8f){last1 = false;} 
        if (keyIsPressed(KEY_R)) {
            bird.frames.clear();
            animation = true;
            last1 = true;
            frames1.clear();
            init();
        }
        return;
    }
backgroundX -= 100 * dt;
// increase pipe speed every 5 points

    pipeSpeed = 200 + (score /5) * 7;

int gap = std::max(150, 180 - score * 2);

if (backgroundX <= -WINDOW_WIDTH){
    backgroundX = 0;}

    if (keyIsPressed(KEY_SPACE) && !spacePressedLastFrame) {
        bird.vel.y = jumpForce;
        playAudio(jump,1.0f);
    }

    spacePressedLastFrame = keyIsPressed(KEY_SPACE);
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

        resetPipe(pipePair.top, pipePair.bottom, maxX + 300, gap);
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
                some = getTimeInSeconds();
                stopAudio(background);
                playAudio(death,1.0f);
                gameOver = true;
                animation  = false;
                }else {
                respawnBirdSafe(bird, pipes);
                }
        }  

    }

    if (bird.pos.y - bird.radius < 0 ||
    bird.pos.y + bird.radius > WINDOW_HEIGHT){
        lives--;
        if (lives <= 0) {
            gameOver = true;
        }else {
            respawnBirdSafe(bird, pipes);
            }}
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
// creating the background 
drawTexture(background2, Vec2(backgroundX,0), Vec2(WINDOW_WIDTH,WINDOW_HEIGHT));
drawTexture(background2,
            Vec2(backgroundX + WINDOW_WIDTH,0),
            Vec2(WINDOW_WIDTH,WINDOW_HEIGHT));

// draw the pipes
 for (const auto &pipePair : pipes) {drawTexture(pipeTopTexture,pipePair.top.pos,pipePair.top.size);
    drawTexture(pipeBottomTexture,pipePair.bottom.pos,pipePair.bottom.size);}

// drawing the bird by frame by frame
if (last1){
if (animation){
    if (heart.active) {
        drawTexture(heart1,heart.pos,heart.size);}
    float current = getTimeInSeconds();
    float elapsed = current -start;
    int frameindex =(int)((elapsed/0.8f)*8);
    frameindex %= 8;
    drawTexture(bird.frames[frameindex],bird.pos-bird.size/2,bird.size);}else{
    float current = getTimeInSeconds();
    float elapsed = current -start;
    int frameinde =(int)((elapsed/0.8f)*8);
    frameinde %= 8;
    drawTexture(frames1[frameinde],bird.pos,bird.size*2);}}
        if(last1 == false){
    drawTexture(frames1[7],bird.pos,bird.size*2);}
    

    if(showTutorial){
    drawText(150, 200,
        (char*)"Press SPACE to jump",
        255, 255, 255, 255);
    }
    // render the score and lives
    drawText(20, 20, (char*)("Score: " + to_string(score)).c_str(), 255,255,255,255);
    drawText(20, 60, (char*)("Lives: " + to_string(lives)).c_str(), 255,255,255,255);

    if (gameOver){
    drawTexture(restart,Vec2(WINDOW_WIDTH/2 -125,WINDOW_HEIGHT/2 -125),Vec2(250,250));
}
}

// Close the Game
void close() {
stopAudio(background);
}