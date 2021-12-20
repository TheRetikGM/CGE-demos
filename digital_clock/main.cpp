#include <ctime>
#include <cstdint>
#include <cstring>
#include <CursesGameEngine.hpp>

using cge::Vec2f;

// Each digit represents one row of the number (Bits make up the number).
int32_t numbers[10] = {
    0x75557,    // 0
    0x11111,    // 1
    0x71747,    // 2
    0x71717,    // 3
    0x55711,    // 4
    0x74717,    // 5
    0x74757,    // 6
    0x71111,    // 7
    0x75757,    // 8
    0x75711     // 9
};
int32_t double_dot = 0x02020;
//              3 3   3 3   3 3
// 00:00:00 --> 0-0-:-0-0-:-0-0 --> 6 * 3 + 7 + 2 = 27 bits
int32_t display[5] = { 0, 0, 0, 0, 0 };
const int display_width = 27;

class DigicalClock : public cge::CursesGameEngine
{
public:
    DigicalClock() {}
protected:
    bool run = true;
    float tm = 0.0f;
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    
    bool show_dots = true;
    float dots_timer = 0.0f;

    float disp_x = 0.0f;
    float disp_y = 0.0f;
    float center_x = 0.0f;
    float center_y = 0.0f;

    bool OnGameStart() override
    {
        std::time_t now = std::time(0);
        std::tm *ltm = std::localtime(&now);
        hours = ltm->tm_hour;
        minutes = ltm->tm_min;
        seconds = ltm->tm_sec;

        disp_x = std::min(WinHeight(), WinWidth()) * 0.25f;
        center_x = WinWidth() / 2.0f;
        center_y = WinHeight() / 2.0f;

        return true;
    }

    int GetDigitRow(int magic_number, int row)
    {
        // 0000 0000 0000 0000 0000
        int shift = (4 - row) * 4;
        return (magic_number >> shift) & 0x7;
    }
    void TimeToDisplay()
    {
        for (int row = 0; row < 5; row++)
        {
            char buf[6];
            std::sprintf(buf, "%02i%02i%02i", hours, minutes, seconds);

            display[row] = 0;
            int shifts[] = { 24, 20, 14, 10, 4, 0 };
            for (int i = 0; i < 6; i++)
                display[row] |= GetDigitRow(numbers[buf[i] - '0'], row) << shifts[i];

            if (show_dots) {
                display[row] |= GetDigitRow(double_dot, row) << 7;
                display[row] |= GetDigitRow(double_dot, row) << 17;
            }
        }
    }
    void RenderDisplay(int x, int y)
    {
        for (int row = 0; row < 5; row++)
        {
            for (int column = 0; column < display_width; column++)
            {
                int shift = display_width - column - 1;
                if (display[row] & (1 << shift))
                    Draw(x + column, y + row, COLOR_RED);
            }
        }
    }

    bool OnGameUpdate(float dt) override
    {
        // Update
        tm += dt;
        if (tm > 1.0f) {
            seconds += int(tm);
            tm = tm - float(int(tm));
        }
        if (seconds > 59) {
            minutes++;
            seconds = seconds - 60;
        }
        if (minutes > 59) {
            hours++;
            minutes = minutes - 60;
            
            if (hours > 99)
                return false;
        }

        dots_timer += dt;
        if (dots_timer > 0.5f) {
            dots_timer = dots_timer - 0.5f;
            show_dots = !show_dots;
        }

        float a = 1.5f * dt;
        disp_x = std::cos(a) * disp_x - std::sin(a) * disp_y;
        disp_y = std::sin(a) * disp_x + std::cos(a) * disp_y;
        Vec2f offset = Vec2f(center_x - display_width / 2, center_y - 2);

        // Render
        Clear(COLOR_WHITE);
        RenderDisplay(offset.x + disp_x, offset.y + disp_y);
        TimeToDisplay();

        return run;
    }
    void OnKeyPressed(int key) override
    {
        switch (key)
        {
        case 27:    // ESC
            run = false;
            break;
        default:
            break;
        }
    }
};

int main()
{
    DigicalClock* game = new DigicalClock();
    std::vector<std::string> err_buf;

    if (game->Construct(-1, -1, -1, -1, true))
        game->Start();
    else
    {
        err_buf = game->Errors;
        game->Errors.clear();
    }

    delete game;

    for (auto& err : err_buf)
        std::cerr << "[ERROR] " << err << std::endl;
}
