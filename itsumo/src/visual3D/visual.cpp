// TODO: substituir o isHorizontal e o isVertical por funcoes
// que usem erro relativo..

// as outras comparacoes (maior, menor, etc) podem contiuar usando
// soh o fp_error direto

// pintar os nodos de borda!!!

// substituir os printf por cout


#include <cstdlib>
#include<fstream>


#include "definitions.h"
#include "nodo.h"
#include "aux.h"
#include "section.h"
#include "laneset.h"
//#include "glpng.h"
#include "fontes.h"
#include "zpr.h"

Observador Camera1;

// Parametros de projecao
GLfloat fAspect;
GLfloat angle = 80;    // angulo de abertura para proj. perspectiva

GLdouble windowWidth = 800;
GLdouble windowHeight = 800;

GLdouble translateX=0.0, translateY=0.0, translateZ=0.0;
GLdouble scale=1.0;
GLdouble zInitial=50.0;

// planos near e far para recorte em profundidade
GLdouble near_plane= 1.0;
GLdouble far_plane=-10;

double lane_width = 0.5;
double car_width;
double car_height;


nodos_da_topologia nodos;
sections_da_topologia sections;
lanesets_da_topologia lanesets;

lane_semaphore      mapping_lane_to_semaphore_polygon;
lane_end_point      mapping_lane_to_end_point;
laneset_start_point mapping_laneset_to_start_point;
lane_to_lane_polygon mapping_lane_to_lane_polygon;

lane_id_to_parent_section_ptr mapping_lane_id_section_ptr;
laneset_id_to_parent_section_ptr mapping_laneset_id_section_ptr;

current_signalplan mapping_trafficlight_to_current_signalplan;
current_phase mapping_signalplan_to_current_phase;

open_directions_vector open_directions;
cars_and_locations  running_cars;
car_id_to_driver_name mapping_car_id_to_driver_name;

int iterations=0;
int cur_iter = SIMULATION_NOT_STARTED;

int nsections=0;
char network_name[4096];

string info_to_print;
int last_info_box_number = 0;

/* storage for textures  */
GLuint texture[NTEXTURAS];
GLuint number_texture[MAX_NUMBER_TEXTURE];

/* escala padrao de cores */
color escala[11];

int usar_texturas=0;
int toggle_see_scale=1;

/* define se as informacoes pintadas serao relativas a 
   sections (SHOW_SECTIONS_INFO) ou lanesets (SHOW_LANESETS_INFO) */
int tipo_de_exibicao = SHOW_SECTIONS_INFO;

// defines the state of the visualization (playing or stopped)
int state=STOPPED;

ifstream topo_file;




/*************************************************************************/
/*************************************************************************/


nodo* getNodeById (int id) {

  for (unsigned int i = 0; i < nodos.size(); i++)
    if ((nodos[i].first) == id)
      return nodos[i].second;

  return NULL;
}


/*************************************************************************/
/*************************************************************************/


section* getSectionById (int id) {

  for (unsigned int i = 0; i < sections.size(); i++)
    if ((sections[i].first) == id)
      return sections[i].second;

  return NULL;
}




/*************************************************************************/
/*************************************************************************/
 


void SetupRC(void)
{

//   glClearColor(0.25f, 0.25f, 0.25f, 1.0f);  // cor de fundo = cinza

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // cor de fundo = preto
//   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // cor de fundo = preto

  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);
    
//   glEnable(GL_ALPHA_TEST);

  // isso deixa MUITO infinitamente lento se sao usadas texturas.. go figure
  //   glEnable (GL_LINE_SMOOTH);

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //   glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  //   glShadeModel (GL_FLAT); 

  Camera1.atx=0.0;
  Camera1.aty=0.0;
  Camera1.atz=zInitial;
                         
  Camera1.tox=0.0;
  Camera1.toy=0.0;
  Camera1.toz=0.0;

}




/*************************************************************************/
/*************************************************************************/

// void desenha_mundo_com_textura()
// {

//   glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
//   glBindTexture(GL_TEXTURE_2D, texture[GRASS]);
  
// //   // chao
//    glBindTexture(GL_TEXTURE_2D, texture[GRASS]);
//    glBegin (GL_POLYGON);
//    glColor3f(1.0f, 1.0f, 1.0f);
//       glTexCoord2f(0.0f, 0.0f);              glVertex3f (-999999, -999999, 5);
//       glTexCoord2f(0.0f, 999999/50);         glVertex3f (-999999, +999999, 5);
//       glTexCoord2f(999999/50, 999999/50);    glVertex3f(+999999, +999999, 5);
//       glTexCoord2f(999999/50, 0.0f);         glVertex3f (+999999, -999999, 5);
//    glEnd(); 

//   // paisagem da esquerda
//   glBindTexture(GL_TEXTURE_2D, texture[LANDSCAPE1]);
//   glBegin (GL_POLYGON);
//   glColor3f(1.0f, 1.0f, 1.0f);  // cor de fundo = azul preto
//      glTexCoord2f(0.0f, 1.0f); glVertex3f (-999999, -999999, 4);
//      glTexCoord2f(1.0f, 1.0f); glVertex3f (-999999, +999999, 4);
//      glTexCoord2f(1.0f, 0.0f); glVertex3f (-999999, +999999, -999999/2);
//      glTexCoord2f(0.0f, 0.0f); glVertex3f (-999999, -999999, -999999/2);
//   glEnd(); 

//   // paisagem da frente
//   glBindTexture(GL_TEXTURE_2D, texture[LANDSCAPE2]);
//   glBegin (GL_POLYGON);
//   glColor3f(1.0f, 1.0f, 1.0f);  // cor de fundo = azul preto
//      glTexCoord2f(0.0f, 1.0f); glVertex3f (-999999, +999999, 4);
//      glTexCoord2f(1.0f, 1.0f); glVertex3f (+999999, +999999, 4);
//      glTexCoord2f(1.0f, 0.0f); glVertex3f (+999999, +999999, -999999/2);
//      glTexCoord2f(0.0f, 0.0f); glVertex3f (-999999, +999999, -999999/2);
//   glEnd(); 


