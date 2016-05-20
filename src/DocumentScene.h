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
        std::vector<CellChunk*> cellChunks;
        std::vector<CellChunk*>::iterator cellChunksIterator;
        SDL_Event event;

        DocumentScene () {
            this->cx = 0;
            this->cy = 0;

            for (int yy = 0; yy < 4; yy++) {
                this->cellChunks.push_back(new CellChunk(0, (yy*CELL_SIZE)*CELLCHUNK_HEIGHT));
            }
        }

        void tick(float delta) {
            camera->tick(delta);
            if (state[SDL_SCANCODE_LEFT]) {
                cx -= 1;
            }
            if (state[SDL_SCANCODE_RIGHT]) {
                cx += 1;
            }
            if (state[SDL_SCANCODE_UP]) {
                cy -= 1;

                if (camera->dy > -124.0f)
                camera->dy -= std::max(camera->y, (float)cy) - std::min(camera->y, (float)cy);
            }
            if (state[SDL_SCANCODE_DOWN]) {
                cy += 1;

                if (camera->dy < 124.0f)
                camera->dy += std::max(camera->y, (float)cy) - std::min(camera->y, (float)cy);
            }
            if (state[SDL_SCANCODE_BACKSPACE]) {
                cx -= 1;
                this->getCurrentChunk()->cells[cx][(cy % CELLCHUNK_HEIGHT)]->character = "";
            }
            if (state[SDL_SCANCODE_RETURN]) {
                cx = 0;
                cy += 1;
            }

            CellChunk *chunk;
            for (cellChunksIterator = this->cellChunks.begin() ; cellChunksIterator != this->cellChunks.end(); cellChunksIterator++) {
                chunk = &**cellChunksIterator;
                chunk->tick(delta);
            }
            //this->getCurrentChunk()->tick(delta);

            for (int xx = 0; xx < sizeof(this->getCurrentChunk()->cells)/sizeof(*this->getCurrentChunk()->cells); xx++) {
                for(int yy = 0; yy < sizeof(this->getCurrentChunk()->cells[xx])/sizeof(*this->getCurrentChunk()->cells[xx]); yy++) {
                    if (cx == xx && (cy % CELLCHUNK_HEIGHT) == yy) {
                        this->getCurrentChunk()->cells[xx][yy]->selected = true;
                    }
                }
            }
        }

        void textEvent(string text) {
            this->getCurrentChunk()->cells[cx][(cy % CELLCHUNK_HEIGHT)]->character = text;
            this->getCurrentChunk()->cells[cx][(cy % CELLCHUNK_HEIGHT)]->writeTimer = 10.0f;
            cx++;
        }

        void keyUpEvent(SDL_Event e) {
        }

        void draw(float delta) {
            CellChunk *chunk;
            for (cellChunksIterator = this->cellChunks.begin() ; cellChunksIterator != this->cellChunks.end(); cellChunksIterator++) {
                chunk = &**cellChunksIterator;
                chunk->draw(delta);
            }
            //this->getCurrentChunk()->draw(delta); 
        }

        CellChunk * getCurrentChunk() {
            return cellChunks[(cy / CELLCHUNK_HEIGHT) % 4];
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
