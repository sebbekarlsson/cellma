#pragma once


class Scene {
    public:
        Camera *camera;
        bool initialized;

        Scene () {
            this->initialized = false;
            this->camera = new Camera(0.0f, 0.0f);
        }

        /**
         * This function is used to draw all instances in this scene.
        */
        void drawInstances(float delta) {
        }

        /**
         * This function is used to tick all instances in this scene.
        */
        void tickInstances(float delta) {
        }

        virtual void initialize(float delta) = 0;
        virtual void tick(float delta) = 0;
        virtual void draw(float delta) = 0;
        virtual void textEvent(string text) = 0;
        virtual void keyUpEvent(SDL_Event e) = 0;
};
