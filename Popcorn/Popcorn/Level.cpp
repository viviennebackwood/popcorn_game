#include "Level.h"

//level board
char ALevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
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

// ALevel
//---------------------------------------------------------------------------------------------------------
ALevel::ALevel()
: Active_Brick(EBT_Blue), Brick_Purple_Pen(0), Brick_Blue_Pen(0), Letter_Pen(0), Brick_Purple_Brush(0), Brick_Blue_Brush(0), Level_Rect{}
{
}
//---------------------------------------------------------------------------------------------------------
void ALevel::Init()
{
   Letter_Pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));

   AsConfig::Create_Pen_Brush(AsConfig::Purple_Brick_Color, Brick_Purple_Pen, Brick_Purple_Brush);
   AsConfig::Create_Pen_Brush(AsConfig::Blue_Brick_Color, Brick_Blue_Pen, Brick_Blue_Brush);

   Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
   Level_Rect.top = AsConfig::AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
   Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::AsConfig::Level_Height * AsConfig::Global_Scale;
   Level_Rect.bottom = Level_Rect.top + AsConfig::AsConfig::Cell_Height * AsConfig::AsConfig::Level_Width * AsConfig::Global_Scale;
}
//---------------------------------------------------------------------------------------------------------
void ALevel::Check_Level_Brick_Hit(int& next_y_pos, double& ball_direction)
{//Adjusting the reflection position from bricks

   int brick_y_pos = AsConfig::Level_Y_Offset + AsConfig::Level_Height * AsConfig::Cell_Height;

   for (int i = AsConfig::Level_Height - 1; i >= 0; --i)
   {
      for (int j = 0; j < AsConfig::Level_Width; j++)
      {
         if (Level_01[i][j] == 0)
            continue;

         if (next_y_pos < brick_y_pos)
         {
            next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
            ball_direction = -ball_direction;
         }
      }

      brick_y_pos -= AsConfig::Cell_Height;
   }
}
//--------------------------------------------------------------------------------------------------------- 
void ALevel::Draw(HWND hwnd, HDC& hdc, RECT& paint_area)
{
   RECT intersect_rect;

   if (!IntersectRect(&intersect_rect, &paint_area, &Level_Rect))
      return;

   for (int i = 0; i < AsConfig::Level_Height; i++)
      for (int j = 0; j < AsConfig::Level_Width; ++j)
         Draw_Brick(hdc, AsConfig::Level_X_Offset + j * AsConfig::Cell_Width, AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height, (EBrick_Type)Level_01[i][j]);

   Active_Brick.Draw(hdc, paint_area);
}
//---------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC& hdc, int x, int y, EBrick_Type brick_type)
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

   RoundRect(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Brick_Width) * AsConfig::Global_Scale, (y + AsConfig::Brick_Height) * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//---------------------------------------------------------------------------------------------------------
void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
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
      back_brush = Brick_Purple_Brush;
   }
}
//---------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick_Letter(HDC& hdc, int x, int y, int rotation_step, EBrick_Type brick_type, ELetter_Type letter_type)
{
   bool switch_color;

   double offset;
   int back_part_offset;

   float rotation_angle;
   int brick_half_height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;

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
      Rectangle(hdc, x, y + brick_half_height - 3, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + brick_half_height);

      //Front of brick
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      Rectangle(hdc, x, y + brick_half_height, x + AsConfig::Brick_Width * AsConfig::Global_Scale, y + brick_half_height + 2);
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
      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)AsConfig::Global_Scale;
      back_part_offset = (int)round(offset);

      //Back of brick
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);
      Rectangle(hdc, 0, -brick_half_height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height - back_part_offset);

      //Front of brick
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      Rectangle(hdc, 0, -brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height);

      if ((rotation_step >= 4 && rotation_step <= 12))
      {
         switch (letter_type)
         {
         case ELT_None:
            break;
         case ELT_O:
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, (-5 * AsConfig::Global_Scale) / 2, 0 + 10 * AsConfig::Global_Scale, 5 * AsConfig::Global_Scale / 2);
            break;
         default:
            break;
         }
      }

      SetWorldTransform(hdc, &old_xform);
   }
}
//---------------------------------------------------------------------------------------------------------
