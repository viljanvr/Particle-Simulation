// ParticleToy.cpp : Defines the entry point for the console application.
//

#include <EulerScheme.h>
#include <ImplicitEulerScheme.h>
#include <IntegrationScheme.h>
#include <MidpointScheme.h>
#include <RungeKuttaScheme.h>


#include "Constraint.h"
#include "Force.h"
#include "Particle.h"
#include "ScenePresets.h"
#include "SpringForce.h"
#include "gfx/vec2.h"
#include "imageio.h"

#include <BlowForce.h>
#include <LinearForce.h>
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#if defined(__APPLE__) && defined(__aarch64__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* macros */

/* external definitions (from solver) */
extern void simulation_step(const std::vector<Particle *> &pVector, const std::vector<Force *> &fVector,
                            const std::vector<Constraint *> &cVector, const std::vector<CollideableObject *> &oVector,
                            const float dt, IntegrationScheme &integration_scheme);

/* global variables */

static int N;
static float dt, d;
static int dsim;
static int dump_frames;
static int frame_number;

static int win_id;
static int win_x, win_y;
static int mouse_state[3] = {GLUT_UP};
static int omx, omy, mx, my; // Old mouse position, and new mouse position
static int hmx, hmy; // Hover mouse position

static bool visualizeForces = false;
static std::vector<Particle *> pVector;
static std::vector<Force *> fVector;
static std::vector<Constraint *> cVector;
static std::vector<CollideableObject *> oVector;
static Force *mouse_interact_force;
static BlowForce *mouse_blow_force;
static std::unique_ptr<Particle> mouse_particle = std::make_unique<Particle>(Vec2f(mx, my), visualizeForces, 100);
static std::unique_ptr<IntegrationScheme> integration_scheme = std::make_unique<RungeKuttaScheme>();
extern std::string currentSceneName;

/*
----------------------------------------------------------------------
free/clear/allocate simulation data
----------------------------------------------------------------------
*/


// Clears the content of the particle, force and constraints vectors
static void clear_vector_data(void) {
    for (auto p: pVector) {
        delete p;
    }
    for (auto f: fVector) {
        delete f;
    }
    for (auto c: cVector) {
        delete c;
    }
    for (auto o: oVector) {
        delete o;
    }
    pVector.clear();
    fVector.clear();
    cVector.clear();
    oVector.clear();
}

// Frees all data
static void free_data() {
    clear_vector_data();

    delete mouse_interact_force;
    mouse_interact_force = nullptr;
}

static void init_system(void) { set_scene(1, pVector, fVector, cVector, oVector, visualizeForces); }

/*
----------------------------------------------------------------------
OpenGL specific drawing routines
----------------------------------------------------------------------
*/

