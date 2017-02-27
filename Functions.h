#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "myLibs.h"

struct Line
{
    float x1;
    float x2;
    float y1;
    float y2;
};

inline int sgn(int n);
inline float absol(float num);
inline float maxim(float i, float j);

inline float convertedYToDisplay(float y, int YSize);

inline void DrawRedLine(float x1, float y1, float x2, float y2, sf::RenderWindow &window);
inline void DrawBlueLine(float x1, float y1, float x2, float y2, sf::RenderWindow &window);
inline void DrawBlackLine(float x1, float y1, float x2, float y2, sf::RenderWindow &window);

void OpticLine(int XSize, int YSize, sf::RenderWindow &window);
void DrawLen(float x, float h, bool type, int LenSize, int YSize, sf::RenderWindow &window);
void DrawLens(float h, bool change, bool first, int LenStep, int LenSize, int XSize, int YSize, sf::RenderWindow &window);
void DrawLight(Line actLine, float f, bool change, float &ymax, bool showPos, int LenStep, int XSize, int YSize, sf::RenderWindow &window);

void selectedOptions(int choice, float &f, bool &changeLens, bool &firstLen);

#endif