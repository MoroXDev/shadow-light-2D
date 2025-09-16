#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <iostream>

void calc_corners(int edge, const Rectangle &rect, Vector2 (&corners)[2])
{
  Vector2 origin = {rect.width / 2, rect.height / 2};
  switch (edge)
  {
  case 0:
    corners[1] = {rect.x - origin.x, rect.y - origin.y};
    corners[0] = {rect.x - origin.x, rect.y + rect.height - origin.y};
    break;
  case 1:
    corners[1] = {rect.x - origin.x, rect.y + rect.height - origin.y};
    corners[0] = {rect.x + rect.width - origin.x, rect.y + rect.height - origin.y};
    break;
  case 2:
    corners[1] = {rect.x + rect.width - origin.x, rect.y + rect.height - origin.y};
    corners[0] = {rect.x + rect.width - origin.x, rect.y - origin.y};
    break;
  case 3:
    corners[1] = {rect.x + rect.width - origin.x, rect.y - origin.y};
    corners[0] = {rect.x - origin.x, rect.y - origin.y};
    break;
  }
}

void draw_shadows(const std::vector<Rectangle> &rects)
{
  for (auto &rect : rects)
  {
    Vector2 corners[2] = {Vector2Zero(), Vector2Zero()};
    for (int edge = 0; edge < 4; edge++)
    {
      calc_corners(edge, rect, corners);
      Vector2 far[2] = {Vector2Zero(), Vector2Zero()};
      for (int i = 0; i < 2; i++)
      {
        Vector2 ray_dir = Vector2Normalize(Vector2Subtract(corners[i], GetMousePosition()));
        far[i] = corners[i] + ray_dir * (float)GetScreenWidth();
      }

      DrawTriangle(corners[0], corners[1], far[0], BLACK);
      DrawTriangle(corners[1], far[1], far[0], BLACK);
    }
  }
}

int main()
{
  InitWindow(1000, 500, "light shadows");
  const int MONITOR_W = GetMonitorWidth(GetCurrentMonitor());
  const int MONITOR_H = GetMonitorHeight(GetCurrentMonitor());
  SetWindowSize((int)(MONITOR_W * 0.75f), (int)(MONITOR_H * 0.75f));
  SetWindowPosition(MONITOR_W / 2 - GetScreenWidth() / 2, MONITOR_H / 2 - GetScreenHeight() / 2);

  std::vector<Rectangle> objects;
  Shader light_sh = LoadShader(NULL, "shaders/light.frag");
  const float flash_light_radius = 200;

  while (!WindowShouldClose())
  {
    const Vector2 sh_light_pos = {GetMousePosition().x, GetScreenHeight() - GetMousePosition().y};

    SetShaderValue(light_sh, GetShaderLocation(light_sh, "light_pos"), &sh_light_pos, SHADER_UNIFORM_VEC2);
    SetShaderValue(light_sh, GetShaderLocation(light_sh, "radius"), &flash_light_radius, SHADER_UNIFORM_FLOAT);
    
    BeginDrawing();
    
    BeginShaderMode(light_sh);
    ClearBackground(BLACK);
    
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), WHITE);
    draw_shadows(objects);
    for (auto &obj : objects)
    {
      DrawRectanglePro(obj, {obj.width / 2, obj.height / 2}, 0, RED);
    }

    EndShaderMode();
    EndDrawing();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      objects.push_back({GetMousePosition().x, GetMousePosition().y, 50.f, 50.f});
  }

  UnloadShader(light_sh);
  CloseWindow();
  return 0;
}
