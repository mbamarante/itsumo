#include "definitions.h"
#include "nodo.h"
#include "section.h"
#include "aux.h"
#include "glpng.h"
#include "fontes.h"



extern GLuint texture[NTEXTURAS];
extern GLuint number_texture[MAX_NUMBER_TEXTURE];

extern color  escala[11];
extern int cur_iter;
extern int iterations;
extern char network_name[4096];
extern double car_width;
extern double car_height;


/*************************************************************************/
/*************************************************************************/

// calculates the polygon of a cell inside the lane polygon 'pol'
poligono_simples* cell_polygon(poligono_simples *pol, double cell, double cell_size)
{
  ponto *p_esq_ini = new ponto();
  ponto *p_dir_ini = new ponto();
  ponto *p_esq_fim = new ponto();
  ponto *p_dir_fim = new ponto();

  poligono_simples *p = new poligono_simples();

  p_esq_ini->x = (*pol)[0]->x + cell*cell_size * ((*pol)[1]->x - (*pol)[0]->x);
  p_esq_ini->y = (*pol)[0]->y + cell*cell_size * ((*pol)[1]->y - (*pol)[0]->y);

  p_esq_fim->x = (*pol)[0]->x + (cell+1)*cell_size * ((*pol)[1]->x - (*pol)[0]->x);
  p_esq_fim->y = (*pol)[0]->y + (cell+1)*cell_size * ((*pol)[1]->y - (*pol)[0]->y);

  p_dir_ini->x = (*pol)[3]->x + cell*cell_size * ((*pol)[2]->x - (*pol)[3]->x);
  p_dir_ini->y = (*pol)[3]->y + cell*cell_size * ((*pol)[2]->y - (*pol)[3]->y);

  p_dir_fim->x = (*pol)[3]->x + (cell+1)*cell_size * ((*pol)[2]->x - (*pol)[3]->x);
  p_dir_fim->y = (*pol)[3]->y + (cell+1)*cell_size * ((*pol)[2]->y - (*pol)[3]->y);

  p->push_back(p_esq_ini);
  p->push_back(p_esq_fim);
  p->push_back(p_dir_fim);
  p->push_back(p_dir_ini);

  return p;

}


/*************************************************************************/
/*************************************************************************/


void define_escala()
{

    escala[0].R = 0.58;    escala[0].G = 0.89;   escala[0].B = 0.36;
    escala[1].R = 0.32;    escala[1].G = 0.61;   escala[1].B = 0.11;
    escala[2].R = 0.19;    escala[2].G = 0.39;   escala[2].B = 0.05;
    escala[3].R = 0.99;    escala[3].G = 0.95;   escala[3].B = 0.00;
    escala[4].R = 1.00;    escala[4].G = 0.74;   escala[4].B = 0.01;
    escala[5].R = 1.00;    escala[5].G = 0.52;   escala[5].B = 0.00;
    escala[6].R = 1.00;    escala[6].G = 0.37;   escala[6].B = 0.03;
    escala[7].R = 1.00;    escala[7].G = 0.25;   escala[7].B = 0.30;
    escala[8].R = 0.85;    escala[8].G = 0.13;   escala[8].B = 0.13;
    escala[9].R = 0.50;    escala[9].G = 0.02;   escala[9].B = 0.02;

  // Escala da Bazzan
//   escala[0].R = 0.40;    escala[0].G = 0.80;   escala[0].B = 0.66;
//   escala[1].R = 0.00;    escala[1].G = 1.00;   escala[1].B = 0.00;
//   escala[2].R = 0.00;    escala[2].G = 0.39;   escala[2].B = 0.00;
//   escala[3].R = 0.42;    escala[3].G = 0.55;   escala[3].B = 0.13;
//   escala[4].R = 1.00;    escala[4].G = 1.00;   escala[4].B = 0.00;
//   escala[5].R = 1.00;    escala[5].G = 0.84;   escala[5].B = 0.00;
//   escala[6].R = 0.85;    escala[6].G = 0.64;   escala[6].B = 0.12;
//   escala[7].R = 1.00;    escala[7].G = 0.27;   escala[7].B = 0.00;
//   escala[8].R = 1.00;    escala[8].G = 0.00;   escala[8].B = 0.00;
//   escala[9].R = 0.58;    escala[9].G = 0.00;   escala[9].B = 0.82;

  escala[10] = escala[9];

}





