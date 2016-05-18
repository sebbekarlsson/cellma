#include "SDLImageLoader.h"
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <memory>
#include <list>
#include <iterator>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include "Instance.h"
#include "Camera.h"
#include "App.h"
#include "DocumentScene.h"


const Uint8 *keys;
int main (int argc, char* args[]) {
    keys = SDL_GetKeyboardState(NULL);
    App app;
    app.init();

    app.scenes.insert(app.scenes.begin(), new DocumentScene());

    SDL_Event e;
    float delta;
    int newTime;
    int oldTime = SDL_GetTicks(); 
    while (!app.quit) {
        newTime = SDL_GetTicks(); 
        delta = (float) (newTime - oldTime) / 1000;

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_TEXTINPUT) {
                app.textEvent(e.text.text);
            }
            if (e.type == SDL_KEYUP) {
                app.keyUpEvent(e);
            }
            if (e.type == SDL_QUIT) {
                app.quit = true;
            }
        }

        app.tick(delta);
        app.draw(delta);

        SDL_GL_SwapWindow(display);
        SDL_Delay(1);
        oldTime = newTime; 
    }
    app.close();

    return 0;
}
