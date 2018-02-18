typedef struct Triangle {
  /* Center of the triangle */
  float cx;
  float cy;
  float cz;

  float dpos[9]; /* Relative coordinates from the center of the triangle */
  float nor[9];  /* Normal vectors */
  float tex[9];  /* Texture coordinates */
} Triangle;

Triangle *generateFromMesh(Mesh3D *mesh);

void explodeTriangles(Triangle *tri, int trinum, float time);
