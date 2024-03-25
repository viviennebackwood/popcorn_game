#pragma once

#include "Config.h"
#include "Level.h"

//class AsPlatform;
//class AsEngine;
//class ALevel;

//---------------------------------------------------------------------------------------------------------
class ABall
{ 
public:
   ABall();

   void Init();

   void Draw(HDC hdc, RECT& paint_area);
   void Move(HWND hwnd, ALevel* level, int platform_x_pos, int platform_width);

private:
   int Ball_X_Pos, Ball_Y_Pos;
   double Ball_Speed;
   double Ball_Direction;

   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   RECT Ball_Rect, Prev_Ball_Rect;

};
//---------------------------------------------------------------------------------------------------------