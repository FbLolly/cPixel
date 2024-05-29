#include <stdint.h>
#include <stdlib.h>

#include "raylib.h"

#define WIDTH           1920
#define HEIGHT          1080

#define PIXELSIZE       drawing->zoom

typedef struct{
    Color pixels[500][500];
    Vector2 posPixels[500][500];

    Vector2 maxPixel;
    unsigned int zoom;
}img;

typedef struct{
    Color array[10];

    Vector2 lastFramePixel;
    unsigned int selected;
}user;

typedef struct{
    unsigned int R;
    unsigned int G;
    unsigned int B;
    unsigned int A;

    int picked;
}palette;

void setDrawing(img* drawing);
void manageZoom(img* drawing);
void manageDrawing(img* drawing, user* user);
void drawDrawing(img* drawing);

void setUser(user* user);

void setPalette(palette* palette);
void drawPalette(palette* palette, Vector2 offset);
void managePalette(palette* palette, user* user, Vector2 offset);

void drawUserColor(user* user);
void manageUserColor(user* user);

int main(){
    int i, ii;
    img drawing;
    user user;
    palette palette;

    drawing.zoom = 1;
    drawing.maxPixel = (Vector2){32, 32};

    palette.picked = 255;

    InitWindow(WIDTH, HEIGHT, "");
    SetTargetFPS(60);

    setDrawing(&drawing);

    for (i = 0; i < drawing.maxPixel.y; i++){
        for (ii = 0; ii < drawing.maxPixel.x; ii++){
            drawing.pixels[i][ii] = (Color){0, 0, 0, 0};
        }
    }

    setUser(&user);
    setPalette(&palette);

    while (!WindowShouldClose()){
        manageZoom(&drawing);
        manageDrawing(&drawing, &user);
        managePalette(&palette, &user, (Vector2){10, 10});
        manageUserColor(&user);

        BeginDrawing();
            ClearBackground(GRAY);
        
            drawDrawing(&drawing);
            drawPalette(&palette, (Vector2){10, 10});
            drawUserColor(&user);
        EndDrawing();
    }

    return 0;
}


void setDrawing(img* drawing){
    int i, ii;

    for (i = 0; i < drawing->maxPixel.y; i++){
        for (ii = 0; ii < drawing->maxPixel.x; ii++){
            drawing->posPixels[i][ii] = (Vector2){((WIDTH/2.0)-((drawing->zoom*drawing->maxPixel.x)/2.0)+(ii*drawing->zoom)), ((HEIGHT/2.0)-((drawing->zoom*drawing->maxPixel.y)/2.0)+(i*drawing->zoom))};
        }
    }
}


void drawDrawing(img* drawing){
    int i, ii;

    for (i = 0; i < drawing->maxPixel.y; i++){
        for (ii = 0; ii < drawing->maxPixel.x; ii++){
            if (i % 2 == 0){
                if (ii % 2 == 0)
                    DrawRectangle(drawing->posPixels[i][ii].x, drawing->posPixels[i][ii].y, PIXELSIZE, PIXELSIZE, DARKGRAY);
                else
                    DrawRectangle(drawing->posPixels[i][ii].x, drawing->posPixels[i][ii].y, PIXELSIZE, PIXELSIZE, LIGHTGRAY);
            }else{
                if (ii % 2 == 0)
                    DrawRectangle(drawing->posPixels[i][ii].x, drawing->posPixels[i][ii].y, PIXELSIZE, PIXELSIZE, LIGHTGRAY);
                else
                    DrawRectangle(drawing->posPixels[i][ii].x, drawing->posPixels[i][ii].y, PIXELSIZE, PIXELSIZE, DARKGRAY);
            }


            DrawRectangle(drawing->posPixels[i][ii].x, drawing->posPixels[i][ii].y, drawing->zoom, drawing->zoom, drawing->pixels[i][ii]);
        }
    }
}


void manageZoom(img* drawing){
    if (IsKeyDown(KEY_Z)){
        if (drawing->zoom < 500){
            drawing->zoom += 1;
            setDrawing(drawing);
        }
    }else if (IsKeyDown(KEY_U)){
        if (drawing->zoom > 0){
            drawing->zoom -= 1;
            setDrawing(drawing);
        }
    }
}


