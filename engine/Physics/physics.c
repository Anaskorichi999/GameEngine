#include "raylib.h"
#include "raymath.h"
#include "physics.h"

typedef struct Line
{
    Vector2 point1;
    Vector2 point2;
} Line;
enum LineToRect
{
    LINE_LEFT=0,
    LINE_RIGHT,
    LINE_UP,
    LINE_DOWN
};

Vector2 LineVsRectangle(Rectangle rect,Line line,int* flag);
Rectangle GetRectangle(Vector2 Position,Vector2 Size);
void CollisionReactPhysicsBodyVsStaticBody(PhysicsBody *body1,StaticBody *body2);
void CollisionReactPhysicsBodyVsPhysicsBody(PhysicsBody *body1,PhysicsBody *body2);
void UpdatePhysicsBody(PhysicsBody *body,float dt,Vector2 gravty);


Rectangle GetRectangle(Vector2 Position,Vector2 Size)
{
    return (Rectangle){Position.x-(Size.x/2),
                       Position.y-(Size.y/2),
                       Size.x,
                       Size.y};
}
Vector2 LineVsLine(Line line1,Line line2)
{
	float uA = ((line2.point2.x-line2.point1.x)*(line1.point1.y-line2.point1.y) - (line2.point2.y-line2.point1.y)*(line1.point1.x-line2.point1.x)) / ((line2.point2.y-line2.point1.y)*(line1.point2.x-line1.point1.x) - (line2.point2.x-line2.point1.x)*(line1.point2.y-line1.point1.y));
	float uB = ((line1.point2.x-line1.point1.x)*(line1.point1.y-line2.point1.y) - (line1.point2.y-line1.point1.y)*(line1.point1.x-line2.point1.x)) / ((line2.point2.y-line2.point1.y)*(line1.point2.x-line1.point1.x) - (line2.point2.x-line2.point1.x)*(line1.point2.y-line1.point1.y));
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
    {
        return (Vector2){(line1.point1.x + (uA * (line1.point2.x-line1.point1.x))) , (line1.point1.y + (uA * (line1.point2.y-line1.point1.y)))};
    }
    return (Vector2){999999999999 ,999999999999};
}

Line GetLineFromRect(Rectangle rect,int type)
{
    if(type == LINE_UP)
        return (Line){(Vector2){rect.x,rect.y},(Vector2){rect.x+rect.width,rect.y}};
    if(type == LINE_DOWN)
        return (Line){(Vector2){rect.x,rect.y+rect.height},(Vector2){rect.x+rect.width,rect.y+rect.height}};
    if(type == LINE_RIGHT)
        return (Line){(Vector2){rect.x+rect.width,rect.y},(Vector2){rect.x+rect.width,rect.y+rect.height}};
    if(type == LINE_LEFT)
        return (Line){(Vector2){rect.x,rect.y},(Vector2){rect.x,rect.y + rect.height}};
}

Vector2 LineVsRectangle(Rectangle rect,Line line,int* flag)
{
    Vector2 up    = LineVsLine(GetLineFromRect(rect,LINE_UP),line);
    Vector2 down  = LineVsLine(GetLineFromRect(rect,LINE_DOWN),line);
    Vector2 right = LineVsLine(GetLineFromRect(rect,LINE_RIGHT),line);
    Vector2 left  = LineVsLine(GetLineFromRect(rect,LINE_LEFT),line);
    if(!(up.x == 999999999999 && up.y == 999999999999))
    {
        *flag = LINE_UP;
        return up;
    }
    if(!(down.x == 999999999999 && down.y == 999999999999))
    {
        *flag = LINE_DOWN;
        return down;
    }
    if(!(right.x == 999999999999 && right.y == 999999999999))
    {
        *flag = LINE_RIGHT;
        return right;
    }
    if(!(left.x == 999999999999 && left.y == 999999999999))
    {
        *flag = LINE_LEFT;
        return left;
    }
}
void CollisionReactPhysicsBodyVsStaticBody(PhysicsBody *body1,StaticBody *body2)
{
    if(CheckCollisionRecs(GetRectangle(body1->Position,body1->Size),GetRectangle(body2->Position,body2->Size)))
    {
        int flag;
        Vector2 distance = Vector2Subtract(body1->Position,LineVsRectangle(GetRectangle(body2->Position,Vector2Add(body2->Size,body1->Size)),(Line){body2->Position,body1->LestPosition},&flag));
        if (flag == LINE_UP)
        {     
            body1->Position.y -= distance.y;
            body1->Velocity.y *= -(body1->Bounciness+body2->Bounciness)/2;
        }
        if (flag == LINE_DOWN)
        {
            body1->Position.y -= distance.y;
            body1->Velocity.y *= -(body1->Bounciness+body2->Bounciness)/2;
        }
        if (flag == LINE_RIGHT)
        {
            body1->Position.x -= distance.x;
            body1->Velocity.x *= -(body1->Bounciness+body2->Bounciness)/2;
        }
        if (flag == LINE_LEFT)
        {
            body1->Position.x -= distance.x;
            body1->Velocity.x *= -(body1->Bounciness+body2->Bounciness)/2;
        }
    }
}
void CollisionReactPhysicsBodyVsPhysicsBody(PhysicsBody *body1,PhysicsBody *body2)
{
    if(CheckCollisionRecs(GetRectangle(body1->Position,body1->Size),GetRectangle(body2->Position,body2->Size)))
    {
        int flag;
        Vector2 distance = Vector2Subtract(body1->Position,LineVsRectangle(GetRectangle(body2->Position,Vector2Add(body2->Size,body1->Size)),(Line){body2->Position,body1->LestPosition},&flag));
        if (flag == LINE_UP)
        {     
            body1->Position.y += distance.y;
        }
        if (flag == LINE_DOWN)
        {
            body1->Position.y -= distance.y;
        }
        if (flag == LINE_RIGHT)
        {
            body1->Position.x += distance.x;
        }
        if (flag == LINE_LEFT)
        {
            body1->Position.x -= distance.x;
        }
    }
}
void UpdatePhysicsBody(PhysicsBody *body,float dt,Vector2 gravty)
{
    body->LestPosition = body->Position;
    body->Acceleration.x = gravty.x + (body->Force.x/body->Mass);
    body->Acceleration.y = gravty.y + (body->Force.y/body->Mass);
    body->Velocity.x += body->Acceleration.x*dt;
    body->Velocity.y += body->Acceleration.y*dt;
    body->Position.x += body->Velocity.x*dt;
    body->Position.y += body->Velocity.y*dt;
}
void ApplyForce(PhysicsBody *body,Vector2 Force)
{
    body->Force.x += Force.x;
    body->Force.y += Force.y;
}
PhysicsBodyID NewPhysicsBody(World *world,Vector2 Position,Vector2 Size,float Mass,float Bounciness,int Class)
{
    world->physicsBodysSize++;
    PhysicsBody *body = {0};
    if(world->physicsBodysSize == 1)
    {
        world->physicsBodys = calloc(1,sizeof(PhysicsBody));
        body = world->physicsBodys;
    }
    else
    {
        world->physicsBodys = realloc(world->physicsBodys,sizeof(PhysicsBody)*world->physicsBodysSize);
        body = world->physicsBodys + world->physicsBodysSize-1;
    }
    body->Position = Position;
    body->Size = Size;
    body->Mass = Mass;
    body->Bounciness = Bounciness;
    body->Class = Class;
    return world->physicsBodysSize-1;
}

