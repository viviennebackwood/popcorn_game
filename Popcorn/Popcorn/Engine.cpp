#include "Engine.h"
#define _USE_MATH_DEFINES
#include <math.h>

//bricks
HPEN Brick_Purple_Pen, Brick_Blue_Pen;
HBRUSH Brick_Purple_Brush, Brick_Blue_Brush;
//platform
HPEN Platform_Circle_Purple_Pen, Platform_Blue_Pen;
HBRUSH Platform_Circle_Purple_Brush, Platform_Blue_Brush;
//Background
HPEN BG_Pen;
HBRUSH BG_Brush;
//highlight for platform circle
HPEN Highlight_Pen;
//Letter
HPEN Letter_Pen;
//
HWND Hwnd;
//
RECT Platform_Rect, Prev_Platform_Rect;
RECT Level_Rect;

enum EBrick_Type
{
   EBT_None,
   EBT_Purple,
   EBT_Blue
};

enum ELetter_Type
{
   ELT_None,
   ELT_O
};

//scaling 
const int Global_Scale = 3;

//bricks
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Level_Width = 12;
const int Level_Height = 14;

//platform
const int Circle_Size = 7;
const int Platform_Y_Pos = 185;
const int Platform_Height = 7;
int Inner_Width = 21;
int Platform_X_Pos = 100;
int Platform_X_Step = Global_Scale + 2;
int Platform_Width = 28;


//level board
char Level_01[Level_Height][Level_Width] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}
//---------------------------------------------------------------------------------------------------------
void Redraw_Platform()
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = (Level_X_Offset + Platform_X_Pos) * Global_Scale;
   Platform_Rect.top = Platform_Y_Pos * Global_Scale;
   Platform_Rect.right = Platform_Rect.left + Platform_Width * Global_Scale;
   Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Global_Scale;


   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//---------------------------------------------------------------------------------------------------------
