#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <sstream> // Include for string stream

using namespace sf;
using namespace std;

const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point
{
    int x, y;
} a[4], b[4], nextBlock[4]; // Renamed next to nextBlock

int figures[7][4] =
{
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S
    3, 5, 4, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5, // O
};

int nextColorNum;

bool check()
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return false;
        else if (field[a[i].y][a[i].x])
            return false;
    }
    return true;
}

bool checkGameOver()
{
    for (int i = 0; i < 4; i++)
    {
        if (field[a[i].y][a[i].x] != 0)
            return true;
    }
    return false;
}

void spawnNewBlock(int &colorNum)
{
    for (int i = 0; i < 4; i++)
    {
        a[i] = nextBlock[i]; // Using nextBlock
    }
    colorNum = nextColorNum;

    int n = rand() % 7;
    for (int i = 0; i < 4; i++)
    {
        nextBlock[i].x = figures[n][i] % 2; // Using nextBlock
        nextBlock[i].y = figures[n][i] / 2; // Using nextBlock
    }
    nextColorNum = 1 + rand() % 7;
}

enum GameState
{
    START,
    MENU,
    GAME,
    PAUSE,
    GAME_OVER,
    DIFFERENT
};

string intToString(int num)
{
    std::ostringstream ss;
    ss << num;
    return ss.str();
}