//   // paisagem da direita
//   glBindTexture(GL_TEXTURE_2D, texture[LANDSCAPE3]);
//   glBegin (GL_POLYGON);
//   glColor3f(1.0f, 1.0f, 1.0f);  // cor de fundo = azul preto
//      glTexCoord2f(0.0f, 1.0f); glVertex3f (+999999, +999999, 4);
//      glTexCoord2f(1.0f, 1.0f); glVertex3f (+999999, -999999, 4);
//      glTexCoord2f(1.0f, 0.0f); glVertex3f (+999999, -999999, -999999/2);
//      glTexCoord2f(0.0f, 0.0f); glVertex3f (+999999, +999999, -999999/2);
//   glEnd(); 

//   // paisagem atras
//   glBindTexture(GL_TEXTURE_2D, texture[LANDSCAPE4]);
//   glBegin (GL_POLYGON);
//   glColor3f(1.0f, 1.0f, 1.0f);  // cor de fundo = azul preto
//     glTexCoord2f(0.0f, 1.0f); glVertex3f (+999999, -999999, 4);
//     glTexCoord2f(1.0f, 1.0f); glVertex3f (-999999, -999999, 4);
//     glTexCoord2f(1.0f, 0.0f); glVertex3f (-999999, -999999, -999999/2);
//     glTexCoord2f(0.0f, 0.0f); glVertex3f (+999999, -999999, -999999/2);
//   glEnd(); 

// }

/*************************************************************************/
/*************************************************************************/

void desenha_mundo_sem_textura()
{

  glDisable(GL_TEXTURE_2D);
  
  // chao
  glBegin (GL_POLYGON);
  glColor3f(CHAO_R, CHAO_G, CHAO_B);
     glVertex3f (-999999, -999999, -5);
     glVertex3f (-999999, +999999, -5);
     glVertex3f(+999999, +999999,  -5);
     glVertex3f (+999999, -999999, -5);
  glEnd(); 


}


/*************************************************************************/
/*************************************************************************/


 
void desenha_fases ( int modo )
{

  for (unsigned int i = 0; i < open_directions.size(); i++) {
    int lane_id = open_directions[i].first;
    int laneset_id = open_directions[i].second;

    poligono_simples *semaph_pol = mapping_lane_to_semaphore_polygon[lane_id];

    poligono_simples::iterator p;
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    for (p = semaph_pol->begin(); p != semaph_pol->end(); p++)
      glVertex3f((*p)->x, (*p)->y, -0.35); 
    glEnd();
    
    glDisable(GL_LINE_STIPPLE);

    glColor3f(0.8, 0.2, 0.0);
    ponto *lane_ori = mapping_lane_to_end_point[lane_id];
    ponto *laneset_dest = mapping_laneset_to_start_point[laneset_id];
    glLineWidth(1.0);
    glBegin(GL_LINES);
       glVertex3f( lane_ori->x, lane_ori->y, -0.35);
       glVertex3f( laneset_dest->x, laneset_dest->y, -0.30);
    glEnd();

  }

}


/*************************************************************************/
/*************************************************************************/

void desenha_numero_planos( int modo )
{

  current_signalplan::iterator p;
  nodo *tlight_node;
  int tlight_id, cur_signalplan, cur_phase;


  for (p = mapping_trafficlight_to_current_signalplan.begin(); 
       p != mapping_trafficlight_to_current_signalplan.end(); ++p)
    {

      tlight_id   = (p->first);
      cur_signalplan = (p->second).first;
      tlight_node = (p->second).second;
      cur_phase   = mapping_signalplan_to_current_phase[tlight_id].first;
      
      //Soma 1 para comecar do 1 e nao do 0
      tlight_node -> desenha_numero_plano_semaforico_atual(/*tlight_id, */cur_signalplan+1, modo);
      tlight_node -> desenha_numero_fase_atual(/*tlight_id, */cur_phase, modo);
      
      
    }
}


/*************************************************************************/
/*************************************************************************/


void desenha_carros( int modo)
{

  for(unsigned int i=0; i < running_cars.size(); i++)  {
    
    int car_id, in_lane, in_cell, total_cells;
    string driver_name;

    car_id = running_cars[i].first;
    in_lane = running_cars[i].second.first;
    in_cell = running_cars[i].second.second.first;
    total_cells = running_cars[i].second.second.second.first;
    driver_name = running_cars[i].second.second.second.second;

    mapping_car_id_to_driver_name[car_id] = driver_name;
    
    if (modo==SELECT)
      glPushName(-(1000+car_id));

    poligono_simples *lane_pol = mapping_lane_to_lane_polygon[in_lane];

    poligono_simples *cell_pol;
    cell_pol = cell_polygon(lane_pol, in_cell, (1.0)/total_cells);

    glEnable(GL_TEXTURE_2D);			// Enable Texture mapping

    // PAREI: nao esquecer de descomentar isso depois que resolver o
    // problema dos carros sumindo do engarrafamento (voltando pra 1a celula?)
    if ((i%5) == 0)
//     if (driver_name == "Nasch")
      glBindTexture(GL_TEXTURE_2D, texture[CAR]);
    else
      glBindTexture(GL_TEXTURE_2D, texture[CAR_SPECIAL]);


    // PAREI: falta arrumar o nasch.. no takedecision parece ok, mas
    // no commit ele nao muda de faixa direito.. carro burro
    
    glColor4f(1.0, 1.0, 1.0, ALPHA);
    
    glBegin(GL_POLYGON);
      glTexCoord2f(0.0f, 1.0f); glVertex3f( (*cell_pol)[0]->x, (*cell_pol)[0]->y, .55 );
      glTexCoord2f(0.0f, 0.0f); glVertex3f( (*cell_pol)[1]->x, (*cell_pol)[1]->y, .55 );
      glTexCoord2f(1.0f, 0.0f); glVertex3f( (*cell_pol)[2]->x, (*cell_pol)[2]->y, .55 );
      glTexCoord2f(1.0f, 1.0f); glVertex3f( (*cell_pol)[3]->x, (*cell_pol)[3]->y, .55 );
    glEnd();

    glDisable(GL_TEXTURE_2D);

    
    if (modo==SELECT)
      glPopName();
    

  }

}

