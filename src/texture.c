#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <unistd.h>
#include <math.h>
#include <png.h>

#include "texture.h"

/* First texture loading function later I will extend
   with other texture loading and generating functions */

GLuint loadPNGTexture(const char *filename){
  FILE        *texturefile;
  png_structp  texture_ptr;
  png_infop    texture_info;
  png_infop    png_end;
  png_bytep   *row;
  GLubyte     *image;
  GLuint       texture;
  GLenum       png_format;
  unsigned int i;

  /* initialization steps */
  texturefile  = fopen(filename, "r");

  texture_ptr  = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                       NULL,
                                       NULL,
                                       NULL);

  texture_info = png_create_info_struct(texture_ptr);
  png_end      = png_create_info_struct(texture_ptr);
  png_init_io(texture_ptr, texturefile);
  png_read_info(texture_ptr, texture_info);
  row          = malloc(sizeof(png_bytep) * png_get_image_height(texture_ptr, texture_info));
  image        = malloc(png_get_image_width(texture_ptr, texture_info) *
                        png_get_image_height(texture_ptr, texture_info) *
                        png_get_channels(texture_ptr, texture_info));

  switch(png_get_channels(texture_ptr, texture_info)){
     case 4:  png_format = GL_RGBA;            break;
     case 3:  png_format = GL_RGB;             break;
     case 2:  png_format = GL_LUMINANCE_ALPHA; break;
     default: png_format = GL_R;
  };

  if(png_get_channels(texture_ptr, texture_info) == 3){
     png_format = GL_RGB;
  }

  /* We set the png row pointer to the stored texture data */
  for(i = 0; i < png_get_image_height(texture_ptr, texture_info); i++){
     row[png_get_image_height(texture_ptr, texture_info) - 1 - i] = image + i * (png_get_image_width(texture_ptr, texture_info) * png_get_channels(texture_ptr, texture_info));
  }

  /* Read the file */
  png_read_image(texture_ptr, row);  

  /* OpenGL related stuff. The png data go to the OpenGL state machine */
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, png_get_image_width(texture_ptr, texture_info),
              png_get_image_height(texture_ptr, texture_info), 0, png_format, GL_UNSIGNED_BYTE,
               image);
  /*glEnable(GL_TEXTURE_2D);*/

  /* The data is in the state machine we do not need this datas any more */
  png_destroy_read_struct(&texture_ptr, &texture_info, &png_end);
  free(image);
  free(row);
  fclose(texturefile);

  return(texture);
}

