#include "definitions.h"
#include "section.h"
#include "laneset.h"
#include "nodo.h"
#include "aux.h"

extern double lane_width;
extern color  escala[11];

extern lane_semaphore      mapping_lane_to_semaphore_polygon;
extern lane_end_point      mapping_lane_to_end_point;
extern laneset_start_point mapping_laneset_to_start_point;
extern lane_to_lane_polygon mapping_lane_to_lane_polygon;

extern int tipo_de_exibicao;

/*************************************************************************/
/*************************************************************************/

void laneset::calcula_poligonos_lanes()
{

  int nlanes = lanes.size();
  double angle;
  poligonos_lanes.clear();
  poligonos_semaphores.clear();
  lane_start_points.clear();
  lane_end_points.clear();

  // if laneset is facing one way (forward), we calculate the lanes polygons in a specific order
  if (arrow_direction == FORWARD_ARROW) {
	  
    // the ordering of lanes in a forward laneset must be a adjusted
    // in order to make the lane labelling consistent with opengl coordinate system

    //reverse(lanes.begin(), lanes.end());

    //     printf("laneset %d is FORWARD\n", id);
      
    for (int i=0; i < nlanes; i++) {
      
      poligono_simples *lane_pol = new poligono_simples();
      ponto *esquerda_inicio = new ponto();
      ponto *esquerda_fim = new ponto();
      ponto *direita_inicio = new ponto();
      ponto *direita_fim = new ponto();
      
      // calculates the lane's polygon points
      esquerda_inicio = point_in_grid(0, i,   1, 1.0/nlanes);
      esquerda_fim    = point_in_grid(1, i,   1, 1.0/nlanes);
      direita_inicio  = point_in_grid(0, i+1, 1, 1.0/nlanes);
      direita_fim     = point_in_grid(1, i+1, 1, 1.0/nlanes);
      
      lane_pol->push_back(esquerda_inicio);
      lane_pol->push_back(esquerda_fim);
      lane_pol->push_back(direita_fim);
      lane_pol->push_back(direita_inicio);
      
      poligonos_lanes.push_back(lane_pol);
      mapping_lane_to_lane_polygon[lanes[i]] = lane_pol;

      // calculates the lane's starting and ending points
      ponto *end_connection_point = new ponto();
      ponto *start_connection_point = new ponto();
      
      end_connection_point->x = esquerda_fim->x + (direita_fim->x - esquerda_fim->x)/2.0;
      end_connection_point->y = esquerda_fim->y + (direita_fim->y - esquerda_fim->y)/2.0;
      start_connection_point->x = esquerda_inicio->x + (direita_inicio->x - esquerda_inicio->x)/2.0;
      start_connection_point->y = esquerda_inicio->y + (direita_inicio->y - esquerda_inicio->y)/2.0;

      lane_start_points.push_back(start_connection_point);
      lane_end_points.push_back(end_connection_point);

      // adds the lane's ending point to a hash table, so it can be fastly recovered
      mapping_lane_to_end_point[lanes[i]] = end_connection_point;


      // calculates the semaphore polygon for this lane
      double radius = 0.30*sqrt((direita_fim->x - esquerda_fim->x)/2.0 * (direita_fim->x - esquerda_fim->x)/2.0 + 
				(direita_fim->y - esquerda_fim->y)/2.0 * (direita_fim->y - esquerda_fim->y)/2.0);

      poligono_simples *semaphore_pol = new poligono_simples();
      for (int k = 0; k < 8; k++) {    
	angle = 2*PI*k/8;
	ponto *p = new ponto();
	p->x = end_connection_point->x + radius * cos(angle);
	p->y = end_connection_point->y + radius * sin(angle);
	semaphore_pol->push_back(p);
      }
      poligonos_semaphores.push_back(semaphore_pol);

      // adds the lane's semaphore polygon to a hash table, so it can be fastly recovered
      mapping_lane_to_semaphore_polygon[lanes[i]] = semaphore_pol;

    }
  }


  // if laneset is facing another way (backwards), we calculate the lanes polygons in reverse order
  else {
	  
    //     printf("laneset %d is BACKWARDS\n", id);
    for (int i=0; i < nlanes; i++) {
      //    for (int i=nlanes-1; i >= 0; i--) {

      poligono_simples *lane_pol = new poligono_simples();
      ponto *esquerda_inicio = new ponto();
      ponto *esquerda_fim = new ponto();
      ponto *direita_inicio = new ponto();
      ponto *direita_fim = new ponto();
      
      // calculates the lane's polygon points
      esquerda_inicio = point_in_grid(1, i+1,   1, 1.0/nlanes);
      esquerda_fim    = point_in_grid(0, i+1,   1, 1.0/nlanes);
      direita_inicio  = point_in_grid(1, i, 1, 1.0/nlanes);
      direita_fim     = point_in_grid(0, i, 1, 1.0/nlanes);

      
      lane_pol->push_back(esquerda_inicio);
      lane_pol->push_back(esquerda_fim);
      lane_pol->push_back(direita_fim);
      lane_pol->push_back(direita_inicio);
      
      poligonos_lanes.push_front(lane_pol);
      mapping_lane_to_lane_polygon[lanes[i]] = lane_pol;

      // calculates the lane's starting and ending points
      ponto *end_connection_point = new ponto();
      ponto *start_connection_point = new ponto();

      start_connection_point->x = esquerda_inicio->x + (direita_inicio->x - esquerda_inicio->x)/2.0;
      start_connection_point->y = esquerda_inicio->y + (direita_inicio->y - esquerda_inicio->y)/2.0;
      end_connection_point->x = esquerda_fim->x + (direita_fim->x - esquerda_fim->x)/2.0;
      end_connection_point->y = esquerda_fim->y + (direita_fim->y - esquerda_fim->y)/2.0;

      lane_start_points.push_back(start_connection_point);
      lane_end_points.push_back(end_connection_point);

      // adds the lane's ending point to a hash table, so it can be fastly recovered
      mapping_lane_to_end_point[lanes[i]] = end_connection_point;


      // calculates the semaphore polygon for this lane
      double radius = 0.50*sqrt((direita_inicio->x - esquerda_inicio->x)/2.0 * \
				(direita_inicio->x - esquerda_inicio->x)/2.0 + \
				(direita_inicio->y - esquerda_inicio->y)/2.0 * \
				(direita_inicio->y - esquerda_inicio->y)/2.0);

      poligono_simples *semaphore_pol = new poligono_simples();
      for (int k = 0; k < 6; k++) {    
		angle = 2*PI*k/6;
		ponto *p = new ponto();
		p->x = end_connection_point->x + radius * cos(angle);
		p->y = end_connection_point->y + radius * sin(angle);
		semaphore_pol->push_back(p);
      }
      poligonos_semaphores.push_back(semaphore_pol);

      // adds the lane's semaphore polygon to a hash table, so it can be fastly recovered
      mapping_lane_to_semaphore_polygon[lanes[i]] = semaphore_pol;

      
    }
  } // else


  // returns the lanes vector to its original state
  //  reverse(lanes.begin(), lanes.end());
  
}


