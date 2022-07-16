#include <sys/timeb.h> 
#include <math.h>
#include "include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"
#include "List.h"
#include "Circle.h"

#define WIDTH 1200
#define HEIGHT 800
#define TITLE "Circles dude, a lot of circles..."
#define ZOOM_EXP 4

void DrawLines(List* pCirclesList, float GUIZoomExp) {
    Iterator* tempIterator = IteratorCreate(pCirclesList);
    float firstX = WIDTH / 2 + IteratorGet(tempIterator)->x * ZOOM_EXP * GUIZoomExp;
    float firstY = HEIGHT / 2 + IteratorGet(tempIterator)->y * ZOOM_EXP * GUIZoomExp;
    while (IteratorNext(tempIterator)) {
        float currentX = WIDTH / 2 + IteratorGet(tempIterator)->x * ZOOM_EXP * GUIZoomExp;
        float currentY = HEIGHT / 2 + IteratorGet(tempIterator)->y * ZOOM_EXP * GUIZoomExp;
        DrawLine(firstX, firstY, currentX, currentY, BLACK);
        firstX = currentX;
        firstY = currentY;
    }
    free(tempIterator);
}

void DrawCircles(List* pCirclesList, float GUIZoomExp) {
    Iterator* tempIterator = IteratorCreate(pCirclesList);
    do {
        DrawCircle(WIDTH / 2 + IteratorGet(tempIterator)->x * ZOOM_EXP * GUIZoomExp, HEIGHT / 2 + IteratorGet(tempIterator)->y * ZOOM_EXP * GUIZoomExp, IteratorGet(tempIterator)->radius * ZOOM_EXP * GUIZoomExp, IteratorGet(tempIterator)->color);
        DrawCircle(WIDTH / 2 + IteratorGet(tempIterator)->x * ZOOM_EXP * GUIZoomExp, HEIGHT / 2 + IteratorGet(tempIterator)->y * ZOOM_EXP * GUIZoomExp, IteratorGet(tempIterator)->radius * ZOOM_EXP * GUIZoomExp - ZOOM_EXP / 2 * GUIZoomExp, GRAY);
        DrawCircle(WIDTH / 2 + IteratorGet(tempIterator)->x * ZOOM_EXP * GUIZoomExp, HEIGHT / 2 + IteratorGet(tempIterator)->y * ZOOM_EXP * GUIZoomExp, ZOOM_EXP / 2 * GUIZoomExp, BLACK);
    } while (IteratorNext(tempIterator));
    free(tempIterator);
}

void SimulateFrame(List* pCirclesList, float deltaT) {
    Iterator* tempIterator = IteratorCreate(pCirclesList);
    float parentX = IteratorGet(tempIterator)->x;
    float parentY = IteratorGet(tempIterator)->y;
    while (IteratorNext(tempIterator)) {
        Circle* pCurrentCircle = IteratorGet(tempIterator);
        pCurrentCircle->wPosition = fmod((pCurrentCircle->wPosition + pCurrentCircle->w * deltaT), (2 * PI));  //TO DO: FIX W TO DELTA T
        float currentX = parentX + pCurrentCircle->rodLength * cos(pCurrentCircle->wPosition);
        float currentY = parentY + pCurrentCircle->rodLength * sin(pCurrentCircle->wPosition);
        pCurrentCircle->x = currentX;
        pCurrentCircle->y = currentY;
        parentX = currentX;
        parentY = currentY;
    }
    free(tempIterator);
}

void AddCircle(List* pCirclesList, Color color, float radius, float rodLength, float w) {
    float parentX = .0f, parentY = .0f;
    if (pCirclesList->size != 0) {
        parentX = pCirclesList->pTail->pData->x, parentY = pCirclesList->pTail->pData->y;
    }
    Circle* pCreatedCircle = CircleCreate(color, radius, rodLength, w, parentX, parentY);
    ListAdd(pCirclesList, pCreatedCircle);
}

void RemoveCircle(List* pCirclesList) {
    if (pCirclesList->size > 1) {
        ListRemove(pCirclesList);
    }
}

