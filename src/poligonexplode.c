#include <GL/gl.h>
#include <stdlib.h>
#include "mesh3d.h"
#include "poligonexplode.h"

/*
  Convert the non redundant obj format to a redundant one.
  With it we can remove some poligon from the mesh without
  the collapse of the poligon.
*/
Triangle *generateFromMesh(Mesh3D *mesh){
  Triangle *triangles;
  int i, j, k;
  float v[9];

  triangles = malloc(sizeof(Triangle) * mesh->icount / 3);

  for(i = 0; i < mesh->icount; i += 3){

    for(j = 0; j < 3; j++){
      for(k = 0; k < 3; k++){
        v[j * 3 + k] = mesh->vertices[mesh->indices[i + j] * 3 + k];
        triangles[i / 3].tex[j * 2 + k] = mesh->texcoords[mesh->indices[i + j] * 2 + k];
        triangles[i / 3].nor[j * 3 + k] = mesh->normals[mesh->indices[i + j] * 3 + k];
      }
    }

    triangles[i/3].cx = (v[0] + v[3] + v[6]) / 3.0;
    triangles[i/3].cy = (v[1] + v[4] + v[7]) / 3.0;
    triangles[i/3].cz = (v[2] + v[5] + v[8]) / 3.0;

    for(j = 0; j < 9; j+=3){
      triangles[i/3].dpos[j + 0] = triangles[i/3].cx - v[j + 0];
      triangles[i/3].dpos[j + 1] = triangles[i/3].cy - v[j + 1];
      triangles[i/3].dpos[j + 2] = triangles[i/3].cz - v[j + 2];
    }

  }

  return(triangles);
}

/* We blow up the mesh */
void explodeTriangles(Triangle *tri, int trinum, float time){
  int i;
  GLuint indices[3] = {0, 1, 2};
  GLfloat v[9];

  glDisableClientState(GL_COLOR_ARRAY);

  for(i = 0; i < trinum; i++){

    if( i < time ){
      tri[i].cy = tri[i].cy + (time - (float)i) / 3500.0;
    }

    v[0] = tri[i].cx - tri[i].dpos[0];
    v[1] = tri[i].cy - tri[i].dpos[1];
    v[2] = tri[i].cz - tri[i].dpos[2];
    v[3] = tri[i].cx - tri[i].dpos[3];
    v[4] = tri[i].cy - tri[i].dpos[4];
    v[5] = tri[i].cz - tri[i].dpos[5];
    v[6] = tri[i].cx - tri[i].dpos[6];
    v[7] = tri[i].cy - tri[i].dpos[7];
    v[8] = tri[i].cz - tri[i].dpos[8];

    glVertexPointer(3, GL_FLOAT, 0, v);
    glNormalPointer(GL_FLOAT, 0, tri[i].nor);
    glTexCoordPointer(2, GL_FLOAT, 0, tri[i].tex);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indices);
  }

  glEnableClientState(GL_COLOR_ARRAY);
}