/*************************************************************************/
/*************************************************************************/


// rotate a point 'ang' radians
void rot(GLfloat *x, GLfloat *y, double ang)
{
  GLfloat tmpx, tmpy;
  tmpx = *x;
  tmpy = *y;
  
  tmpx = *x * cos(ang) - *y * sin(ang);
  tmpy = *y * cos(ang) + *x * sin(ang);

  *x = tmpx;
  *y = tmpy;
  
}


/*************************************************************************/
/*************************************************************************/


// divides the laneset polygon into a grid and returns the point in the ith
// position in x axis and jth position in y axis
ponto* laneset::point_in_grid(double i, double j, double xdivisions, double ydivisions)
{
  ponto p_esq;
  ponto p_dir;

  ponto *p = new ponto();

  poligono_simples pol = poligono_da_laneset;

  p_esq.x = pol[0]->x + i*xdivisions * (pol[1]->x - pol[0]->x);
  p_esq.y = pol[0]->y + i*xdivisions * (pol[1]->y - pol[0]->y);
       
  p_dir.x = pol[3]->x + i*xdivisions * (pol[2]->x - pol[3]->x);
  p_dir.y = pol[3]->y + i*xdivisions * (pol[2]->y - pol[3]->y);

  p->x = p_esq.x + j*ydivisions * (p_dir.x - p_esq.x);
  p->y = p_esq.y + j*ydivisions * (p_dir.y - p_esq.y);

  return p;

}



/*************************************************************************/
/*************************************************************************/


