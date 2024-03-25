#pragma once
#include "Config.h"

enum EBrick_Type
{
   EBT_None,
   EBT_Purple,
   EBT_Blue
};
//---------------------------------------------------------------------------------------------------------
class Active_Brick
{
public:
   Active_Brick(EBrick_Type brick_type);

   void Act(HWND hwnd);
   void Draw(HDC& hdc, RECT& paint_area);

   static void Setup_Colors();

private:
   int Fade_Step;

   RECT Brick_Rect;
   EBrick_Type Brick_Type;

   static unsigned char Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step);
   static void Get_Fading_Color(const AColor& color, int step, HPEN& pen, HBRUSH& brush);

   static const int Max_Fade_Step = 80;

   static HPEN Fade_Purple_Brick_Pens[Max_Fade_Step];
   static HBRUSH Fade_Purple_Brick_Brushes[Max_Fade_Step];

   static HPEN Fade_Blue_Brick_Pens[Max_Fade_Step];
   static HBRUSH Fade_Blue_Brick_Brushes[Max_Fade_Step];

};
//---------------------------------------------------------------------------------------------------------