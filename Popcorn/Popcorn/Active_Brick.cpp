#include "Active_Brick.h"

// Active_Brick
HPEN Active_Brick::Fade_Purple_Brick_Pens[Max_Fade_Step];
HBRUSH Active_Brick::Fade_Purple_Brick_Brushes[Max_Fade_Step];
HPEN Active_Brick::Fade_Blue_Brick_Pens[Max_Fade_Step];
HBRUSH Active_Brick::Fade_Blue_Brick_Brushes[Max_Fade_Step];
//---------------------------------------------------------------------------------------------------------
Active_Brick::Active_Brick(EBrick_Type brick_type)
: Fade_Step(0), Brick_Type(brick_type)
{
};
//---------------------------------------------------------------------------------------------------------
void Active_Brick::Act(HWND hwnd)
{
   if (Fade_Step < Max_Fade_Step - 1)
   {
      ++Fade_Step;
      InvalidateRect(hwnd, &Brick_Rect, FALSE);
   }
}
//---------------------------------------------------------------------------------------------------------
void Active_Brick::Draw(HDC& hdc, RECT& paint_area)
{
   HPEN pen = 0;
   HBRUSH brush = 0;

   switch (Brick_Type)
   {
   case EBT_Blue:
      pen = Fade_Blue_Brick_Pens[Fade_Step];
      brush = Fade_Blue_Brick_Brushes[Fade_Step];
      break;

   case EBT_Purple:
      pen = Fade_Purple_Brick_Pens[Fade_Step];
      brush = Fade_Purple_Brick_Brushes[Fade_Step];
      break;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   Brick_Rect.left = (AsConfig::Level_X_Offset + AsConfig::Cell_Width) * AsConfig::Global_Scale;
   Brick_Rect.top = (AsConfig::Level_Y_Offset + AsConfig::Cell_Height) * AsConfig::Global_Scale;
   Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
   Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

   RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//---------------------------------------------------------------------------------------------------------
void Active_Brick::Setup_Colors()
{
   for (int i = 0; i < Max_Fade_Step; i++)
   {
      //blue
      Get_Fading_Color(AsConfig::Blue_Brick_Color, i, Fade_Blue_Brick_Pens[i], Fade_Blue_Brick_Brushes[i]);

      //purple
      Get_Fading_Color(AsConfig::Purple_Brick_Color, i, Fade_Purple_Brick_Pens[i], Fade_Purple_Brick_Brushes[i]);
   }
}
//---------------------------------------------------------------------------------------------------------
unsigned char Active_Brick::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
   return color - step * (color - bg_color) / (Max_Fade_Step - 1);
}
//---------------------------------------------------------------------------------------------------------
void Active_Brick::Get_Fading_Color(const AColor &color, int step, HPEN &pen, HBRUSH &brush)
{
   unsigned char r, g, b;

   r = Get_Fading_Channel(color.R, AsConfig::BG_Color.R, step);
   g = Get_Fading_Channel(color.G, AsConfig::BG_Color.G, step);
   b = Get_Fading_Channel(color.B, AsConfig::BG_Color.B, step);

   AsConfig::Create_Pen_Brush(r, g, b, pen, brush);
}