void laneset::calcula_poligono_seta_direcional()
{
  
  ponto *p1;
  ponto *p2;
  ponto *p3;
  ponto *p4;
  ponto *p5;
  ponto *p6;
  ponto *p7;
  
  double grid_step_dx = 1.0/5.0;
  double grid_step_dy = 1.0/5.0;

  if (arrow_direction == FORWARD_ARROW) {
    p1 = point_in_grid(1,   3, grid_step_dx, grid_step_dy);
    p2 = point_in_grid(1,   2, grid_step_dx, grid_step_dy);
    p3 = point_in_grid(3,   2, grid_step_dx, grid_step_dy);
    p4 = point_in_grid(3,   1, grid_step_dx, grid_step_dy);
    p5 = point_in_grid(4, 2.5, grid_step_dx, grid_step_dy);
    p6 = point_in_grid(3,   4, grid_step_dx, grid_step_dy);
    p7 = point_in_grid(3,   3, grid_step_dx, grid_step_dy);
  } else {
    p1 = point_in_grid(5-1, 5-  3, grid_step_dx, grid_step_dy);
    p2 = point_in_grid(5-1, 5-  2, grid_step_dx, grid_step_dy);
    p3 = point_in_grid(5-3, 5-  2, grid_step_dx, grid_step_dy);
    p4 = point_in_grid(5-3, 5-  1, grid_step_dx, grid_step_dy);
    p5 = point_in_grid(5-4, 5-2.5, grid_step_dx, grid_step_dy);
    p6 = point_in_grid(5-3, 5-  4, grid_step_dx, grid_step_dy);
    p7 = point_in_grid(5-3, 5-  3, grid_step_dx, grid_step_dy);
  }
    
  poligono_da_seta_direcional.clear();

  poligono_da_seta_direcional.push_back(p1);
  poligono_da_seta_direcional.push_back(p2);
  poligono_da_seta_direcional.push_back(p3);
  poligono_da_seta_direcional.push_back(p4);
  poligono_da_seta_direcional.push_back(p5);
  poligono_da_seta_direcional.push_back(p6);
  poligono_da_seta_direcional.push_back(p7);
 
  
}


/*************************************************************************/
/*************************************************************************/


laneset::laneset(int _id, int _position_in_section, int _total_number_lanesets, 
		 vetor_de_lanes _lanes, int _arrow_direction)
{

  id = _id;
  position_in_section = _position_in_section;
  total_number_lanesets = _total_number_lanesets;
  lanes = _lanes;
  arrow_direction = _arrow_direction;

  // cor inicial da rua eh verde bem claro
  cor[RED]   = INICIAL_R;
  cor[GREEN] = INICIAL_G;
  cor[BLUE]  = INICIAL_B;

}



/*************************************************************************/
/*************************************************************************/

void laneset::desenha_seta_direcional(int modo) 
{

  if (modo==SELECT)
    glPushName(id);

  if (tipo_de_exibicao == SHOW_LANES_INFO)
    glColor4f(0.6,0.8,1.0,0.25);
  else
    glColor4f(0.2,0.2,0.2,0.70);


  glBegin(GL_POLYGON);
      glVertex3f(poligono_da_seta_direcional[0]->x, poligono_da_seta_direcional[0]->y, +0.45);
      glVertex3f(poligono_da_seta_direcional[1]->x, poligono_da_seta_direcional[1]->y, +0.45);
      glVertex3f(poligono_da_seta_direcional[2]->x, poligono_da_seta_direcional[2]->y, +0.45);
      glVertex3f(poligono_da_seta_direcional[6]->x, poligono_da_seta_direcional[6]->y, +0.45);
  glEnd();					    
  glBegin(GL_POLYGON);				    
      glVertex3f(poligono_da_seta_direcional[3]->x, poligono_da_seta_direcional[3]->y, +0.45);
      glVertex3f(poligono_da_seta_direcional[4]->x, poligono_da_seta_direcional[4]->y, +0.45);
      glVertex3f(poligono_da_seta_direcional[5]->x, poligono_da_seta_direcional[5]->y, +0.45);
  glEnd();

  if (modo==SELECT)
    glPopName();

}

/*************************************************************************/
/*************************************************************************/

