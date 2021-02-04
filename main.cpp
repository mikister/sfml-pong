#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <math.h>

#define PI 3.14159265f

void calcBallDir(float angle, sf::Vector2f *vector2)
{
    vector2->x = std::cos(angle * PI / 180.f);
    vector2->y = std::sin(angle * PI / 180.f);
}

bool hasCollidedWithPaddle(sf::RectangleShape ball, sf::RectangleShape paddle)
{
    sf::Vector2f leftB = ball.getPosition();
    sf::Vector2f rightB = leftB + ball.getSize();

    sf::Vector2f leftP = paddle.getPosition();
    sf::Vector2f rightP = leftP + paddle.getSize();

    if (leftB.x > rightP.x || leftP.x > rightB.x)
        return false;

    if (leftB.y > rightP.y || leftP.y > rightB.y)
        return false;

    return true;
}

int main()
{
    const sf::Vector2<int> WINDOW_SIZE(640, 320);
    const sf::Vector2f BALL_SIZE(10.f, 10.f);
    const sf::Vector2f PADDLE_SIZE(10.f, 50.f);

    sf::Vector2f ballDir(0.f, 0.f);
    // std::cout << ballDir.x << ' ' << ballDir.y << std::endl;
    // std::cout << 30.f * PI / 180.f << ' ' << 30.f * PI / 180.f << std::endl;
    float ballSpeed = 10.f;
    float ballDirAngle = 30.f;

    sf::Vector2f playerDir(0.f, 0.f);
    sf::Vector2f enemyDir(0.f, 0.f);
    float paddleSpeed = 4.f;

    int score = 0;

    sf::Clock clock;

    // Create window
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "PONG");

    // Draw middle line
    sf::VertexArray middleLine(sf::Lines, 2);
    middleLine[0].position = sf::Vector2f(WINDOW_SIZE.x / 2, 0);
    middleLine[1].position = sf::Vector2f(WINDOW_SIZE.x / 2, WINDOW_SIZE.y);

    // Draw ball
    sf::RectangleShape ball(sf::Vector2f(BALL_SIZE.x, BALL_SIZE.y));
    // ball.setFillColor(sf::Color::Green);
    ball.setPosition(50.f, (WINDOW_SIZE.y - BALL_SIZE.y) / 2);

    // Draw paddles
    sf::RectangleShape player(sf::Vector2f(PADDLE_SIZE.x, PADDLE_SIZE.y));
    sf::RectangleShape enemy(sf::Vector2f(PADDLE_SIZE.x, PADDLE_SIZE.y));

    player.setPosition(sf::Vector2f(10.f, (WINDOW_SIZE.y - PADDLE_SIZE.y) / 2));
    enemy.setPosition(sf::Vector2f(WINDOW_SIZE.x - 10.f - PADDLE_SIZE.x, (WINDOW_SIZE.y - PADDLE_SIZE.y) / 2));

    // Scoreboard
    sf::Font MyFont;
    if (!MyFont.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-M.ttf"))
    {
        // Error...
    }
    sf::Text scoreboard;
    scoreboard.setPosition(25.f, 10.f);
    scoreboard.setFont(MyFont);
    scoreboard.setCharacterSize(20);
    scoreboard.setFillColor(sf::Color::White);
    scoreboard.setString(std::to_string(score));

    while (window.isOpen())
    {
        sf::Time elapsed = clock.getElapsedTime();
        sf::Int32 delta = elapsed.asMilliseconds();
        // std::cout << delta << std::endl;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            else if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::W:
                    playerDir.y = -1.f;
                    break;
                case sf::Keyboard::S:
                    playerDir.y = 1.f;
                    break;

                default:
                    break;
                }
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::W:
                case sf::Keyboard::S:
                    playerDir.y = 0.f;
                    break;

                default:
                    break;
                }
            }
        }

        // physics
        // if (delta >= 100.f)
        if (delta >= 16.f)
        {
            // collision checks

            // wall collisions
            sf::Vector2f ballPos = ball.getPosition();

            if (ballPos.y >= (WINDOW_SIZE.y - BALL_SIZE.y))
            {
                ballDirAngle = -ballDirAngle;
                ball.setPosition(ballPos.x, WINDOW_SIZE.y - BALL_SIZE.y);
            }
            else if (ballPos.y <= 0)
            {
                ballDirAngle = -ballDirAngle;
                ball.setPosition(ballPos.x, 0);
            }

            if (ballPos.x >= (WINDOW_SIZE.x - BALL_SIZE.x))
            {
                ballDirAngle = 180.f - ballDirAngle;
                ball.setPosition(WINDOW_SIZE.x - BALL_SIZE.x, ballPos.y);
                score++;
            }
            else if (ballPos.x <= 0)
            {
                ballDirAngle = 180.f - ballDirAngle;
                ball.setPosition(0, ballPos.y);
                score--;
            }

            // paddle collisions
            if (hasCollidedWithPaddle(ball, player))
            {
                ballDirAngle = 180.f - ballDirAngle;
                ball.setPosition(player.getPosition().x + PADDLE_SIZE.x, ballPos.y);
            }
            else if (hasCollidedWithPaddle(ball, enemy))
            {
                ballDirAngle = 180.f - ballDirAngle;
                ball.setPosition(enemy.getPosition().x - BALL_SIZE.x, ballPos.y);
            }

            // movements
            calcBallDir(ballDirAngle, &ballDir);
            ball.move(ballDir * ballSpeed);

            sf::Vector2f playerPos = player.getPosition();
            sf::Vector2f enemyPos = enemy.getPosition();

            if (ballPos.y < (enemyPos.y + PADDLE_SIZE.y / 2))
                enemyDir.y = -1.f;
            else if (ballPos.y > (enemyPos.y + PADDLE_SIZE.y / 2))
                enemyDir.y = 1.f;
            else
                enemyDir.y = 0;

            player.move(playerDir * paddleSpeed);
            enemy.move(enemyDir * paddleSpeed);

            if (playerPos.y < 0)
                player.setPosition(playerPos.x, 0);
            else if (playerPos.y > (WINDOW_SIZE.y - PADDLE_SIZE.y))
                player.setPosition(playerPos.x, (WINDOW_SIZE.y - PADDLE_SIZE.y));

            if (enemyPos.y < 0)
                enemy.setPosition(enemyPos.x, 0);
            else if (enemyPos.y > (WINDOW_SIZE.y - PADDLE_SIZE.y))
                enemy.setPosition(enemyPos.x, (WINDOW_SIZE.y - PADDLE_SIZE.y));

            // scoreboard update
            scoreboard.setString(std::to_string(score));

            clock.restart();
        }

        window.clear();
        window.draw(middleLine);
        window.draw(ball);
        window.draw(player);
        window.draw(enemy);
        window.draw(scoreboard);
        window.display();
    }

    return 0;
}
