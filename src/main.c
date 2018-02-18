#include <stdio.h>
#include <GL/gl.h>

#include "cg_x11.h"
#include "cg_music.h"
#include "mesh3d.h"
#include "texture.h"
#include "aeronautism.h"

int main(int argc, char *argv[]){
   unsigned int w = 1024; /* Screen width  */
   unsigned int h = 768;  /* Screen height */

   CreateScene(w, h);

   init_aeronautism(w, h);

   InitMusic("data/newfashion.ogg");

   BEGIN_EVENT
      draw_aeronautism();
      SyncPlay();
   END_EVENT

   free_aeronautism();

   FreeMusic();
   RestoreScene();
   return(EXIT_SUCCESS);
}