/*************************************************************************/
/*************************************************************************/


void desenhaCoisas(int modo)
{
  unsigned int i;

  GLdouble xmin, xmax, ymin, ymax;
  xmin = section::xmin();
  xmax = section::xmax();
  ymin = section::ymin();
  ymax = section::ymax();


//   if (usar_texturas) {
//     desenha_mundo_com_textura();
//     // ruas
//     for (i = 0; i < sections.size(); i++)
//       (sections[i].second) -> desenha_com_textura(modo);
//     // cruzamentos
//     for (i = 0; i < nodos.size(); i++)
//       (nodos[i].second) -> desenha_cruzamento_com_textura(modo);
//   } 

//   else {

    desenha_mundo_sem_textura();
    

    if (tipo_de_exibicao == SHOW_SECTIONS_INFO) {
      // pinta sections
      for (i = 0; i < sections.size(); i++)
	(sections[i].second) -> desenha_sem_textura(modo);
    }
    else if (tipo_de_exibicao == SHOW_LANESETS_INFO) {
      for (i = 0; i < lanesets.size(); i++) {
	(lanesets[i].second) -> desenha_sem_textura(modo);
	// 	(lanesets[i].second) -> desenha_semaforos(modo);
	(lanesets[i].second) -> desenha_divisorias_lanes(modo);
	(lanesets[i].second) -> desenha_seta_direcional(modo);
      }
      desenha_fases(modo);
      desenha_numero_planos( modo );
    }
    else if (tipo_de_exibicao == SHOW_LANES_INFO) {
      for (i = 0; i < lanesets.size(); i++) {
	// 	(lanesets[i].second) -> desenha_semaforos(modo);
// 	(lanesets[i].second) -> desenha_divisorias_lanes(modo);
	(lanesets[i].second) -> desenha_seta_direcional(modo);
	(lanesets[i].second) -> desenha_lanes(modo);
      }
      desenha_fases(modo);
      desenha_numero_planos( modo );
      desenha_carros(modo);
    }
    else {
      printf("Tipo de exibicao desconhecido!! (%d)\n", tipo_de_exibicao);
      exit(1);
    }
    

    

    // cruzamentos
    for (i = 0; i < nodos.size(); i++) {
      //       (nodos[i].second) -> desenha_cruzamento_apenas_tlights_com_textura(modo);
      (nodos[i].second) -> desenha_cruzamento_sem_textura(modo);

    }

//   }


}


/*************************************************************************/
/*************************************************************************/



void PerspectiveViewing(void)
{

  glMatrixMode(GL_PROJECTION);
  // transformacoes sobre o tipo de projecao que serah feito (perspectiva, ortogonal, etc)

  glLoadIdentity();
  gluPerspective(angle,fAspect,near_plane,far_plane);


  glMatrixMode(GL_MODELVIEW);  
  // transformacoes de coordenadas do objeto, posicionamento da camera, etc.
  // transformacoes para modelagem dos objetos e para visualizacao dos objetos

  glLoadIdentity();
  gluLookAt (Camera1.atx,Camera1.aty,Camera1.atz,Camera1.tox,Camera1.toy,Camera1.toz, 0,1,0);
     
}



/*************************************************************************/
/*************************************************************************/

       
    
void zoomAll()
{
  GLdouble scale, translateX, translateY;
  

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt (Camera1.atx,Camera1.aty,Camera1.atz,Camera1.tox,Camera1.toy,Camera1.toz, 0,1,0);
  
  scale = 2 * (tan(angle/2*PI/180)) * Camera1.atz / section::bounding_box_diagonal();
  glScalef(scale, scale, scale);
  
  // o translate y estah ao contrario pq a rotina de render pinta as coisas em um mundo
  // rotacionado 180o em torno do eixo x. Logo, as coordenadas no eixo y ficam trocadas
  translateX = -section::xmax()+((section::xmax()-section::xmin())/2);
  translateY = -(section::ymin()+((section::ymax()-section::ymin())/2));
  
  glTranslatef(translateX, translateY, translateZ);

}


/*************************************************************************/
/*************************************************************************/


void ChangeSize(GLsizei w, GLsizei h)
{
  //  printf("change size\n");
  if ( h == 0 )
    h = 1;

  // Especifica o tamanho da viewport
  glViewport(0, 0, w, h);
  windowWidth = w;
  windowHeight = h;
  // Calcula a correcao de aspecto
  fAspect = (GLfloat)w/(GLfloat)h;

  PerspectiveViewing();
  zoomAll();

}

/*************************************************************************/
/*************************************************************************/

void RenderScene(void)
{

  static int firstTime=1;


  // Limpa a janela
  glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);  		
	//  glPushMatrix();
	//  glRotatef(180.0, 1.0, 0.0, 0.0);
	desenhaCoisas(RENDER);
	//  glPopMatrix();

  if (toggle_see_scale) {
    if (tipo_de_exibicao != SHOW_LANES_INFO)
      desenha_escala();
    desenha_legendas();
    desenha_barra_de_controle();
    desenha_info_objeto( info_to_print );
  }

  
  if (firstTime) {
    zoomAll();
    firstTime=0;
  }
  
  glFlush();
  glutSwapBuffers();
  
}


/*************************************************************************/
/*************************************************************************/

