#include <stdio.h>           // Standard C/C++ Input-Output
#include <math.h>            // Math Functions
#ifdef WIN32
#include <windows.h>         // Standard Header For MSWindows Applications
#endif

#include <time.h>
#include <unistd.h>


#ifdef WIN32
#include <windows.h>
#endif
#include "SOIL.h"
#include <stdlib.h>
#include <cmath>

#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif
// The Following Directive Fixes The Problem With Extra Console Window
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

// Uncomment the following if you want to use the glut library from the current directory
//#pragma comment(lib, "lib/glut32.lib")

// Global Variables
bool g_gamemode;				// GLUT GameMode ON/OFF
bool g_fullscreen;				// Fullscreen Mode ON/OFF (When g_gamemode Is OFF)
bool b_culling = false;			// Culling Enabled is Mandatory for this assignment do not change
float aspect = 1;


// angle of rotation for the camera direction
float angle_theta=0.0;
float angle_phi=0.0;
// actual vector representing the camera's direction
float lx=0.0f,ly=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,y=1.0f,z=3.0f;


const float PI = 3.1415926535897932384626433832795028;
const float epsilon = 0.001;


//Function Prototypes
void render(void);
void initLights(void);
bool init(void);
void reshape(int w,int h);
void keyboard(unsigned char key, int x, int y);
void special_keys(int a_keys, int x, int y);
void drawAxes();
void timer( int value );
void update_propellers_speed(const double dt);
void DrawPropeller();
void DrawWing();
void DrawBody();
void DrawFire();
void DrawWingCircle();
void DrawBomb();
void DrawRocket();
void DrawCylinder();
void DrawAirplane();
void DrawSun();
void DrawEarth();
void DrawMoon();
int LoadGLTextures();
void DrawStar(float x,float y);
void makeAflip();
float rspeed_propeller=0.0;
float rspeed_airplane_flip=0.0;
int flipflag=0;
float flipCounter=0.0;
float generalspeed=0.0;
float earthSpin=0.0;
float sunRotateSpeed=0.0;
float colorCounter=0;

float red=0;
float green=0;
float blue=0;

GLuint	texture[2];			// Storage For One Texture ( NEW )
GLUquadricObj *pSphere = NULL;


void initLights(void) {
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    
    //Add general light1
    GLfloat lightColor1[] = {0.5, 0.5, 0.5, 1}; //Color (0.5, 0.2, 0.2)
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    
    // Quick And Dirty Lighting (Assumes Light0 Is Set Up)
    glEnable(GL_LIGHTING);							   // Enable Lighting
}

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
    /* load an image file directly as a new OpenGL texture */
    texture[0] = SOIL_load_OGL_texture
    (
     "/Users/michaelpanayiotou/Documents/CG Final Project/CG Final Project/Data/Earth.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_INVERT_Y
     );
    
    if(texture[0] == 0)
        return false;
    
    
    //Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    /* load an image file directly as a new OpenGL texture */
    texture[1] = SOIL_load_OGL_texture
    (
     "/Users/michaelpanayiotou/Documents/CG Final Project/CG Final Project/Data/Moon.bmp",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_INVERT_Y
     );
    
    if(texture[0] == 0)
        return false;
    
    
    //Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    return true;										// Return Success
}
void update_propellers_speed( const double dt )
{
    // in degrees per second
    const float SPEED = 400.0f;
    
    sunRotateSpeed+=0.5;
    colorCounter+=0.01;
    rspeed_propeller += ( SPEED * dt );
    rspeed_airplane_flip += ( SPEED * dt );
    generalspeed += ( SPEED * dt );
    earthSpin +=( SPEED/10 * dt );
    flipCounter+=0.03;
    flipflag=1;
    
    glClearColor(red, green, blue, 0.5f);			   // Black Background (CHANGED)
    
    //    if (fmod(flipCounter, 100)==0) {
    //
    //        flipflag=1;
    //    }
    //    if (fmod(flipCounter, 200)==0) {
    //
    //        flipflag=0;
    //    }
    
    
    
    
}