void Init_Engine(HWND hWnd)
{
   Hwnd = hWnd;

   //Background pen&brush
   Create_Pen_Brush(31, 31, 31, BG_Pen, BG_Brush);

   //Bricks pen&brush
   Create_Pen_Brush(255, 143, 255, Brick_Purple_Pen, Brick_Purple_Brush);
   Create_Pen_Brush(159, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);

   //Platform pen&brush
   Create_Pen_Brush(223, 0, 223, Platform_Circle_Purple_Pen, Platform_Circle_Purple_Brush);
   Create_Pen_Brush(0, 223, 223, Platform_Blue_Pen, Platform_Blue_Brush);

   //Highlight pen
   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

   //Letter pen
   Letter_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

   Level_Rect.left = Level_X_Offset * Global_Scale;
   Level_Rect.top = Level_Y_Offset * Global_Scale;
   Level_Rect.right = Level_X_Offset + Cell_Width * Level_Height * Global_Scale;
   Level_Rect.bottom = Level_Y_Offset + Cell_Height * Level_Width * Global_Scale;

   Redraw_Platform();
}
//---------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC& hdc, int x, int y, EBrick_Type brick_type)
{//Drawing a brick

   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBT_None:
      return;

   case EBT_Purple:
      pen = Brick_Purple_Pen;
      brush = Brick_Purple_Brush;
      break;

   case EBT_Blue:
      pen = Brick_Blue_Pen;
      brush = Brick_Blue_Brush;
      break;

   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 2 * Global_Scale, 2 * Global_Scale);
}
//---------------------------------------------------------------------------------------------------------
void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
{
   if (is_switch_color)
   {
      front_pen = Brick_Purple_Pen;
      front_brush = Brick_Purple_Brush;

      back_pen = Brick_Blue_Pen;
      back_brush = Brick_Blue_Brush;
   }
   else
   {
      front_pen = Brick_Blue_Pen;
      front_brush = Brick_Blue_Brush;

      back_pen = Brick_Purple_Pen;
      back_brush = Brick_Purple_Brush;;
   }
}
//---------------------------------------------------------------------------------------------------------
void Draw_Brick_Letter(HDC& hdc, int x, int y, int rotation_step, EBrick_Type brick_type, ELetter_Type letter_type)
{
   bool switch_color;

   double offset;
   int back_part_offset;

   float rotation_angle;
   int brick_half_height = (Brick_Height/2) * Global_Scale;

   XFORM xform, old_xform;

   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;

   if (brick_type == 0)
   {
      return;
   }

   //Range Guarantee
   rotation_step = rotation_step % 16;

   if (rotation_step < 8)
   {
      rotation_angle = 2.0f * (float)M_PI / 16.0f * (float)rotation_step;
   }
   else
   {//if rotation_step >= 8 then we start animation like from the start
      rotation_angle = 2.0f * (float)M_PI / 16.0f * (float)(8 - rotation_step);
   }
   //Rotation coloring
   if ((rotation_step > 4 && rotation_step <= 12))
   {
      switch_color = brick_type == EBT_Blue;
   }
   else
   {
      switch_color = brick_type == EBT_Purple;
   }
   Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

   //Special case for output 4 and 12 brick turnings
   if (rotation_step == 4 || rotation_step == 12)
   {
      //Back of brick
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);
      Rectangle(hdc, x, y + brick_half_height - 3, x + Brick_Width * Global_Scale, y + brick_half_height);

      //Front of brick
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * Global_Scale, y + brick_half_height + 2);
   }
   else
   {
      SetGraphicsMode(hdc, GM_ADVANCED);

      //Rotation matrix, to rotate the brick with respect to x
      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = cosf(rotation_angle);
      xform.eDx = (float)x;
      xform.eDy = (float)y + (float)brick_half_height;
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      //Offset calculation for dynamic rotation of bricks back part
      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Global_Scale;
      back_part_offset = (int)round(offset);

      //Back of brick
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);
      Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * Global_Scale, brick_half_height - back_part_offset);

      //Front of brick
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      Rectangle(hdc, 0, -brick_half_height, Brick_Width * Global_Scale, brick_half_height);

      if ((rotation_step >= 4 && rotation_step <= 12))
      {
         switch (letter_type)
         {
         case ELT_None:
            break;
         case ELT_O:
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * Global_Scale, -brick_half_height + 2, 0 + 10 * Global_Scale, brick_half_height - 2);
            break;
         default:
            break;
         }
      }

      SetWorldTransform(hdc, &old_xform);
   }

}
//---------------------------------------------------------------------------------------------------------
void Draw_Level(HDC& hdc)
{
   for (int i = 0; i < 14; i++)
      for (int j = 0; j < 12; ++j)
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}
//---------------------------------------------------------------------------------------------------------
void Draw_Platform(HDC& hdc, int x, int y)
{
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);
   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   //Process of drawing platform side balls
   SelectObject(hdc, Platform_Circle_Purple_Pen);
   SelectObject(hdc, Platform_Circle_Purple_Brush);

   Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Circle_Size + Inner_Width) * Global_Scale, (y + Circle_Size) * Global_Scale);

   //Arc(hdc,
   //   (x + 2) * Global_Scale, (y + 2) * Global_Scale, (x + Circle_Size - 2) * Global_Scale, (y + Circle_Size - 2) * Global_Scale,
   //   (x + 3) * Global_Scale, (y + 2) * Global_Scale, (x + 1) * Global_Scale, (y + 4) * Global_Scale);

   //Process of drawing a highlight for platform ball
   SelectObject(hdc, Highlight_Pen);

   Arc(hdc,
      (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale,
      (x + 2) * Global_Scale, (y + 1) * Global_Scale, (x + 1) * Global_Scale, (y + (Circle_Size / 2)) * Global_Scale);

   //Process of drawing a platform rectangle
   SelectObject(hdc, Platform_Blue_Pen);
   SelectObject(hdc, Platform_Blue_Brush);

   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 3 * Global_Scale, 3 * Global_Scale);
}
//---------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC& hdc, RECT& paint_area)
{//Drawing a game

   RECT intersect_rect;

   if (IntersectRect(&intersect_rect, &paint_area, &Level_Rect))
      Draw_Level(hdc);

   if (IntersectRect(&intersect_rect, &paint_area, &Platform_Rect))
      Draw_Platform(hdc, Level_X_Offset + Platform_X_Pos, Platform_Y_Pos);

   //for (int i = 0; i < 16; i++)
   //{
   //   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, i, EBT_Blue, ELT_O);
   //   Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, i, EBT_Purple, ELT_O);
   //}

}
//--------------------------------------------------------------------------------------------------------- 
int On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Right:
      Platform_X_Pos += Platform_X_Step;
      Redraw_Platform();
      break;
   case EKT_Left:
      Platform_X_Pos -= Platform_X_Step;
      Redraw_Platform();
      break;
   case EKT_Space:
      break;
   }

   return 0;
}