void desenha_info_objeto(string info)
{

  if (info == "")
    return;

  double initial_x = -2.8, initial_y = -1.35, initial_z = -3.5;

  double font_height = 0.15;

  char str_to_print[4096];
  strncpy(str_to_print, info.c_str(), 4095);

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glLoadIdentity();

  glColor4f(1.0, 1.0, 1.0, ALPHA);

  glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping

  glBindTexture(GL_TEXTURE_2D, texture[INFO_BOX_BACKG]);

  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (-3.3, -2.5, -4);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (-3.3, -1.2, -4);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (-1.4,  -1.2, -4);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (-1.4,  -2.5, -4);
  glEnd();

  glDisable(GL_TEXTURE_2D);


  glColor3f(0.68, 0.9, 0.59);

  int line=0;
  char *cur_line;

  cur_line = strtok (str_to_print, "\n");
  while (cur_line != NULL)
  {
    glRasterPos3f(initial_x, initial_y-(line*font_height), initial_z);
    glPrint(cur_line);
    cur_line = strtok (NULL, "\n");
    line += 1;
  }

  glPopMatrix();

}



void desenha_escala()
{

  double h=0.15;
  double w=0.5;
  
  double spacing = 0.03;

  glDisable(GL_TEXTURE_2D);

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glLoadIdentity();

  glTranslatef(2.5, -1.5, 0.0);

  glDisable(GL_LINE_STIPPLE);
  glLineWidth(3.0);
  glBegin (GL_LINE_LOOP);
       glColor3f(0.7, 0.7, 0.7);
       glVertex3f (0.0-spacing, -9*h-spacing, -4);
       glVertex3f (0.0-spacing,  h  +spacing, -4);
       glVertex3f (w  +spacing,  h  +spacing, -4);
       glVertex3f (w  +spacing, -9*h-spacing, -4);
  glEnd();

  for(int i=0; i<10; i++) {
    glLineWidth(1.0);
    glBegin (GL_POLYGON);
         glColor3f(escala[i].R, escala[i].G, escala[i].B);
	 glVertex3f (0.0,  0.0, -4);
	 glVertex3f (0.0,  h,   -4);
	 glVertex3f (  w,  h,   -4);
	 glVertex3f (  w,  0.0, -4);
    glEnd(); 

    glTranslatef(0.0, -h, 0.0);

  }

//   glColor3f(0.68, 0.9, 0.59);
//   glRasterPos3f(2.7, -4.8, -10);
  
//   string timestep_msg;
//   if (cur_iter == SIMULATION_NOT_STARTED)
//     timestep_msg = "Simulation not started";
//   else if (cur_iter == SIMULATION_OVER)
//     timestep_msg = "Simulation OVER (" + intToString(iterations) + ")";
//   else
//     timestep_msg = "timestep: " + intToString(cur_iter);
  
//   glPrint((char *)timestep_msg.c_str());
  
//   glRasterPos3f(-10.5, 10.0, -10);
//   string network_msg = "Network: " + string(network_name);
//   glPrint((char *)network_msg.c_str()); 


  glPopMatrix();

}


void desenha_legendas()
{

  glDisable(GL_TEXTURE_2D);

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glLoadIdentity();

  glTranslatef(2.5, -2.8, 0.0);

  glColor3f(0.68, 0.9, 0.59);
  glRasterPos3f(2.7, -4.8, -10);
  
  string timestep_msg;
  if (cur_iter == SIMULATION_NOT_STARTED)
    timestep_msg = "Simulation not started";
  else if (cur_iter == SIMULATION_OVER)
    timestep_msg = "Simulation OVER (" + intToString(iterations) + ")";
  else
    timestep_msg = "timestep: " + intToString(cur_iter);
  
  glPrint((char *)timestep_msg.c_str());
  
  glRasterPos3f(-10.5, 10.0, -10);
  string network_msg = "Network: " + string(network_name);
  glPrint((char *)network_msg.c_str()); 

  glPopMatrix();

}



