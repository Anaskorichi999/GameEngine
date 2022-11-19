#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "const.h"
#include "engine/Physics/physics.h"
void DrawRect(Rectangle rect,Color color);
void MoveBody(PhysicsBody *body, float dt);
int getintfromcolor(Color color);
typedef struct Map
{
    int Width;
    int Height;
    int *data;
} Map;

Map LoadMap(Image *image,int color(Color));

PhysicsBodyID body1;
PhysicsBodyID body6;
StaticBodyID body2;
StaticBodyID body3;
StaticBodyID body4;
StaticBodyID body5;
World world;

int main()
{

    //SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(WINDOWWIDTH,WINDOWHEIGHT, "");
    RenderTexture2D  rt = LoadRenderTexture(VIRTUALWIDTH,VIRTUALHEIGHT); 

    world = NewWorld((Vector2){0, 2000});
    Image image = LoadImage("build/res/Sprite-0002-export.png");
    //Map map = LoadMap(&image,getintfromcolor);
    body1 = NewPhysicsBody(&world,(Vector2){VIRTUALWIDTH / 2, VIRTUALHEIGHT/2}, (Vector2){40, 75}, 4, 0, 0);
    body2 = NewStaticBody(&world,(Vector2){VIRTUALWIDTH / 2, VIRTUALHEIGHT}, (Vector2){VIRTUALWIDTH, 50}, 1, 0);
    body3 = NewStaticBody(&world,(Vector2){VIRTUALWIDTH, VIRTUALHEIGHT / 2}, (Vector2){50, VIRTUALHEIGHT}, 1, 0);
    body4 = NewStaticBody(&world,(Vector2){0,VIRTUALHEIGHT / 2}, (Vector2){50, VIRTUALHEIGHT}, 1, 0);
    body5 = NewStaticBody(&world,(Vector2){VIRTUALWIDTH / 2, 0}, (Vector2){VIRTUALWIDTH, 50}, 1, 0);
    Camera2D camera = {0};
    camera.zoom = 1;
    camera.rotation = 0;
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        camera.offset = (Vector2){(VIRTUALWIDTH / 2)+dt,(VIRTUALHEIGHT/2)+dt};
        MoveBody(GetPhysicsBody(body1,&world), dt);
        UpdateWorld(&world,dt);
        camera.target = GetPhysicsBody(body1,&world)->Position;
        BeginTextureMode(rt);
            ClearBackground(WHITE);
            BeginMode2D(camera);
                DrawWorld(&world,DrawRect);
            EndMode2D();
        EndTextureMode();
        BeginDrawing();
            DrawTexturePro(rt.texture,(Rectangle){0,0,VIRTUALWIDTH,VIRTUALHEIGHT},(Rectangle){WINDOWWIDTH/2,WINDOWHEIGHT/2,WINDOWWIDTH,WINDOWHEIGHT},(Vector2){WINDOWWIDTH/2,WINDOWHEIGHT/2},180,WHITE);
        EndDrawing();
    }
    DestroyWorld(&world);
    CloseWindow();
}
void MoveBody(PhysicsBody *body, float dt)
{
    bool isGrunded = QeuryWorld(&world,GetRectangle((Vector2){body->Position.x,body->Position.y+(body->Size.y/2)+1},(Vector2){body->Size.x,2}),1);
        int maxv;
    if(IsKeyDown(KEY_LEFT_SHIFT))
        maxv = 700;
    else
        maxv = 400;
    if(isGrunded)
    {    
        if (IsKeyDown(KEY_A)&&body->Velocity.x <maxv)
            body->Velocity.x += 800*dt;
        else if (IsKeyDown(KEY_D)&&body->Velocity.x >-maxv)
            body->Velocity.x -= 800*dt;
        else
            body->Velocity.x -= body->Velocity.x*dt*2;
        if (IsKeyDown(KEY_SPACE) && isGrunded)
        {
            body->Velocity.y -= 200000*dt;
            isGrunded = false;
        }

    }
    else
    {
        if (IsKeyDown(KEY_A))
            body->Velocity.x += 100*dt;
        else if (IsKeyDown(KEY_D))
            body->Velocity.x -= 100*dt;
        else
            body->Velocity.x -= body->Velocity.x*dt;
    }
}
void DrawRect(Rectangle rect,Color color) {DrawRectangleLinesEx(rect,1,color);}
Map LoadMap(Image *image,int color(Color))
{
    Map map = {0};
    map.Width = image->width;
    map.Height = image->height;
    map.data = malloc(sizeof(int)*image->width*image->height);
    for(int x = 0;x < image->width;x++)
    {
        for(int y = 0; y < image->height;y++)
        {
            map.data[(x*image->width)+y] = color(GetImageColor(*image,x,y));
        }
    }
}
int getintfromcolor(Color color)
{
    if (color.a == BLACK.a && color.b == color.b && color.g == BLACK.g && color.r == BLACK.r)
    return 1;
    else
    return 0;
}