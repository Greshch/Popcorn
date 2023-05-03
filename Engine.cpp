#include "Engine.h"

int const Global_Scale = 3;// proto 2
int const Brick_Width = 15;
int const Brick_Height = 7;
int const Cell_Width = 16;
int const Cell_Height = 8;
int const Level_X_Offset = 8;
int const Level_Y_Offset = 6;

HPEN Brick_Red_Pen, Brick_Blue_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

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
void Init()// настройка игры при старте
{
    Brick_Red_Pen = CreatePen(PS_SOLID, 0, RGB(255, 85, 85));
    Brick_Red_Brush = CreateSolidBrush(RGB(255, 85, 85));

    Brick_Blue_Pen = CreatePen(PS_SOLID, 0, RGB(95, 252, 255));
    Brick_Blue_Brush = CreateSolidBrush(RGB(95, 252, 255));
}
//--------------------------------------------------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{//Вывод кирпича
    HPEN hpen;
    HBRUSH hbrush;

    /*HPEN Brick_Red_Pen, Brick_Blue_Pen;
    HBRUSH Brick_Red_Brush, Brick_Blue_Brush;*/



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
void Draw_Frame(HDC hdc)
{//Отрисовка экрана игры
    Draw_Level(hdc);
}
