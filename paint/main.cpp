#include <iostream>
#include <cstdio>
#include <cmath>
#include "CursesGameEngine.hpp"
#include "Mat2_generic.hpp"


using cge::Vec2f;
typedef cge::Mat2_generic<float> Mat2f;

struct Shape
{
    uint8_t color;
    Vec2f p0, p1;
    int mode;
};

class TestGame : public cge::CursesGameEngine
{
    Vec2f center;
public:
    TestGame() {}
private:
    bool run = true;
    float rotate = 0.0f;
    vector<Shape> shapes;
    MEVENT mEvent;
    bool draw = false;
    Vec2f p0, p1;
    uint8_t rectColor = 1;
    int mode = 1;

protected:
    bool OnGameStart() override
    {
        center.x = WinWidth() / 2;
        center.y = WinHeight() / 2;

        return true;        
    }
    void OnKeyPressed(int key) override
    {        
        switch(key)
        {
            case 'x': run = false; break;
            case 'c': shapes.clear(); break;
            case 49: mode = 1; break;
            case 50: mode = 2; break;
            case 51: mode = 3; break;
            default: break;
        }
    }
    void OnMouseEvent(int x, int y, mmask_t bstate) override
    {        
        if (bstate == BUTTON1_PRESSED)
        {
            if (!draw) {
                draw = true;
                p0.x = x;
                p0.y = y;
            }
            p1.x = x;
            p1.y = y;
        }
        else if (bstate == BUTTON1_RELEASED)
        {
            draw = false;
            shapes.push_back(Shape{rectColor, p0, p1, mode});
        }
        else if (bstate == BUTTON4_PRESSED)
        {
            rectColor++;
        }
        else if (bstate == 0x00200000)
        {
            if (rectColor != 0)
                rectColor--;
        }        
        if (draw) {
            p1.x = x;
            p1.y = y;
        }
    }    
    bool OnGameUpdate(float delta) override
    {
        Clear(COLOR_BLACK);
        
        for (auto r : shapes)
        {
            if (r.mode == 1) {
                DrawRectangle(r.p0.x, r.p0.y, r.p1.x, r.p1.y, r.color);
            }
            else if (r.mode == 2) {
                DrawCircle(r.p0.x, r.p0.y, (r.p1 - r.p0).Mag(), r.color);
            }
            else if (r.mode == 3) {
                DrawLine(r.p0.x, r.p0.y, r.p1.x, r.p1.y, r.color);
            }
        }
        if (draw)
        {
            if (mode == 1) {
                DrawRectangle(p0.x, p0.y, p1.x, p1.y, rectColor);
                DrawLine(p0.x, p0.y, p1.x, p1.y, COLOR_BLUE);
            }
            else if (mode == 2) {
                DrawCircle(p0.x, p0.y, (p1 - p0).Mag(), rectColor);
                DrawLine(p0.x, p0.y, p1.x, p1.y, COLOR_BLUE);
            }
            else if (mode == 3) {
                DrawLine(p0.x, p0.y, p1.x, p1.y, rectColor);
            }
            DrawString(0, 1 * 2, "Drawing... p0" + p0.to_string() + " p1" + p1.to_string(), COLOR_GREEN);            
        }
                

        DrawString(0, 0, L"Fps: " + std::to_wstring(1.0f / delta), COLOR_RED, true, cge::Align::Center);        
        DrawHUD();        

        return run;        
    }
    void DrawHUD()
    {
        char buf[20];
        sprintf(buf, "Color: %3i [", rectColor);
        DrawString(0, 2*2, buf, COLOR_WHITE);
        DrawString(13, 2*2, L"███", rectColor);
        DrawString(17, 2*2, "]", COLOR_WHITE);
        DrawString(0, 3*2, "   Mode          Key", COLOR_WHITE);
        DrawString(0, 4*2, "Rectangle         1", mode == 1 ? COLOR_GREEN : COLOR_WHITE, true, cge::Align::Left);
        DrawString(0, 5*2, "Circle            2", mode == 2 ? COLOR_GREEN : COLOR_WHITE, true, cge::Align::Left);
        DrawString(0, 6*2, "Line              3", mode == 3 ? COLOR_GREEN : COLOR_WHITE, true, cge::Align::Left);
        DrawString(0, 8*2, "        Controls", COLOR_WHITE);
        DrawString(0, 9*2, "'x' - exit", COLOR_WHITE);
        DrawString(0, 10*2,"'c' - clear objects", COLOR_WHITE);
        DrawString(0, 11*2,"'mouse wheel' - change color", COLOR_WHITE);
        DrawString(0, 12*2,"'mouse left button' - draw", COLOR_WHITE);
    }
};

int main()
{
    TestGame game;

    if (game.Construct(-1, -1, -1, -1, false))
        game.Start();
    else
    {
        for (string s : game.Errors) {
            fprintf(stderr, "%s\n", s.c_str());
        }
        game.Errors.clear();
    } 

    return 0;
}
