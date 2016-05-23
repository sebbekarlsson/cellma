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
        bool latch;
        bool actionState;
        bool cameraToCursor;

        std::vector<CellChunk*> cellChunks;
        std::vector<CellChunk*>::iterator cellChunksIterator;
        SDL_Event event;

        DocumentScene () {
            this->cx = 0;
            this->cy = 0;
            this->latch = false;
            this->actionState = false;
            this->cameraToCursor = false;

            for (int yy = 0; yy < 100; yy++) {
                this->cellChunks.push_back(new CellChunk(0, (yy*CELL_SIZE)*CELLCHUNK_HEIGHT));
            }
        }

        void tick(float delta) {
            this->actionState = false;

            if (state[SDL_SCANCODE_LALT] && state[SDL_SCANCODE_LSHIFT]) {
                this->actionState = true;
            }

            if (state[SDL_SCANCODE_LEFT] && cx > 0) {
                cx -= 1;
            }

            if (state[SDL_SCANCODE_RIGHT] && cx < 80-1) {
                cx += 1;
            }

            if (state[SDL_SCANCODE_UP] && cy > 0) {
                if (actionState) {
                    camera->dy -= 32.0f;
                    latch = false;
                } else {
                    cy -= 1;
                }
            }

            if (state[SDL_SCANCODE_DOWN]) {
                if (actionState) {
                    camera->dy += 32.0f;
                    latch = false;
                } else {
                    cy += 1;
                }
            }

            if (actionState && state[SDL_SCANCODE_RETURN] && latch == true) {
                if (cameraToCursor) {
                    cameraToCursor = false;
                } else {
                    cameraToCursor = true;
                }

                latch = false;
            }

            if (cameraToCursor) {
                if (camera->y > (cy*CELL_SIZE) - ((HEIGHT*SCALE)/2)) {
                    camera->dy -= 5.0f;
                }
                if (camera->y < (cy*CELL_SIZE) - ((HEIGHT*SCALE)/2)) {
                    camera->dy += 5.0f;
                }
            }

            if (state[SDL_SCANCODE_BACKSPACE] && cx > 0) {
                cx -= 1;
                this->getCurrentChunk()->cells[cx][(cy % CELLCHUNK_HEIGHT)]->character = "";
            }
            if (state[SDL_SCANCODE_RETURN] && latch == true) {
                cx = 0;
                cy += 1;
                for (int xx = 0; xx < 80; xx++) {
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
                if (cx >= 80-1) { cx = 0; }
                latch = false;
            }

            if (state[SDL_SCANCODE_TAB] && latch == true) {
                cx += 4;
                latch = false;
            }

            CellChunk *chunk;
            for (cellChunksIterator = this->cellChunks.begin() ; cellChunksIterator != this->cellChunks.end(); cellChunksIterator++) {
                chunk = &**cellChunksIterator;

                chunk->isVisible = false;
                if ((chunk->y+(CELLCHUNK_HEIGHT*CELL_SIZE)) >= camera->y && (chunk->y+(CELLCHUNK_HEIGHT*CELL_SIZE)) <= camera->y+(WIDTH*SCALE)) {
                    chunk->tick(delta);
                    chunk->isVisible = true;
                }
            }

            for (int xx = 0; xx < sizeof(this->getCurrentChunk()->cells)/sizeof(*this->getCurrentChunk()->cells); xx++) {
                for(int yy = 0; yy < sizeof(this->getCurrentChunk()->cells[xx])/sizeof(*this->getCurrentChunk()->cells[xx]); yy++) {
                    if (cx == xx && (cy % CELLCHUNK_HEIGHT) == yy) {
                        this->getCurrentChunk()->cells[xx][yy]->selected = true;
                    }
                }
            }

            camera->tick(delta);
        }

        void textEvent(string text) {
            if (latch == true && !actionState && cx < 80) {
                this->getCurrentChunk()->cells[cx][(cy % CELLCHUNK_HEIGHT)]->character = text;
                this->getCurrentChunk()->cells[cx][(cy % CELLCHUNK_HEIGHT)]->writeTimer = 10.0f;
                cx++;
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