StaticBodyID NewStaticBody(World *world,Vector2 Position,Vector2 Size,int Class,float Bounciness)
{
    world->staticBodysSize++;
    StaticBody *body = {0};
    if(world->staticBodysSize == 1)
    {    world->staticBodys = malloc(sizeof(PhysicsBody));
        body = world->staticBodys;
    }
    else
    {
        world->staticBodys = realloc(world->staticBodys,sizeof(PhysicsBody)*world->staticBodysSize);
        body = world->staticBodys + world->staticBodysSize-1;
    }
    body->Position = Position;
    body->Size = Size;
    body->Bounciness = Bounciness;
    body->Class = Class;
    return world->staticBodysSize-1;
}
World NewWorld(Vector2 gravty)
{
    World world = {0};
    world.gravty = gravty;
    return world;
}
void UpdateWorld(World *world,float dt)
{
    PhysicsBody *phyptr = world->physicsBodys;
    for(int i = 0; i < world->physicsBodysSize; i++)
    {
        UpdatePhysicsBody(phyptr,dt,world->gravty);
        //PhysicsBody *phyptr2 = world->physicsBodys;
        //for(int j = 0; j < world->staticBodysSize;j++)
        //{
        //    if(i<j || j==i)
        //    continue;
        //    CollisionReactPhysicsBodyVsPhysicsBody(phyptr,phyptr2);
        //    phyptr2++;
        //}
        StaticBody *staptr = world->staticBodys;
        for(int j = 0; j < world->staticBodysSize;j++)
        {
            CollisionReactPhysicsBodyVsStaticBody(phyptr,staptr);
            staptr++;
        }
        phyptr++;
    }
} 
void DrawWorld(World *world,void Draw(Rectangle,Color))
{
    PhysicsBody *phyptr = world->physicsBodys;
    StaticBody *staptr = world->staticBodys;
    
    for(int i = 0; i < world->physicsBodysSize;i++)
    {
        Draw(GetRectangle(phyptr->Position,phyptr->Size),GREEN);
        phyptr++;
    }
    for(int i = 0; i < world->staticBodysSize;i++)
    {
        Draw(GetRectangle(staptr->Position,staptr->Size),BLACK);
        staptr++;
    }
}
PhysicsBody *GetPhysicsBody(PhysicsBodyID physicsBodyID,World *world)
{
    return world->physicsBodys + physicsBodyID;
}
StaticBody *GetStaticBody(StaticBodyID staticBodyID,World *world)
{
    return world->staticBodys + staticBodyID;
}

bool QeuryWorld(World* world,Rectangle rect,int Class)
{
    PhysicsBody *phyptr = world->physicsBodys;
    for(int i = 0; i<world->physicsBodysSize;i++)
    {
        if(phyptr->Class == Class)
        {
            bool x = CheckCollisionRecs(rect,GetRectangle(phyptr->Position,phyptr->Size));
            if(x)
            return x;
        }
        phyptr++;
    }
    StaticBody *staptr = world->staticBodys;
    for(int i = 0; i<world->staticBodysSize;i++)
    {
        if(staptr->Class == Class)
        {    
            bool x = CheckCollisionRecs(rect,GetRectangle(staptr->Position,staptr->Size));
            if(x)
            return x;
        }
        staptr++;
    }
    return false;
}
void DestroyWorld(World *world)
{
    free(world->physicsBodys);
    free(world->staticBodys );
}