int main()
{
    srand(time(0));
    float oper = 0.3;
    RenderWindow window(VideoMode(320, 480), "The Game!");

    Texture t1, t2, tStart, tMenu, tGameOver, tDifferent, level2, level3, level4, level5,tbreak;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/未命名2.png");
    level2.loadFromFile("images/最終2.png");
    level3.loadFromFile("images/最終3.png");
    level4.loadFromFile("images/最終4.png");
    level5.loadFromFile("images/最終5.png");
    tStart.loadFromFile("images/newstart.png");
    tMenu.loadFromFile("images/menu.png");
    tGameOver.loadFromFile("images/未命名.png");
    tDifferent.loadFromFile("images/different.png");
    tbreak.loadFromFile("images/暫停.png");
    

    // Load font for score display
    Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        return -1;
    }

    // Create score text
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(250, 430);

    int score = 0;

    // Add background music
    Music music;
    if (!music.openFromFile("background.ogg"))
    {
        return -1;
    }
    music.setLoop(true);
    music.play();

    Music gameOverMusic;
    if (!gameOverMusic.openFromFile("Gameover.ogg"))
    {
        return -1;
    }

    Sprite s(t1),ttbreak(tbreak), background(t2), background2(level2), background3(level3), background4(level4), background5(level5), startScreen(tStart), menuScreen(tMenu), gameOverScreen(tGameOver), differentScreen(tDifferent);

    int dx = 0;
    bool rotate = false;
    int colorNum = 1;
    float timer = 0, delay = 0.3;

    Clock clock;

    GameState state = START;

    int n = rand() % 7;
    for (int i = 0; i < 4; i++)
    {
        nextBlock[i].x = figures[n][i] % 2; // Using nextBlock
        nextBlock[i].y = figures[n][i] / 2; // Using nextBlock
    }
    nextColorNum = 1 + rand() % 7;

    spawnNewBlock(colorNum);

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        if (state == GAME)
            timer += time * oper;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::O)
                {
                    if (state == START)
                    {
                        state = DIFFERENT;
                    }
                    else if (state == MENU)
                    {
                        state = GAME;
                    }
                    else if (state == GAME_OVER)
                    {
                        window.close();
                    }
                }

                if (e.key.code == Keyboard::H)
                {
                    if (state == GAME)
                    {
                        state = PAUSE;
                    }
                    else if (state == PAUSE)
                    {
                        state = GAME;
                    }
                }

                if (state == GAME)
                {
                    if (e.key.code == Keyboard::Up)
                        rotate = true;
                    else if (e.key.code == Keyboard::Left)
                        dx = -1;
                    else if (e.key.code == Keyboard::Right)
                        dx = 1;
                }

                if (state == DIFFERENT)
                {
                    if (e.key.code == Keyboard::Z)
                    {
                        oper = 0.3;
                        state = MENU;
                    }
                    else if (e.key.code == Keyboard::X)
                    {
                        oper = 0.4;
                        state = MENU;
                    }
                    else if (e.key.code == Keyboard::C)
                    {
                        oper = 0.5;
                        state = MENU;
                    }
                    else if (e.key.code == Keyboard::V)
                    {
                        oper = 0.6;
                        state = MENU;
                    }
                    else if (e.key.code == Keyboard::B)
                    {
                        oper = 0.7;
                        state = MENU;
                    }
                }
            }
        }

        if (state == GAME)
        {
            if (Keyboard::isKeyPressed(Keyboard::Down))
                delay = 0.05;

            //// <- Move -> ///
            for (int i = 0; i < 4; i++)
            {
                b[i] = a[i];
                a[i].x += dx;
            }
            if (!check())
            {
                for (int i = 0; i < 4; i++)
                    a[i] = b[i];
            }

            //////Rotate//////
            if (rotate)
            {
                Point p = a[1]; // center of rotation
                for (int i = 0; i < 4; i++)
                {
                    int x = a[i].y - p.y;
                    int y = a[i].x - p.x;
                    a[i].x = p.x - x;
                    a[i].y = p.y + y;
                }
                if (!check())
                {
                    for (int i = 0; i < 4; i++)
                        a[i] = b[i];
                }
            }

            ///////Tick//////
            if (timer > delay)
            {
                for (int i = 0; i < 4; i++)
                    b[i] = a[i];
                for (int i = 0; i < 4; i++)
                    a[i].y += 1;

                if (!check())
                {
                    for (int i = 0; i < 4; i++)
                        field[b[i].y][b[i].x] = colorNum;

                    spawnNewBlock(colorNum);

                    if (checkGameOver())
                    {
                        state = GAME_OVER;
                        music.stop();
                        gameOverMusic.play();
                    }
                }

                timer = 0;
            }

            ///////check lines//////////
            int k = M - 1;
            for (int i = M - 1; i > 0; i--)
            {
                int count = 0;
                for (int j = 0; j < N; j++)
                {
                    if (field[i][j])
                        count++;
                    field[k][j] = field[i][j];
                }
                if (count < N)
                    k--;
                else
                    score += 100; // Increase score by 100 for each cleared line
            }

            dx = 0;
            rotate = false;
            delay = 0.3;
        }

        window.clear(Color::White);

        if (state == START)
        {
            window.draw(startScreen);
        }
        else if (state == MENU)
        {
            window.draw(menuScreen);
        }
        else if (state == GAME)
        {
            window.draw(background);

            for (int i = 0; i < M; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    if (field[i][j] == 0)
                        continue;
                    s.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
                    s.setPosition(j * 18, i * 18);
                    s.move(28, 31); // offset
                    window.draw(s);
                }
            }

            for (int i = 0; i < 4; i++)
            {
                s.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
                s.setPosition(a[i].x * 18, a[i].y * 18);
                s.move(28, 31); // offset
                window.draw(s);
            }

            for (int i = 0; i < 4; i++)
            {
                s.setTextureRect(IntRect(nextColorNum * 18, 0, 18, 18));
                s.setPosition(nextBlock[i].x * 18, nextBlock[i].y * 18); // Using nextBlock
                s.move(250, 265); // offset for next block
                window.draw(s);
            }

           

            // Display score
            scoreText.setString(intToString(score));
            scoreText.setPosition(257, 95);
            window.draw(scoreText);
        }
        else if (state == PAUSE)
        {
            window.draw(ttbreak);
        }
        else if (state == GAME_OVER)
        {
            window.draw(gameOverScreen);
            scoreText.setString(intToString(score));
            scoreText.setPosition(160, 360);
            window.draw(scoreText);
        }
        else if (state == DIFFERENT)
        {
            window.draw(differentScreen);
        }

        window.display();
    }

    return 0;
}