void main() {
//----------------------------------Locals-----------------------------------//
    struct timeb startT, endT;
    ftime(&startT);

    float GUIZoomExp = 1.f;
    const char* GUIZoomExpText[128];

    float GUIRadius = 1.f;
    const char* GUIRadiusText[128];
    float GUIRodLength = 2.f;
    const char* GUIRodLengthText[128];
    int GUIW = 0;
    const char* GUIWText[128];
    int GUIColor = 0;
    Color colorList[5] = {
        RED,
        BLUE,
        GREEN,
        ORANGE,
        PURPLE
    };

    int isPaused = false;

    List* pCirclesList = ListCreate();
    AddCircle(pCirclesList, GRAY, 1, 0, 0);

    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(60);

    float deltaT = 0;
    while(!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

//-----------------------------------Body------------------------------------//
        if (!isPaused) {
            SimulateFrame(pCirclesList, deltaT);
        }
        else {
            DrawText("PAUSED", WIDTH / 2 - 410, HEIGHT / 2 - 90, 200, LIGHTGRAY);
        }
        DrawLines(pCirclesList, GUIZoomExp);
        DrawCircles(pCirclesList, GUIZoomExp);

//-----------------------------------GUI-------------------------------------//
        //Radius & Pause
        GUIRadius = GuiSlider((Rectangle) { 100, 10, 110, 15 }, "Radius  ", GUIRadiusText, GUIRadius, 0, 10);
        if (GUIRadius < 1) GUIRadius = 1.f;
        GuiSlider((Rectangle) { 1200, 10, 110, 15 }, "Press [SPACE] to pause.  ", "", 0, 0, 1);
        snprintf(GUIRadiusText, 64, "%.1f", GUIRadius);

        //RodLength
        GUIRodLength = GuiSlider((Rectangle) { 100, 30, 110, 15 }, "Rod Length  ", GUIRodLengthText, GUIRodLength , 0, 20);
        if (GUIRodLength < pCirclesList->pTail->pData->radius + GUIRadius) GUIRodLength = pCirclesList->pTail->pData->radius + GUIRadius;
        if (GUIRodLength < 2) GUIRodLength = 2.f;
        snprintf(GUIRodLengthText, 64, "%.1f", GUIRodLength);  

        //W
        GUIW = (int)round(GuiSlider((Rectangle) { 100, 50, 110, 15 }, "Angular Speed  ", GUIWText, GUIW, -360, 360));
        snprintf(GUIWText, 64, "%d", GUIW);

        //Color
        DrawRectangle(100, 68, 19, 19, RED);
        DrawRectangle(103, 71, 13, 13, WHITE);
        if(GuiCheckBox((Rectangle) { 102, 70, 15, 15 }, NULL, GUIColor == 0)) GUIColor = 0;
        DrawRectangle(123, 68, 19, 19, BLUE);
        DrawRectangle(126, 71, 13, 13, WHITE);
        if(GuiCheckBox((Rectangle) { 125, 70, 15, 15 }, NULL, GUIColor == 1)) GUIColor = 1;
        DrawRectangle(146, 68, 19, 19, GREEN);
        DrawRectangle(149, 71, 13, 13, WHITE);
        if(GuiCheckBox((Rectangle) { 148, 70, 15, 15 }, NULL, GUIColor == 2)) GUIColor = 2;
        DrawRectangle(168, 68, 19, 19, ORANGE);
        DrawRectangle(171, 71, 13, 13, WHITE);
        if(GuiCheckBox((Rectangle) { 170, 70, 15, 15 }, NULL, GUIColor == 3)) GUIColor = 3;
        DrawRectangle(191, 68, 19, 19, PURPLE);
        DrawRectangle(194, 71, 13, 13, WHITE);
        if(GuiCheckBox((Rectangle) { 193, 70, 15, 15 }, NULL, GUIColor == 4)) GUIColor = 4;

        //Buttons
        if (GuiButton((Rectangle) { 100, 90, 110, 15 }, "Add Circle")) {
            AddCircle(pCirclesList, colorList[GUIColor], GUIRadius, GUIRodLength, GUIW);
        }
        if (GuiButton((Rectangle) { 100, 130, 110, 15 }, "Add Rnd. Circle")) {
            GUIColor = rand() % 5;
            GUIRadius = (rand() % 91 + 10) / 10.f;
            int minRodLength = (int)((pCirclesList->pTail->pData->radius + GUIRadius) * 10);
            GUIRodLength = (rand() % (200 - minRodLength + 1) + minRodLength) / 10.f;
            GUIW = rand() % 721 - 360;
            AddCircle(pCirclesList, colorList[GUIColor], GUIRadius, GUIRodLength, GUIW);
        }
        if (GuiButton((Rectangle) { 100, 150, 110, 15 }, "Add 10 Rnd. Circles")) {
            for (int i = 0; i < 10; i++) {
                GUIColor = rand() % 5;
                GUIRadius = (rand() % 91 + 10) / 10.f;
                int minRodLength = (int)((pCirclesList->pTail->pData->radius + GUIRadius) * 10);
                GUIRodLength = (rand() % (200 - minRodLength + 1) + minRodLength) / 10.f;
                GUIW = rand() % 721 - 360;
                AddCircle(pCirclesList, colorList[GUIColor], GUIRadius, GUIRodLength, GUIW);
            }
        }
        if (GuiButton((Rectangle) { 100, 190, 110, 15 }, "Remove Circle")) {
            RemoveCircle(pCirclesList);
        }
        if (GuiButton((Rectangle) { 100, 190, 110, 15 }, "Remove 10 Circles")) {
            for (int i = 0; i < 10; i++) {
                RemoveCircle(pCirclesList);
            }
        }

        //Zoom
        GUIZoomExp += (GetMouseWheelMove() * .1f);
        GUIZoomExp = GuiSlider((Rectangle) { 100, HEIGHT - 25, 110, 15 }, "Zoom Multiplier  ", GUIZoomExpText, GUIZoomExp, 0.1, 5);
        snprintf(GUIZoomExpText, 64, "%.1f", GUIZoomExp);

        //Pausing
        if(IsKeyPressed(KEY_SPACE)) {
            isPaused ^= 1;
        }

        //TO DO: Add focusing?

        EndDrawing();

        ftime(&endT);
        deltaT = endT.time - startT.time + (endT.millitm - startT.millitm) / 1000.0f;
        startT = endT;
    }

    CloseWindow();
    free(pCirclesList);
}
