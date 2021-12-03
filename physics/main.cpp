#include <iostream>
#include <CursesGameEngine.hpp>
#include <Physics2D.h>
#include <exception>
#include <unistd.h>
#include "Helper.hpp"

using cge::Vec2f;
using namespace Physics2D;

class TestGame : public cge::CursesGameEngine
{
public:
    TestGame() {}
protected:
    bool run = true;
    std::shared_ptr<PhysicsWorld> world;
    glm::vec2 WinSize = glm::vec2(0.0f);
    glm::vec2 MousePosition = glm::vec2(0.0f);

    void GenerateMap()
    {
        std::vector<std::string> map = {
            "#.................#",
            "#.................#",
            "#.................#",
            "#.................#",
            "#.................#",
            "#.................#",
            "#.................#",
            "#.................#",
            "#.................#",
            "#.................#",
            "#.................#",
            "###################",
        };

        glm::vec2 size = WinSize / glm::vec2(map[0].length(), map.size());

        for (int y = 0; y < map.size(); y++)
        {
            for (int x = 0; x < map[0].length(); x++)
            {
                if (map[y][x] == '#')
                {
                    glm::vec2 pos = glm::vec2(x, y) * size;
                    world->AddRectangleBody(pos, size, 5.0f, true, 0.5f, true);
                }
            }
        }
    }
    void RenderWorld()
    {
        for (int i = 0; i < world->BodyCount(); i++)
        {
            RigidBody* body = world->GetBody(i);
            glm::vec2 position = body->GetAABB().GetPosition(true);
            glm::vec2 size = body->GetAABB().GetSize(true);
            glm::vec2 center = position + size / 2.0f;
            if (body->GetCollider()->GetType() == ColliderType::rectangle)
                DrawRectangle(int(position.x), int(position.y), int(position.x + size.x), int(position.y + size.y), body->IsStatic() ? COLOR_BLUE : COLOR_GREEN);
            if (body->GetCollider()->GetType() == ColliderType::circle)
                DrawCircle(int(center.x), int(center.y), int(size.x / 2.0f), COLOR_RED);
            if (body->GetCollider()->GetType() == ColliderType::polygon)
            {
                int n_vertices = body->GetCollider()->Vertices.size();
                for (int j = 0; j < n_vertices; j++)
                {
                    glm::vec2 vA = world->ToUnits(body->GetCollider()->Vertices[j]);
                    glm::vec2 vB = world->ToUnits(body->GetCollider()->Vertices[(j + 1) % n_vertices]);
                    DrawLine(vA.x, vA.y, vB.x, vB.y, COLOR_MAGENTA);
                }
            }
        }
    }
    void RenderHUD()
    {
        std::vector<std::string> HUD_strings = {
            "Press ESC to quit.",
            "Press 'c' to clear map.",
            "Press 'a' to spawn circle at mouse position.",
            "Press 's' to spawn convex polygon at mouse position.",
            "Press 'd' to spawn rectangle at mouse position.",
            "Note: You must click atleast once, for it to",
            "      register mouse position"
        };

        for (int y = 0; y < HUD_strings.size(); y++)
            DrawString(0, y * 2, HUD_strings[y], COLOR_BLACK, false);
        DrawString(0, 2 * HUD_strings.size(), "Mouse position: [000, 000]", COLOR_BLACK, false);
        char buf[15];
        sprintf(buf, "%3i", (int)MousePosition.x);
        DrawString(std::string("Mouse position: [").length(), 2 * HUD_strings.size(), std::string(buf), COLOR_RED);
        sprintf(buf, "%3i", (int)MousePosition.y);
        DrawString(std::string("Mouse position: [000, ").length(), 2 * HUD_strings.size(), std::string(buf), COLOR_RED);
    }

    bool OnGameStart() override
    {
        WinSize = glm::vec2(WinWidth(), WinHeight());
        try
        {
            world = std::make_shared<PhysicsWorld>(glm::vec2(0.0f), WinSize, 1.0f);
            GenerateMap();
        }
        catch (const std::runtime_error& e) {
            printw("[ERROR] %s\n", e.what());
            getch();
            return false;
        }
        return true;
    }
    bool OnGameUpdate(float dt) override
    {
        // Update
        world->Update(dt, 10);

        // Render
        Clear(COLOR_WHITE);
        RenderWorld();
        RenderHUD();

        return run;
    }
    void OnKeyPressed(int key) override
    {
        RigidBody* body = nullptr;
        glm::vec2 rand_size(0.0f, 0.0f);
        switch (key)
        {
        case 27:    // ESC
            run = false;
            break;
        case 'c':
            world->RemoveAllBodies();
            GenerateMap();
            break;
        case 'a':
            body = world->AddCircleBody(MousePosition, (float)Helper::RandomInt(2, 6), 5.0f, false, Helper::RandomFloat_0_1(), true);
            body->GravityScale = 4.0f;
            break;
        case 'd':
            rand_size = glm::vec2(Helper::RandomInt(4, 8), Helper::RandomInt(4, 8));
            body = world->AddRectangleBody(MousePosition - rand_size / 2.0f, rand_size, 5.0f, false, Helper::RandomFloat_0_1(), true);
            body->GravityScale = 4.0f;
            break;
        default:
            break;
        }

        if (key == 's')
        {
            std:vector<float> rand_angles;
            int n_angles = Helper::RandomInt(3, 10);
            for (int i = 0; i < n_angles; i++)
                rand_angles.push_back(Helper::RandomFloat(0.0f, 2 * M_PI));
            std::sort(rand_angles.begin(), rand_angles.end(), [](const float& a, const float& b) { return a < b; });
            std::vector<glm::vec2> vertices;
            float rand_radius = (float)Helper::RandomInt(2, 6);
            for (int i = 0; i < n_angles; i++)
            {
                glm::vec2 vertex = glm::vec2(glm::cos(rand_angles[i]), glm::sin(rand_angles[i]));
                vertices.push_back(MousePosition + vertex * rand_radius);
            }
            body = world->AddPolygonBody(MousePosition, vertices, 5.0f, false, Helper::RandomFloat_0_1(), true);
            body->GravityScale = 3.0f;
        }
    }
    void OnMouseEvent(int x, int y, mmask_t bstate) override
    {
        MousePosition = glm::vec2(x, y);
    }
};

int main()
{
    TestGame* game = new TestGame();
    std::vector<std::string> err_buf;

    if (game->Construct(-1, -1, -1, -1, false))
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
