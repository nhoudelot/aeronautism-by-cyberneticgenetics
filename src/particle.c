#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "texture.h"
#include "particle.h"
/* Create one particle. The particle index is given in the num argument */
void createParticle(float starttime, Particle *p, unsigned int num, float sx, float sy, float sz, float dx, float dy, float dz){

   /* Initial coordinates of the particle */
   p->scoord[num * 3]     = sx;
   p->scoord[num * 3 + 1] = sy;
   p->scoord[num * 3 + 2] = sz;

   /* Actual coordinate is the start coordinate */
   p->coord[num * 3]      = sx;
   p->coord[num * 3 + 1]  = sy;
   p->coord[num * 3 + 2]  = sz;

   /* Destination of the particle */
   p->dcoord[num * 3]     = dx + (drand48() / 10.0);
   p->dcoord[num * 3 + 1] = dy + (drand48() / 10.0);
   p->dcoord[num * 3 + 2] = dz + (drand48() / 10.0);

   /*particle index */
   p->indices[num]              = num;

   /* Time of birth and lifetime */
   p->births[num]               = starttime;
   p->lifes[num]                = drand48() * 0.3;
}

Particle setupParticle(float start, float sx, float sy, float sz, float dx, float dy, float dz){
   unsigned int i;
   Particle particle;

   /* Particle texture */
   particletex = loadPNGTexture("/usr/share/aeronautism-by-cyberneticgenetics/data/particle_texture.png");/*TODO it should be a parameter */
   glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

   /* Memory allocation for all of the particles */
   particle.num          = MAXPARTICLE;
   particle.scoord       = malloc(sizeof(float) * MAXPARTICLE * 3);
   particle.coord        = malloc(sizeof(float) * MAXPARTICLE * 3);
   particle.dcoord       = malloc(sizeof(float) * MAXPARTICLE * 3);
   particle.indices      = malloc(sizeof(unsigned int) * MAXPARTICLE);
   particle.births       = malloc(sizeof(float) * MAXPARTICLE);
   particle.lifes        = malloc(sizeof(float) * MAXPARTICLE);

   /* particle creation */
   for(i = 0; i < MAXPARTICLE; i++){
      createParticle(start, &particle, i, sx, sy, sz, dx, dy, dz);
   }

   return particle;
}

void drawParticle(Particle particle, float time, float sx, float sy, float sz, float dx, float dy, float dz){
   unsigned int i;
   float percent;

   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glEnable(GL_POINT_SPRITE);
   glPointSize(9.0f);
   glBindTexture(GL_TEXTURE_2D, particletex);

   for(i = 0; i < MAXPARTICLE; i++){
      percent = (time - particle.births[i]) / particle.lifes[i];

      /* Set the actual coordinates of the particle */
      particle.coord[i * 3]     = particle.scoord[i * 3    ] + (particle.dcoord[i * 3    ] - particle.scoord[i * 3    ]) * percent;
      particle.coord[i * 3 + 1] = particle.scoord[i * 3 + 1] + (particle.dcoord[i * 3 + 1] - particle.scoord[i * 3 + 1]) * percent;
      particle.coord[i * 3 + 2] = particle.scoord[i * 3 + 2] + (particle.dcoord[i * 3 + 2] - particle.scoord[i * 3 + 2]) * percent;

      /* Check the particle life. If it is too old then reborn */
      if(percent > 1.0){
         createParticle(time, &particle, i, sx, sy, sz, dx, dy, dz);
      }

   }

   /* The coordinate pointers of the particle load to the OpenGL state machine */
   glVertexPointer(3,GL_FLOAT, 0, particle.coord);
   glDrawElements(GL_POINTS, particle.num, GL_UNSIGNED_INT, particle.indices);
   glDisable(GL_POINT_SPRITE);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void freeParticle(Particle p){
   free(p.coord);
   free(p.scoord);
   free(p.dcoord);
   free(p.indices);
   free(p.births);
   free(p.lifes);
}
