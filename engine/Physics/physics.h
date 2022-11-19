#pragma ones
#include <stdlib.h>
typedef size_t PhysicsBodyID;
typedef size_t StaticBodyID;
typedef struct PhysicsBody
{
    Vector2 Position;
    Vector2 Velocity;
    Vector2 LestPosition;
    Vector2 Size;
    Vector2 Acceleration;
    Vector2 Force;
    float Mass;
    float Bounciness;
    int Class;
} PhysicsBody;

typedef struct StaticBody
{
    Vector2 Position;
    Vector2 Size;
    float Bounciness;
    int Class;
}StaticBody;

typedef struct World
{
    StaticBody *staticBodys;
    size_t staticBodysSize;
    PhysicsBody *physicsBodys;
    size_t physicsBodysSize;
    Vector2 gravty;
}World;

PhysicsBodyID NewPhysicsBody(World *world,Vector2 Position,Vector2 Size,float Mass,float Bounciness,int Class);
StaticBodyID NewStaticBody(World *world,Vector2 Position,Vector2 Size,int Class,float Bounciness);
World NewWorld(Vector2 gravty);
void UpdateWorld(World *world,float dt);
void DrawWorld(World *world,void Draw(Rectangle,Color));
Rectangle GetRectangle(Vector2 Position,Vector2 Size);
StaticBody *GetStaticBody(StaticBodyID staticBodyID,World *world);
PhysicsBody *GetPhysicsBody(PhysicsBodyID physicsBodyID,World *world);
bool QeuryWorld(World* world,Rectangle rect,int Class);
void DestroyWorld(World *world);