#include "Scene.h"
#include "Cell.h"
#include "CellChunk.h"
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>


extern const int CELL_SIZE;
extern const int CELLCHUNK_HEIGHT;

const Uint8 *state = SDL_GetKeyboardState(NULL);

using namespace std;

class DocumentScene: public Scene {
    public:
        int cx;
        int cy;
        string command;
        bool latch;
        bool actionState;
        bool cameraToCursor;
        bool commandState;

        std::vector<CellChunk*> cellChunks;
        std::vector<CellChunk*>::iterator cellChunksIterator;
        SDL_Event event;

        DocumentScene () {
            this->cx = 1;
            this->cy = 0;
            this->latch = false;
            this->actionState = false;
            this->cameraToCursor = false;
            this->commandState = true;

            for (int yy = 0; yy < 100; yy++) {
                this->cellChunks.push_back(new CellChunk(0, (yy*CELL_SIZE)*CELLCHUNK_HEIGHT));
            }
        }

        void tick(float delta) {
            this->actionState = false;

            if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_LSHIFT]) {
                this->actionState = true;
            }

            if (this->actionState && state[SDL_SCANCODE_C] && latch) {
                if (this->commandState) {
                    this->commandState = false;
                    latch = false;
                } else {
                    this->commandState = true;
                    latch = false;
                }
            }

            if (this->commandState) {
                if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_Z]) {
                    this->command = "";
                }
            }

            if (state[SDL_SCANCODE_LEFT] && cx > 1 && !commandState) {
                cx -= 1;
            }

            if (state[SDL_SCANCODE_RIGHT] && cx < 81-1 && !commandState) {
                cx += 1;
            }

            if (state[SDL_SCANCODE_UP] && cy > 0 && !commandState) {
                if (actionState) {
                    camera->dy -= 32.0f;
                    latch = false;
                } else {
                    if (((cy-2)*CELL_SIZE) <= camera->y) {
                        camera->dy -= ((CELL_SIZE*2)*camera->friction);
                    }
                    cy -= 1;
                }
            }

            if (state[SDL_SCANCODE_DOWN] && !commandState) {
                if (actionState) {
                    camera->dy += 32.0f;
                    latch = false;
                } else {
                    if (((cy+2)*CELL_SIZE) >= camera->y+(HEIGHT*SCALE)) {
                        camera->dy += ((CELL_SIZE*2)*camera->friction);
                    }
                    cy += 1;
                }
            }

            if (actionState && state[SDL_SCANCODE_RETURN] && latch == true && !commandState) {
                if (cameraToCursor) {
                    cameraToCursor = false;
                } else {
                    cameraToCursor = true;
                }

                latch = false;
            }

            if (state[SDL_SCANCODE_BACKSPACE] && !commandState) {
                this->getCurrentChunk()->cells[cx][(cy % CELLCHUNK_HEIGHT)]->character = "";

                if (cx > 1) {
                    cx -= 1;
                }
            }

            if (state[SDL_SCANCODE_RETURN] && latch == true) {

                if (!commandState) {
                    cx = 1;

                    if (((cy+2)*CELL_SIZE) >= camera->y+(HEIGHT*SCALE)) {
                        camera->dy += ((CELL_SIZE*2)*camera->friction);
                    }

                    cy += 1;
                    for (int xx = 1; xx < 81; xx++) {
                        if(
                              this->getChunk(((cy-1) / CELLCHUNK_HEIGHT) % 100)->cells[xx][((cy-1) % CELLCHUNK_HEIGHT)]->character == "" ||
                              this->getChunk(((cy-1) / CELLCHUNK_HEIGHT) % 100)->cells[xx][((cy-1) % CELLCHUNK_HEIGHT)]->character == " " ||
                              this->getChunk(((cy-1) / CELLCHUNK_HEIGHT) % 100)->cells[xx][((cy-1) % CELLCHUNK_HEIGHT)]->character == "\n"
                          ) {
                            cx = xx;
                        } else {
                            cx = xx;
                            break;
                        }
                    }
                    if (cx >= 81-1 || cx <= 0) { cx = 1; }
                } else {
                    this->command = "";
                }
                latch = false;
            }

            if (state[SDL_SCANCODE_TAB] && latch == true && !commandState) {
                if (cx + 4 < 81-1) {
                    cx += 4;
                    latch = false;
                }
            }

            CellChunk *chunk;
            for (cellChunksIterator = this->cellChunks.begin() ; cellChunksIterator != this->cellChunks.end(); cellChunksIterator++) {
                chunk = &**cellChunksIterator;

                chunk->isVisible = false;
                if ((chunk->y+(CELLCHUNK_HEIGHT*CELL_SIZE)) >= camera->y && chunk->y <= camera->y+(HEIGHT*SCALE)) {
                    chunk->tick(delta);
                    chunk->isVisible = true;

                    if(!chunk->initialized)
                    {    
                        chunk->initialize();
                        chunk->initialized = true;
                    }
                } else {
                    chunk->initialized = false;
                }
            }

            for (int xx = 0; xx < sizeof(this->getCurrentChunk()->cells)/sizeof(*this->getCurrentChunk()->cells); xx++) {
                for(int yy = 0; yy < sizeof(this->getCurrentChunk()->cells[xx])/sizeof(*this->getCurrentChunk()->cells[xx]); yy++) {
                    if (cx == xx && (cy % CELLCHUNK_HEIGHT) == yy) {
                        this->getCurrentChunk()->cells[xx][yy]->hover = true;
                    }
                }
            }
        
            if (cameraToCursor) {
                if (camera->y > (cy*CELL_SIZE) - ((HEIGHT*SCALE)/2)) {
                    camera->dy -= 5.0f;
                }
                if (camera->y < (cy*CELL_SIZE) - ((HEIGHT*SCALE)/2)) {
                    camera->dy += 5.0f;
                }
            }

            camera->tick(delta);
        }

        void textEvent(string text) {
            if (latch == true && !actionState) {

                if (!commandState) {
                    this->getCurrentChunk()->cells[cx][(cy % CELLCHUNK_HEIGHT)]->character = text;
                    this->getCurrentChunk()->cells[cx][(cy % CELLCHUNK_HEIGHT)]->writeTimer = 10.0f;

                    if (cx < 80) {
                        cx++;
                    }
                } else {
                    this->command += text;
                }
            }
        }

        void keyUpEvent(SDL_Event e) {
            latch = true;
        }

        void draw(float delta) {
            CellChunk *chunk;
            for (cellChunksIterator = this->cellChunks.begin() ; cellChunksIterator != this->cellChunks.end(); cellChunksIterator++) {
                chunk = &**cellChunksIterator;

                if (chunk->isVisible) {
                    chunk->draw(delta);
                }
            }
        }

        void drawGUI(float delta) {
            glDisable(GL_TEXTURE_2D);

            if (this->commandState) {
                glColor4f(mono_yellow->r/255.0f, mono_yellow->g/255.0f, mono_yellow->b/255.0f, 0.8f);
                glPushMatrix();
                
                glBegin(GL_QUADS);
                glVertex2f(0.0f, 0.0f);
                glVertex2f(WIDTH, 0.0f);
                glVertex2f(WIDTH, 24.0f);
                glVertex2f(0.0f, 24.0f);
                glEnd();

                glPopMatrix();

                glPushMatrix();
                glColor3f(255/255.0f, 255/255.0f, 255/255.0f);
                glTranslatef(8.5f, 16.0f, -0.5f);
                glScalef(CELL_SIZE/186.0f, -(CELL_SIZE/186.0f), CELL_SIZE/186.0f);

                for (auto c = this->command.begin(); c != this->command.end(); ++c)
                {
                    glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *c);
                }

                glPopMatrix();
            }

            glEnable(GL_TEXTURE_2D);
        }

        CellChunk * getCurrentChunk() {
            return cellChunks[(cy / CELLCHUNK_HEIGHT) % 100];
        }

        CellChunk * getChunk(int y) {
            return cellChunks[y];
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