void manageDrawing(img* drawing, user* user){
    int i, ii, iii;
    float b;
    float yIndex;
    Vector2 maxX;

    for (i = 0; i < drawing->maxPixel.y; i++){
        for (ii = 0; ii < drawing->maxPixel.x; ii++){
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){drawing->posPixels[i][ii].x, drawing->posPixels[i][ii].y, PIXELSIZE, PIXELSIZE})){
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                    drawing->pixels[i][ii] = user->array[user->selected];
                    user->lastFramePixel = (Vector2){ii, i};
                }else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
                    drawing->pixels[i][ii] = (Color){0, 0, 0, 0};
                }
            }
        }
    }
}


void setUser(user* user){
    int i;

    for (i = 0; i < 10; i++){
        user->array[i] = BLACK;
    }

    user->lastFramePixel = (Vector2){-1, -1};
    user->selected = 0;
}



void setPalette(palette* palette){
    int i;

    for (i = 0; i < 255; i++){
        palette->R = 0;
        palette->G = 0;
        palette->B = 0;
        palette->A = 255;
    }

    palette->picked = 0;
}


void drawPalette(palette* palette, Vector2 offset){
    int i, ii;
    Color tmp;
    tmp = (Color){palette->R, palette->G, palette->B, palette->A};

    for (i = 0; i < 255; i++){
        DrawRectangle(offset.x+i, offset.y, 1, 20, (Color){i, palette->G, palette->B, palette->A});
        DrawRectangle(offset.x+i, offset.y+30, 1, 20, (Color){palette->R, i, palette->B, palette->A});
        DrawRectangle(offset.x+i, offset.y+60, 1, 20, (Color){palette->R, palette->G, i, palette->A});
        DrawRectangle(offset.x+i, offset.y+90, 1, 20, (Color){palette->R, palette->G, palette->B, i});
    }

    for (i = 255; i >= 0; i--){
        DrawRectangle(offset.x+(255-i), offset.y+120, 1, 20, (Color){tmp.r+i, tmp.g+i, tmp.b+i, tmp.a});
    }
}


void managePalette(palette* palette, user* user, Vector2 offset){
    int i, ii;

    palette->R = user->array[user->selected].r;
    palette->G = user->array[user->selected].g;
    palette->B = user->array[user->selected].b;
    palette->A = user->array[user->selected].a;

    for (i = 0; i <= 90; i += 30){
        for (ii = 0; ii < 255; ii++){
            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){offset.x+ii, offset.y+i, 1, 20}) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                switch (i) {
                    case 0:
                        palette->R = ii;
                    break;
                    case 30:
                        palette->G = ii;
                    break;
                    case 60:
                        palette->B = ii;
                    break;
                    case 90:
                        palette->A = ii;
                    break;
                }

                user->array[user->selected] = (Color){palette->R, palette->G, palette->B, palette->A};
            }
        }
    }
}


void drawUserColor(user* user){
    int i;

    for (i = 0; i < 10; i++){
        DrawRectangle(10, (22*i)+200, 20, 20, user->array[i]);
    }

    DrawCircle(40, (22*user->selected)+210, 5, BLUE);
}

void manageUserColor(user* user){
    int i;

    for (i = 0; i < 10; i++){
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){10, (22*i)+200, 20, 20}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            user->selected = i;
        }
    }

    if (IsKeyPressed(KEY_ONE))
        user->selected = 0;
    else if (IsKeyPressed(KEY_TWO))
        user->selected = 1;
    else if (IsKeyPressed(KEY_THREE))
        user->selected = 2;
    else if (IsKeyPressed(KEY_FOUR))
        user->selected = 3;
    else if (IsKeyPressed(KEY_FIVE))
        user->selected = 4;
    else if (IsKeyPressed(KEY_SIX))
        user->selected = 5;
    else if (IsKeyPressed(KEY_SEVEN))
        user->selected = 6;
    else if (IsKeyPressed(KEY_EIGHT))
        user->selected = 7;
    else if (IsKeyPressed(KEY_NINE))
        user->selected = 8;
    else if (IsKeyPressed(KEY_ZERO))
        user->selected = 9;
}