/*
   Particle system without glBegin - glEnd.
   Using point sprites.
*/

#define MAXPARTICLE 200

typedef struct Particle {
   unsigned int  num;
   float        *scoord; /* Start coordinates       */
   float        *coord;  /* Actual coordinates      */
   float        *dcoord; /* Destination coordinates */
   unsigned int *indices;
   float        *births;
   float        *lifes;
}Particle;

GLuint particletex;

/* Create one particle. The particle index is given in the num argument */
void createParticle(float starttime, Particle *p, unsigned int num, float sx, float sy, float sz, float dx, float dy, float dz);

Particle setupParticle(float start, float sx, float sy, float sz, float dx, float dy, float dz);

void drawParticle(Particle particle, float time, float sx, float sy, float sz, float dx, float dy, float dz);

void freeParticle(Particle p);
