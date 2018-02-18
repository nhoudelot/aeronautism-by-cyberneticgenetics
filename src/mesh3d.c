#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mesh3d.h"

Mesh3D *loadOBJ(const char *filename){

   FILE   *objfile;
   char    line[200];
   Mesh3D *mesh;
   char   *str;
   int     i;

   objfile = fopen(filename, "r");
   if(objfile == NULL) return(NULL);

   /* Allocate memory for mesh and set initial values */
   mesh = malloc(sizeof(Mesh3D));
   if(mesh == NULL) return(NULL);
   mesh->vcount   = 0;
   mesh->ccount   = 0;
   mesh->icount   = 0;
   mesh->tcount   = 0;
   mesh->ncount   = 0;
   mesh->vertices = NULL;
   mesh->colors   = NULL;
   mesh->indices  = NULL;
   mesh->texcoords= NULL;
   mesh->normals  = NULL;

   while(fgets(line, 200, objfile) != NULL){
      if(line[0] == '#') continue; /* skip comments */

      /* vertices and its color */
      if(line[0] == 'v' && line[1] == ' '){
         str = strtok(line, " \n");
         for(i = 0; i < 3; i++){
            str = strtok(NULL, " \n");
            /*
               Maybe this memory allocation is not a perfect solution,
               but I do not want to allocate more memory than I need.
            */
            mesh->vertices = realloc(mesh->vertices,
                                     sizeof(GLfloat) * (mesh->vcount + 1));
            if(mesh->vertices == NULL) return(NULL);

            mesh->colors   = realloc(mesh->colors,
                                     sizeof(GLfloat) * (mesh->ccount + 1));
            if(mesh->colors   == NULL) return(NULL);

            mesh->vertices[mesh->vcount++] = atof(str);
            /*
               The OBJ file format does not contain color information.
               We set it to black.
            */
            mesh->colors[mesh->ccount++] = 1.0;
         }

         /*
            Alpha channel settings
         */
         mesh->colors = realloc(mesh->colors, 
                                sizeof(GLfloat) * (mesh->ccount + 1));
         if(mesh->colors == NULL) return(NULL);
         mesh->colors[mesh->ccount++] = 1.0;
      }

      /* Texture coordinatas */
      if(line[0] == 'v' && line[1] == 't'){
         str = strtok(line, " \n");
	 for(i = 0; i < 2; i++){
            str = strtok(NULL, " \n");
	    mesh->texcoords = realloc(mesh->texcoords,
			              sizeof(GLfloat) * (mesh->tcount + 1));
	    if(mesh->texcoords == NULL) return(NULL);
	    mesh->texcoords[mesh->tcount++] = atof(str);
	 }
      }

      /* indices */
      if(line[0] == 'f'){
         str = strtok(line, " \n");
         for(i = 0; i < 3; i++){
            str = strtok(NULL, " \n");

            mesh->indices = realloc(mesh->indices,
                                    sizeof(GLuint) * (mesh->icount + 1));

            mesh->indices[mesh->icount++] = atoi(str) - 1;
         }
      }

      /* The normal vectors */
      if(line[0] == 'v' && line[1] == 'n'){
         str = strtok(line, " \n");
         for(i = 0; i < 3; i++){
            str = strtok(NULL, " \n");
            mesh->normals = realloc(mesh->normals,
                                    sizeof(GLfloat) * (mesh->ncount + 1));
            if(mesh->normals == NULL) return(NULL);
            mesh->normals[mesh->ncount++] = atof(str);
         }
      }
   }

   fclose(objfile);
   return(mesh);
}

/* 
   Very simple mesh drawing without texture coordinates.
   I think late the OpenGL connected routine go another file
*/
int drawMesh3D(Mesh3D *mesh){
  
   glTexCoordPointer(2, GL_FLOAT, 0, mesh->texcoords);
   glNormalPointer(     GL_FLOAT, 0, mesh->normals);
   glColorPointer(   4, GL_FLOAT, 0, mesh->colors);
   glVertexPointer(  3, GL_FLOAT, 0, mesh->vertices);

   glDrawElements(GL_TRIANGLES, mesh->icount, GL_UNSIGNED_INT, mesh->indices);
   return(0);

}

/* Release mesh resources */
int freeMesh3D(Mesh3D *mesh){
   free(mesh->vertices);
   free(mesh->colors);
   free(mesh->texcoords);
   free(mesh->normals);
   free(mesh->indices);
   free(mesh);
   return(0);
}

/*Change the color of the mesh */
int changeMeshColor(Mesh3D *mesh, float r, float g, float b, float a){
   int i = 0;

   while(i < mesh->ccount){
      mesh->colors[i++] = r;
      mesh->colors[i++] = g;
      mesh->colors[i++] = b;
      mesh->colors[i++] = a;
   }

   /* No problem: return 0*/
   return(0);
}

/* This method work only with the monster obj file */
GLfloat *loadMonsterAttrib(){
   GLfloat *ret;
   int i = 0;
   FILE *attrfile;
   char line[20];

   ret = malloc(sizeof(GLfloat) * 5504); /* It is only for the monster, so I use this magic number */
   attrfile = fopen("data/monster_attr.txt", "r");
   while(fgets(line, 20, attrfile) != NULL){
      ret[i] = atof(line);
      i++;
   }
   fclose(attrfile);
   return(ret);
}
