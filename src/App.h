#include "Scene.h"


const int CELL_SIZE = 14;
const int CELLCHUNK_HEIGHT = 16;
const int LINE_NUMBER_MARGIN = 4;
const int WIDTH = (CELL_SIZE*(80+(LINE_NUMBER_MARGIN+1)));
const int HEIGHT = WIDTH / 16 * 9;
const int SCALE = 1;


SDL_Window* display = NULL;
SDL_Renderer* renderer = NULL;
class App {
    public:

        bool quit;
        SDL_GLContext context;
        std::vector<Scene*> scenes;
        int sceneIndex;


        /**
         * Constructor
         */
        App () {
            this->sceneIndex = 0;
            this->quit = false;
        }


        /**
         * This function is used to initialize the openGL.
         * @return <bool>
         */
        bool initGL () {
            bool success = true;
            GLenum error = GL_NO_ERROR;

            glClearColor(0, 0, 0, 0);
            glClearDepth(1.0f);

            glViewport(0, 0, (WIDTH * SCALE), (HEIGHT * SCALE));

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glOrtho(0, (WIDTH * SCALE), (HEIGHT * SCALE), 0, 1, -1);

            glMatrixMode(GL_MODELVIEW);

            glDisable(GL_TEXTURE_2D);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_TEXTURE_2D);
        
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glLoadIdentity();    

            glClearColor(
                    (float)(0/255),
                    (float)(0/255),
                    (float)(0/255),
                    1.0f
                    );


            return success;
        }


        /**
         * This function is used to initialize the display/window with
         * the OpenGL context.
         * @return <bool>
         */
        bool init () {
            bool success = true;

            if (!SDL_Init(SDL_INIT_VIDEO) < 0) {
                printf("Could not initialize video", SDL_GetError());
                success = false;
            } else {
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

                display = SDL_CreateWindow (
                        "App Title",
                        0,
                        0,
                        WIDTH * SCALE,
                        HEIGHT * SCALE,
                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
                        );
                renderer = SDL_CreateRenderer(display, -1, SDL_RENDERER_ACCELERATED);

                if (display == NULL) {
                    printf("Could not create display", SDL_GetError());
                } else {
                    context = SDL_GL_CreateContext(display);

                    if (context == NULL) {
                        printf("Could not create context", SDL_GetError());
                        success = false;
                    } else {
                        if (!initGL()) {
                            printf("Could not initialize OpenGL", SDL_GetError());
                            success = false;
                        }
                    }
                }
            }
            
            int argc = 1;
            char *argv[1] = {(char*)"Something"};
            glutInit(&argc, argv);

            return success;
        }


        /**
         * Tick/Update function.
         */
        void tick (float delta) {
            Scene * scene = getCurrentScene();
            if (scene->initialized == false) {
                scene->initialize(delta);
                scene->initialized = true;
            }

            scene->tick(delta);
        }


        /**
         * This function is used to draw.
         */
        void draw (float delta) {
            Scene * scene = getCurrentScene();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();
            //glTranslatef((WIDTH * SCALE)/2, (HEIGHT * SCALE)/2, 0.0f);
            //glScalef(scene->camera->zoom, scene->camera->zoom, 1.0f);
            //glTranslatef(-(WIDTH * SCALE)/2, -(HEIGHT * SCALE)/2, 0.0f);
            glTranslatef(-scene->camera->x, -scene->camera->y, 0.0f);
            scene->draw(delta);
            glPopMatrix();
            
            //glPushMatrix();
            //scene->camera->draw(delta);
            //glPopMatrix();
        }

        void textEvent(string text) {
            getCurrentScene()->textEvent(text);
        }

        void keyUpEvent(SDL_Event &e) {
            getCurrentScene()->keyUpEvent(e);
        }

        Scene* getCurrentScene() {
            return scenes[sceneIndex];
        }


        /**
         * This function is used to terminating and killing the program.
         */
        void close () {
            SDL_DestroyWindow(display);
            display = NULL;
            SDL_Quit(); 
        }
};