void desenha_barra_de_controle()
{

  double first_x = -1.49;
  double button_width = 0.26;
  double button_spacing = 0.20;
  double scale_button_width = 0.17;
  double navigation_bar_width = 0.27;
  double navigation_button_width = 0.15;
  double start = 0.0;


  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_LINE_STIPPLE);

  glColor4f(1.0, 1.0, 1.0, ALPHA);

  glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping

  // ---------- background -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[CTRL_BAR_BACKG]);

  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (-1.7, -3.3, -4);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (-1.7, -2.5, -4);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (1.7,  -2.5, -4);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (1.7,  -3.3, -4);
  glEnd();


  // ---------- from-start button -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[FROM_START]);

  start = first_x + (button_width + button_spacing) * 0;
  glPushName(CLICKED_FROM_START);
  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (start, -2.88, -3.8);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (start, -2.62, -3.8);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (start + button_width, -2.62, -3.8);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (start + button_width, -2.88, -3.8);
  glEnd();
  glPopName();


  // ---------- play button -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[PLAY]);

  start = first_x + (button_width + button_spacing) * 1;
  glPushName(CLICKED_PLAY);
  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (start, -2.88, -3.8);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (start, -2.62, -3.8);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (start + button_width, -2.62, -3.8);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (start + button_width, -2.88, -3.8);
  glEnd();
  glPopName();


  // ---------- stop button -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[STOP]);

  start = first_x + (button_width + button_spacing) * 2;
  glPushName(CLICKED_STOP);
  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (start, -2.88, -3.8);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (start, -2.62, -3.8);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (start + button_width, -2.62, -3.8);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (start + button_width, -2.88, -3.8);
  glEnd();
  glPopName();


  // ---------- step button -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[STEP]);

  start = first_x + (button_width + button_spacing) * 3;
  glPushName(CLICKED_STEP);
  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (start, -2.88, -3.8);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (start, -2.62, -3.8);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (start + button_width, -2.62, -3.8);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (start + button_width, -2.88, -3.8);
  glEnd();
  glPopName();


  // ---------- increase scale button -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[INCREASE_SCALE]);

  start = (first_x + (button_width + button_spacing) * 4) - 0.05;
  glPushName(CLICKED_INCREASE_SCALE);
  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (start,                      -2.74,                     -3.8);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (start,                      -2.74 +scale_button_width, -3.8);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (start + scale_button_width, -2.74 +scale_button_width, -3.8);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (start + scale_button_width, -2.74,                     -3.8);
  glEnd();
  glPopName();



  // ---------- decrease scale button -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[DECREASE_SCALE]);

  start = (first_x + (button_width + button_spacing) * 4) - 0.05;
  glPushName(CLICKED_DECREASE_SCALE);
  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (start,                      -2.74 -scale_button_width-0.01,   -3.8);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (start,                      -2.74 -0.01, -3.8);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (start + scale_button_width, -2.74 -0.01, -3.8);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (start + scale_button_width, -2.74 -scale_button_width-0.01,   -3.8);
  glEnd();
  glPopName();


  // ---------- navigation bar -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[NAVIGATION_BAR]);

  start = (first_x + (button_width + button_spacing) * 5) - 0.25;
  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (start,                        -2.74 -navigation_bar_width,   -3.8);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (start,                        -2.74 +navigation_bar_width, -3.8);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (start + 2*navigation_bar_width, -2.74 +navigation_bar_width, -3.8);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (start + 2*navigation_bar_width, -2.74 -navigation_bar_width,   -3.8);
  glEnd();


  // ---------- zoom all button -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[ZOOM_ALL]);

  start = (first_x + (button_width + button_spacing) * 6) - 0.11;
  glPushName(CLICKED_ZOOM_ALL);
  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (start,                           -2.85,   -3.8);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (start,                           -2.85 +scale_button_width+0.02, -3.8);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (start + scale_button_width+0.02, -2.85 +scale_button_width+0.02, -3.8);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (start + scale_button_width+0.02, -2.85,   -3.8);
  glEnd();
  glPopName();



  // ---------- zoom in button -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[ZOOM_IN]);

  start = (first_x + (button_width + button_spacing) * 6) + 0.10;
  glPushName(CLICKED_ZOOM_IN);
  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (start,                           -2.65,   -3.8);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (start,                           -2.65 +scale_button_width+0.02, -3.8);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (start + scale_button_width+0.02, -2.65 +scale_button_width+0.02, -3.8);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (start + scale_button_width+0.02, -2.65,   -3.8);
  glEnd();
  glPopName();


  // ---------- zoom out button -----------------------
  glBindTexture(GL_TEXTURE_2D, texture[ZOOM_OUT]);

  start = (first_x + (button_width + button_spacing) * 6) + 0.10;
  glPushName(CLICKED_ZOOM_OUT);
  glBegin (GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f (start,                           -3.05,   -3.8);
       glTexCoord2f(0.0f, 0.0f); glVertex3f (start,                           -3.05 +scale_button_width+0.02, -3.8);
       glTexCoord2f(1.0f, 0.0f); glVertex3f (start + scale_button_width+0.02, -3.05 +scale_button_width+0.02, -3.8);
       glTexCoord2f(1.0f, 1.0f); glVertex3f (start + scale_button_width+0.02, -3.05,   -3.8);
  glEnd();
  glPopName();



  glDisable(GL_TEXTURE_2D);


  glColor4f(1.0, 1.0, 1.0, 0.0);

  // ---------- pan up button -----------------------
  start = (first_x + (button_width + button_spacing) * 5) - 0.08;
  glPushName(CLICKED_PAN_UP);
  glBegin (GL_POLYGON);
       glVertex3f (start,                           -2.62,   -3.7);
       glVertex3f (start,                           -2.62 +navigation_button_width, -3.7);
       glVertex3f (start + navigation_button_width, -2.62 +navigation_button_width, -3.7);
       glVertex3f (start + navigation_button_width, -2.62,   -3.7);
  glEnd();
  glPopName();
	   

  // ---------- pan down button -----------------------
  start = (first_x + (button_width + button_spacing) * 5) - 0.08;
  glPushName(CLICKED_PAN_DOWN);
  glBegin (GL_POLYGON);
       glVertex3f (start,                           -2.89,   -3.7);
       glVertex3f (start,                           -2.89 +navigation_button_width, -3.7);
       glVertex3f (start + navigation_button_width, -2.89 +navigation_button_width, -3.7);
       glVertex3f (start + navigation_button_width, -2.89,   -3.7);
  glEnd();
  glPopName();


  // ---------- pan left button -----------------------
  start = (first_x + (button_width + button_spacing) * 5) - 0.21;
  glPushName(CLICKED_PAN_LEFT);
  glBegin (GL_POLYGON);
       glVertex3f (start,                           -2.75,   -3.7);
       glVertex3f (start,                           -2.75 +navigation_button_width, -3.7);
       glVertex3f (start + navigation_button_width, -2.75 +navigation_button_width, -3.7);
       glVertex3f (start + navigation_button_width, -2.75,   -3.7);
  glEnd();
  glPopName();


  // ---------- pan right button -----------------------
  start = (first_x + (button_width + button_spacing) * 5) + 0.06;
  glPushName(CLICKED_PAN_RIGHT);
  glBegin (GL_POLYGON);
       glVertex3f (start,                           -2.75,   -3.7);
       glVertex3f (start,                           -2.75 +navigation_button_width, -3.7);
       glVertex3f (start + navigation_button_width, -2.75 +navigation_button_width, -3.7);
       glVertex3f (start + navigation_button_width, -2.75,   -3.7);
  glEnd();
  glPopName();

  glPopMatrix();

}



