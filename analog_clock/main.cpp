#include <ctime>
#include <cstdint>
#include <cstring>
#include <CursesGameEngine.hpp>

using cge::Vec2f;

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
    
    bool OnGameStart() override
    {
        std::time_t now = std::time(0);
        std::tm *ltm = std::localtime(&now);
        hours = ltm->tm_hour;
        minutes = ltm->tm_min;
        seconds = ltm->tm_sec;

        return true;
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
            
            if (hours > 23)
                hours = 0;
        }

        // Render
        Clear(COLOR_WHITE);

	Vec2f wsize(WinWidth(), WinHeight());
	Vec2f center = wsize * 0.5f;
	float radius = std::min(wsize.x, wsize.y) / 2.0f - 1.0f;
	float i_radius = 0.9 * radius;

	for (float i = 0.0f; i < 12.0f; i++)
	{
		float a = i * (M_PI / 6.0f) + M_PI_2;
		float x1 = center.x + std::cos(a) * radius;
		float y1 = center.y - std::sin(a) * radius;
		float x2 = center.x + std::cos(a) * i_radius;
		float y2 = center.y - std::sin(a) * i_radius;
		DrawLine(x1, y1, x2, y2, 52);
	}
	DrawCircle(center.x, center.y, radius, COLOR_RED);
	DrawCircle(center.x, center.y, radius + 1, COLOR_RED);

	float hours_size = radius * 0.5f;
	float min_size = radius * 0.8f;
	float sec_size = radius * 0.9f;

	float s_angle = -(seconds) * (M_PI / 30.0f) + M_PI_2;
	float s_x = center.x + std::cos(s_angle) * sec_size;
	float s_y = center.y - std::sin(s_angle) * sec_size;
	DrawLine(center.x, center.y, s_x, s_y, COLOR_BLUE);

	float m_angle = -(minutes + seconds / 60.0f) * (M_PI / 30.0f) + M_PI_2;
	float m_x = center.x + std::cos(m_angle) * min_size;
	float m_y = center.y - std::sin(m_angle) * min_size;
	DrawLine(center.x, center.y, m_x, m_y, COLOR_GREEN);

	float h_angle = -(hours + minutes / 60.0f + seconds / 3600.0f) * (M_PI / 6.0f) + M_PI_2;
	float h_x = center.x + std::cos(h_angle) * hours_size;
	float h_y = center.y - std::sin(h_angle) * hours_size;
	DrawLine(center.x, center.y, h_x, h_y, COLOR_CYAN);

	FillCircle(center.x, center.y, 0.05f * radius, COLOR_RED);

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
