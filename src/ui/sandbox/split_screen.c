#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define MAX_INPUT_CHARS 20

typedef struct _cmd {
	char *cmd;
	char *action;
} cmd_t;

int cmd_init(cmd_t *c, char *cmd1, char *action) {
    if (c == NULL) {
    	printf("error\n");
	}
	
	c->cmd = cmd1;
    c->action = action;
    return 1;
}

cmd_t* cmd_new(char *cmd1, char *action) {
    cmd_t *c;
    int rc;

    c = malloc(sizeof(cmd_t));

    if (c == NULL) {
        printf("Could not allocate memory");
        return NULL;
    }

    rc = cmd_init(c, cmd1, action);
    if (rc != 1) {
        printf("Could not initialize cmd\n");
        return NULL;
    }

    return c;
}

int main() {

	char *look = "look";
	char *see = "You see a small green leaf on the path.";

	char *grab = "grab";
	char *leaf = "You grab a leaf from the path.";

	//initializing test commands
	cmd_t *look_cmd = cmd_new(look, see);
	cmd_t *grab_cmd = cmd_new(grab, leaf);

	//initialize the window height and width
	int ScreenWidth = 800;
	int ScreenHeight = 500;

	InitWindow(ScreenWidth, ScreenHeight, "load image sandbox program");

	//loading the texture into memory
	Image room = LoadImage("example3.png");

	//converting the image to a texture for raylib in VRAM
	Texture2D texture = LoadTextureFromImage(room);

	//creating a rectangle the size of the window
	int WindowWidth = 1200;
	int WindowHeight = 700;
	Rectangle window = { 0, 0, WindowWidth, WindowHeight };
	
	//initializing input text box
	char name[MAX_INPUT_CHARS + 1] = "\0";
	int letterCount = 0;
	int textBoxY = ScreenHeight - 30;
	int textBoxWidth = 225;
	int textBoxHeight = 30;

	Rectangle textBox = { 0, textBoxY, textBoxWidth, textBoxHeight };
	bool mouseOnText = false;

	//initializing output text box
	int outputX = 10;
	int outputHeight = 120;
	Rectangle output = { outputX, ScreenHeight - 140, ScreenWidth, outputHeight };
	char *output_text = "You see a path. There is a hollow log on the ground.";

   	int framesCounter = 0;
    SetTargetFPS(10);

	//loop to produce window of image and text box
	while (!WindowShouldClose()) {
		if (CheckCollisionPointRec(GetMousePosition(), window)) 
			mouseOnText = true;
        else 
			mouseOnText = false;

        if (mouseOnText) {
            // Get pressed key (character) on the queue
        	int key = GetKeyPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0) {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                {
                    name[letterCount] = (char)key;
                    letterCount++;
                }

                key = GetKeyPressed();  // Check next character in the queue
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                name[letterCount] = '\0';

                if (letterCount < 0) letterCount = 0;
            }
        }

		if (mouseOnText) framesCounter++;
		else framesCounter = 0;

		if (IsKeyPressed(KEY_ENTER)) {
			if (strcmp(name, look_cmd->cmd) == 0) 
				output_text = look_cmd->action;
			else if (strcmp(name, grab_cmd->cmd) == 0) 
				output_text = grab_cmd->action;

			// erases text in the text input, clearing the screen
			int length = letterCount;
			for(int i = 0; i < length; i++) {
				letterCount--;
				name[letterCount] = '\0';
			}
				
			if (letterCount < 0) 
				letterCount = 0;
		}

		//Draw Image
		BeginDrawing();

		ClearBackground(RAYWHITE);
		DrawTexture(texture, ScreenWidth/2 - texture.width/2,
					0, WHITE);
		DrawRectangleRec(textBox, WHITE);
		DrawRectangle(0, ScreenHeight - 150, ScreenWidth, 120, WHITE);
		DrawRectangleLines(0, ScreenHeight - 150, ScreenWidth, 150, BLACK);

		if (mouseOnText) {
			DrawRectangleLines(textBox.x, textBox.y, textBox.width, textBox.height, DARKGRAY);

			if (((framesCounter / 5)%2) == 0)
				DrawText("_", textBox.x + 5 + MeasureText(name, 21), textBox.y + 10, 20, DARKGRAY);
		}
    
		DrawText(name, textBox.x + 5, textBox.y + 8, 20, BLACK);
		Font test = GetFontDefault();
		DrawTextRec(test, output_text, output, 20, 5, true, BLACK);
		EndDrawing(); 
	}

	UnloadTexture(texture);
	CloseWindow();
	return 0;
}