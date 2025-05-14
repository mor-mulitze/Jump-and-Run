#include "obstacle.h"

Obstacle::Obstacle(int x, int y, int width, int height)
    : rect(x, y, width, height), speed(5)  //speed 1 zum debuggen orginal speed 5!!

{}

void Obstacle::move()
{
    rect.moveLeft(rect.left() - speed);  // Bewege das Hindernis nach links
}

void Obstacle::reset(int screenWidth, int screenHeight)
{
    if (rect.right() < 0) {
        // Wenn das Hindernis den Bildschirm verlässt, wird es neu generiert
        rect.moveLeft(screenWidth);
        rect.moveTop(screenHeight = 437);  // Setze es am unteren Bildschirmrand
    }
}

QRect Obstacle::getRect() const
{
    return rect;
}
