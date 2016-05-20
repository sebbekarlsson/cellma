extern const int CELLCHUNK_HEIGHT;
class CellChunk {
    public:
        int x, y;
        Cell* cells[80][CELLCHUNK_HEIGHT];

        CellChunk (int x, int y) {
            this->x = x;
            this->y = y;

            for (int xx = 0; xx < sizeof(cells)/sizeof(*cells); xx++) {
                for(int yy = 0; yy < sizeof(cells[xx])/sizeof(*cells[xx]); yy++) {
                    cells[xx][yy] = new Cell((x+xx*CELL_SIZE), (y+yy*CELL_SIZE), (string)"");
                }
            }
        }

        void tick(float delta) {
            for (int xx = 0; xx < sizeof(cells)/sizeof(*cells); xx++) {
                for(int yy = 0; yy < sizeof(cells[xx])/sizeof(*cells[xx]); yy++) {
                    cells[xx][yy]->tick(delta);
                }
            }
        }

        void draw(float delta) {
            for (int xx = 0; xx < sizeof(cells)/sizeof(*cells); xx++) {
                for(int yy = 0; yy < sizeof(cells[xx])/sizeof(*cells[xx]); yy++) {
                    cells[xx][yy]->draw(delta);
                }
            }
        }
};