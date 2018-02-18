#include <GL/gl.h>
#include "cg_x11.h"

char cg_isrunning = 1;

void SetScreenSize(int width, int height){
   XRRScreenSize *sizes;
   int snum, rnum, i;
   short *rates;

   cg_screen_conf = XRRGetScreenInfo(cg_display, cg_window);
   cg_orig_size   = XRRConfigCurrentConfiguration(cg_screen_conf, &cg_rotation);
   cg_orig_rate   = XRRConfigCurrentRate(cg_screen_conf);

   /* Lekérdezzük az elérhetö felbontásokat */
   sizes = XRRSizes(cg_display, cg_visual->screen, &snum);
   for(i = 0; i < snum; i++){
      if((sizes[i].width == width) && (sizes[i].height == height)){
         rates = XRRRates(cg_display, cg_visual->screen, i, &rnum);
         XRRSetScreenConfigAndRate(cg_display, cg_screen_conf, DefaultRootWindow(cg_display), i, cg_rotation, rates[0], CurrentTime);
      }
   }
}

void HideCursor(){
   Cursor hide;
   Pixmap empty;
   static char empty_data[] = {0,0,0,0,0,0,0,0};
   static XColor black = {0,0,0,0,0,0};

   empty = XCreateBitmapFromData(cg_display, cg_window, empty_data, 8, 8);
   hide  = XCreatePixmapCursor(cg_display, empty, empty, &black, &black, 0, 0);
   XDefineCursor(cg_display, cg_window, hide);
}

void CreateScene(int width, int height){
   int conf[] = {GLX_DOUBLEBUFFER, 
                 GLX_RGBA, 
                 GLX_DEPTH_SIZE, 12, 
                 GLX_RED_SIZE, 1, 
                 GLX_BLUE_SIZE, 1, 
                 GLX_GREEN_SIZE, 1, 
                 None};
   GLXContext context;
   XSetWindowAttributes swa;

   Atom fullscreen;

   cg_display = XOpenDisplay(NULL);
   cg_visual  = glXChooseVisual(cg_display, DefaultScreen(cg_display), conf);

   context = glXCreateContext(cg_display, cg_visual, NULL, True);
   swa.colormap = XCreateColormap(cg_display, RootWindow(cg_display, cg_visual->screen), cg_visual->visual, AllocNone);
   swa.border_pixel = 0;
   swa.event_mask = ExposureMask | StructureNotifyMask | ButtonPressMask | Button1MotionMask | KeyPressMask;
   cg_window  = XCreateWindow(cg_display, RootWindow(cg_display, cg_visual->screen),
                           0, 0, width, height, 0, cg_visual->depth, InputOutput,
                           cg_visual->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
   /*fullscreen = XInternAtom(cg_display, "_NET_WM_STATE_FULLSCREEN", False);
   XChangeProperty(cg_display, cg_window, XInternAtom(cg_display, "_NET_WM_STATE", False), 
                   XA_ATOM, 32, PropModeReplace,
                   (unsigned char *)&fullscreen, 1);

   HideCursor();
   /* Az aktuális méret lekérdezése */
  /* SetScreenSize(width, height);*/

   glXMakeCurrent(cg_display, cg_window, context);
   XMapWindow(cg_display, cg_window);

   /* Innentöl már csak opengl utasítások jönnek */
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 800.0, 600.0, 0.0, -10.0, 600.0);
}

void RestoreScene(){
   XRRSetScreenConfigAndRate(cg_display, cg_screen_conf, DefaultRootWindow(cg_display), cg_orig_size, cg_rotation, cg_orig_rate, CurrentTime);
   XRRFreeScreenConfigInfo(cg_screen_conf);
}
