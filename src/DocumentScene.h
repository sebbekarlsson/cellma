#include "Scene.h"
#include "Cell.h"
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>

extern const int CELL_SIZE;
using namespace std;
class DocumentScene: public Scene {
    public:
        int w;
        int h;
        std::list<Cell*> cells;
        std::list<Cell*>::iterator iter;

        void tick(float delta) {
            camera->tick(delta);
            this->w = 80;
            this->h = 80;

            fstream fin;
            fin.open("src/file.txt", ios::in);

            char my_character ;
            int xx = 0;
            int yy = 0;

            while (!fin.eof() ) {
                string sym(1, my_character); 
                std::string str(sym.c_str());
                cells.insert(cells.begin(), new Cell(xx*CELL_SIZE, yy*CELL_SIZE, str));
                fin.get(my_character);
                
                xx = xx+1;
                if (my_character == '\n' || xx >= 80){
                    yy = yy+1;
                    xx = 0;
                }
            }
        }

        void draw(float delta) {
            Cell *cell;
            for (iter = this->cells.begin() ; iter != this->cells.end(); iter++) {
                cell = &**iter;
                cell->draw(delta);
            }
        }

        void initialize(float delta) {
        }
};
