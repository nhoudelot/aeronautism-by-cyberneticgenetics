#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdlib.h>
#include "mesh3d.h"
#include "texture.h"
#include "aeronautism.h"
#include "cg_time.h"
#include "shader.h"
#include "particle.h"
#include "poligonexplode.h"

#define WIDTH 20
#define HEIGHT 20

unsigned int screenwidth;
unsigned int screenheight;

/* Meshes */
Mesh3D *monster     = NULL;
Mesh3D *balloon     = NULL;
Mesh3D *terrain     = NULL;
Mesh3D *logo        = NULL;
Mesh3D *dragon      = NULL;
Mesh3D *cave        = NULL;
Mesh3D *tree        = NULL;
Mesh3D *tree_crown  = NULL;
Mesh3D *teleport    = NULL;

/* Attribute array for monster movements */
GLfloat *attrib;

/* Textures */
GLuint  logotex     = 0;
GLuint  ballstiltex = 0;
GLuint  granitetex  = 0;
GLuint  monstertex  = 0;
GLuint  balloontex  = 0;
GLuint  slatetex    = 0;
GLuint  foresttex   = 0;
GLuint  cloudtex    = 0;
GLuint  leaftex     = 0;
GLuint  treetex     = 0;
GLuint  tillingtex  = 0;
GLuint  eyestiltex  = 0;
GLuint  girlstiltex = 0;
GLuint  shadowtex   = 0;
GLuint  greetex     = 0;
GLuint  teleporttex = 0;
GLuint  grasstex    = 0;
GLuint  travistex   = 0;
GLuint  shamentex   = 0;
GLuint  functex     = 0;
GLuint  cgtex       = 0;

/* Shaders */
GLuint  default_prg = 0;
GLuint  cave_prg    = 0;
GLuint  tilling_prg = 0;
GLuint  shadow_prg  = 0;
GLuint  fbreak_prg  = 0;
GLuint  wind_prg    = 0;
GLuint  move_prg    = 0;
GLuint  bunch_prg   = 0;
GLuint  credit_prg  = 0;
GLuint  title_prg   = 0;
GLuint  fade_prg    = 0;

/* Particles */
Particle leftjet;
Particle rightjet;

Triangle *ballexplode = NULL;

/* MAIN DEMO HANDLING UNITS */

/* initialize the canvas for the graphics (logo, balloon) */
void initcanvas(){
   int x,y,i = 0,j = 0,index,c=0;

   logo = malloc(sizeof(Mesh3D));
   logo->vertices = malloc(sizeof(GLfloat) * WIDTH * HEIGHT * 3);
   logo->normals  = malloc(sizeof(GLfloat) * WIDTH * HEIGHT * 3);
   logo->colors   = malloc(sizeof(GLfloat) * WIDTH * HEIGHT * 4);
   logo->texcoords= malloc(sizeof(GLfloat) * WIDTH * HEIGHT * 2);
   logo->indices  = malloc(sizeof(GLfloat) * WIDTH * HEIGHT * 6);

   logo->vcount   = WIDTH * HEIGHT * 3;
   logo->ncount   = WIDTH * HEIGHT * 3;
   logo->ccount   = WIDTH * HEIGHT * 4;
   logo->tcount   = WIDTH * HEIGHT * 2;
   logo->icount   = WIDTH * HEIGHT * 6;

   for(x = 0; x < WIDTH; x++)
      for(y = 0; y < HEIGHT; y++){
         index = y * WIDTH * 3 + x * 3;
	 logo->vertices[index]     = (GLfloat)x * 0.082f;
	 logo->vertices[index + 1] = (GLfloat)y * 0.082f * (800.0 / 600.0);
	 logo->vertices[index + 2] = -2.0f;

	 logo->colors[c++]   = 1.0f;
	 logo->colors[c++]   = 1.0f;
	 logo->colors[c++]   = 1.0f;
	 logo->colors[c++]   = 1.0f;

	 logo->normals[index]      = (GLfloat)x * 0.05f;
	 logo->normals[index + 1]  = (GLfloat)y * 0.05f;
	 logo->normals[index + 2]  = -1.0f;

	 logo->texcoords[j++] = (GLfloat)x / (GLfloat)WIDTH;
	 logo->texcoords[j++] = (GLfloat)y / (GLfloat)HEIGHT;

	 if((y == HEIGHT - 1) || (x == WIDTH - 1)) continue;

	 logo->indices[i++] = y * WIDTH + x;
	 logo->indices[i++] = y * WIDTH + x + WIDTH;
	 logo->indices[i++] = y * WIDTH + x + 1;

	 logo->indices[i++] = y * WIDTH + x + 1;
	 logo->indices[i++] = y * WIDTH + x + WIDTH + 1;
	 logo->indices[i++] = y * WIDTH + x + WIDTH;
      }

   logo->icount = i;
}