void carrega_texturas()
{

  pngInfo info;

  //---------------------------------------------------------------------------
  //this car is inserted every 5 steps, change the texture source if you wanna see a diferent one
  texture[CAR] = pngBind("car_special.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[CAR] == 0) {
    printf("Can't load texture for car!\n");
    exit(1);
  }
   printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);
  car_width = info.Width;
  car_height = info.Height;


  //---------------------------------------------------------------------------
  texture[CAR_SPECIAL] = pngBind("car_special.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
				 &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
				 GL_LINEAR_MIPMAP_NEAREST);
  if (texture[CAR_SPECIAL] == 0) {
    printf("Can't load texture for special car!\n");
    exit(1);
  }
//   printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[CTRL_BAR_BACKG] = pngBind("ctrl-bar-backg.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[CTRL_BAR_BACKG] == 0) {
    printf("Can't load texture for control bar background!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[INFO_BOX_BACKG] = pngBind("info-box-backg.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[INFO_BOX_BACKG] == 0) {
    printf("Can't load texture for info box background!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[FROM_START] = pngBind("from-start.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[FROM_START] == 0) {
    printf("Can't load texture for from-start button #3!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[PLAY] = pngBind("play.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[PLAY] == 0) {
    printf("Can't load texture for play button #3!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[STOP] = pngBind("stop.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[STOP] == 0) {
    printf("Can't load texture for stop button #3!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[STEP] = pngBind("step.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[STEP] == 0) {
    printf("Can't load texture for step button #3!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[INCREASE_SCALE] = pngBind("explode.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[INCREASE_SCALE] == 0) {
    printf("Can't load texture for increase-scale button #3!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[DECREASE_SCALE] = pngBind("implode.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[DECREASE_SCALE] == 0) {
    printf("Can't load texture for decrease-scale button #3!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[NAVIGATION_BAR] = pngBind("navigation-bar.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[NAVIGATION_BAR] == 0) {
    printf("Can't load texture for navigation-bar button #3!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[ZOOM_IN] = pngBind("zoom_in.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[ZOOM_IN] == 0) {
    printf("Can't load texture for zoom-in button #3!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[ZOOM_OUT] = pngBind("zoom_out.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[ZOOM_OUT] == 0) {
    printf("Can't load texture for zoom-out button #3!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);


  //---------------------------------------------------------------------------
  texture[ZOOM_ALL] = pngBind("zoom_all.png", PNG_BUILDMIPMAPS, PNG_ALPHA, 
			  &info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
			  GL_LINEAR_MIPMAP_NEAREST);
  if (texture[ZOOM_ALL] == 0) {
    printf("Can't load texture for zoom-all button #3!\n");
    exit(1);
  }
//    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);



  char number_str[7];
  for (int i=0; i<=10; i++) {
    sprintf(number_str, "%d.png", i);
    number_texture[i] = pngBind(number_str, PNG_BUILDMIPMAPS, PNG_ALPHA, 
				&info, GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, 
				GL_LINEAR_MIPMAP_NEAREST);
    if (number_texture[i] == 0) {
      printf("Can't load texture for number %d!\n", i);
      exit(1);
    }
    //    printf("Size=%i,%i Depth=%i Alpha=%i\n", info.Width, info.Height, info.Depth, info.Alpha);
  }



}



/*************************************************************************/
/*************************************************************************/



void desenha_ponto(ponto p)
{
  glColor3f(0.66, 0.078, 0.11);  // vermelho
  glPointSize(8.0);
  glBegin(GL_POINTS);
  glVertex3f(p.x, p.y, +0.55);
  glEnd();
}


/*************************************************************************/
/*************************************************************************/


void desenha_ponto(nodo *p)
{
  glColor3f(0.66, 0.078, 0.11);  // vermelho
  glPointSize(8.0);
  glBegin(GL_POINTS);
  glVertex3f((p->localizacao).x, (p->localizacao).y, 0);
  glEnd();
}


/*************************************************************************/
/*************************************************************************/


void desenha_segmento(nodo *inicio, nodo *fim)
{
  glLineWidth (2.0);
  glBegin(GL_LINES);
  glColor3f(0.78, 0.78, 0.23);    // amarelo
  glVertex3f ((inicio->localizacao).x, (inicio->localizacao).y, 0.5);
  glVertex3f ((fim->localizacao).x, (fim->localizacao).y, 0.5);
  glEnd();

//   desenha_ponto(inicio);
//   desenha_ponto(fim);
}


/*************************************************************************/
/*************************************************************************/


void desenha_pontilhado_ptr(segmento_ptr_pontos seg, int qual)
{

  glEnable (GL_LINE_STIPPLE);
  glLineStipple (1, 0x00FF);  /*  dashed  */
  if (qual==ESQUERDA)
    glColor3f(0.41, 0.7, 0.64);  // azul
  else
    glColor3f(0.88, 0.278, 0.34);  // rosa

  glLineWidth (2);
  glBegin(GL_LINES);
  glVertex3f (seg.ponto1->x, seg.ponto1->y,0);
  glVertex3f (seg.ponto2->x, seg.ponto2->y,0);
  glEnd();

//   desenha_ponto(*(seg.ponto1));
//   desenha_ponto(*(seg.ponto2));

  glDisable (GL_LINE_STIPPLE);
}

/*************************************************************************/
/*************************************************************************/


int interseccao(ponto *p, segmento_ptr_pontos s1, segmento_ptr_pontos s2)
{


  GLfloat x1=s1.ponto1->x;
  GLfloat y1=s1.ponto1->y;
  GLfloat x2=s1.ponto2->x;
  GLfloat y2=s1.ponto2->y;
  GLfloat x3=s2.ponto1->x;
  GLfloat y3=s2.ponto1->y;
  GLfloat x4=s2.ponto2->x;
  GLfloat y4=s2.ponto2->y;

  GLfloat denominador = (y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1);

  // sao retas paralelas
  if (equal(denominador, 0.0)) {
  //Debug	printf("PARALLEL\n");
	return PARALLEL;
  }

 
  GLfloat ua = (x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3);
  ua /= denominador;

  GLfloat ub= (x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3);
  ub/= denominador;

  p->x = x1 + ua*(x2 - x1);
  p->y = y1 + ua*(y2 - y1);


  if ((greaterThan(ua, 0.0)) && (lessThan(ua, 1.0))) {
    if ((greaterThan(ub, 0.0)) && (lessThan(ub, 1.0))) {
  //Debug      printf("INSIDE BOTH\n");
      return INSIDE_BOTH;
    }
    else {
  //Debug      printf("INSIDE FIRST\n");
      return INSIDE_FIRST;
    }
  }
  else
    if ((greaterThan(ub, 0.0)) && (lessThan(ub, 1.0))) {
  //Debug      printf("INSIDE SECOND\n");
      return INSIDE_SECOND;
    }
    else {
  //Debug      printf("INSIDE NONE\n");
  //Debug      printf("ua=%f ub=%f\n", ua, ub);
      return INSIDE_NONE;
    }


}


/*************************************************************************/
/*************************************************************************/


float max(float a, float b)
{
  if (a >= b)
    return a;
  else
    return b;
}

/*************************************************************************/
/*************************************************************************/


bool equal(float a, float b)
{

  if ((fabs(a-b)) <= FP_ERROR)
    return true;
  else
    return false;
  
}

/*************************************************************************/
/*************************************************************************/


bool greaterThan(float a, float b)
{
  if (((a + FP_ERROR) >= b) || ((a - FP_ERROR) >= b))
    return true;
  else
    return false;
    
}



bool lessThan(float a, float b)
{
  if (((a + FP_ERROR) <= b) || ((a - FP_ERROR) <=b))
    return true;
  else
    return false;
    
}

/*************************************************************************/
/*************************************************************************/

std::string intToString(int i)
{
	std::ostringstream tmp;
	tmp << i;

	return tmp.str();
}
