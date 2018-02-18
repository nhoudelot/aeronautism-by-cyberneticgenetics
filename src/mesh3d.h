/*
   Mesh3D struct simply store the 3d object
   OpenGL can easily use this
*/
typedef struct Mesh3D{
   GLfloat *vertices;
   GLfloat *normals;
   GLfloat *colors;
   GLfloat *texcoords;
   GLuint  *indices;

   int      vcount;/* Number of vertices            */
   int      ncount;/* Number of normals             */
   int      ccount;/* Number of colors              */
   int      tcount;/* Number of texture coordinatas */
   int      icount;/* Number of indices             */
} Mesh3D;

/* 
   This load OBJ file and store it in Mesh3D struct if 
   something goes wrong, it is returns NULL.
*/
Mesh3D *loadOBJ(const char *filename);

int drawMesh3D(Mesh3D *mesh);
int drawMesh3D_2(Mesh3D *mesh);

int freeMesh3D(Mesh3D *mesh);

int changeMeshColor(Mesh3D *mesh, float r, float g, float b, float a);

GLfloat *loadMonsterAttrib();
