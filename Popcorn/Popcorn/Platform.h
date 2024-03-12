#pragma once

#include "Config.h"

//---------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
   AsPlatform();

   void Init();
   void Redraw_Platform(HWND hwnd);
   void Draw(HDC& hdc, HPEN bg_pen, HBRUSH bg_brush, RECT& paint_area);

   int X_Pos;
   int Width;
   int X_Step;

private:
   int Inner_Width;

   HPEN Platform_Circle_Purple_Pen, Platform_Blue_Pen, Highlight_Pen;
   HBRUSH Platform_Circle_Purple_Brush, Platform_Blue_Brush;

   //Structures
   RECT Platform_Rect, Prev_Platform_Rect;

   static const int Height = 7;
   static const int Circle_Size = 7;

};
//---------------------------------------------------------------------------------------------------------