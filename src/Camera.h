class Camera: public Instance {
    public:
        float zoom;
        float dx, dy;
        float friction;

        Camera (float x, float y) : Instance(x, y) {
            this->friction = 0.95f;
            this->dx = 0.0f;
            this->dy = 0.0f;
        }

        void tick(float delta) {
            this->updatePhysics(delta);
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

        void updatePhysics (float delta) {
            if(dx > 0){
                if(dx - friction < 0){
                    dx = 0;
                }else{
                    dx -= friction;
                }
            }
            if(dx < 0){
                if(dx + friction > 0){
                    dx = 0;
                }else{
                    dx += friction;
                }
            }

            if(dy > 0){
                if(dy - friction < 0){
                    dy = 0;
                }else{
                    dy -= friction;
                }
            }
            if(dy < 0){
                if(dy + friction > 0){
                    dy = 0;
                }else{
                    dy += friction;
                }
            }
            
            x += dx * delta;
            y += dy * delta;
        }
};
