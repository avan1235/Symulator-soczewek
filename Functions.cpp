#include "Functions.h"
#include "myLibs.h"

inline int sgn(int n)
{
    if(n > 0)
        return 1;
    else if(n == 0)
        return 0;
    else
        return -1;
}

inline float absol(float num)
{
    if(num >= 0)
        return num;
    else
        return ((-1)*num);
}

inline float maxim(float i, float j)
{
    if(i >= j)
        return i;
    else
        return j;
}

inline float convertedYToDisplay(float y, int YSize)
{
    return absol(y-(YSize/2));
}

inline void DrawRedLine(float x1, float y1, float x2, float y2, sf::RenderWindow &window)
{
    sf::Vertex point[2]=
    {
        sf::Vertex(sf::Vector2f(x1, y1), sf::Color(255, 0, 0, 255)),
        sf::Vertex(sf::Vector2f(x2, y2), sf::Color(255, 0, 0, 255))
    };

    window.draw(point, 2, sf::Lines);
}

inline void DrawBlueLine(float x1, float y1, float x2, float y2, sf::RenderWindow &window)
{
    sf::Vertex point[2]=
    {
        sf::Vertex(sf::Vector2f(x1, y1),  sf::Color(0, 128, 255, 255)),
        sf::Vertex(sf::Vector2f(x2, y2),  sf::Color(0, 128, 255, 255))
    };

    window.draw(point, 2, sf::Lines);
}

inline void DrawBlackLine(float x1, float y1, float x2, float y2, sf::RenderWindow &window)
{
    sf::Vertex point[2]=
    {
        sf::Vertex(sf::Vector2f(x1, y1), sf::Color(0, 0, 0, 192)),
        sf::Vertex(sf::Vector2f(x2, y2), sf::Color(0, 0, 0, 192))
    };

    window.draw(point, 2, sf::Lines);
}

void OpticLine(int XSize, int YSize, sf::RenderWindow &window)
{
    float y = 0;
    float x = 0;
    float xStep = XSize/100;
    while(x < XSize)
    {
        DrawBlackLine(x, convertedYToDisplay(y, YSize), x+xStep, convertedYToDisplay(y, YSize), window);
        x = x+(2*xStep);
    }
}

void DrawLen(float x, float h, bool type, int LenSize, int YSize, sf::RenderWindow &window)
{
    float YS = 2*h+5*LenSize;

    DrawBlueLine(x, convertedYToDisplay(YS/2, YSize), x, convertedYToDisplay(-1*(YS/2), YSize), window);

    if(type)
    {
        DrawBlueLine(x, convertedYToDisplay(YS/2, YSize), x+LenSize, convertedYToDisplay(YS/2+LenSize, YSize), window);
        DrawBlueLine(x, convertedYToDisplay(YS/2, YSize), x-LenSize, convertedYToDisplay(YS/2+LenSize, YSize), window);
        DrawBlueLine(x, convertedYToDisplay(-1*(YS/2), YSize), x+LenSize, convertedYToDisplay(-1*(YS/2)-LenSize, YSize), window);
        DrawBlueLine(x, convertedYToDisplay(-1*(YS/2), YSize), x-LenSize, convertedYToDisplay(-1*(YS/2)-LenSize, YSize), window);
    }
    else
    {
        DrawBlueLine(x, convertedYToDisplay(YS/2, YSize), x+LenSize, convertedYToDisplay(YS/2-LenSize, YSize), window);
        DrawBlueLine(x, convertedYToDisplay(YS/2, YSize), x-LenSize, convertedYToDisplay(YS/2-LenSize, YSize), window);
        DrawBlueLine(x, convertedYToDisplay(-1*(YS/2), YSize), x+LenSize, convertedYToDisplay(-1*(YS/2)+LenSize, YSize), window);
        DrawBlueLine(x, convertedYToDisplay(-1*(YS/2), YSize), x-LenSize, convertedYToDisplay(-1*(YS/2)+LenSize, YSize), window);
    }
}

void DrawLens(float h, bool change, bool first, int LenStep, int LenSize, int XSize, int YSize, sf::RenderWindow &window)
{
    float x = LenStep;
    switch (change)
    {
    case 0:
        while(x < XSize)
        {
            DrawLen(x, h, 0, LenSize, YSize, window);
            x = x + LenStep;
        }
        break;
    case 1:
    {
        int i = first;
        while(x < XSize)
        {
            DrawLen(x, h, i%2, LenSize, YSize, window);
            x = x + LenStep;
            i++;
        }
    }
    break;
    }
}

void DrawLight(Line actLine, float f, bool change, float &ymax, bool showPos, int LenStep, int XSize, int YSize, sf::RenderWindow &window)
{
    if(actLine.x1 < XSize)
    {
        DrawRedLine(actLine.x1, convertedYToDisplay(actLine.y1, YSize), actLine.x2, convertedYToDisplay(actLine.y2, YSize), window);
        
		Line newLine;
        newLine.x1 = actLine.x2;
        newLine.y1 = actLine.y2;
        newLine.x2 = actLine.x2 + LenStep;
	newLine.y2 = actLine.y2*(2-(LenStep/f)) - actLine.y1;

        ymax = maxim(absol(ymax), absol(actLine.y1));
        ymax = maxim(absol(ymax), absol(actLine.y2));

        if(change)
            f = (-1)*f;

        DrawLight(newLine, f, change, ymax, showPos, LenStep, XSize, YSize, window);
    }
}

void selectedOptions(int choice, float &f, bool &changeLens, bool &firstLen)
{
	switch(choice)
    {
    case 1:
    {
        f = f;
        changeLens = 0;
        firstLen = 0;
    }
    break;
    case 2:
    {
        f = f;
        changeLens = 1;
        firstLen = 0;
    }
    break;
    case 3:
    {
        f = (-1)*f;
        changeLens = 1;
        firstLen = 1;
    }
    break;
    }
}
