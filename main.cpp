#include <GL/freeglut.h>
#include <cmath>

// Global variables
float angle = 0.0f;
float waveTime = 0.0f;

// --- Wave Function ---
float getWaveHeight(float x, float z) {
    return 0.12f * sin(4.0f * x + waveTime) +
           0.08f * cos(3.0f * z + waveTime * 1.3f) +
           0.04f * sin(2.0f * x + 2.5f * z + waveTime * 0.7f);
}

// --- Normal Calculation ---
void computeNormal(float x, float z, float &nx, float &ny, float &nz) {
    float delta = 0.01f;

    float hL = getWaveHeight(x - delta, z);
    float hR = getWaveHeight(x + delta, z);
    float hD = getWaveHeight(x, z - delta);
    float hU = getWaveHeight(x, z + delta);

    nx = hL - hR;
    ny = 2.0f * delta;
    nz = hD - hU;

    float len = sqrt(nx * nx + ny * ny + nz * nz);
    nx /= len;
    ny /= len;
    nz /= len;
}

// --- Ocean Rendering ---
void drawOcean() {
    int grid = 180;
    float size = 6.0f;

    for (int x = -grid; x < grid; x++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int z = -grid; z <= grid; z++) {
            for (int i = 0; i < 2; i++) {

                float X = (x + i) * size / grid;
                float Z = z * size / grid;
                float Y = getWaveHeight(X, Z);

                float nx, ny, nz;
                computeNormal(X, Z, nx, ny, nz);

                float fresnel = 0.35f + 0.65f * fabs(nx);

                float r = 0.0f + 0.15f * fresnel;
                float g = 0.25f + 0.35f * fresnel;
                float b = 0.45f + 0.55f * fresnel;

                glColor3f(r, g, b);
                glNormal3f(nx, ny, nz);
                glVertex3f(X, Y, Z);
            }
        }
        glEnd();
    }
}

// --- Mist Effect ---
void drawMist() {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    for (int i = 0; i < 10; i++) {
        float h = 0.05f + i * 0.1f;
        float alpha = 0.4f - i * 0.03f;

        glColor4f(0.95f, 0.95f, 0.95f, alpha);
        glVertex3f(-7.0f, h, -4.0f);
        glVertex3f(7.0f, h, -4.0f);
        glVertex3f(7.0f, h, 4.0f);
        glVertex3f(-7.0f, h, 4.0f);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// --- Sun ---
void drawSun() {
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 0.92f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0f, 1.3f, -2.5f);
    glutSolidSphere(0.15f, 40, 40);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

// --- Sun Reflection ---
void drawSunReflection() {
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glBegin(GL_TRIANGLE_STRIP);
    for (float i = -0.4f; i <= 0.4f; i += 0.02f) {
        glColor4f(1.0f, 0.9f, 0.6f, 0.25f);
        glVertex3f(i, 0.0f, -1.5f);

        glColor4f(1.0f, 0.8f, 0.4f, 0.0f);
        glVertex3f(i, 0.0f, 3.5f);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// --- Lighting ---
void setupLighting() {
    GLfloat light_pos[] = { 1.5f, 1.0f, 0.3f, 0.0f };
    GLfloat ambient[]   = { 0.20f, 0.20f, 0.25f, 1.0f };
    GLfloat diffuse[]   = { 1.0f, 0.9f, 0.7f, 1.0f };
    GLfloat specular[]  = { 1.0f, 0.95f, 0.85f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);
}

// --- Display ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float camX = 3.8f * sin(angle);
    float camZ = 3.8f * cos(angle);

    gluLookAt(camX, 0.7f, camZ,
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    setupLighting();

    drawOcean();
    drawMist();
    drawSun();
    drawSunReflection();

    glutSwapBuffers();
}

// --- Timer ---
void timer(int) {
    angle += 0.007f;
    waveTime += 0.05f;

    if (angle > 2 * M_PI)
        angle -= 2 * M_PI;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// --- Init ---
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glShadeModel(GL_SMOOTH);

    glClearColor(0.6f, 0.85f, 1.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 16.0 / 9.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// --- Main ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);

    glutCreateWindow("Real-Time Ocean Simulation");

    initOpenGL();

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