void ajusta(section *r1, section *r2, nodo *n)
{

  int result;
  ponto p;

  ponto p1, p2;

  //Debug  printf("\najustando %d e %d\n", r1->id, r2->id);
  
  segmento_ptr_pontos segmento_r1, segmento_r2;
  segmento_ptr_pontos segmento_oposto_r1, segmento_oposto_r2;



  if (r1->nodo_fim == n) {
  //Debug    printf("       r1 esquerda\n");
    segmento_r1 = r1->seg_esquerda;
    segmento_oposto_r1 = r1->seg_direita;
  }
  else {
    if (r1->nodo_inicio == n) {
  //Debug      printf("       r1 direita\n");
      segmento_r1 = r1->seg_direita;
      segmento_oposto_r1 = r1->seg_esquerda;
    }
    else {
  //Debug      printf("       opa! tentando ajustar sections %d e %d, mas o nodo %d nao estah conectado a %d\n", r1->id, r2->id, n->id, r1->id);
      exit(1);
      }
  }

  if (r2->nodo_fim == n) {
  //Debug    printf("       r2 direita\n");
    segmento_r2 = r2->seg_direita;
    segmento_oposto_r2 = r2->seg_esquerda;
  }
  else {
    if (r2->nodo_inicio == n) {
  //Debug      printf("       r2 esquerda\n");
      segmento_r2 = r2->seg_esquerda;
      segmento_oposto_r2 = r2->seg_direita;
    }
    else {
  //Debug      printf("       opa! tentando ajustar sections %d e %d, mas o nodo %d nao estah conectado a %d\n", r1->id, r2->id, n->id, r2->id);
      exit(1);
      }
  }

  result = interseccao(&p, segmento_r1, segmento_r2);
    
  switch (result) {
  case INSIDE_BOTH:
    r1 -> empurra_esquina(n, p, DESLOCAMENTO_DEFAULT);
    r2 -> empurra_esquina(n, p, DESLOCAMENTO_DEFAULT);
    break;

  case INSIDE_FIRST:
    r1 -> empurra_esquina(n, p, DESLOCAMENTO_DEFAULT);
    break;
      
  case INSIDE_SECOND:
    r2 -> empurra_esquina(n, p, DESLOCAMENTO_DEFAULT);
    break;


  case PARALLEL:
    // se sao paralelas, calcula-se um ponto intermediario
    // entre elas e entao empurra-se cada uma para longe
    // deste ponto

  //Debug   printf("Paralelas.. pegando os segmentos certos..\n");
   
   if ((r1->nodo_inicio == n) && (r2->nodo_inicio == n)) {
     p1 = r1 -> esquina_esquerda_nodo_inicio;
     p2 = r2 -> esquina_direita_nodo_inicio;
   }
   else {
     if (r1->nodo_fim == n)
       p1 = r1 -> esquina_esquerda_nodo_fim;
     else
       p1 = r1 -> esquina_direita_nodo_inicio;

     if (r2->nodo_fim == n)
       p2 = r2 -> esquina_direita_nodo_fim;
     else
       p2 = r2 -> esquina_esquerda_nodo_inicio;
     
   }
   
   p.x = p1.x + ((p2.x - p1.x)/2);
   p.y = p1.y + ((p2.y - p1.y)/2);
   
   
   r1 -> empurra_esquina(n, p, DESLOCAMENTO_DEFAULT + LARGURA_DEFAULT_DE_CRUZAMENTO);
   r2 -> empurra_esquina(n, p, DESLOCAMENTO_DEFAULT + LARGURA_DEFAULT_DE_CRUZAMENTO);
    
   break;

  case INSIDE_NONE:
  //Debug    printf("              Checando outro lado..\n");
    result = interseccao(&p, segmento_oposto_r1, segmento_oposto_r2);

    switch(result) {

    case INSIDE_BOTH:
  //Debug      printf("              deslocando r1 e r2\n");
  //Debug      printf("---------------------------------------\n");
      r1 -> empurra_esquina(n, p, DESLOCAMENTO_DEFAULT);
      r2 -> empurra_esquina(n, p, DESLOCAMENTO_DEFAULT);
  //Debug      printf("---------------------------------------\n");
      break;
    case INSIDE_FIRST:
  //Debug      printf("              deslocando r1\n");
  //Debug      printf("---------------------------------------\n");
      r1 -> empurra_esquina(n, p, DESLOCAMENTO_DEFAULT);
  //Debug      printf("---------------------------------------\n");
      break;
    case INSIDE_SECOND:
  //Debug      printf("              deslocando r2\n");
  //Debug      printf("---------------------------------------\n");
      r2 -> empurra_esquina(n, p, DESLOCAMENTO_DEFAULT);
  //Debug      printf("---------------------------------------\n");
      break;
    case PARALLEL:
  //Debug      printf("              tah, NAO PODERIA TER DADO UM NEGOCIO PARALELO!!!! erro apocaliptico #666\n");
      exit(1);
    case INSIDE_NONE:
  //Debug      printf("              continuando a vida como se nada tivesse acontecido\n");
      break;
    }

    break;
  }

}


/*************************************************************************/
/*************************************************************************/

void calcula_poligono_para_nodo_sozinho(nodo *n)
{
  
}

/*************************************************************************/
/*************************************************************************/

void ajusta_nodos()
{

  float diagonais_total=0;

  for (unsigned int i = 0; i < nodos.size(); i++) {
    nodo *n = nodos[i].second;

    vector < section_e_inclinacao > sections_do_nodo = n->sections_conectadas;
    section *r1, *r2;
    unsigned int j;

    if (sections_do_nodo.size() > 1) {
      for (j = 0; j < sections_do_nodo.size() - 1; j++) {
	r1 = sections_do_nodo[j].second;
	r2 = sections_do_nodo[j+1].second;
  //Debug	printf("------- AJUSTANDO %d com %d ---------\n", r1->id, r2->id);
	ajusta(r1, r2, n);
      }
      r1 = sections_do_nodo[j].second;
      r2 = sections_do_nodo[0].second;
  //Debug      printf("------- AJUSTANDO %d com %d ---------\n", r1->id, r2->id);
      ajusta(r1, r2, n);
      fflush(stdout);
    }
    else {
      // nodo "sozinho" (conectado a apenas uma rua).. precisa forcar o calculo do poligono
      if (sections_do_nodo.size() == 1) {
	//     calcula_poligono_para_nodo_sozinho(n);
      }
      else {
  //Debug	printf("Hey! Nodo %d nao estah conectado a nada!!!\n", nodos[i].first);
	exit(1);
      }
    }
  }



  for (unsigned int i = 0; i < nodos.size(); i++) {
    nodo *n = nodos[i].second;
    n->calcula_poligono_do_cruzamento();
    diagonais_total += n->diagonal_length();
  }

  nodo::diagonal_media = diagonais_total/nodos.size();

  //   printf("---------->>>> MEDIA_x: %f <<<<------------\n", nodo::diagonal_media);

  for (unsigned int i = 0; i < sections.size(); i++) {
    (sections[i].second)->calcula_poligono_da_section();
  }


}