void laneset::desenha_sem_textura(int modo)
{
  poligono_simples::iterator p;
  

  glDisable(GL_TEXTURE_2D);

  glColor3f(cor[RED], cor[GREEN], cor[BLUE]);

  if (modo==SELECT)
    glPushName(id);
 
  // desenha laneset
  glBegin(GL_POLYGON);
     for (p = poligono_da_laneset.begin(); p != poligono_da_laneset.end(); p++)
       glVertex3f((*p)->x, (*p)->y, +0.25);
  glEnd();


  // desenha linha pontilhada divisoria de lanesets
  if (position_in_section != total_number_lanesets-1) {
    glColor3f(1.0, 0.75, 0.0);
    glLineWidth(2.0);
    // Enable line stippling
    glEnable(GL_LINE_STIPPLE);
    // Set the stippling pattern
    glLineStipple(5, 0xAAAA);
    
    glBegin(GL_LINES);
       glVertex3f( poligono_da_laneset[3]->x, poligono_da_laneset[3]->y, +0.45);
       glVertex3f( poligono_da_laneset[2]->x, poligono_da_laneset[2]->y, +0.45);
    glEnd();
  }
  
  if (modo==SELECT)
    glPopName();

}


/*************************************************************************/
/*************************************************************************/


void laneset::desenha_lanes(int modo)
{

  list < poligono_simples * >::iterator lane_pol;
  int i;
  for (i=0, lane_pol = poligonos_lanes.begin(); lane_pol != poligonos_lanes.end(); lane_pol++, i++) {

    if (modo==SELECT)
      glPushName(lanes[i]);

    glColor3f(escala[1-((i%2)*1)].R, escala[1-((i%2)*1)].G, escala[1-((i%2)*1)].B);
    glBegin(GL_POLYGON);
    
    poligono_simples::iterator p;    
    for (p = (*lane_pol)->begin(); p != (*lane_pol)->end(); p++) {
      glVertex3f((*p)->x, (*p)->y, +0.25);
    }
    glEnd();

    if (modo==SELECT)
      glPopName();

  }
 
}


/*************************************************************************/
/*************************************************************************/


void laneset::desenha_divisorias_lanes(int modo)
{

  list < poligono_simples * >::iterator lane_pol;
  for (lane_pol = poligonos_lanes.begin(); lane_pol != poligonos_lanes.end(); lane_pol++) {

    if (arrow_direction == BACKWARDS_ARROW) {
      if (lane_pol == poligonos_lanes.begin())
	continue;
    }


    glLineWidth(1.5);
    glEnable(GL_LINE_STIPPLE);
    // Set the stippling pattern
    glLineStipple(5, 0xAAAAAA);

    glColor4f(1-cor[RED], 1-cor[GREEN], 1-cor[BLUE],0.4);
    glBegin(GL_LINES);
       poligono_simples *lane = *lane_pol;
       glVertex3f( (*lane)[2]->x, (*lane)[2]->y, +0.5);
       glVertex3f( (*lane)[3]->x, (*lane)[3]->y, +0.5);

    glEnd();

  }
  
}


/*************************************************************************/
/*************************************************************************/


void laneset::desenha_semaforos(int modo)
{

  poligono_simples::iterator p;    


  glColor3f(0.8, 0.2, 0.0);
  for (unsigned int k = 0; k < poligonos_semaphores.size(); k++) {
    poligono_simples *semaph_pol = poligonos_semaphores[k];

    glBegin(GL_TRIANGLE_FAN);
       for (p = semaph_pol->begin(); p != semaph_pol->end(); p++)
         glVertex3f((*p)->x, (*p)->y, +0.30); 
    glEnd();
  }

  // desenha os connection points das lanes
  //   for (unsigned int i=0; i < lanes.size(); i++) {
  //     glPointSize(8.0);
  //     glColor3f(0.0, 0.9, 0.0);
  //     glBegin(GL_POINTS);
  //        glVertex3f(lane_start_points[i]->x, lane_start_points[i]->y, -0.4);
  //     glEnd();
  //     glColor3f(0.0, 0.0, 0.9);
  //     glBegin(GL_POINTS);
  //        glVertex3f(lane_end_points[i]->x, lane_end_points[i]->y, -0.4);
  //     glEnd();
  //   }
    
  // desenha o connection point da laneset
  //   glPointSize(8.0);
  //   glColor3f(0.0, 0.9, 0.0);
  //   glBegin(GL_POINTS);
  //   glVertex3f(laneset_start_point.x, laneset_start_point.y, -0.4);
  //   glEnd();


}





/*************************************************************************/
/*************************************************************************/

void laneset::update_color(float density)
{

  _current_density = density;


  int valor_escala = int(density*100) / 10;
  cor[RED]   = escala[valor_escala].R;
  cor[GREEN] = escala[valor_escala].G;
  cor[BLUE]  = escala[valor_escala].B;



}
/*************************************************************************/
/*************************************************************************/



laneset::~laneset() 
{
}