void init_aeronautism(unsigned int w, unsigned int h){
   GLfloat amblight[]  = {0.0f , 0.0f , 0.0f , 0.0f};
   GLfloat lowlight[]  = {0.75f, 0.75f, 0.75f, 1.0f};
   GLfloat difflight[] = {1.0f , 1.0f , 1.0f , 1.0f};
   GLfloat lightpos[]  = {1.0f , 1.0f , 1.0f , 1.0f};
   GLuint  cave_shaders[2]    = {0, 0};
   GLuint  tilling_shaders[2] = {0, 0};
   GLuint  shadow_shaders[2]  = {0, 0};
   GLuint  filmbreak_sh[2]    = {0, 0};
   GLuint  wind_shaders[2]    = {0, 0};
   GLuint  default_sh[2]      = {0, 0};
   GLuint  moving_shaders[2]  = {0, 0};
   GLuint  bunch_shaders[2]   = {0, 0};
   GLuint  credit_shaders[2]  = {0, 0};
   GLuint  title_shaders[2]   = {0, 0};
   GLuint  fade_shaders[2]    = {0, 0};
   GLuint  defaultvs          = 0;
   GLuint  defaultfs          = 0;

   /* Screen size settings */
   screenwidth = w;
   screenheight= h;

   /* load meshes */
   terrain     = loadOBJ("/usr/share/aeronautism-by-cyberneticgenetics/data/terrain.obj");
   balloon     = loadOBJ("/usr/share/aeronautism-by-cyberneticgenetics/data/balloon.obj");
   monster     = loadOBJ("/usr/share/aeronautism-by-cyberneticgenetics/data/monster.obj");
   dragon      = loadOBJ("/usr/share/aeronautism-by-cyberneticgenetics/data/dragon.obj");
   cave        = loadOBJ("/usr/share/aeronautism-by-cyberneticgenetics/data/cave.obj");
   tree        = loadOBJ("/usr/share/aeronautism-by-cyberneticgenetics/data/tree.obj");
   tree_crown  = loadOBJ("/usr/share/aeronautism-by-cyberneticgenetics/data/crown.obj");
   teleport    = loadOBJ("/usr/share/aeronautism-by-cyberneticgenetics/data/teleport.obj");
   initcanvas();

   /* texture initialization */
   logotex     = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/aeronautism_logo.png");
   ballstiltex = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/balloon_stil.png");
   granitetex  = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/granite_texture.png");
   balloontex  = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/balloon_texture.png");
   monstertex  = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/monster_texture.png");
   slatetex    = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/swirlyslate_texture.png");
   foresttex   = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/forest_texture.png");
   cloudtex    = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/cloud_texture.png");
   leaftex     = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/leaf_texture.png");
   treetex     = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/tree_texture.png");
   tillingtex  = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/tilling_texture.png");
   eyestiltex  = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/eye_stil.png");
   girlstiltex = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/girl_stil.png");
   shadowtex   = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/shadow_texture.png");
   greetex     = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/greetings_texture.png");
   teleporttex = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/teleport_texture.png");
   grasstex    = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/grass_texture.png");
   travistex   = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/travis_texture.png");
   shamentex   = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/shamen_texture.png");
   cgtex       = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/cybernetic_texture.png");
   functex     = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/function_texture2.png");

   /* particle initialization */
   leftjet     = setupParticle(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
   rightjet    = setupParticle(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

   ballexplode = generateFromMesh(balloon);

   /* opengl related initialization steps */
   glClearColor(0.42,0.5,0.55,1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, 800.0 / 600.0, 1.0, 15000.0);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);

   glEnableClientState(GL_COLOR_ARRAY);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);

   glEnable(GL_TEXTURE_2D);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   /*lighting*/
   glEnable(GL_LIGHTING);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amblight);
   glLightfv(GL_LIGHT0, GL_AMBIENT, lowlight);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, difflight);
   glLightfv(GL_LIGHT0, GL_SPECULAR, difflight);
   glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
   glEnable(GL_LIGHT0);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
   glMaterialfv(GL_FRONT, GL_SPECULAR, difflight);
   glMateriali(GL_FRONT, GL_SHININESS, 10);
   glMaterialfv(GL_FRONT, GL_EMISSION, amblight);

   defaultvs = loadShader(GL_VERTEX_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/default.vs");
   defaultfs = loadShader(GL_FRAGMENT_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/default.fs");

   default_sh[0]       = defaultvs;
   default_sh[1]       = defaultfs;
   default_prg         = createProgram(2, default_sh);

   cave_shaders[0]     = loadShader(GL_FRAGMENT_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/cave.fs");
   cave_shaders[1]     = loadShader(GL_VERTEX_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/moving_monster.vs");
   cave_prg            = createProgram(2, cave_shaders);

   tilling_shaders[0]  = loadShader(GL_FRAGMENT_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/pixelize.fs");
   tilling_shaders[1]  = defaultvs;
   tilling_prg         = createProgram(2, tilling_shaders);

   shadow_shaders[0]   = loadShader(GL_FRAGMENT_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/shadow.fs");
   shadow_shaders[1]   = defaultvs;
   shadow_prg          = createProgram(2, shadow_shaders);

   filmbreak_sh[0]     = loadShader(GL_FRAGMENT_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/fbreak.fs");
   filmbreak_sh[1]     = defaultvs;
   fbreak_prg          = createProgram(2, filmbreak_sh);

   wind_shaders[0]     = loadShader(GL_FRAGMENT_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/wind.fs");
   wind_shaders[1]     = defaultvs;
   wind_prg            = createProgram(2, wind_shaders);

   moving_shaders[0]   = cave_shaders[1];
   moving_shaders[1]   = defaultfs;
   move_prg            = createProgram(1, moving_shaders);

   bunch_shaders[0]    = loadShader(GL_VERTEX_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/bunch.vs");
   bunch_shaders[1]    = loadShader(GL_FRAGMENT_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/bunch.fs");
   bunch_prg           = createProgram(2, bunch_shaders);

   credit_shaders[0]   = loadShader(GL_FRAGMENT_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/credits.fs");
   credit_shaders[1]   = defaultvs;
   credit_prg          = createProgram(2, credit_shaders);

   title_shaders[0]    = loadShader(GL_FRAGMENT_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/titles.fs");
   title_shaders[1]    = defaultvs;
   title_prg           = createProgram(2, title_shaders);

   fade_shaders[0]     = loadShader(GL_FRAGMENT_SHADER, "/usr/share/aeronautism-by-cyberneticgenetics/shaders/fadeout.fs");
   fade_shaders[1]     = defaultvs;
   fade_prg            = createProgram(2, fade_shaders);

   /* Load the monster vertex attributes */
   attrib = loadMonsterAttrib();

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   startTime();
}

void free_aeronautism(){
   GLuint textures[21];

   /* free textures */
   textures[0] = logotex;
   textures[1] = ballstiltex;
   textures[2] = granitetex;
   textures[3] = balloontex;
   textures[4] = monstertex;
   textures[5] = slatetex;
   textures[6] = foresttex;
   textures[7] = cloudtex;
   textures[8] = leaftex;
   textures[9] = treetex;
   textures[10]= tillingtex;
   textures[11]= eyestiltex;
   textures[12]= girlstiltex;
   textures[13]= shadowtex;
   textures[14]= greetex;
   textures[15]= teleporttex;
   textures[16]= grasstex;
   textures[17]= travistex;
   textures[18]= shamentex;
   textures[19]= cgtex;
   textures[20]= functex;
   glDeleteTextures(21, textures);

   /* free meshes */
   freeMesh3D(monster);
   freeMesh3D(balloon);
   freeMesh3D(terrain);
   freeMesh3D(logo);
   freeMesh3D(dragon);
   freeMesh3D(cave);
   freeMesh3D(tree);
   freeMesh3D(tree_crown);
   freeMesh3D(teleport);

   /* free particles */
   freeParticle(leftjet);
   freeParticle(rightjet);

   free(ballexplode);

   free(attrib);
}

/* TODO redundant functions. Need only one. */
void drawsky(){
   GLfloat texcoords[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
   GLfloat vertexes[] = {-300.0, -200.0, -300.0,
                         -300.0, 200.0, -300.0,
                          300.0, 200.0, -300.0,
                          300.0, -200.0, -300.0};
   GLfloat normals[] = {-300.0, -200.0, -299.0,
                        -300.0, 200.0, -299.0,
                         300.0, 200.0, -299.0,
                         300.0, -200.0, -299.0};
   GLuint indices[] = {0, 1, 2, 3};
   glDisableClientState(GL_COLOR_ARRAY);
   glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cloudtex);
    glVertexPointer(3, GL_FLOAT, 0, vertexes);
    glNormalPointer(GL_FLOAT, 0, normals);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, indices);
   glPopMatrix();
   glEnableClientState(GL_COLOR_ARRAY);
}

void drawsky2(){
   GLfloat texcoords[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
   GLfloat vertexes[] = { 300.0,-200.0,  300.0,  
                          300.0, 200.0,  300.0,  
                          300.0, 200.0, -300.0,  
                          300.0,-200.0, -300.0};
   GLfloat normals[] = {299.0,-200.0,  300.0,
                        299.0, 200.0,  300.0,
                        299.0, 200.0, -300.0,
                        299.0,-200.0, -300.0};
   GLuint indices[] = {0, 1, 2, 3};

   glDisableClientState(GL_COLOR_ARRAY);
   glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cloudtex);
    glVertexPointer(3, GL_FLOAT, 0, vertexes);
    glNormalPointer(GL_FLOAT, 0, normals);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, indices);
   glPopMatrix();
   glEnableClientState(GL_COLOR_ARRAY);
}

void drawsky3(){
   GLfloat texcoords[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
   GLfloat vertexes[] = { -300.0,-200.0, -300.0,  
                          -300.0, 200.0, -300.0,  
                          -300.0, 200.0,  300.0,  
                          -300.0,-200.0,  300.0};

   GLfloat normals[] = {-299.0,-200.0,  300.0,
                        -299.0, 200.0,  300.0,
                        -299.0, 200.0, -300.0,
                        -299.0,-200.0, -300.0};

   GLuint indices[] = {0, 1, 2, 3};

   glDisableClientState(GL_COLOR_ARRAY);

   glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cloudtex);
    glVertexPointer(3, GL_FLOAT, 0, vertexes);
    glNormalPointer(GL_FLOAT, 0, normals);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, indices);
   glPopMatrix();

   glEnableClientState(GL_COLOR_ARRAY);
}

void draw_a_quad(){
   GLfloat vertices[] = {-0.77, -0.6, -1.0, 0.77, -0.6, -1.0, 0.77, 0.6,-1.0, -0.77, 0.6, -1.0};
   GLfloat texture_coords[] = {0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0};
   GLuint indices[] = {0,1,2,3};

   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);

   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);
   glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, indices);

   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

}

void tilling_picture(float start, float time, GLuint inittexture, GLuint endtexture){
   GLuint uni;
   float percent;

   percent = 1.0 - log( time - start + 1) / 4.7; /* Ten seconds */

   if(percent < 0.5013) percent = 0.5013;
   glUseProgram(tilling_prg);

   glActiveTexture(GL_TEXTURE0);
   uni = glGetUniformLocation(tilling_prg, "balloon");
   glUniform1i(uni, 0);
   glBindTexture(GL_TEXTURE_2D, inittexture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   glActiveTexture(GL_TEXTURE1);
   uni = glGetUniformLocation(tilling_prg, "tilling");
   glUniform1i(uni, 1);
   glBindTexture(GL_TEXTURE_2D, tillingtex);

   glActiveTexture(GL_TEXTURE2);
   uni = glGetUniformLocation(tilling_prg, "newimage");
   glUniform1i(uni, 2);
   glBindTexture(GL_TEXTURE_2D, endtexture);

   uni = glGetUniformLocation(tilling_prg, "zoom");
   glUniform1f(uni, percent);

   draw_a_quad();
}

/* Demo parts */

/* First scene. We draw the waveing logo */
void drawlogo(double start, double time){
   int x,y;
   float value, factor;

   glClearColor(0.42,0.5,0.55,0.0);
   factor = (time - start) / 5.0f * 4.0f;

   for(x = 0; x < WIDTH; x++)
      for(y = 0; y < HEIGHT; y++){
         value = ((cos(x / 2.0f + factor) * cos(y / 2.0f + factor)) / 8.0f) - 1.8;
	 logo->vertices[y * WIDTH * 3 + x * 3 + 2] = value;
	 logo->normals[y * WIDTH * 3 + x * 3 + 2] = value + 1.0f;
      }

   glPushMatrix();
    glTranslatef(-1.08 - factor / 40.0,-0.8 - factor / 40.0,-3.0);
    glScalef(1.0 + factor / 40.0, 1.0 + factor / 40.0, 1.0);
    glRotatef(180.0,1.0,0.0,0.0);
    glRotatef(-90.0,0.0,0.0,1.0);

    glBindTexture(GL_TEXTURE_2D, logotex);
    drawMesh3D(logo);
   glPopMatrix();
}

void stage_1(double start, double time){

   glPushMatrix();
    glTranslatef(-20.0, 0.0, -40.0);
    glRotatef(53.0, 1.0, 0.0, 0.0);
    glRotatef(351.0, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, foresttex);
    drawMesh3D(terrain);

    glPushMatrix();
     glTranslatef((time - start) / 0.50 + 7.0, 0.0, 1.0);
     glRotatef(108.0, 0.0,1.0,0.0);
     glRotatef(-12.0, 0.0, 0.0, 1.0);
     glRotatef(5.0, 1.0, 0.0, 0.0);
     glBindTexture(GL_TEXTURE_2D, balloontex);
     drawMesh3D(balloon);
    glPopMatrix();
   glPopMatrix();

}

void stage_2(double start, double time){

   glPushMatrix();
    glTranslatef(-20.0, 0.0, -40.0);
    glRotatef(0.0, 1.0, 0.0, 0.0);
    glRotatef(299.0, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, foresttex);
    drawMesh3D(terrain);

    glPushMatrix();
     glTranslatef((time - start) / 0.5000 + 35.54, 0.0, 1.0);
     glRotatef(108.0, 0.0,1.0,0.0);
     glRotatef(-12.0, 0.0, 0.0, 1.0);
     glRotatef(5.0, 1.0, 0.0, 0.0);
     glBindTexture(GL_TEXTURE_2D, balloontex);
     drawMesh3D(balloon);
    glPopMatrix();
   glPopMatrix();

   drawsky();
}

void stage_3(double start, double time){
   GLuint uni;

   drawsky();

   glUseProgram(default_prg);
   uni = glGetUniformLocation(default_prg, "texture");
   glActiveTexture(GL_TEXTURE0);
   glUniform1i(uni, 0);
   glPushMatrix();
    glTranslatef(0.0, 0.0, 10.0);
    glRotatef(0.0, 1.0, 0.0, 0.0);
    glRotatef(94.0, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, foresttex);
    drawMesh3D(terrain);

    glPushMatrix();
     glTranslatef((time - start) / 0.5000 + 11.90, 0.0, 1.0);
     glRotatef(113.0, 0.0,1.0,0.0);
     glRotatef(-12.0, 0.0, 0.0, 1.0);
     glRotatef(5.0, 1.0, 0.0, 0.0);
     glBindTexture(GL_TEXTURE_2D, balloontex);
     drawMesh3D(balloon);
     drawParticle(leftjet, time, 0.4, 0.1, 0.1, 0.42, 0.03, -0.1);
     drawParticle(rightjet, time, -0.31, -0.09, -0.25, -0.29, -0.16, -0.45);
    glPopMatrix();

   glPopMatrix();
}

void stage_4(double start, double time){

   glPushMatrix();
    glTranslatef(0.0, 0.0, (time - start) / 0.2000 + 30.0);
    glRotatef(-2.0, 1.0, 0.0, 0.0);
    glRotatef(-267.0, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, foresttex);
    drawMesh3D(terrain);
   glPopMatrix();

   glPushMatrix();
    glTranslatef(0.0, 0.0, -2.0);
    glRotatef(-137.0, 0.0,1.0,0.0);
    glRotatef(11.0, 1.0, 0.0, 0.0);
    glRotatef(-6.0, 0.0, 0.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, balloontex);
    drawMesh3D(balloon);
    drawParticle(leftjet, time, 0.4, 0.1, 0.1, 0.42, 0.03, -0.1);
    drawParticle(rightjet, time, -0.31, -0.09, -0.25, -0.29, -0.16, -0.45);
   glPopMatrix();

   drawsky();

}

void stage_5(double start, double time){
/* See the head of the dragon */
  GLfloat roty;

  roty = 105.0 - (float)(time - start) / 0.200;
  glPushMatrix();
     glTranslatef(0.0,-10.0,-21.0);
     glRotatef(62.0,0.0,0.0,0.0);
     glRotatef(roty,0.0,1.0,0.0);
     glBindTexture(GL_TEXTURE_2D, slatetex);
     drawMesh3D(dragon);
  glPopMatrix();
  glPushMatrix();
   glTranslatef(0.0, 0.0, 100.0);
   glRotatef(roty - 90.0, 0.0, 1.0, 0.0);
   drawsky();
  glPopMatrix();
}

void stage_6(double start, double time){
   GLfloat balloon_x = (time - start) / 0.200 - 30.0;
   GLfloat roty = (time - start) / 0.300 - 20.0;

   drawsky();

   glPushMatrix();

   glTranslatef(0.0, 0.0, 18.0);
   glRotatef(roty, 0.0, 1.0, 0.0);
   glRotatef(8.0, 1.0, 0.0, 0.0);

   glPushMatrix();
      glTranslatef(balloon_x, 0.0, -59.0);
      glRotatef(108.0, 0.0,1.0,0.0);
      glRotatef(-12.0, 0.0, 0.0, 1.0);
      glRotatef(5.0, 1.0, 0.0, 0.0);
      glBindTexture(GL_TEXTURE_2D, balloontex);
      drawMesh3D(balloon);
     drawParticle(leftjet, time, 0.4, 0.1, 0.1, 0.42, 0.03, -0.1);
     drawParticle(rightjet, time, -0.31, -0.09, -0.25, -0.29, -0.16, -0.45);
   glPopMatrix();

   glPushMatrix();
      glTranslatef(0.0, -22.0, -39.0);
      glRotatef(-80.0, 0.0, 1.0, 0.0);
      glBindTexture(GL_TEXTURE_2D, slatetex);
      drawMesh3D(dragon);
   glPopMatrix();

   glPopMatrix();
}

void drawBalloon(float time, float end){
   GLuint uni;

   glPushMatrix();
    glUseProgram(wind_prg);

    uni = glGetUniformLocation(wind_prg, "percentage");
    glUniform1f(uni, time / end);

    glActiveTexture(GL_TEXTURE0);
    uni = glGetUniformLocation(wind_prg, "texture");
    glUniform1i(uni, 0);
    glBindTexture(GL_TEXTURE_2D, ballstiltex);

    draw_a_quad();
   glPopMatrix();

}

void monster_from_cave(float start, float time){
   float x;
   GLuint unitext;
   GLuint unitime;
   GLuint att;

   x = -10.8 + (time - start) / 0.800;
   glUseProgram(cave_prg);
   unitext = glGetUniformLocation(cave_prg, "texture");
   unitime = glGetUniformLocation(cave_prg, "time");
   att     = glGetAttribLocation(cave_prg, "movements");

   glPushMatrix();
     glTranslatef(-0.3, 0.0, x);
     glRotatef(-7.0,1.0,0.0,0.0);
     glRotatef(-133.0, 0.0, 1.0, 0.0);
     glRotatef(-4.0, 0.0, 0.0, 1.0);
     glActiveTexture(GL_TEXTURE0);
     glUniform1i(unitext, 0);
     glBindTexture(GL_TEXTURE_2D, monstertex);
     glUniform1f(unitime, time * 5.0);

     glEnableVertexAttribArray(att);
     glVertexAttribPointer(att, 1, GL_FLOAT, GL_FALSE, 0, attrib);
     drawMesh3D(monster);
     glDisableVertexAttribArray(att);
   glPopMatrix();

   glPushMatrix();
     glTranslatef(-0.8,-1.0,-9.0);
     glActiveTexture(GL_TEXTURE0);
     glUniform1i(unitext, 0);
     glUniform1f(unitime, 0.0);
     glBindTexture(GL_TEXTURE_2D, granitetex);
     drawMesh3D(cave);
   glPopMatrix();

}

void monster_in_forest(float start, float time){
   float balloonpos;
   float monsterpos;
   GLuint unitime, att, uni;

   glUseProgram(default_prg);
   uni = glGetUniformLocation(default_prg, "texture");
   glUniform1i(uni, 0);
   glActiveTexture(GL_TEXTURE0);

   if( time < 121.0 ){
     balloonpos = 10.0 - (time - start) * 1.5;
     monsterpos = -1.9;
   }
   else{
     balloonpos = -12.5;
     monsterpos = -1.9 - (time - 121.0) * 1.5;
   }

   drawsky();

   glPushMatrix();
    glTranslatef(-4.8f, -6.2f, -6.5f);
    glRotatef(-53.0, 1.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, treetex);
    drawMesh3D(tree);
    glBindTexture(GL_TEXTURE_2D, leaftex);
    drawMesh3D(tree_crown);
   glPopMatrix();

   glPushMatrix();
    glTranslatef(4.7f, -3.7f, -6.5f);
    glRotatef(-62.0, 1.0, 0.0, 0.0);
    glRotatef(36.0, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, treetex);
    drawMesh3D(tree);
    glBindTexture(GL_TEXTURE_2D, leaftex);
    drawMesh3D(tree_crown);
   glPopMatrix();

   glPushMatrix();
    glTranslatef(1.2, -10.0, -12.6);
    glRotatef(-67.0, 1.0, 0.6, 0.0);
    glBindTexture(GL_TEXTURE_2D, treetex);
    drawMesh3D(tree);
    glBindTexture(GL_TEXTURE_2D, leaftex);
    drawMesh3D(tree_crown);
   glPopMatrix();

   glUseProgram(move_prg);
   unitime = glGetUniformLocation(move_prg, "time");
   att     = glGetAttribLocation(move_prg, "movements");
   glEnableVertexAttribArray(att);
   glUniform1f(unitime, time * 5.0);

   glPushMatrix();
    glTranslatef(0.2, -0.5, monsterpos);
    glRotatef(52.0, 0.0, 1.0, 0.0);
    glRotatef(8.0, 1.0, 0.0, 0.0);
    glRotatef(2.0, 0.0, 0.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, monstertex);
    glVertexAttribPointer(att, 1, GL_FLOAT, GL_FALSE, 0, attrib);
    drawMesh3D(monster);
   glPopMatrix();

   glDisableVertexAttribArray(att);
   glUseProgram(default_prg);
   glUniform1i(uni, 0);

   glPushMatrix();
    glTranslatef(balloonpos, 3.8, -14.0);
    glRotatef(-24.0, 1.0, 0.0, 0.0);
    glRotatef(-70.0, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, balloontex);
    drawMesh3D(balloon);
    drawParticle(leftjet, time, 0.4, 0.1, 0.1, 0.42, 0.03, -0.1);
    drawParticle(rightjet, time, -0.31, -0.09, -0.25, -0.29, -0.16, -0.45);
   glPopMatrix();

   glUseProgram(0);
}

void shadow_case(float start, float time){
   float terrainpos;
   GLuint uni;
   terrainpos = -72.0 + (time - start) * 10.0;

   glUseProgram(shadow_prg);

   glPushMatrix();
    glTranslatef(terrainpos, -1.1, -160.0);
    glRotatef(-38.0, 0.0, 1.0, 0.0);

    glActiveTexture(GL_TEXTURE0);
    uni = glGetUniformLocation(shadow_prg, "shadow");
    glUniform1i(uni, 0);
    glBindTexture(GL_TEXTURE_2D, shadowtex);

    glActiveTexture(GL_TEXTURE1);
    uni = glGetUniformLocation(shadow_prg, "forest");
    glUniform1i(uni, 1);
    glBindTexture(GL_TEXTURE_2D, foresttex);

    uni = glGetUniformLocation(shadow_prg, "shadow_offset");
    glUniform2f(uni, -0.19 + (time - start) / 60.0, -0.93 + (time - start) / 60.0);

    drawMesh3D(terrain);
   glPopMatrix();

   glUseProgram(0);
   glActiveTexture(GL_TEXTURE0);
   drawsky();
}

void monster_viewpoint(float start, float time){
   float pos, bias;
   GLfloat unitime, att, uni;

   pos = -246.0 + (time - start) * 10;
   bias = sin((time - start)) / 10.0;

   glPushMatrix();
    glTranslatef(32.0, 2.0, pos);
    glRotatef(4.0, 1.0, 0.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, foresttex);
    drawMesh3D(terrain);
   glPopMatrix();

   glUseProgram(move_prg);
   unitime = glGetUniformLocation(move_prg, "time");
   att     = glGetAttribLocation(move_prg, "movements");
   glEnableVertexAttribArray(att);
   glUniform1f(unitime, time * 5.0);

   glPushMatrix();
    glTranslatef(0.3 + bias, -0.8, -1.4);
    glRotatef(52.0, 0.0, 1.0, 0.0);
    glRotatef(8.0, 1.0, 0.0, 0.0);
    glRotatef(2.0, 0.0, 0.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, monstertex);
    glVertexAttribPointer(att, 1, GL_FLOAT, GL_FALSE, 0, attrib);
    drawMesh3D(monster);
   glPopMatrix();

   glDisableVertexAttribArray(att);
   glUseProgram(default_prg);
   uni = glGetUniformLocation(default_prg, "texture");
   glUniform1i(uni, 0);

   glPushMatrix();
     glTranslatef(0.3, 0.0, -8.2);
     glRotatef(-3.0,1.0, 0.0, 0.0);
     glRotatef(201, 0.0, 1.0, 0.0);
     glRotatef(-15.0, 0.0, 0.0, 1.0);
     glBindTexture(GL_TEXTURE_2D, balloontex);
     drawMesh3D(balloon);
     drawParticle(leftjet, time, 0.4, 0.1, 0.1, 0.42, 0.03, -0.1);
     drawParticle(rightjet, time, -0.31, -0.09, -0.25, -0.29, -0.16, -0.45);
   glPopMatrix();

   drawsky();
   glUseProgram(0);
}

void dramatic_event(float start, float time){
   float pos, rad1 = 0.0, rad2 = 0.0;
   GLuint uni;

   pos = 4.0 - (time - start) * 4.0;
   if(pos < 0.9){
      pos = 0.9;
      rad1 = (time - start - 0.88) * 0.2;
      rad2 = (time - start - 1.0) * 0.1;
   }

   glPushMatrix();
    glTranslatef(-0.7, -0.2, -2.0);
    glBindTexture(GL_TEXTURE_2D, balloontex);
    drawMesh3D(balloon);
   glPopMatrix();

   glPushMatrix();
    glTranslatef(pos, -0.1, -3.0);
    glRotatef(-21.0, 1.0, 0.0, 0.0);
    glRotatef(141.2, 0.0, 1.0, 0.0);
    glRotatef(9.2, 0.0, 0.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, monstertex);
    drawMesh3D(monster);
   glPopMatrix();

   drawsky();
   glFlush();

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, 0);
   glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, screenwidth, screenheight, 0);

   glUseProgram(fbreak_prg);
   uni = glGetUniformLocation(fbreak_prg, "texture");
   glUniform1i(uni, 0);
   uni = glGetUniformLocation(fbreak_prg, "radius1");
   glUniform1f(uni, rad1);
   uni = glGetUniformLocation(fbreak_prg, "radius2");
   glUniform1f(uni, rad2);

   draw_a_quad();
   glUseProgram(0);
}

void greetings(/*float time*/){
   GLuint tex;

   glUseProgram(cave_prg);
   glActiveTexture(GL_TEXTURE0);
   tex = glGetUniformLocation(cave_prg, "texture");
   glUniform1i(tex, 0);
   glBindTexture(GL_TEXTURE_2D, greetex);   
   draw_a_quad();
   glUseProgram(default_prg);
}

void balloon_escape(float start, float time){
   float x,y,z;

   if(time - start < 1.5){
     x = 0.42;
     y = 0.03;
     z = -0.1;
   }
   else{
     x = 0.4;
     y = 0.0;
     z = -0.6;
   }

   glPushMatrix();
   glTranslatef(-0.3, -0.2, -1.5);
   glRotatef(-57.0, 0.0, 1.0, 0.0);
   glBindTexture(GL_TEXTURE_2D, balloontex);
   drawMesh3D(balloon);
   drawParticle(leftjet, time, 0.4, 0.1, 0.1, x, y, z);
   glPopMatrix();

   drawsky();
}

void chase_front(float start, float time){
   float pos, r1 = 0.0, r2 = 0.0;
   GLuint unitime, att, uni;
   pos = (time - start) * 8.0;

   if(time - start > 1.2){
    r1 = -4.0;
    r2 = 181.0;
   }

   drawsky();

   glPushMatrix();

   glRotatef(r1, 1.0, 0.0, 0.0);
   glRotatef(r2, 0.0, 1.0, 0.0);

    glPushMatrix();
     glTranslatef(56.6, 16.8, 59.4);
     glRotatef(-191.0, 1.0, 0.0, 0.0);
     glRotatef(248.0, 0.0, 1.0, 0.0);
     glBindTexture(GL_TEXTURE_2D, foresttex);
     drawMesh3D(terrain);
    glPopMatrix();

    glUseProgram(move_prg);

    unitime = glGetUniformLocation(move_prg, "time");
    att     = glGetAttribLocation(move_prg, "movements");
    glEnableVertexAttribArray(att);
    glUniform1f(unitime, time * 8.0);

    glPushMatrix();
     glTranslatef(0.9, -0.1, -10.7 + pos);
     glRotatef(-374.0, 1.0, 0.0, 0.0);
     glRotatef(-497.0, 0.0, 1.0, 0.0);
     glBindTexture(GL_TEXTURE_2D, monstertex);
     glVertexAttribPointer(att, 1, GL_FLOAT, GL_FALSE, 0, attrib);
     drawMesh3D(monster);
    glPopMatrix();

    glDisableVertexAttribArray(att);
    glUseProgram(default_prg);
    uni = glGetUniformLocation(default_prg, "texture");
    glUniform1i(uni, 0);

    glPushMatrix();
     glTranslatef(-1.3, 0.0, -9.6 + pos);
     glRotatef(-345.0, 1.0, 0.0, 0.0);
     glRotatef(-339.0, 0.0, 1.0, 0.0);
     glRotatef(-29.0, 0.0, 0.0, 1.0);
     glBindTexture(GL_TEXTURE_2D, balloontex);
     drawMesh3D(balloon);
     drawParticle(leftjet, time, 0.4, 0.1, 0.1, 0.4, 0.0, -0.6);
     drawParticle(rightjet, time, -0.31, -0.09, -0.25, -0.31, -0.19, -0.95);
    glPopMatrix();

   glPopMatrix();
   glUseProgram(0);
}

void sideview(float start, float time){
   float pos;
   GLuint unitime, att, uni;
   pos = (time - start) * 30.0;

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(20.0, 800.0/600.0, 1.0, 15000.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glPushMatrix();
    glTranslatef(30.0, 0.0, 130 - pos);
    glRotatef(-191.0 + pos / 15.0, 1.0, 0.0, 0.0);
    glRotatef(248.0, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, foresttex);
    drawMesh3D(terrain);
    drawsky2();
   glPopMatrix();

   glUseProgram(move_prg);

   unitime = glGetUniformLocation(move_prg, "time");
   att     = glGetAttribLocation(move_prg, "movements");
   glEnableVertexAttribArray(att);
   glUniform1f(unitime, time * 18.0);

   glPushMatrix();
    glTranslatef(0.9, -0.1, -10.7);
    glRotatef(-374.0, 1.0, 0.0, 0.0);
    glRotatef(-497.0, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, monstertex);
    glVertexAttribPointer(att, 1, GL_FLOAT, GL_FALSE, 0, attrib);
    drawMesh3D(monster);
   glPopMatrix();

   glDisableVertexAttribArray(att);
   glUseProgram(default_prg);
   uni = glGetUniformLocation(default_prg, "texture");
   glUniform1i(uni, 0);

   glPushMatrix();
    glTranslatef(-0.4 + drand48() / 100.0, 0.0 + drand48() / 100.0, -1.6);
    glRotatef(-343.0, 1.0, 0.0, 0.0);
    glRotatef(15, 0.0, 1.0, 0.0);
    glRotatef(-13.3, 0.0, 0.0, 1.0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, balloontex);
    drawMesh3D(balloon);
   glPopMatrix();

   glUseProgram(0);
}

void see_the_teleport(float start, float time){
   float terrain_rot;
   float chase_pos;
   GLuint texture;
   GLuint unitime;

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, 800.0/600.0, 1.0, 15000.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   terrain_rot = 90.0 - (time - start) * 5.58;
   chase_pos = (time - start) *0.9;

   glPushMatrix();
    glTranslatef(50.25, 0.0, -71.6);
    glRotatef(11.0, 1.0, 0.0, 0.0);
    glRotatef(terrain_rot, 0.0, 1.0, 0.0);/* 111 - 25*/
    glBindTexture(GL_TEXTURE_2D, foresttex);
    drawMesh3D(terrain);

    drawsky();drawsky2();drawsky3();

    glPushMatrix();
     glUseProgram(bunch_prg);
     texture = glGetUniformLocation(bunch_prg, "texture");
     glUniform1i(texture, 0);
     unitime = glGetUniformLocation(bunch_prg, "time");
     glUniform1f(unitime, time - start);
     glTranslatef(-59.86, 10.01, -11.0);
     glBindTexture(GL_TEXTURE_2D, teleporttex);
     drawMesh3D(teleport);
     glUseProgram(0);
    glPopMatrix();

    glPushMatrix();
     glTranslatef(-35.95, 16.4, -62.7 + chase_pos);
     glRotatef(-9.0, 1.0, 0.0, 0.0);
     glRotatef(167.0, 0.0, 1.0, 0.0);
     glBindTexture(GL_TEXTURE_2D, monstertex);
     drawMesh3D(monster);
    glPopMatrix();

    glPushMatrix();
     glTranslatef(-35.84, 16.7, -60.1 + chase_pos);
     glRotatef(-353.0, 1.0, 0.0, 0.0);
     glRotatef(-14.0, 0.0, 1.0, 0.0);
     glBindTexture(GL_TEXTURE_2D, balloontex);
     drawMesh3D(balloon);
    glPopMatrix();

   glPopMatrix();

}

void go_to_the_teleport(float start, float time){
   float terrain_pos;
   float chase_pos;
   GLuint texture;
   GLuint unitime;

   terrain_pos = -40.0 + (time - start) * 5.58;
   chase_pos = (time - start) *0.9;

   glPushMatrix();
    glTranslatef(terrain_pos, -20.2, 19.3);
    glRotatef(5.0, 1.0, 0.0, 0.0);
    glRotatef(-78.0, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, foresttex);
    drawMesh3D(terrain);

    drawsky();drawsky2();drawsky3();

    glPushMatrix();
     glUseProgram(bunch_prg);
     texture = glGetUniformLocation(bunch_prg, "texture");
     glUniform1i(texture, 0);
     unitime = glGetUniformLocation(bunch_prg, "time");
     glUniform1f(unitime, time - start);
     glTranslatef(-59.86, 10.01, -11.0);
     glBindTexture(GL_TEXTURE_2D, teleporttex);
     drawMesh3D(teleport);
     glUseProgram(0);
    glPopMatrix();

    glPushMatrix();
     glTranslatef(-35.95, 16.4, -62.7 + chase_pos);
     glRotatef(-9.0, 1.0, 0.0, 0.0);
     glRotatef(167.0, 0.0, 1.0, 0.0);
     glBindTexture(GL_TEXTURE_2D, monstertex);
     drawMesh3D(monster);
    glPopMatrix();

    glPushMatrix();
     glTranslatef(-35.84, 16.7, -60.1 + chase_pos);
     glRotatef(-353.0, 1.0, 0.0, 0.0);
     glRotatef(-14.0, 0.0, 1.0, 0.0);
     glBindTexture(GL_TEXTURE_2D, balloontex);
     drawMesh3D(balloon);
    glPopMatrix();

   glPopMatrix();

}

void explode_balloon(float start, float time){
   GLuint texture, unitime;

   glPushMatrix();
    glUseProgram(bunch_prg);
    texture = glGetUniformLocation(bunch_prg, "texture");
    glUniform1i(texture, 0);
    unitime = glGetUniformLocation(bunch_prg, "time");
    glUniform1f(unitime, time - start);
    glBindTexture(GL_TEXTURE_2D, teleporttex);
    drawMesh3D(teleport);
    glUseProgram(0);
   glPopMatrix();

   glPushMatrix();
    glTranslatef(0.0, 0.0, -2.0);
    glRotatef(-90.0 + time - start, 0.0, 1.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, balloontex);
    explodeTriangles(ballexplode, balloon->icount / 3, (time - start) * 209.22);
   glPopMatrix();
}

void unlimited_picture_zoom(float start, float time){
   if(time < 281.5) tilling_picture(start, time, ballstiltex, eyestiltex);
   if(time > 281.5) tilling_picture(281.5, time, eyestiltex, girlstiltex);
glUseProgram(0);
}

void grass(float time){
  GLuint unitex, unitime, uniasp;

  glUseProgram(credit_prg);
  glActiveTexture(GL_TEXTURE0);
  unitex = glGetUniformLocation(credit_prg, "who");
  glUniform1i(unitex, 0);
  glBindTexture(GL_TEXTURE_2D, grasstex);
  unitime = glGetUniformLocation(credit_prg, "time");
  glUniform1f(unitime, time);
  uniasp = glGetUniformLocation(credit_prg, "aspect");
  glUniform1f(uniasp, 1.92);
  draw_a_quad();
}

void travis(float time){
  GLuint unitex, unitime, uniasp;

  glUseProgram(credit_prg);
  glActiveTexture(GL_TEXTURE0);
  unitex = glGetUniformLocation(credit_prg, "who");
  glUniform1i(unitex, 0);
  glBindTexture(GL_TEXTURE_2D, travistex);
  unitime = glGetUniformLocation(credit_prg, "time");
  glUniform1f(unitime, time);
  uniasp = glGetUniformLocation(credit_prg, "aspect");
  glUniform1f(uniasp, 2.95);

  draw_a_quad();
}

void shamen(float time){
  GLuint unitex, unitime, uniasp;

  glUseProgram(credit_prg);
  glActiveTexture(GL_TEXTURE0);
  unitex = glGetUniformLocation(credit_prg, "who");
  glUniform1i(unitex, 0);
  glBindTexture(GL_TEXTURE_2D, shamentex);
  unitime = glGetUniformLocation(credit_prg, "time");
  glUniform1f(unitime, time);
  uniasp = glGetUniformLocation(credit_prg, "aspect");
  glUniform1f(uniasp, 3.24);

  draw_a_quad();
}

void cybernetic(/*float time*/){
  GLuint unitex, uniasp;

  glUseProgram(title_prg);
  glActiveTexture(GL_TEXTURE0);
  unitex = glGetUniformLocation(credit_prg, "who");
  glUniform1i(unitex, 0);
  glBindTexture(GL_TEXTURE_2D, cgtex);
  uniasp = glGetUniformLocation(credit_prg, "aspect");
  glUniform1f(uniasp, 1.52);
  draw_a_quad();
}

void function(float time){
  GLuint unitex, uniper;

  glUseProgram(fade_prg);
  glActiveTexture(GL_TEXTURE0);
  unitex = glGetUniformLocation(fade_prg, "texture");
  glUniform1i(unitex, 0);
  glBindTexture(GL_TEXTURE_2D, functex);
  uniper = glGetUniformLocation(fade_prg, "percent");
  glUniform1f(uniper, time / 9.51);

  draw_a_quad();
}

/* End of demo parts*/

/* Main drawing unit. All part of the demo is controlled by this. */
void draw_aeronautism(){
   double time;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   time = getTimeInterval();

   if(time < 33.0)                     drawlogo(0.0, time);
   if(time > 33.00 && time < 41.00)    stage_1(33.0, time);
   if(time > 41.00 && time < 45.00)    stage_2(41.0, time);
   if(time > 45.00 && time < 51.00)    stage_3(45.0, time);
   if(time > 51.00 && time < 64.00)    stage_4(51.0, time);
   if(time > 64.00 && time < 69.00)    stage_5(64.0, time);
   if(time > 69.00 && time < 73.02)    stage_6(69.0, time);
   if(time > 73.02 && time < 99.00)    drawBalloon(time - 83.02, 99.0 - 83.02);
   if(time > 99.00 && time < 106.0)    monster_from_cave(99.0, time);
   if(time > 106.0 && time < 126.0)    monster_in_forest(106.0, time);
   if(time > 126.0 && time < 139.0)    shadow_case(126.0, time);
   if(time > 139.0 && time < 152.0)    monster_viewpoint(139.0, time);
   if(time > 152.0 && time < 165.5)    dramatic_event(152.0, time);
   if(time > 165.5 && time < 192.0)    greetings(/*time - 165.5*/);
   if(time > 192.0 && time < 195.0)    balloon_escape(192.0, time);
   if(time > 195.0 && time < 197.6)    chase_front(195.0, time);
   if(time > 197.6 && time < 210.0)    sideview(197.6, time);
   if(time > 210.0 && time < 231.0)    see_the_teleport(210.0, time);
   if(time > 231.0 && time < 245.0)    go_to_the_teleport(231.0, time);
   if(time > 245.0 && time < 272.0)    explode_balloon(245.0, time);
   if(time > 272.0 && time < 301.0)    unlimited_picture_zoom(272.0, time);
   if(time > 301.0 && time < 308.0)    grass(time - 301.0);
   if(time > 308.0 && time < 315.0)    travis(time - 308.0);
   if(time > 315.0 && time < 324.0)    shamen(time - 315.0);
   if(time > 324.0 && time < 338.0)    cybernetic(/*time - 324.0*/);
   if(time > 338.0)                    function(time - 338.0);
}
