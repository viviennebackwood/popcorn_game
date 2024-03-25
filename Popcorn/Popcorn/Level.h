﻿#pragma once
#include "Config.h"
#include "Active_Brick.h"

enum ELetter_Type
{
   ELT_None,
   ELT_O
};

class ALevel
{
public:
   ALevel();

   void Init();
   void Check_Level_Brick_Hit(int& next_y_pos, double& ball_direction);
   void Draw(HWND hwnd, HDC& hdc, RECT& paint_area);

   Active_Brick Active_Brick;

private:
   void Draw_Brick(HDC& hdc, int x, int y, EBrick_Type brick_type);
   void Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush);
   void Draw_Brick_Letter(HDC& hdc, int x, int y, int rotation_step, EBrick_Type brick_type, ELetter_Type letter_type);

   HPEN Brick_Purple_Pen, Brick_Blue_Pen, Letter_Pen;
   HBRUSH Brick_Purple_Brush, Brick_Blue_Brush;
   RECT Level_Rect;

   static char Level_01[AsConfig::Level_Height][AsConfig::Level_Width];
};
//---------------------------------------------------------------------------------------------------------