void makeAflip(){
    
    
    glRotatef(cos(flipCounter*1)*20, 0, 0, 1);
    
}
void makeArocketflip(){
    
    
    glRotatef(cos(flipCounter*2.5)*25, 0, 1, 1);
    
    
}
// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};
// Our GL Specific Initializations. Returns true On Success, false On Fail.

bool init(void)
{
    if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
    {
        char * dir = getcwd(NULL, 0); // Platform-dependent, see reference link below
        printf("Current dir: %s", dir);
        
        return false;									// If Texture Didn't Load Return FALSE
    }
    
    glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    
    pSphere = gluNewQuadric();
    gluQuadricDrawStyle(pSphere, GLU_FILL);
    gluQuadricNormals(pSphere, GLU_SMOOTH);
    gluQuadricTexture(pSphere, GLU_TRUE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);             // Pixel Storage Mode To Byte Alignment
    glEnable(GL_TEXTURE_2D);                           // Enable Texture Mapping
    glClearColor(red, blue, green, 0.5f);			   // Black Background (CHANGED)
    glClearDepth(1.0f);								   // Depth Buffer Setup
    glDepthFunc(GL_LEQUAL);							   // The Type Of Depth Testing To Do
    glEnable(GL_DEPTH_TEST);						   // Enables Depth Testing
    //glShadeModel(GL_SMOOTH);						   // Enable Smooth Shading
    initLights();
    glEnable(GL_COLOR_MATERIAL);					   // Enable Material Coloring
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Hint for nice perspective interpolation
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);	// Set the color tracking for both faces for both the ambient and diffuse components
    glEnable(GL_NORMALIZE);
    glFrontFace(GL_CCW);                               //Counter Clock Wise definition of the front and back side of faces
    glCullFace(GL_BACK);                               //Hide the back side
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    GLfloat mat_specular[] = { 0.7, 0.7, 0.7, 1 };
    GLfloat mat_shininess[] = { 10.0 };
    glShadeModel (GL_SMOOTH);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    //Add positioned light
    
    
    glDisable(GL_SPECULAR);
    
    
    return true;
}


void positionCamera(){
    
    glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
    
    glLoadIdentity();                // Reset The Projection Matrix
    gluPerspective(45.0f, aspect, 0.1, 100.0);
    gluLookAt(0.0f,0.0f,30.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f);
    
    
    //camera transformations go here
    glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
    
}

// Our Reshaping Handler (Required Even In Fullscreen-Only Modes)
void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    // Calculate The Aspect Ratio And Set The Clipping Volume
    if (h == 0) h = 1;
    aspect = (float)w/(float)h;
    positionCamera();
    glMatrixMode(GL_MODELVIEW);      // Select The Modelview Matrix
    glLoadIdentity(); // Reset The Modelview Matrix
    initLights();
}

// Our Keyboard Handler (Normal Keys)
void processNormalKeys(unsigned char key, int x, int y)
{
    float fraction = 1.5f;
    
    switch (key) {
        case 'w':
            x += lx * fraction;
            z += lz * fraction;
            break;
        case 's':
            x -= lx * fraction;
            z -= lz * fraction;
            break;
            
        case 27:        // When Escape Is Pressed...
            exit(0);    // Exit The Program
            break;      // Ready For Next Case
        default:
            break;
    }
    glutPostRedisplay();
}


// Our Keyboard Handler For Special Keys (Like Arrow Keys And Function Keys)
void processSpecialKeys(int key, int xx, int yy) {
    
    float fraction = 0.1f;
    
    switch (key) {
        case GLUT_KEY_LEFT :
            angle_theta -= 0.01f;
            lx = sin(angle_theta);
            lz = -cos(angle_theta);
            break;
        case GLUT_KEY_RIGHT :
            angle_theta += 0.01f;
            lx = sin(angle_theta);
            lz = -cos(angle_theta);
            break;
        case GLUT_KEY_UP :
            angle_phi -= 0.01f;
            ly = sin(angle_phi);
            break;
        case GLUT_KEY_DOWN :
            angle_phi += 0.01f;
            ly = sin(angle_phi);
            break;
    }
}
void timer( int value )
{
    glutPostRedisplay();
    glutTimerFunc( 16, timer, 0 );
}