/*************************************************************************/
/*************************************************************************/


void calcula_poligonos_lanesets()
{

  for (unsigned int i = 0; i < lanesets.size(); i++) {
    
    section *section_ptr = lanesets[i].first;
    laneset *ls_ptr = lanesets[i].second;

    ls_ptr->poligono_da_laneset = section_ptr->polygon_nth_laneset(ls_ptr->position_in_section, 
								   section_ptr -> nlanesets);

    
    ls_ptr -> calcula_poligonos_lanes();
    ls_ptr -> calcula_poligono_seta_direcional();

    // calcula ponto inicial da laneset
    ponto *p1, *p2;
    if (ls_ptr->arrow_direction == FORWARD_ARROW) {
      p1 = ls_ptr->poligono_da_laneset[3];
      p2 = ls_ptr->poligono_da_laneset[0];
      (ls_ptr->laneset_start_point).x = p1->x - (p1->x - p2->x)/2.0;
      (ls_ptr->laneset_start_point).y = p1->y - (p1->y - p2->y)/2.0;
      mapping_laneset_to_start_point[ls_ptr->id] = &(ls_ptr->laneset_start_point);
    }
    else {
      p1 = ls_ptr->poligono_da_laneset[1];
      p2 = ls_ptr->poligono_da_laneset[2];
      (ls_ptr->laneset_start_point).x = p1->x - (p1->x - p2->x)/2.0;
      (ls_ptr->laneset_start_point).y = p1->y - (p1->y - p2->y)/2.0;
      mapping_laneset_to_start_point[ls_ptr->id] = &(ls_ptr->laneset_start_point);
    }


  }

}


/*************************************************************************/
/*************************************************************************/









