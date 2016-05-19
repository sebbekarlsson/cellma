#include "Scene.h"
#include "Cell.h"
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>

extern const int CELL_SIZE;
const Uint8 *state = SDL_GetKeyboardState(NULL);
using namespace std;
class DocumentScene: public Scene {
    public:
        int cx;
        int cy;
        Cell* cells[80][300];
        SDL_Event event;

        DocumentScene () {
            this->cx = -1;
            this->cy = 0;

            for (int xx = 0; xx < sizeof(cells)/sizeof(*cells); xx++) {
                for(int yy = 0; yy < sizeof(cells[xx])/sizeof(*cells[xx]); yy++) {
                    cells[xx][yy] = new Cell((xx*CELL_SIZE), (yy*CELL_SIZE), (string)"");
                }
            }
        }

        void tick(float delta) {
            for (int xx = 0; xx < sizeof(cells)/sizeof(*cells); xx++) {
                for(int yy = 0; yy < sizeof(cells[xx])/sizeof(*cells[xx]); yy++) {
                    cells[xx][yy]->tick(delta);
                }
            }
            camera->tick(delta);
            if (state[SDL_SCANCODE_LEFT]) {
                cx -= 1;
            }
            if (state[SDL_SCANCODE_RIGHT]) {
                cx += 1;
            }
            if (state[SDL_SCANCODE_UP]) {
                cy -= 1;
            }
            if (state[SDL_SCANCODE_DOWN]) {
                cy += 1;
            }
            if (state[SDL_SCANCODE_BACKSPACE]) {
                cx -= 1;
                cells[cx][cy]->character = "";
            }
            if (state[SDL_SCANCODE_RETURN]) {
                cx = 0;
                cy += 1;
            }
        }

        void textEvent(string text) {
            cells[cx][cy]->character = text;
            cells[cx][cy]->writeTimer = 10.0f;
            cx++;
        }

        void keyUpEvent(SDL_Event e) {
        }

        void draw(float delta) {
            for (int xx = 0; xx < sizeof(cells)/sizeof(*cells); xx++) {
                for(int yy = 0; yy < sizeof(cells[xx])/sizeof(*cells[xx]); yy++) {
                    if (cx == xx && cy == yy) {
                        cells[xx][yy]->selected = true;
                    } else {
                        cells[xx][yy]->selected = false;
                    }
                    cells[xx][yy]->draw(delta);
                }
            }
        }

        void initialize(float delta) {
            /*
             * What I am doing here is basically importing a file into
             * the editor. Uncomment it to see.
             */
            /*fstream fin;
              fin.open("src/file.txt", ios::in);

              char my_character ;
              int xx = 0;
              int yy = 0;

              while (!fin.eof() ) {
              string sym(1, my_character); 
              std::string str(sym.c_str());
              cells[xx][yy]->character = str;
            //cells.insert(cells.begin(), new Cell(xx*CELL_SIZE, yy*CELL_SIZE, str));
            fin.get(my_character);

            xx = xx+1;
            if (my_character == '\n' || xx >= 80){
            yy = yy+1;
            xx = 0;
            }
            }*/
        }
};