// Main Function For Bringing It All Together.
int main(int argc, char** argv)
{
    
    glutInit(&argc, argv);                           // GLUT Initializtion
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE); // (CHANGED)|
    if (g_gamemode) {
        glutGameModeString("1024x768:32");            // Select 1024x768 In 32bpp Mode
        if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
            glutEnterGameMode();                     // Enter Full Screen
        else
            g_gamemode = false;                     // Cannot Enter Game Mode, Switch To Windowed
    }
    if (!g_gamemode) {
        glutInitWindowPosition(100, 100);
        glutInitWindowSize(1000, 1000);                  // Window Size If We Start In Windowed Mode
        glutCreateWindow("MHYP-443"); // Window Title
    }
    if (!init()) {                                   // Our Initialization
        fprintf(stderr,"Initialization failed.");
        return -1;
    }
    glutDisplayFunc(render);                     // Register The Display Function
    glutTimerFunc( 0, timer, 0 );
    glutReshapeFunc(reshape);                    // Register The Reshape Handler
    glutKeyboardFunc(processNormalKeys);                  // Register The Keyboard Handler
    glutSpecialFunc(processSpecialKeys);               // Register Special Keys Handler
    glutIdleFunc(NULL);                        	 // We Do Rendering In Idle Time
    glutSetCursor(GLUT_CURSOR_NONE);
    // OpenGL init
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();                              // Go To GLUT Main Loop
    
    return 0;
}

void DrawSun(){
    
    glPushMatrix();
    
    GLfloat lightColor0[] = {1, 1, 1, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {0, 0, 3, 1.0f}; //Positioned at (0, 0, 6)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    
    
    GLfloat lightColor1[] = {1, 1, 1, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos1[] = {10, 0, 13, 1.0f}; //Positioned at (0, 0, 6)
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos1);
    
    glScalef(1, 1, 1);
    glColor4f(1, 0.4, 0, 1);
    glutSolidSphere(1, 50, 50);
    glColor4f(1, 0.7, 0, 0.8);
    glutSolidSphere(1.5, 50, 50);
    glColor4f(1, 0.8, 0, std::abs(sin(flipCounter+90)));
    glutSolidSphere(2, 50, 50);
    glColor4f(1, 1, 0, std::abs(cos(flipCounter))-0.4);
    glutSolidSphere(2.5, 50, 50);
    glPopMatrix();
    
}
void DrawEarth(){
    
    
    glPushMatrix();
    glRotatef(-earthSpin, 0, 1, 0 );
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glColor3f(1.0,1.0,1.0);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPushMatrix();
    glRotatef( -90.0, 1.0, 0.0, 0.0 );
    gluSphere(pSphere, 4, 18, 18);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopMatrix();
}
void DrawMoon(){
    
    glPushMatrix();
    glRotatef(earthSpin/2, 1, 0, 0 );
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture[1]);
    glColor3f(1.0,1.0,1.0);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPushMatrix();
    glRotatef( -90.0, 1.0, 0.0, 0.0 );
    gluSphere(pSphere, 1, 18, 18);
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
    glPopMatrix();
    
}
void DrawStar(int x,int y){
    
    glPushMatrix();
    glTranslatef(x, y, -30);
    glColor4f(1, 1, 1, 1);
    glutSolidSphere(0.04, 50, 50);
    glPopMatrix();
    
}

