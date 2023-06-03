#include "Engine.h"

#define _USE_MATH_DEFINES
#include <math.h>

int const Global_Scale = 3;// proto 2
int const Brick_Width = 15;
int const Brick_Height = 7;
int const Cell_Width = 16;
int const Cell_Height = 8;
int const Level_X_Offset = 8;
int const Level_Y_Offset = 6;
int const Circle_Size = 7;
int Inner_Width = 21;

HPEN Brick_Red_Pen, Brick_Blue_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush;
HPEN Platform_Circle_Pen, Platform_Inner_Pen;
HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;
HPEN Platform_Highlight_Pen;

enum EBrick_Type // proto 3
{
    EBT_None,
    EBT_Red,
    EBT_Blue
};

char Level_01[14][12] = // proto 3
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
//--------------------------------------------------------------------------------------------------------------------
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)// настройка игры при старте
{
    pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
    brush = CreateSolidBrush(RGB(r, g, b));
}
//--------------------------------------------------------------------------------------------------------------------
void Init()// настройка игры при старте
{
    Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
    Create_Pen_Brush(95, 252, 255, Brick_Blue_Pen, Brick_Blue_Brush);
    Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
    Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);

    Platform_Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
}
//--------------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{//Вывод кирпича
    HPEN hpen;
    HBRUSH hbrush;

    switch (brick_type)
    {
    case EBT_None:
        return;

    case EBT_Blue: // blue
        hpen = Brick_Blue_Pen;
        hbrush = Brick_Blue_Brush;
        break;

    case EBT_Red: // violet
        hpen = Brick_Red_Pen;
        hbrush = Brick_Red_Brush;
        break;

    default:
        return;
    }

    SelectObject(hdc, hpen);
    SelectObject(hdc, hbrush);
    RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 2 * Global_Scale, 2 * Global_Scale);
}
//--------------------------------------------------------------------------------------------------------------------
void Draw_Brick_Letter(HDC hdc,int x, int y, int rotation_step)
{// Вывод падающнго кирпича
    double offset;
    int back_part_offset;
    double rotation_angle =  2 * M_PI / 16 * rotation_step; // преобразование шага-доли от круга в угол поворота
    int brick_half_height = Brick_Height * Global_Scale / 2;
    
    XFORM xform, old_form;
    
    if (rotation_step == 4 || rotation_step == 12)
    {
        // выводим фон
        SelectObject(hdc, Brick_Red_Pen);
        SelectObject(hdc, Brick_Red_Brush);
        Rectangle(hdc, x, y + brick_half_height - Global_Scale,x + Brick_Width * Global_Scale, y + brick_half_height);

        // выводим передний план
        SelectObject(hdc, Brick_Blue_Pen);
        SelectObject(hdc, Brick_Blue_Brush);
        Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * Global_Scale, y + brick_half_height + Global_Scale - 1);
    }
    else
    {
        // настраиваем матрицу переворота буквы
        SetGraphicsMode(hdc, GM_ADVANCED);
        GetWorldTransform(hdc, &old_form);
        xform.eM11 = 1.0f;
        xform.eM12 = 0.0f;
        xform.eM21 = 0.0f;
        xform.eM22 = (FLOAT)cos(rotation_angle);
        xform.eDx = (FLOAT)x;
        xform.eDy = (FLOAT)y + (FLOAT)brick_half_height;
        SetWorldTransform(hdc, &xform);

        // выводим фон
        SelectObject(hdc, Brick_Red_Pen);
        SelectObject(hdc, Brick_Red_Brush);
        offset = 3.0 * (1.0f - fabs(xform.eM22)) * (double)Global_Scale;
        back_part_offset = (int)round(offset);
        Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * Global_Scale, brick_half_height - back_part_offset);

        // выводим передний план
        SelectObject(hdc, Brick_Blue_Pen);
        SelectObject(hdc, Brick_Blue_Brush);
        Rectangle(hdc, 0, -brick_half_height, Brick_Width * Global_Scale, brick_half_height);

        SetWorldTransform(hdc, &old_form);
    }
}
//--------------------------------------------------------------------------------------------------------------------
void Draw_Level(HDC hdc)
{//Отрисовка кирпичей(стены)
    int i, j;
    bool blue = false;
    for (i = 0; i < 14; i++)
    {
        for (j = 0; j < 12; j++)
        {
            Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
        }
    }
}
//--------------------------------------------------------------------------------------------------------------------
void Draw_Platformer(HDC hdc, int x, int y)
{//Отрисовка платформы
    // 1. Рисуем ллевый и правый шар
    SelectObject(hdc, Platform_Circle_Brush);
    SelectObject(hdc, Platform_Circle_Pen);
    Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
    Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Inner_Width + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
    // 2. Рисуем блик
    SelectObject(hdc, Platform_Highlight_Pen);
    Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale,
        (x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale, (x + 1) * Global_Scale, (y + 1 + 2) * Global_Scale);
    // 3. Рисуем внутреннюю часть
    SelectObject(hdc, Platform_Inner_Brush);
    SelectObject(hdc, Platform_Inner_Pen);
    RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 3 * Global_Scale, 3 * Global_Scale);   
}
//--------------------------------------------------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
{//Отрисовка экрана игры
    /*Draw_Level(hdc);
    Draw_Platformer(hdc, 50, 100);*/
    for (size_t i = 0; i < 16; i++)
    {
        Draw_Brick_Letter(hdc, 20 + i * Brick_Width * Global_Scale, 100, i);
    }
    
}