void instancia_topologia()
{

  int n_nodes, node_id, section_id, laneset_id, lane_id, length;
  float x_coord, y_coord;
  float menor_section;
  int n_cells_menor_section;
  int section_start_node_id, section_end_node_id, laneset_start_node_id;
  int number_of_lanesets, number_of_lanes, objects_contained;
  char buffer[4096], newline_tmp[2];

  nodo *n;
  section *sec;
  laneset *ls;

  nodos.clear();
  lanesets.clear();
  sections.clear();
  open_directions.clear();
  mapping_trafficlight_to_current_signalplan.clear();
  
  running_cars.clear();
  mapping_car_id_to_driver_name.clear();

  mapping_lane_id_section_ptr.clear();
  mapping_laneset_id_section_ptr.clear();

  cur_iter = SIMULATION_NOT_STARTED;
 
  topo_file >> buffer;
  if (strcmp(buffer, "NETWORK_NAME")) {
    printf("Error parsing topology! Expected a 'NETWORK_NAME' session, not %s\n", buffer);
    exit(-1);
  }

  topo_file.read(newline_tmp, 1); // reads end-of-line
  topo_file.getline(network_name, 4095);

  
  topo_file >> buffer;
  if (strcmp(buffer, "NODES")) {
    printf("Error parsing topology! Expected a 'NODES' session, not %s\n", buffer);
    exit(-1);
  }

  // reads the number of nodes in the topology
  topo_file >> n_nodes;
  // instantiates all the nodes
  for (int i=0; i < n_nodes; i++) {

    char node_name[4096]; 
    int tlight_id=-1, source_id, sink_id;
    int n_sources, n_sinks;
    vector < int > sources;
    vector < int > sinks;
    sources.clear();
    sinks.clear();
    
    topo_file >> node_id >> x_coord >> y_coord;

    topo_file.read(newline_tmp, 1); // reads end-of-line
    topo_file.getline(node_name, 4095);

    topo_file >> objects_contained;

    switch (objects_contained) {
    case SOURCE:
      topo_file >> n_sources;
      for (int s=0; s < n_sources; s++) {
	topo_file >> source_id;
	sources.push_back(source_id);
      }
      break;
    case SOURCE_TRAFFICLIGHT:
      topo_file >> tlight_id;
      topo_file >> n_sources;
      for (int s=0; s < n_sources; s++) {
	topo_file >> source_id;
	sources.push_back(source_id);
      }
      break;
    case SINK:
      topo_file >> n_sinks;
      for (int s=0; s < n_sinks; s++) {
	topo_file >> sink_id;
	sinks.push_back(sink_id);
      }
      break;
    case SINK_TRAFFICLIGHT:
      topo_file >> tlight_id;
      topo_file >> n_sinks;
      for (int s=0; s < n_sinks; s++) {
	topo_file >> sink_id;
	sinks.push_back(sink_id);
      }
      break;
    case SOURCE_SINK:
      topo_file >> n_sources;
      for (int s=0; s < n_sources; s++) {
	topo_file >> source_id;
	sources.push_back(source_id);
      }
      topo_file >> n_sinks;
      for (int s=0; s < n_sinks; s++) {
	topo_file >> sink_id;
	sinks.push_back(sink_id);
      }
      break;
    case TRAFFIC_LIGHT:
      topo_file >> tlight_id;
      break;
    case SOURCE_SINK_TRAFFICLIGHT:
      topo_file >> tlight_id;
      topo_file >> n_sources;
      for (int s=0; s < n_sources; s++) {
	topo_file >> source_id;
	sources.push_back(source_id);
      }
      topo_file >> n_sinks;
      for (int s=0; s < n_sinks; s++) {
	topo_file >> sink_id;
	sinks.push_back(sink_id);
      }
      break;
    case NO_OBJECTS:
      break;
    default:
      cout << "Unknown objects contained (identifier #" << objects_contained << ") in node " << node_id << endl;
      exit(1);
      break;
    }
    
    //Debug     printf("Instatiating node #%d (%f, %f) with traffic+source=%d\n", node_id, x_coord, y_coord, objects_contained);
    n = new nodo(x_coord, y_coord, objects_contained, node_id, node_name, tlight_id, sources, sinks);
    nodos.push_back(make_pair(n->id, n));
  }

  topo_file >> buffer;
  if (strcmp(buffer, "SECTIONS")) {
    printf("Error parsing topology! Expected a 'SECTIONS' session\n");
    exit(-1);
  }

  // reads the number of sections in the topology
  topo_file >> nsections;
  // instantiates all the sections
  for (int i=0; i < nsections; i++) {

    char street_name[4096];

    topo_file >> section_id >> section_start_node_id >> section_end_node_id >> number_of_lanesets >> length;
    topo_file.read(newline_tmp, 1); // reads end-of-line
    topo_file.getline(buffer, 4095);

    topo_file.getline(street_name, 4095);

    sec = new section(getNodeById(section_start_node_id), getNodeById(section_end_node_id), 
		      number_of_lanesets, section_id, length, buffer, street_name);

    
    for (int j=0; j<number_of_lanesets; j++) {
      vetor_de_lanes lanes;
      topo_file >> laneset_id >> laneset_start_node_id >> number_of_lanes;
      sec -> total_number_of_lanes += number_of_lanes;
      

      for (int k=0; k<number_of_lanes; k++) {
	topo_file >> lane_id;
	lanes.push_back(lane_id);
      }
      
      int laneset_direction;
      if (laneset_start_node_id == section_start_node_id) 
	laneset_direction = FORWARD_ARROW;
      else
	laneset_direction = BACKWARDS_ARROW;

      ls = new laneset(laneset_id,
		       j, // position in section
		       number_of_lanesets, // total number of lanesets in this section
		       lanes,
		       laneset_direction);


      for (unsigned int l=0; l < lanes.size(); l++) 
	mapping_lane_id_section_ptr[lanes[l]] = make_pair(sec, ls);

      (sec->lanesets).push_back(make_pair(laneset_id, lanes));

      lanesets.push_back(make_pair(sec, ls));
      mapping_laneset_id_section_ptr[laneset_id] = make_pair(sec, ls);

    }

  //Debug    printf("Instatiating section #%d, also known as '%s' (%d -> %d) with %d lanesets\n", section_id, buffer, section_start_node_id, section_end_node_id, number_of_lanesets);

    sections.push_back(make_pair(sec->id, sec));
  }

  topo_file.read(newline_tmp, 2); // reads end-of-line
  topo_file.getline(buffer, 4095);

  if (!strcmp(buffer, "DATA SECTIONS")) {
    //     printf("lendo sections\n");
    tipo_de_exibicao = SHOW_SECTIONS_INFO;
  } else if (!strcmp(buffer, "DATA LANESETS")) {
    //     printf("lendo lanesets\n");
    tipo_de_exibicao = SHOW_LANESETS_INFO;
  } else if (!strcmp(buffer, "DATA LANES")) {
    //     printf("lendo lanes\n");
    tipo_de_exibicao = SHOW_LANES_INFO;
  }
  else {
    printf("Error parsing topology! Expected a 'DATA {SECTIONS | LANESETS}' session\n");
    exit(-1);
  }
  
 
  topo_file >> iterations;
  printf("Visual3D expecting %d iterations now\n", iterations);


  for (unsigned int i = 0; i < nodos.size(); i++) {
  //Debug    printf("examinando nodo %d\n", nodos[i].first);
  
    vector < section_e_inclinacao > sections_do_nodo = (nodos[i].second)->sections_conectadas;
    for (unsigned int j = 0; j < sections_do_nodo.size(); j++) {
  //Debug      printf("section %d com inclinacao %f\n", (sections_do_nodo[j].second)->id, (sections_do_nodo[j].first));
    }

    (nodos[i].second)->ordena_sections();

  //Debug    printf("\n");
  }


  float comprimento_section;
  menor_section = (sections[0].second)->comprimento();
  n_cells_menor_section = (sections[0].second)->length;

  for (unsigned int i = 0; i < sections.size(); i++) {
    comprimento_section = (sections[i].second)->comprimento();
    if (comprimento_section <= menor_section) {
      menor_section = comprimento_section;
      n_cells_menor_section = (sections[i].second)->length;
    }
    (sections[i].second)->calcula_poligono_da_section();
  }


  double cell_size = (1.0*menor_section) / n_cells_menor_section;
  
//   // super chutao pra estimativa do comprimento de cada lane (1/12.5=8% do comprimento da menor section)
//   lane_width = menor_section / 12.5;

  // considering the cell size, makes the cell (lane) width be proportional to the dimensions of a car
  // (actually 90% of the exact proportional width.. we want the car to be higher than thicker)
  lane_width = ((car_width * cell_size) / car_height) * 0.8;

  for (unsigned int i = 0; i < sections.size(); i++)
    sections[i].second->calcula_esquinas();

  ajusta_nodos();
  calcula_poligonos_lanesets();


}


void remove_info_box(int this_box_number)
{
  // if a new info box is being displayed, do not clear info_to_print;
  // it will be cleared by the timer function setted by the latest
  // info box
  if (this_box_number == last_info_box_number) {
    info_to_print = "";
    glutPostRedisplay();
  }
}


