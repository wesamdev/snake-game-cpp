#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <iostream>

using namespace std;

Color green = {173,204,96,255};
Color darkGreen = {43,51,24,255};

int cellSize = 30;
int cellCount = 25;
int offset = 60;
double speed = 1;
double lastUpdateTime = 0.0;

bool elementInDeque(Vector2 element, deque<Vector2> deque){
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if(Vector2Equals(element, deque[i])){
            return true;
        }
    }
    return false;
}


bool eventTriggred(double interval){
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime > interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}


class Snake{
    public :
        deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        Vector2 direction =  {1,0};
        bool addSegment = false;
    void Draw(){
        for (unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segmant = Rectangle{offset+ x * cellSize, offset+ y * cellSize,   (float)cellSize,(float)cellSize};
            DrawRectangleRounded(segmant,0.5 , 6, darkGreen);
        }
    }
    void Update(){
        body.push_front(Vector2Add(body[0], direction));

        if (addSegment){
            //body.push_back(body.back());
            addSegment = false;
        }
        else{
            body.pop_back();
        }

    }

    void Reset(){
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction =  {1,0};
    }
};
class Food
{
public:
    Vector2 postion;
    Texture2D texture;

    Food(deque<Vector2> SnakeBody){
        Image image = LoadImage("assets/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        postion = GenerateRandomPos(SnakeBody);
    }
    ~Food(){
        UnloadTexture(texture);
    }
    void Draw(){
        // DrawRectangle(postion.x * cellSize, postion.y * cellSize, cellSize, cellSize, darkGreen);
        DrawTexture(texture, offset+ postion.x * cellSize, offset+ postion.y * cellSize, WHITE);
    }

    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2 {x,y};
    }

    /**
     * Generates a random position within the range specified by cellCount.
     *
     * @return Vector2 the randomly generated position
     */
    Vector2 GenerateRandomPos(deque<Vector2> SnakeBody){
        Vector2 postion = GenerateRandomCell();
        while(elementInDeque(postion, SnakeBody)){
            postion = GenerateRandomCell();
        }
        return postion;
    }
};

class Game
{
public:
    Food food = Food(snake.body);
    Snake snake = Snake();
    bool running = true;
    int score = 0;
    void Draw(){
        snake.Draw();
        food.Draw();
    }
    void Update(){
        if (running) {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            // CheckCollisionWithSelf();
        }
    }
    void CheckCollisionWithFood(){
        if(Vector2Equals(snake.body[0], food.postion)){
            food.postion = food.GenerateRandomPos(snake.body);
            cout << "Food Eaten!"<< endl;
            snake.addSegment = true;
            score++;
            cout << "Score: " << score << endl;
        }
    }
    void CheckCollisionWithEdges(){
        if(snake.body[0].x == -1 || snake.body[0].x == cellCount || snake.body[0].y == -1 || snake.body[0].y == cellCount){
            GameOver();
        }
    }

    void CheckCollisionWithSelf(){
        deque<Vector2> headless = snake.body;
        headless.pop_front();
        if (elementInDeque(snake.body[0], headless)){
            GameOver();
        }
    }
    
    void GameOver(){
        cout << "Game Over!" << endl;
        running = false;
        snake.Reset();
        food.postion = food.GenerateRandomPos(snake.body);
        score = 0;
    }
};

int main()
{
    InitWindow(2*offset + cellSize * cellCount, 2*offset +cellSize * cellCount, "Snake game by wesamdev | v1");
    SetTargetFPS(60);
    Game game = Game();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(green);
        if (eventTriggred(speed)){
            if(IsKeyDown(KEY_UP) && game.snake.direction.y != 1){
                game.snake.direction = {0,-1};
                game.running = true;
            }
            if(IsKeyDown(KEY_DOWN) && game.snake.direction.y != -1){
                game.snake.direction = {0,1};
                game.running = true;

            }
            if(IsKeyDown(KEY_LEFT) && game.snake.direction.x != 1){
                game.snake.direction = {-1,0};
                game.running = true;

            }
            if(IsKeyDown(KEY_RIGHT) && game.snake.direction.x != -1){
                game.snake.direction = {1,0};
                game.running = true;
            }
            game.Update();
        }
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellCount * cellSize +10, (float)cellCount * cellSize +10},6, darkGreen);
        DrawText("Snake Game", offset-5, 14, 34, darkGreen);
        DrawText("By wesamdev", offset-5, 36, 20, darkGreen);
        DrawText(TextFormat("%i", game.score), offset-5, offset+cellSize * cellCount + 10, 40, darkGreen);
        game.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

