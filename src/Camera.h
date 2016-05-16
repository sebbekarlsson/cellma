class Camera: public Instance {
    public:
        float zoom;

        Camera (float x, float y) : Instance(x, y) {
            //this->zoom = 3.0f;
        }

        void tick(float delta) {
            //zoom += 0.001f;
        }
        void draw(float delta) {
            /*glDisable(GL_TEXTURE_2D);
            glColor3f(0.0f, 1.0f, 0.0f);
            glPushMatrix();
            glTranslatef(x, y, -1.0f);

            glBegin(GL_LINE_LOOP);

            glVertex2f(0.0f, 0.0f);
            glVertex2f(16, 0.0f);
            glVertex2f(16, 16);
            glVertex2f(0.0f, 16);

            glEnd();

            glPopMatrix();
            glEnable(GL_TEXTURE_2D);*/
        }
};