void le_mais_dados(int redisplay)
{

    float density;
    int ntlights, tlightid, tlight_node_id, cur_signalplan, nphases, cur_phase, phase_dur, fromLane, toLaneset;
    char driver_name[4096], newline_tmp[2];

    if (cur_iter == SIMULATION_OVER) {
      printf("OVER\n");
      return;
    }

    topo_file >> cur_iter;

    if ((cur_iter % 50)==0)
      printf("it=%d\n", cur_iter);
    
    if (cur_iter+1 >= iterations) {
      cur_iter = SIMULATION_OVER;
    }


    //printf("\n\n------------------ %d ----------\n", cur_iter);

      //-------------------------- READS SECTIONS INFO --------------------
    if (tipo_de_exibicao == SHOW_SECTIONS_INFO) {
      
      for (int i=0; i<nsections-1;i++) {
    	  topo_file >> density;
    	  sections[i].second->update_color(density);
      }
      topo_file >> density;
      sections[nsections-1].second->update_color(density);
    



      //-------------------------- READS LANESETS INFO --------------------
    } else if (tipo_de_exibicao == SHOW_LANESETS_INFO) {
      
      // reads the densities
      for (unsigned int i = 0; i < lanesets.size()-1; i++) {
		topo_file >> density;
		(lanesets[i].second) -> update_color(density);
      }
      topo_file >> density;
      ( lanesets[lanesets.size()-1].second )->update_color(density);
      
      // reads the traffic lights status (open directions)
      open_directions.clear();
      mapping_trafficlight_to_current_signalplan.clear();

      topo_file >> ntlights;
      //Debug    printf("Lendo info de %d semaforos\n", ntlights);
      for(int t=0; t < ntlights; t++) {
    	  topo_file >> tlightid >> tlight_node_id >> cur_signalplan >> cur_phase >> phase_dur >> nphases;
	
		nodo *tlight_node = getNodeById(tlight_node_id);
		mapping_trafficlight_to_current_signalplan[tlightid] = make_pair(cur_signalplan, tlight_node);
		mapping_signalplan_to_current_phase[tlightid] = make_pair(cur_phase, phase_dur);
	
		//Debug      printf("\t#%d tem %d fases\n", tlightid, nphases);
		for (int ph=0; ph < nphases; ph++) {
		  topo_file >> fromLane >> toLaneset;
		  open_directions.push_back(make_pair(fromLane, toLaneset));
		  //Debug	printf("\t\tfrom %d to %d\n", fromLane, toLaneset);
		}
      }



      //-------------------------- READS LANES INFO --------------------
    } else if (tipo_de_exibicao == SHOW_LANES_INFO) {
	      running_cars.clear();
	      mapping_car_id_to_driver_name.clear();
	
	      int ncars, car_id, in_lane, in_cell, total_cells;
	      topo_file >> ncars;
	      for(int i=0; i<ncars; i++) {
		topo_file >> car_id >> in_lane >> in_cell >> total_cells;
		topo_file.read(newline_tmp, 1); // reads end-of-line
		topo_file.getline(driver_name, 4095);
		//printf("\tcarro %d na lane %d cell %d\n", car_id, in_lane, in_cell);
		running_cars.push_back(make_pair(car_id, 
						 make_pair(in_lane, 
							   make_pair(in_cell, 
								     make_pair(total_cells, driver_name)))));
      }


      // reads the traffic lights status (open directions)
      open_directions.clear();
      mapping_trafficlight_to_current_signalplan.clear();

      topo_file >> ntlights;
      //Debug    printf("Lendo info de %d semaforos\n", ntlights);
      for(int t=0; t < ntlights; t++) { 
		topo_file >> tlightid >> tlight_node_id >> cur_signalplan >> cur_phase >> phase_dur >> nphases;
	
		nodo *tlight_node = getNodeById(tlight_node_id);
		mapping_trafficlight_to_current_signalplan[tlightid] = make_pair(cur_signalplan, tlight_node);
		mapping_signalplan_to_current_phase[tlightid] = make_pair(cur_phase,phase_dur);
	
		//Debug      printf("\t#%d tem %d fases\n", tlightid, nphases);
		for (int ph=0; ph < nphases; ph++) {
		  topo_file >> fromLane >> toLaneset;
		  open_directions.push_back(make_pair(fromLane, toLaneset));
		  //Debug	printf("\t\tfrom %d to %d\n", fromLane, toLaneset);
		}
      }

    }
  
    else {
      printf("Tipo de exibicao desconhecido!! (%d)\n", tipo_de_exibicao);
      exit(1);
    }

    
    if (state == PLAYING)
      glutTimerFunc(FRAME_TIME, le_mais_dados, 1);

    if (redisplay==1)
      glutPostRedisplay();


}


/*************************************************************************/
/*************************************************************************/

 
void keyboard_normal ( unsigned char key, int x, int y )
{

  pair < int, nodo * > xyz;

  switch (key) {

  case 's':
    state = STOPPED;
    le_mais_dados(0);

    break;
 
  case 'z':
    zoomAll();
    break;

  case 'v':
    toggle_see_scale ^= 1;
    break;
 

//   case '1':
//     nodo::toggle_paint_traffic_lights();
//     break;

//   case '2':
//     nodo::toggle_paint_sources();
//     break;

//  case 't':
//    usar_texturas ^= 1;
//    break;

     
  case '+':
    lane_width += 0.3;

    for (unsigned int i = 0; i < sections.size(); i++)
      sections[i].second->calcula_esquinas();
    
    ajusta_nodos();
    calcula_poligonos_lanesets();

    break;

  case '-':
    lane_width -= 0.3;
    for (unsigned int i = 0; i < sections.size(); i++)
      sections[i].second->calcula_esquinas();
   
    ajusta_nodos();
    calcula_poligonos_lanesets();
    break;


  case 27:
    KillFont();
    topo_file.close();
    exit(0);
    break;
  }

      
  glutPostRedisplay();

}

/*************************************************************************/
/*************************************************************************/



void keyboard_special (int key, int x, int y)
{

  glMatrixMode(GL_MODELVIEW);

  if(key == GLUT_KEY_LEFT)
    glTranslatef(20, 0, 0);

  if(key == GLUT_KEY_RIGHT)
    glTranslatef(-20, 0, 0);

  if(key == GLUT_KEY_UP)
    glTranslatef(0, -20, 0);

  if(key == GLUT_KEY_DOWN)
    glTranslatef(0, 20, 0);
                        
  glutPostRedisplay();

}


/*************************************************************************/
/*************************************************************************/

