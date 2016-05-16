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

            //cells.resize(w);

            fstream fin;
            fin.open("src/file.txt", ios::in);

            char my_character ;
            int number_of_lines = 0;
            int xx = 0;
            int yy = 0;

            while (!fin.eof() ) {
                cout << my_character << endl; // Or whatever
                string sym(1, my_character); 
                std::string str(sym.c_str());
                cells.insert(cells.begin(), new Cell(xx*CELL_SIZE, yy*CELL_SIZE, str));
                fin.get(my_character);
                if (my_character == '\n'){
                    ++number_of_lines;
                    yy = yy+1;
                    xx = 0;
                } else {
                    xx = xx+1;
                }
            }
            cout << "NUMBER OF LINES: " << number_of_lines << endl;

            //for (int xx = 0; xx < w; xx++) {
            //    for(int yy = 0; yy < h; yy++) {
            //                        }
            //}
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