static void pre_display(void) {
    glViewport(0, 0, win_x, win_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void post_display(void) {
    // Write frames if necessary.
    if (dump_frames) {
        const int FRAME_INTERVAL = 4;
        if ((frame_number % FRAME_INTERVAL) == 0) {
            const unsigned int w = glutGet(GLUT_WINDOW_WIDTH);
            const unsigned int h = glutGet(GLUT_WINDOW_HEIGHT);
            unsigned char *buffer = (unsigned char *) malloc(w * h * 4 * sizeof(unsigned char));
            if (!buffer)
                exit(-1);
            // glRasterPos2i(0, 0);
            glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            static char filename[80];
            sprintf(filename, "../snapshots/img%.5i.png", frame_number / FRAME_INTERVAL);
            printf("Dumped %s.\n", filename);
            saveImageRGBA(filename, buffer, w, h);

            free(buffer);
        }
    }
    frame_number++;

    glutSwapBuffers();
}

static void draw_particles(void) {
    for (auto p: pVector) {
        p->draw();
    }
}

static void draw_forces(void) {
    for (auto f: fVector) {
        f->draw();
    }
}

static void draw_constraints(void) {
    for (auto c: cVector) {
        c->draw();
    }
}

static void draw_collidable_objects() {
    for (auto o: oVector) {
        o->draw();
    }
}

static int getBitmapStringWidth(const std::string &text, void *font) {
    int width = 0;
    for (char c: text) {
        width += glutBitmapWidth(font, c);
    }
    return width;
}

static void draw_title() {
    void *font = GLUT_BITMAP_HELVETICA_18;
    // Center depends on string length
    float centered_x = -static_cast<float>(getBitmapStringWidth(currentSceneName, font)) / static_cast<float>(win_x);

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(centered_x, 0.9);
    for (char c: currentSceneName) {
        glutBitmapCharacter(font, c);
    }
}

/*
----------------------------------------------------------------------
relates mouse movements to particle toy construction
----------------------------------------------------------------------
*/

// static Vec2f getAverageParticlePosition() {
//     Vec2f sum(0.0f, 0.0f);
//     for (auto* p : pVector) {
//         sum += p->m_Position;
//     }
//     return sum / static_cast<float>(pVector.size());
// }

static void add_interact_force(int button) {
    float x_scaled = mx / (float) win_x * 2.0 - 1.0;
    float y_scaled = (win_y - my) / (float) win_y * 2.0 - 1.0;

    if (button == GLUT_LEFT_BUTTON && !mouse_interact_force) {
        mouse_particle->m_Position[0] = x_scaled;
        mouse_particle->m_Position[1] = y_scaled;
        for (auto p: pVector) {
            float dx = mouse_particle->m_Position[0] - p->m_Position[0];
            float dy = mouse_particle->m_Position[1] - p->m_Position[1];
            if (dx * dx + dy * dy < 0.05) {
                mouse_interact_force = new SpringForce(mouse_particle.get(), p, 0.0, 1.0, 10.0);
                fVector.push_back(mouse_interact_force);
                break;
            }
        }
    } else if (button == GLUT_RIGHT_BUTTON && !mouse_blow_force) {
        // Only creation, updating done in handle_user_interaction()
        mouse_blow_force = new BlowForce(pVector, Vec2f(0.0f, 0.0f), 0.05f, 0.6f);
        fVector.push_back(mouse_blow_force);
    }
}

static void remove_interact_force() {
    if (mouse_interact_force) {
        fVector.erase(std::remove(fVector.begin(), fVector.end(), mouse_interact_force), fVector.end());
        delete mouse_interact_force;
        mouse_interact_force = nullptr;
    } else if (mouse_blow_force) {
        fVector.erase(std::remove(fVector.begin(), fVector.end(), mouse_blow_force), fVector.end());
        delete mouse_blow_force;
        mouse_blow_force = nullptr;
    }
}

static void handle_user_interaction() {
    // To scale from pixel coordinate to [-1, 1].
    float x_scaled = mx / (float) win_x * 2.0 - 1.0;
    float y_scaled = (win_y - my) / (float) win_y * 2.0 - 1.0;
    // Particle movement
    if (mouse_state[GLUT_LEFT_BUTTON] == GLUT_DOWN) {
        // Update position of (the imaginary) mouse_particle while holding down left mouse
        mouse_particle->m_Position[0] = x_scaled;
        mouse_particle->m_Position[1] = y_scaled;
    } else if (mouse_state[GLUT_RIGHT_BUTTON] == GLUT_DOWN && mouse_blow_force) {
        mouse_blow_force->setOrigin(Vec2f(x_scaled, y_scaled));
    }
    omx = mx;
    omy = my;
}

static void remap_GUI() {
    int ii, size = pVector.size();
    for (ii = 0; ii < size; ii++) {
        pVector[ii]->m_Position[0] = pVector[ii]->m_ConstructPos[0];
        pVector[ii]->m_Position[1] = pVector[ii]->m_ConstructPos[1];
        pVector[ii]->reset();
    }
}

/*
----------------------------------------------------------------------
GLUT callback routines
----------------------------------------------------------------------
*/

static void key_func(unsigned char key, int x, int y) {
    switch (key) {
        case 'c':
        case 'C':
            clear_vector_data();
            break;

        case 'd':
        case 'D':
            dump_frames = !dump_frames;
            break;

        case 'q':
        case 'Q':
            free_data();
            exit(0);
            break;

        case ' ':
            dsim = !dsim;
            break;
        case 'e':
        case 'E':
            integration_scheme = std::make_unique<EulerScheme>();
            std::cout << "Switched to EulerScheme." << std::endl;
            break;
        case 'm':
        case 'M':
            integration_scheme = std::make_unique<MidpointScheme>();
            std::cout << "Switched to MidpointScheme." << std::endl;
            break;
        case 'r':
        case 'R':
            integration_scheme = std::make_unique<RungeKuttaScheme>();
            std::cout << "Switched to RangeKuttaScheme." << std::endl;
            break;
        case 'i':
        case 'I':
            integration_scheme = std::make_unique<ImplicitEulerScheme>();
            std::cout << "Switched to ImplicitEulerScheme." << std::endl;
            break;
        case 'f':
        case 'F':
            visualizeForces = !visualizeForces;
            for (auto particle: pVector) {
                particle->m_forceVisualization = visualizeForces;
            }
        default:
            if (std::isdigit(key)) {
                dsim = 0;
                clear_vector_data();
                set_scene(key - '0', pVector, fVector, cVector, oVector, visualizeForces);
                std::cout << "Switched to scene: " << currentSceneName << "." << std::endl;
            }
    }
}

static void mouse_func(int button, int state, int x, int y) {
    omx = mx = x;
    omx = my = y;

    mouse_state[button] = state;

    // Update hover coordinates
    if (state == GLUT_UP) {
        hmx = x;
        hmy = y;
    }

    if (state == GLUT_DOWN) {
        add_interact_force(button);
    } else if (state == GLUT_UP) {
        remove_interact_force();
    }
}

static void motion_func(int x, int y) {
    mx = x;
    my = y;
}

static void reshape_func(int width, int height) {
    glutSetWindow(win_id);
    glutReshapeWindow(width, height);

    win_x = width;
    win_y = height;
}

static void idle_func(void) {
    handle_user_interaction();
    if (dsim) {
        simulation_step(pVector, fVector, cVector, oVector, dt, *integration_scheme);
    } else {
        remap_GUI();
    }
    glutSetWindow(win_id);
    glutPostRedisplay();
}


static void display_func(void) {
    pre_display();
    draw_collidable_objects();
    draw_forces();
    draw_constraints();
    draw_particles();
    draw_title();

    post_display();
}


/*
----------------------------------------------------------------------
open_glut_window --- open a glut compatible window and set callbacks
----------------------------------------------------------------------
*/

static void open_glut_window(void) {
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(win_x, win_y);
    win_id = glutCreateWindow("Particletoys!");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    pre_display();

    glutKeyboardFunc(key_func);
    glutMouseFunc(mouse_func);
    glutMotionFunc(motion_func);
    glutReshapeFunc(reshape_func);
    glutIdleFunc(idle_func);
    glutDisplayFunc(display_func);
}


/*
----------------------------------------------------------------------
main --- main routine
----------------------------------------------------------------------
*/

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    if (argc == 1) {
        N = 64;
        dt = 0.1f;
        d = 5.f;
        fprintf(stderr, "Using defaults : N=%d dt=%g d=%g\n", N, dt, d);
    } else {
        N = atoi(argv[1]);
        dt = atof(argv[2]);
        d = atof(argv[3]);
        fprintf(stderr, "Using customs : N=%d dt=%g d=%g\n", N, dt, d);
    }

    printf("\n\nHow to use this application:\n\n");
    printf("\t Toggle construction/simulation display with the spacebar key\n");
    printf("\t Dump frames by pressing the 'd' key\n");
    printf("\t Quit by pressing the 'q' key\n");
    printf("\t Switch integration scheme by using 'e', 'm' and 'r'\n");
    printf("\t Toggle force visualization by pressing the 'f' key\n");
    printf("\t Switch to a given scene using '0-9'\n");
    printf("\t Drag particles with a spring force using the left mouse button\n");
    printf("\t Apply a blowing force using the right mouse button \n");

    dsim = 0;
    dump_frames = 0;
    frame_number = 0;

    init_system();

    win_x = 512;
    win_y = 512;
    open_glut_window();

    glutMainLoop();

    exit(0);
}