void pick(GLint name)
{

  GLfloat window_center[4] = { 0,0,0,0 };
  
  int old_state;
  double old_lane_width;

  switch (name) {
   case CLICKED_FROM_START:
     old_state = state;
     old_lane_width = lane_width;

     state = STOPPED;
     topo_file.seekg (0, ios::beg);
     instancia_topologia();

     state = old_state;

     lane_width = old_lane_width;
     for (unsigned int i = 0; i < sections.size(); i++)
       sections[i].second->calcula_esquinas();
     ajusta_nodos();
     calcula_poligonos_lanesets();
     break;

   case CLICKED_PLAY:
     state = PLAYING;
     le_mais_dados(1);
     break;

   case CLICKED_STOP:
     state = STOPPED;
     break;

   case CLICKED_STEP:
     state = STOPPED;
     le_mais_dados(1);
     break;

   case CLICKED_INCREASE_SCALE:
     lane_width += 0.3;
     for (unsigned int i = 0; i < sections.size(); i++)
       sections[i].second->calcula_esquinas();
     ajusta_nodos();
     calcula_poligonos_lanesets();
     break;

   case CLICKED_DECREASE_SCALE:
     lane_width -= 0.3;
     for (unsigned int i = 0; i < sections.size(); i++)
       sections[i].second->calcula_esquinas();
     ajusta_nodos();
     calcula_poligonos_lanesets();
     break;

   case CLICKED_PAN_UP:
     glMatrixMode(GL_MODELVIEW);
     glTranslatef(0, -20, 0);
     glutPostRedisplay();
     break;

   case CLICKED_PAN_DOWN:
     glMatrixMode(GL_MODELVIEW);
     glTranslatef(0, 20, 0);
     glutPostRedisplay();
     break;

   case CLICKED_PAN_LEFT:
     glMatrixMode(GL_MODELVIEW);
     glTranslatef(20, 0, 0);
     glutPostRedisplay();
     break;

   case CLICKED_PAN_RIGHT:
     glMatrixMode(GL_MODELVIEW);
     glTranslatef(-20, 0, 0);
     glutPostRedisplay();
     break;

   case CLICKED_ZOOM_ALL:
     zoomAll();
     break;

   case CLICKED_ZOOM_IN:

     glMatrixMode(GL_MODELVIEW);

     // finds out the worlds coordinate corresponding to the center of the window
     window_to_world(glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_HEIGHT)/2, window_center);

     glTranslatef(window_center[0], window_center[1], window_center[2]);
     glScalef(1.1,1.1,1.1);
     glTranslatef(-window_center[0], -window_center[1], -window_center[2]);

     glutPostRedisplay();
     break;

   case CLICKED_ZOOM_OUT:
     glMatrixMode(GL_MODELVIEW);

     // finds out the worlds coordinate corresponding to the center of the window
     window_to_world(glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_HEIGHT)/2, window_center);

     glTranslatef(window_center[0], window_center[1], window_center[2]);
     glScalef(0.9, 0.9, 0.9);
     glTranslatef(-window_center[0], -window_center[1], -window_center[2]);

     glutPostRedisplay();
     break;

   case -1:
     // object unknown; do nothing
     break;

   default:
     // pode ser: lane, laneset, nodo, trafficlight, carro

     
     // -------------- is the picked object a node?
     nodo *tmp_node = getNodeById (name);
     if (tmp_node != NULL) {
       info_to_print = tmp_node -> info(cur_iter);
       last_info_box_number += 1;
       glutTimerFunc(INFO_BOX_TIME, remove_info_box, last_info_box_number);
       break;
     }


     pair < section *, laneset * > tmp_section_laneset;

     // -------------- is the picked object a lane?
     tmp_section_laneset.first = NULL;
     tmp_section_laneset.second = NULL;
     
     tmp_section_laneset = mapping_lane_id_section_ptr[name];
     if (tmp_section_laneset.first != NULL) {
	 info_to_print = tmp_section_laneset.first -> infoLane( name, tmp_section_laneset.second );
	 last_info_box_number += 1;
	 glutTimerFunc(INFO_BOX_TIME, remove_info_box, last_info_box_number);
	 break;
     }


     // -------------- is the picked object a laneset?   
     tmp_section_laneset.first = NULL;
     tmp_section_laneset.second = NULL;

     tmp_section_laneset = mapping_laneset_id_section_ptr[name];
     if (tmp_section_laneset.first != NULL) {
	 info_to_print = tmp_section_laneset.first -> infoLaneSet( name, tmp_section_laneset.second );
	 last_info_box_number += 1;
	 glutTimerFunc(INFO_BOX_TIME, remove_info_box, last_info_box_number);
	 break;
     }


     // -------------- is the picked object a car?   
     string tmp_driver_name = "";

     tmp_driver_name = mapping_car_id_to_driver_name[-(1000+name)];
     if (tmp_driver_name != "") {
       ostringstream tmp_driver_description; 

       // we have to correct the car id, because when we did the
       // pushname we transformed it to "-(1000+id)"
       tmp_driver_description << "Car: " << -(1000+name) << endl;

       tmp_driver_description << "Driven by a " << tmp_driver_name << endl;
       
       info_to_print = tmp_driver_description.str();
       last_info_box_number += 1;
       glutTimerFunc(INFO_BOX_TIME, remove_info_box, last_info_box_number);
       break;
     }



     printf("Selected unknown object with id  %d\n",name);

     
     
  }

}


/*************************************************************************/
/*************************************************************************/

void RenderSceneSelect(void)
{
    /* Name-stack manipulation for the purpose of
       selection hit processing when mouse button
       is pressed.  Names are ignored in normal
       OpenGL rendering mode.                    */

//   glPushMatrix();
//   glRotatef(180.0, 1.0, 0.0, 0.0);
  desenhaCoisas(SELECT);
//   glPopMatrix();

  if (toggle_see_scale) {
//     desenha_escala();
    desenha_barra_de_controle();
  }
  

}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("Use: ./visual <visualization_data_file>\n");
    exit(1);
  }

  topo_file.open(argv[1]);
  if (topo_file.fail()) {
    printf("File %s does not exist\n", argv[1]);
    exit(1);
  }
  
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

  glutInit(&argc, argv); 

  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("Itsumo visualization tool");
  
  zprInit();
  // funcao que serah chamada quando acontecer um pick
  zprPickFunc(pick);

  // funcao que serah chamada pra fazer o "selection" draw, usado pra determinar
  // o que foi "picked"
  zprSelectionFunc(RenderSceneSelect);

//   glutIdleFunc(le_mais_dados);

  glutKeyboardFunc(keyboard_normal);
  glutSpecialFunc(keyboard_special);
  glutReshapeFunc(ChangeSize);

  // funcao que serah chamada sempre que precisar renderizar a cena
  glutDisplayFunc(RenderScene);

  // configura coisas em geral (cores, efeitos..)
  SetupRC();

  // carrega as fontes
  BuildFont();  

  carrega_texturas();
  
  instancia_topologia();

  // define a escala de cores a ser utilizada
  define_escala();

  glutMainLoop();

  return 0;
}