void DrawAirplane(){
    
    //Planes Reflection
    GLfloat lightColor2[] = {0.3, 0.3, 0.3, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos2[] = {0, -4, 0, 1.0f}; //Positioned at (0, 0, 6)
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
    
    glPushMatrix();
    if (flipflag==1) {
        makeAflip();
    }
    glColor3f(0.32, 0.34, 0.145);
    
    //Body
    glPushMatrix();
    glTranslatef(0, -0.7, -5);
    DrawBody();
    glPopMatrix();
    
    //Wings
    glPushMatrix();
    DrawWing();
    glPopMatrix();
    
    
    //Left Bomb
    glPushMatrix();
    glScaled(0.75, 0.75, 0.75);
    glTranslatef(-8, -3.5, -5);
    
    glPushMatrix();
    glTranslatef(0, 4, 4.5);
    DrawCylinder();
    glPopMatrix();
    
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    DrawBomb();
    glPopMatrix();
    
    //Left 2 Bomb
    glPushMatrix();
    glScaled(0.65, 0.65, 0.65);
    glTranslatef(-16, -3.7, -5);
    glPushMatrix();
    glTranslatef(0, 4, 4.5);
    DrawCylinder();
    glPopMatrix();
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    DrawBomb();
    glPopMatrix();
    //Right Bomb
    glPushMatrix();
    glScaled(0.75, 0.75, 0.75);
    glTranslatef(8, -3.5, -5);
    
    glPushMatrix();
    glTranslatef(0, 4, 4.5);
    DrawCylinder();
    glPopMatrix();
    
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    DrawBomb();
    glPopMatrix();
    //Right 2 Bomb
    glPushMatrix();
    glScaled(0.65, 0.65, 0.65);
    glTranslatef(16, -3.7, -5);
    glPushMatrix();
    glTranslatef(0, 4, 4.5);
    DrawCylinder();
    glPopMatrix();
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    DrawBomb();
    glPopMatrix();
    //Right Propeller
    glPushMatrix();
    glTranslatef(-6, 0, 3.0);
    glRotatef(rspeed_propeller+45, 0, 0, 1);
    glScaled(1.25, 1.25, 0.75);
    DrawPropeller();
    glPopMatrix();
    
    //Right 2 Propeller
    glPushMatrix();
    glTranslatef(-10, 0, 2.6);
    glRotatef(rspeed_propeller, 0, 0, 1);
    glScaled(0.75, 0.75, 0.75);
    DrawPropeller();
    glPopMatrix();
    
    //Left Propeller
    glPushMatrix();
    glTranslatef(6, 0, 3.0);
    glRotatef(rspeed_propeller, 0, 0, 1);
    glScaled(1.25, 1.25,  0.75);
    DrawPropeller();
    glPopMatrix();
    
    //Left Propeller
    glPushMatrix();
    glTranslatef(10, 0, 2.6);
    glRotatef(rspeed_propeller+45, 0, 0, 1);
    glScaled(0.75, 0.75, 0.75);
    DrawPropeller();
    glPopMatrix();
    
    glPopMatrix();
    
}
void DrawCylinder(){
    glColor3f(0, 0.5, 0);
    glPushMatrix();
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quadratic,0.7f,0.7f,3.6f,32,32);
    glPopMatrix();
}
void DrawFire(){
    
    int k=1;
    
    glScaled(0.8, 0.8,2);
    glPushMatrix();
    glColor4f(1, 0, 0,1);
    
    glutSolidSphere(2,20,20);
    glPopMatrix();
    glPushMatrix();
    glColor4f(1, 0, 0,0.5);
    glutSolidSphere(3,20,20);
    glPopMatrix();
    glPushMatrix();
    glColor4f(1, 0.3, 0,0.7);
    glutSolidSphere(3.5,20,20);
    
    
    glColor4f(1, 1, 0, std::abs(sin(flipCounter*19+90)));
    glutSolidSphere(3.8,20,20);
    
    
    glPopMatrix();
    
    
}
void DrawRocket(){
    
    glPushMatrix();
    
    glColor3f(1, 1, 1);
    
    //cylinder
    glPushMatrix();
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0, 0.3, -7);
    glScaled(0.85, 0.75, 3.5);
    gluCylinder(quadratic,2.1f,2.3f,5.0f,32,32);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0.3, 10.2);
    glColor3f(1, 0, 0);
    glScaled(0.95, 0.85, 1.8);
    glutSolidSphere(2, 20, 20);
    glPopMatrix();
    
    
    glPushMatrix();
    
    glColor3f(1, 1, 1);
    glTranslatef(0, 0.3, -9);
    glutSolidSphere(1.6, 20, 20);
    
    //Fire
    
    glPushMatrix();
    
    glTranslatef(0, 0, -9.2);
    DrawFire();
    
    glPopMatrix();
    //ftero
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0, -0.5 , 2);
    glScaled(1.7, 0.15,0.7);
    glutSolidSphere(3.5,20,20);
    glPopMatrix();
    //ftero
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0, 0.5 , 2);
    glScaled(1.7, 0.15,0.7);
    glutSolidSphere(3.5,20,20);
    glPopMatrix();
    //ftero
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(0.5, 0 , 2);
    glScaled(0.15, 1.7,0.7);
    glutSolidSphere(3.5,20,20);
    glPopMatrix();
    //ftero
    glPushMatrix();
    glColor3f(1, 0, 0);
    glTranslatef(-0.5, 0 , 2);
    glScaled(0.15, 1.7,0.7);
    glutSolidSphere(3.5,20,20);
    glPopMatrix();
    
    
    glPopMatrix();
    
    glPopMatrix();
    
    
}
void DrawBomb(){
    
    glPushMatrix();
    
    glColor3f(1, 1, 1);
    
    
    glPushMatrix();
    glTranslatef(0, 0, 4.2);
    glColor3f(0.4, 0.4, 0.4);
    glScaled(0.95, 0.85, 2.3);
    glutSolidSphere(2, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, 8.5);
    glColor3f(1, 0, 0);
    // glScaled(0.95, 0.85, 2.3);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    
    glColor3f(0, 0.5, 0);
    glTranslatef(0, 0, 2);
    //glutSolidSphere(1.6, 20, 20);
    
    
    //ftero
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(0, 0 , 0);
    glScaled(1.2, 0.15,0.7);
    glutSolidSphere(2.5,20,20);
    glPopMatrix();
    
    //ftero
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslatef(0, 0 , 0);
    glScaled(0.15, 1.2,0.7);
    glutSolidSphere(2.5,20,20);
    glPopMatrix();
    
    
    
    glPopMatrix();
    
    glPopMatrix();
    
    
}
void DrawBody(){
    glPushMatrix();
    
    glPushMatrix();
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(0, 0.3, -7);
    glColor3f(0.32, 0.34, 0.38);
    // glColor3f(0.32, 0.34, 0.145);
    glScaled(0.85, 0.75, 3.5);
    gluCylinder(quadratic,2.1f,2.3f,5.0f,32,32);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0.1, 10.2);
    glScaled(0.95, 0.85, 1.8);
    glutSolidSphere(2.1, 20, 20);
    glPopMatrix();
    //tzami
    glPushMatrix();
    glTranslatef(0, 0.6, 9.8);
    glColor4f(0, 1, 1,0.5);
    glScaled(0.95, 0.85, 1.8);
    glutSolidSphere(2, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    
    glColor3f(0.32, 0.34, 0.145);
    glTranslatef(0, 0.3, -7.2);
    glutSolidSphere(1.6, 20, 20);
    //ftero
    glPushMatrix();
    
    glColor3f(0, 0.8, 0);
    glTranslatef(0, 0 , 1);
    glScaled(1.7, 0.15,0.7);
    glutSolidSphere(3.5,20,20);
    glPopMatrix();
    //ftero
    glPushMatrix();
    
    glColor3f(0, 0.8, 0);
    glTranslatef(0, 2 , 1);
    glScaled(0.15, 1,0.4);
    glutSolidSphere(3.5,20,20);
    glPopMatrix();
    
    glPopMatrix();
    
    glPopMatrix();
}
void DrawWing(){
    
    glPushMatrix();
    
    glColor3f(0.32, 0.34, 0.38);
    glScaled(14, 0.45, 2.5);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
    
    /*glPushMatrix();
     
     glColor4f(0, 1, 0,0.2);
     glScaled(14, 0.45, 2.5);
     glutSolidSphere(1.1, 20, 20);
     glPopMatrix();
     */
    glPushMatrix();
    
    glTranslatef(-8, 0.45, 0);
    DrawWingCircle();
    
    glTranslatef(16, 0, 0);
    DrawWingCircle();
    glPopMatrix();
    
    
}
void DrawWingCircle(){
    //circle blue
    glPushMatrix();
    glColor3f(0.32, 0.34, 0.78);
    //glColor3f(0.3, 0.3, 1);
    glScaled(0.7, 0.01, 0.7);
    glutSolidSphere(2, 20, 20);
    glPopMatrix();
    //circle red
    glPushMatrix();
    glTranslatef(0, 0.05, 0);
    glColor3f(0, 1, 0);
    glScaled(0.7, 0.01, 0.7);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
    
}
void DrawPropeller(){
    
    glPushMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, -1);
    glColor3f(0.5,0.5, 1);
    glScaled(0.5, 0.5,0.5);
    glutSolidCone(1, 2, 20, 20);
    glPopMatrix();
    
    glColor3f(1, 1, 1);
    
    glPushMatrix();
    glScaled(0.5, 0.5,0.5);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 1, 0);
    glScaled(0.25, 0.25,0.25);
    glScaled(1, 4, 	1);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, -1, 0);
    glScaled(0.25, 0.25,0.25);
    glScaled(1, 4, 	1);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(1, 0, 0);
    glScaled(0.25, 0.25,0.25);
    glScaled(4, 1, 	1);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-1, 0, 0);
    glScaled(0.25, 0.25,0.25);
    glScaled(4, 1, 	1);
    glutSolidSphere(1, 20, 20);
    glPopMatrix();
    glPopMatrix();
    
}



