#include "raylib.h"
#include <stdlib.h>

#define MAX_DROPS 5

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define SPRITE_SIZE 64

typedef struct Drop {
    Vector2 position;
    Vector2 speed;
} Drop;


int spawnDrop(struct Drop *dropArray, int dropCount) {
    if (dropCount < MAX_DROPS) {
        dropArray[dropCount].position.x = rand() % (SCREEN_WIDTH - SPRITE_SIZE);
        dropArray[dropCount].position.y = -10;
        dropArray[dropCount].speed.y = 250;
        dropArray[dropCount].speed.x = 0;
        return dropCount + 1;
    }
    return dropCount;
}

int collision(struct Drop a, struct Drop b) {
    if (a.position.x < b.position.x + SPRITE_SIZE &&
        a.position.x + SPRITE_SIZE > b.position.x &&
        a.position.y < b.position.y + SPRITE_SIZE &&
        a.position.y + SPRITE_SIZE > b.position.y)
    {
        return 1;
    }
    return 0;
}

int main(void)
{
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;

    int score = 0;

    // init random
    time_t t;
    srand((unsigned)time(&t));

    InitWindow(screenWidth, screenHeight, "raylib rain game");

    // Load bucket texture.
    Image bucketImage = LoadImage("sprites/bucket.png");
    Texture2D bucketTexture = LoadTextureFromImage(bucketImage);
    UnloadImage(bucketImage);

    // load drop texture
    Image dropImage = LoadImage("sprites/drop.png");
    Texture2D dropTexture = LoadTextureFromImage(dropImage);
    UnloadImage(dropImage);

    Drop* drops = (Drop*)malloc(MAX_DROPS * sizeof(Drop));
    int dropsCount = 0;

    float timer = 0;

    SetTargetFPS(60);

    struct Drop tempDrop;

    struct Drop player;

    player.position.x = SCREEN_WIDTH / 2 - (bucketTexture.width / 2);
    player.position.y = 350 - (bucketTexture.height / 2);
    player.speed.y = 0;
    player.speed.x = 0;

    float delta;
    // Main game loop
    while (!WindowShouldClose()) 
    {
        delta = GetFrameTime();
        if (timer > 1) {
            dropsCount = spawnDrop(drops, dropsCount);
            timer = 0;
        }
        if (IsKeyDown(KEY_RIGHT)) player.speed.x = 400;
        if (IsKeyDown(KEY_LEFT)) player.speed.x = -400;
	if ((IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_LEFT)) || (!IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT))) player.speed.x = 0;
        player.position.x += player.speed.x * delta;

        if (player.position.x > (SCREEN_WIDTH - (bucketTexture.width))) player.position.x = (SCREEN_WIDTH - (bucketTexture.width));
        if (player.position.x < 0) player.position.x = 0;

        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            DrawText(TextFormat("%i", score), 48, 48, 48, RED);

            DrawTexture(bucketTexture, player.position.x, player.position.y, WHITE);
            
            for (int i = 0; i < dropsCount; i++) {
                drops[i].position.y += (drops[i].speed.y * delta);
                drops[i].position.x += (drops[i].speed.x * delta);
                if (drops[i].position.y > 400) {
                    // I can't believe this works
                    tempDrop = drops[i];
                    drops[i] = drops[dropsCount - 1];
                    drops[dropsCount - 1] = tempDrop;
                    dropsCount--;
                }
                if (collision(player, drops[i])) {
                    tempDrop = drops[i];
                    drops[i] = drops[dropsCount - 1];
                    drops[dropsCount - 1] = tempDrop;
                    dropsCount--;
                    score++;
                }
                DrawTexture(dropTexture, (int)drops[i].position.x, (int)drops[i].position.y, WHITE);   
            }
        EndDrawing();
        timer += delta;
    }
    
    UnloadTexture(bucketTexture);

    UnloadTexture(dropTexture);

    free(drops);

    CloseWindow();

    return 0;
}


