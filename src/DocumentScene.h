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
        int cx;
        int cy;
        Cell* cells[80][300];

        DocumentScene () {
            this->cx = -1;
            this->cy = 0;

            for (int xx = 0; xx < sizeof(cells)/sizeof(*cells); xx++) {
                for(int yy = 0; yy < sizeof(cells[xx])/sizeof(*cells[xx]); yy++) {
                    //int s = map->cells.size();
                    cells[xx][yy] = new Cell((xx*CELL_SIZE), (yy*CELL_SIZE), (string)"");
                }
            }
        }

        void tick(float delta) {
            camera->tick(delta);
            cx++;
            if (cx >= 80) {
                if (cy < 300) {
                    cy++;
                }
                cx = 0;
            }
        }

        void draw(float delta) {
            for (int xx = 0; xx < sizeof(cells)/sizeof(*cells); xx++) {
                for(int yy = 0; yy < sizeof(cells[xx])/sizeof(*cells[xx]); yy++) {
                    if (cx == xx && cy == yy) {
                        cells[xx][yy]->r = 255.0f;
                    } else {
                        cells[xx][yy]->r = 0.0f; 
                    }
                    cells[xx][yy]->draw(delta);
                }
            }
        }

        void initialize(float delta) {
            fstream fin;
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
            }
        }
};