// Our Rendering Is Done Here
void render(void)
{
    static int prvMs = glutGet( GLUT_ELAPSED_TIME );
    const int curMs = glutGet( GLUT_ELAPSED_TIME );
    
    // dt is in seconds
    const double dt = ( curMs - prvMs ) / 1000.0;
    prvMs = curMs;
    
    // update world state
    update_propellers_speed( dt );
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    
    // Do we have culling enabled?
    if (b_culling == true)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    
    
    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(	x, y, z,
              x+lx, y+ly,  z+lz,
              0.0f, 1.0f,  0.0f);
    
    
    glDisable(GL_LIGHTING);
    
    //    for (int y=-100; y<100; y++) {
    //        for(int x=-100; x<100; x++)
    //
    //            DrawStar(x,y);
    //    }
    //
    glEnable(GL_LIGHTING);
    //
    //glScaled(2, 2, 2);
    
    
    //Draw Sun Matrix
    glPushMatrix();
    //  glRotatef(-18, 0, 1, 0);
    glRotatef(sunRotateSpeed, 0, 0, 1); //rotation around x
    float rotation;
    rotation=(1+sin(sunRotateSpeed/56))/2;
    red=0.529*rotation;
    green=0.81*rotation;
    blue=0.98*rotation;
    // blue=
    glTranslatef(15, 15, -20);
    
    glScaled(1, 1, 1);                  //2 times bigger
    
    DrawSun();
    glPopMatrix();
    
    //    //Draw Earth at the center of everything
    
    DrawEarth();
    
    
    
    //Draw Plane Matrix
    glPushMatrix();
    glRotatef(-60*cos(generalspeed/1500), 0, 1, 0);
    glRotatef(1.5*sunRotateSpeed, 1, 0, 0);
    
    glTranslatef(0, 7, 0);
    glScaled(0.1, 0.1, 0.1);
    
    glPushMatrix();
    
    glTranslatef(0, 0, -38-12*sin(generalspeed/350));
    glScaled(0.3, 0.3, 0.3);
    if (flipflag==1) {
        makeArocketflip();
    }
    DrawRocket();
    glPopMatrix();
    
    DrawAirplane();
    glPopMatrix();
    
    
    //Draw Moon Matrix
    glPushMatrix();
    //glRotatef(-18, 0, 1, 0);
    glRotatef(sunRotateSpeed, 0, 0, 1);
    
    
    glTranslatef(-10, -10, -10);
    
    glScaled(1, 1, 1);
    DrawMoon();
    glPopMatrix();
    
    
    // Swap The Buffers To Make Our Rendering Visible
    glutSwapBuffers();
    glutPostRedisplay(); //animation
    
}
