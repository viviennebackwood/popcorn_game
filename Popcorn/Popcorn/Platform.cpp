﻿#include "Platform.h"

// AsPlatform
//---------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
: Inner_Width(21), X_Pos(AsConfig::Border_X_Offset), X_Step(AsConfig::Global_Scale + 2), Width(28), Platform_Circle_Purple_Pen(0), Platform_Blue_Pen(0), Highlight_Pen(0),
  Platform_Circle_Purple_Brush(0), Platform_Blue_Brush(0), Platform_Rect{}, Prev_Platform_Rect{}
{
}
void AsPlatform::Init()
{
   //Platform pen&brush
   AsConfig::Create_Pen_Brush(223, 0, 223, Platform_Circle_Purple_Pen, Platform_Circle_Purple_Brush);
   AsConfig::Create_Pen_Brush(0, 223, 223, Platform_Blue_Pen, Platform_Blue_Brush);

   //Highlight pen
   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
}
//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(HWND hwnd)
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
   Platform_Rect.top = AsConfig::Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;


   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);
}
//---------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC& hdc, HPEN bg_pen, HBRUSH bg_brush, RECT& paint_area)
{
   int x = X_Pos;
   int y = AsConfig::Y_Pos;

   RECT intersect_rect;

   if (!IntersectRect(&intersect_rect, &paint_area, &Platform_Rect))
      return;

   SelectObject(hdc, bg_pen);
   SelectObject(hdc, bg_brush);
   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   //Process of drawing platform side balls
   SelectObject(hdc, Platform_Circle_Purple_Pen);
   SelectObject(hdc, Platform_Circle_Purple_Brush);

   Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Circle_Size + Inner_Width) * AsConfig::Global_Scale, (y + Circle_Size) * AsConfig::Global_Scale);

   //Arc(hdc,
   //   (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + Circle_Size - 2) * AsConfig::Global_Scale, (y + Circle_Size - 2) * AsConfig::Global_Scale,
   //   (x + 3) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

   //Process of drawing a highlight for platform ball
   SelectObject(hdc, Highlight_Pen);

   Arc(hdc,
      (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + Circle_Size - 1) * AsConfig::Global_Scale, (y + Circle_Size - 1) * AsConfig::Global_Scale,
      (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale, (y + (Circle_Size / 2)) * AsConfig::Global_Scale);

   //Process of drawing a platform rectangle
   SelectObject(hdc, Platform_Blue_Pen);
   SelectObject(hdc, Platform_Blue_Brush);

   RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4 + Inner_Width - 1) * AsConfig::Global_Scale, (y + 1 + 5) * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);
}
//---------------------------------------------------------------------------------------------------------
