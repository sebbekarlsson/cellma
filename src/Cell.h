#include <GL/freeglut.h>
#include <string>

extern const int CELL_SIZE;
using namespace std;

class Cell: public Instance {
    public:
        string character;

        Cell (float x, float y, string character) : Instance(x, y) {
            this->w = CELL_SIZE;
            this->h = CELL_SIZE;
            this->character = character;
        }

        void draw(float delta) {
            glDisable(GL_TEXTURE_2D);
            glColor3f(1.0f, 1.0f, 1.0f);

            glPushMatrix();

            glTranslatef(this->x, this->y, 0.0f);

            glBegin(GL_LINE_LOOP);

            glVertex2f(0.0f, 0.0f);
            glVertex2f(this->w, 0.0f);
            glVertex2f(this->w, this->h);
            glVertex2f(0.0f, this->h);
            
            glEnd();

            glPopMatrix();

            glPushMatrix();
            glColor3f(0.0f, 1.0f, 0.0f);
            glTranslatef(this->x+4, this->y+(13), 0.0f);
            glScalef(1/11.38, -(1/11.38), 1/11.38);

            for (auto c = this->character.begin(); c != this->character.end(); ++c)
            {
                glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *c);
            }

            glPopMatrix();

            glEnable(GL_TEXTURE_2D);
        }

        void tick(float delta) {}
};