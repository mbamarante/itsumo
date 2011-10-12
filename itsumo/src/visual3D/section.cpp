#include "definitions.h"
#include "section.h"
#include "laneset.h"
#include "nodo.h"
#include "aux.h"

extern double lane_width;
extern color  escala[11];

double section::_xmin=999999, section::_xmax=-999999, section::_ymin=999999, section::_ymax=-999999;

/*************************************************************************/
/*************************************************************************/

float section::comprimento()
{
  float tmp1, tmp2;
  
  tmp1 = (nodo_fim->localizacao).x - (nodo_inicio->localizacao).x;
  tmp1 = tmp1*tmp1;

  tmp2 = (nodo_fim->localizacao).y - (nodo_inicio->localizacao).y;
  tmp2 = tmp2*tmp2;

  return (sqrt(tmp1 + tmp2));

}


/*************************************************************************/
/*************************************************************************/


void section::empurra_esquina(nodo *nodo_referencia, ponto a_partir_de, float quanto)
{


  nodo *nodo_oposto;
  ponto *esquina_esquerda, *esquina_direita;
  float dx, dy;

 
  if (nodo_referencia == nodo_inicio) {
    nodo_oposto = nodo_fim;
    esquina_esquerda = &esquina_esquerda_nodo_inicio;
    esquina_direita = &esquina_direita_nodo_inicio;
  }
  else {
    nodo_oposto = nodo_inicio;
    esquina_esquerda = &esquina_esquerda_nodo_fim;
    esquina_direita = &esquina_direita_nodo_fim;
  }


  GLdouble a = (nodo_inicio->localizacao).y - (nodo_fim->localizacao).y;
  GLdouble b = (nodo_fim->localizacao).x - (nodo_inicio->localizacao).x;
  GLdouble m = -a/b;
  GLdouble cte_esquina_esquerda = esquina_esquerda->y - m * esquina_esquerda->x;

  // agora nos projetamos o ponto 'a_partir_de' no segmento da esquerda da section,
  // e empurramos a section a partir desse ponto

  // se a section eh horizontal
  if (isHorizontal((nodo_inicio->localizacao).y, (nodo_fim->localizacao).y)) {
    a_partir_de.y = esquina_esquerda->y;
  }
  else {
    // se a section eh vertical (perpendicular ao eixo x)
    if (isPerpendicular((nodo_inicio->localizacao).x, (nodo_fim->localizacao).x)) {
      a_partir_de.x = esquina_esquerda->x;
    }
    else {

      // acha uma reta perpendicular ('s') a section atual e que passe por 'a_partir_de'
      GLdouble m_perpendicular = -1/m;

      segmento_ptr_pontos s;
      ponto p1, p2, px;
      p1.x = a_partir_de.x;
      p1.y = a_partir_de.y;
      p2.x = a_partir_de.x+50;
      p2.y = p2.x * m_perpendicular + (p1.y - m_perpendicular * p1.x);
      s.ponto1 = &p1;
      s.ponto2 = &p2;

      // calcula-se a interseccao de 's' com o segmento da esquerda da section,
      // de modo a achar o ponto a partir do qual iremos empurrar a section
      interseccao(&px, seg_esquerda, s);

      a_partir_de.x = px.x;
      a_partir_de.y = px.y;

    }
  }


//   printf("       Empurrando esquinas a partir do ponto (%f, %f)\n", a_partir_de.x, a_partir_de.y);

   
  dx = esquina_esquerda->x;
  dy = esquina_esquerda->y;
  


  // section eh perpendicular
  if (isPerpendicular((nodo_inicio->localizacao).x, (nodo_fim->localizacao).x)) {
    esquina_esquerda->y = (1 - quanto)*a_partir_de.y + quanto*(nodo_oposto->localizacao).y;
  }
  // section nao-perpendicular
  else {
    esquina_esquerda->x = (1 - quanto)*a_partir_de.x + quanto*(nodo_oposto->localizacao).x;
    esquina_esquerda->y = m * esquina_esquerda->x + cte_esquina_esquerda;
  }
  
  dx -= esquina_esquerda->x;
  dy -= esquina_esquerda->y;
  
  // agora que jah empurramos a esquina da esquerda, movimentamos a da direita de acordo
  esquina_direita->x -= dx;
  esquina_direita->y -= dy;

}



/*************************************************************************/
/*************************************************************************/


void section::calcula_esquinas()
{


  GLdouble a = (nodo_inicio->localizacao).y - (nodo_fim->localizacao).y;
  GLdouble b = (nodo_fim->localizacao).x - (nodo_inicio->localizacao).x;
  GLdouble c = ((nodo_inicio->localizacao).x * (nodo_fim->localizacao).y) - 
    ((nodo_fim->localizacao).x * (nodo_inicio->localizacao).y);
  GLdouble m = -a/b;
  GLdouble d = lane_width * total_number_of_lanes;

  // calcula vertices de uma das esquinas da section
  GLdouble y1 = d * sqrt (a*a + b*b) - a * m * (nodo_fim->localizacao).y - a * (nodo_fim->localizacao).x - c;
  y1 = y1 / (b - a * m);
  GLdouble y2 = -d * sqrt (a*a + b*b) - a * m * (nodo_fim->localizacao).y - a * (nodo_fim->localizacao).x - c;
  y2 = y2 / (b - a * m);
  GLdouble x1 = (nodo_fim->localizacao).x + m * ((nodo_fim->localizacao).y - y1);
  GLdouble x2 = (nodo_fim->localizacao).x + m * ((nodo_fim->localizacao).y - y2);
  
  // caso especial: section perpendicular (90o)
  if (isPerpendicular((nodo_inicio->localizacao).x, (nodo_fim->localizacao).x)) {
    y1 = (nodo_fim->localizacao).y;
    y2 = (nodo_fim->localizacao).y;

    // dependendo do sentido da section, a esquina esquerda fica em x+d ou x-d
    if (greaterThan((nodo_inicio->localizacao).y, (nodo_fim->localizacao).y)) {
      x1 = (nodo_fim->localizacao).x + d;
      x2 = (nodo_fim->localizacao).x - d; 
    }
    else {
      x1 = (nodo_fim->localizacao).x - d;
      x2 = (nodo_fim->localizacao).x + d; 
    }
  }

  esquina_esquerda_nodo_fim.x = x1;
  esquina_esquerda_nodo_fim.y = y1;
  esquina_direita_nodo_fim.x = x2;
  esquina_direita_nodo_fim.y = y2;



  // calcula vertices da outra esquina da section
  y1 = d * sqrt (a*a + b*b) - a * m * (nodo_inicio->localizacao).y - a * (nodo_inicio->localizacao).x - c;
  y1 = y1 / (b - a * m);
  y2 = -d * sqrt (a*a + b*b) - a * m * (nodo_inicio->localizacao).y - a * (nodo_inicio->localizacao).x - c;
  y2 = y2 / (b - a * m);
  x1 = (nodo_inicio->localizacao).x + m * ((nodo_inicio->localizacao).y - y1);
  x2 = (nodo_inicio->localizacao).x + m * ((nodo_inicio->localizacao).y - y2);

  // caso especial: section perpendicular (90o)
  if (isPerpendicular((nodo_inicio->localizacao).x, (nodo_fim->localizacao).x)) {
    y1 = (nodo_inicio->localizacao).y;
    y2 = (nodo_inicio->localizacao).y;

    // dependendo do sentido da section, a esquina esquerda fica em x+d ou x-d
    if (greaterThan((nodo_inicio->localizacao).y, (nodo_fim->localizacao).y)) {
      x1 = (nodo_inicio->localizacao).x + d;
      x2 = (nodo_inicio->localizacao).x - d; 
    }
    else {
      x1 = (nodo_inicio->localizacao).x - d;
      x2 = (nodo_inicio->localizacao).x + d; 
    }
  }

  esquina_esquerda_nodo_inicio.x = x1;
  esquina_esquerda_nodo_inicio.y = y1;
  esquina_direita_nodo_inicio.x = x2;
  esquina_direita_nodo_inicio.y = y2;

  // cria os meio-fios de acordo com os pontos das esquinas que acabou de calcular
  seg_esquerda.ponto1 = &esquina_esquerda_nodo_inicio;
  seg_esquerda.ponto2 = &esquina_esquerda_nodo_fim;
  seg_direita.ponto1 = &esquina_direita_nodo_inicio;
  seg_direita.ponto2 = &esquina_direita_nodo_fim;

 
}


/*************************************************************************/
/*************************************************************************/



section::section(nodo *inicio, nodo *fim, int _nlanesets, int _id, int _length, char *_name, char *_street_name)
{

  GLdouble x1, x2, y1, y2;

  id = _id;
  name = _name;
  street_name = _street_name;

  nodo_inicio = inicio;
  nodo_fim = fim;

  nodo_inicio->adiciona_section(this);
  nodo_fim->adiciona_section(this);

  x1 = (nodo_inicio->localizacao).x;
  y1 = (nodo_inicio->localizacao).y;
  x2 = (nodo_fim->localizacao).x;
  y2 = (nodo_fim->localizacao).y;

  // atualiza bounding box
  if (x1<xmin())
    set_xmin(x1);
  if (x2<xmin())
    set_xmin(x2);
  if (y1<ymin())
    set_ymin(y1);
  if (y2<ymin())
    set_ymin(y2);

  if (x1>xmax())
    set_xmax(x1);
  if (x2>xmax())
    set_xmax(x2);
  if (y1>ymax())
    set_ymax(y1);
  if (y2>ymax())
    set_ymax(y2);
 
  total_number_of_lanes = 0;
  nlanesets = _nlanesets;
  length = _length;
  calcula_esquinas();
  
  // cor inicial da rua eh verde bem claro
  cor[RED]   = INICIAL_R;
  cor[GREEN] = INICIAL_G;
  cor[BLUE]  = INICIAL_B;


}


/*************************************************************************/
/*************************************************************************/


double section::bounding_box_diagonal()
{
  return sqrt((xmax()-xmin())*(xmax()-xmin())+(ymax()-ymin())*(ymax()-ymin()));
}


/*************************************************************************/
/*************************************************************************/


// void section::desenha_com_textura(int modo)
// {

//   printf("tah aqui porra\n");
//   glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
//   glBindTexture(GL_TEXTURE_2D, texture[ROAD]);

//   glColor4f(cor[RED], cor[GREEN], cor[BLUE], ALPHA);

//   if (modo==SELECT)
//     glPushName(id);

//   glBegin(GL_POLYGON);
//       glTexCoord2f(0.0f, 0.0f); glVertex3f(poligono_da_section[0]->x, poligono_da_section[0]->y, 0);
//       glTexCoord2f(0.0f, comprimento()/15); glVertex3f(poligono_da_section[1]->x, poligono_da_section[1]->y, 0);
//       glTexCoord2f(1.0f, comprimento()/15); glVertex3f(poligono_da_section[2]->x, poligono_da_section[2]->y, 0);
//       glTexCoord2f(1.0f, 0.0f); glVertex3f(poligono_da_section[3]->x, poligono_da_section[3]->y, 0);
//   glEnd();

//   if (modo==SELECT)
//     glPopName();


// }


/*************************************************************************/
/*************************************************************************/

void section::desenha_sem_textura(int modo)
{
  poligono_simples::iterator p;


  glDisable(GL_TEXTURE_2D);

  glColor3f(cor[RED], cor[GREEN], cor[BLUE]);

  if (modo==SELECT)
    glPushName(id);
 
  glBegin(GL_POLYGON);
     for (p = poligono_da_section.begin(); p != poligono_da_section.end(); p++)
       glVertex3f((*p)->x, (*p)->y, 0);
  glEnd();

  if (modo==SELECT)
    glPopName();


}


/*************************************************************************/
/*************************************************************************/

poligono_simples section::polygon_nth_laneset(int nth_laneset, int total_lanesets)
{
  
  ponto *esquerda_inicio = new ponto();
  ponto *esquerda_fim = new ponto();
  ponto *direita_inicio = new ponto();
  ponto *direita_fim = new ponto();

  double dx = esquina_direita_nodo_inicio.x - esquina_esquerda_nodo_inicio.x;
  double dy = esquina_direita_nodo_inicio.y - esquina_esquerda_nodo_inicio.y;

  poligono_simples laneset_polygon;

  esquerda_inicio->x = esquina_esquerda_nodo_inicio.x + nth_laneset * (dx / total_lanesets);
  esquerda_inicio->y = esquina_esquerda_nodo_inicio.y + nth_laneset * (dy / total_lanesets);
//   printf("#1 x=%f y=%f\n", esquerda_inicio->x, esquerda_inicio->y);
  
  esquerda_fim->x = esquina_esquerda_nodo_fim.x + nth_laneset * (dx / total_lanesets);
  esquerda_fim->y = esquina_esquerda_nodo_fim.y + nth_laneset * (dy / total_lanesets);
//   printf("#2 x=%f y=%f\n", esquerda_fim->x, esquerda_fim->y);

  direita_inicio->x = esquina_esquerda_nodo_inicio.x + (nth_laneset+1) * (dx / total_lanesets);
  direita_inicio->y = esquina_esquerda_nodo_inicio.y + (nth_laneset+1) * (dy / total_lanesets);
//   printf("#3 x=%f y=%f\n", direita_inicio->x, direita_inicio->y);

  direita_fim->x = esquina_esquerda_nodo_fim.x + (nth_laneset+1) * (dx / total_lanesets);
  direita_fim->y = esquina_esquerda_nodo_fim.y + (nth_laneset+1) * (dy / total_lanesets);
//   printf("#4 x=%f y=%f\n", direita_fim->x, direita_fim->y);

  laneset_polygon.push_back(esquerda_inicio);
  laneset_polygon.push_back(esquerda_fim);
  laneset_polygon.push_back(direita_fim);
  laneset_polygon.push_back(direita_inicio);

  return laneset_polygon;
 
}


/*************************************************************************/
/*************************************************************************/

void section::calcula_poligono_da_section()
{

  poligono_da_section.push_back(&esquina_esquerda_nodo_inicio);
  poligono_da_section.push_back(&esquina_esquerda_nodo_fim);
  poligono_da_section.push_back(&esquina_direita_nodo_fim);
  poligono_da_section.push_back(&esquina_direita_nodo_inicio);

  
}


/*************************************************************************/
/*************************************************************************/

void section::update_color(float density)
{

  _current_density = density;

  //  Esse trecho faz a escala de cores ser Amarelo -> Verde -> Azul -> Vermelho
  //    RGB comeca em amarelo (1 1 0)
  //   if (density < 0.33) {
  //     //       isso faz tender pra verde (0 1 0)
  //     cor[RED] = 1.0 - (density/0.33);
  //   }
  //   else {
  //     if (density < 0.66) {
  //       //         isso faz tender pro azul (0 0 1)
  //       cor[GREEN] = 1.0 - ((density-0.33)/(0.66-0.33));
  //       cor[BLUE] = 1.0 - cor[GREEN];
  //     }
  //     else {
  //       //         e isso faz tender pro vermelho (1 0 0)
  //       cor[RED] = (density-0.66)/(1.0-0.66);
  //       cor[BLUE] = 1.0 - cor[RED];
  //     }
  //   }


  // Esse trecho faz a escala de cores ser Amarelo -> Verde -> Vermelho
  //   // RGB comeca em amarelo (1 1 0)
  //   if (density < 0.5) {
  //     // isso faz tender pra verde (0 1 0)
  //     cor[RED] = 1.0 - (density/0.5);
  //   }
  //   else {
  //     // e isso faz tender pro vermelho (1 0 0)
  //     cor[GREEN] = 1.0 - ((density-0.5)/(1.0-0.5));
  //     cor[RED] = 1.0 - cor[GREEN];
  //   }


  // Esse trecho faz a escala de cores ser Verde -> Amarelo -> Vermelho
  //     if (density < 0.5) {
  //       // isso faz tender pra amarelo (1 1 0)
  //       cor[RED] = density/0.5;
  //     }
  //     else {
  //       // e isso faz tender pro vermelho (1 0 0)
  //       cor[GREEN] = 1.0 - ((density-0.5)/(1.0-0.5));
  //     }
  
  //  Esse trecho faz a escala de cores ser Branco-esverdeado -> Verde -> Amarelo -> Vermelho
  //    RGB comeca em branco (1 1 1)
  //   if (density < 0.33) {
  //     // isso faz tender pra verde (0 1 0)
  //     cor[RED] = 0.8 - (density/0.33);
  //     cor[BLUE] = 0.8 - (density/0.33);
  //   }
  //   else {
  //     if (density < 0.66) {
  //       // isso faz tender pro amarelo (1 1 0)
  //       cor[RED] = (density-0.33)/(1.0-0.66);
  //     }
  //     else {
  //       // e isso faz tender pro vermelho (1 0 0)
  //       cor[GREEN] = 1.0 - ((density-0.66)/(1.0-0.66));
  //     }
  //   }

  int valor_escala = int(density*100) / 10;
  cor[RED]   = escala[valor_escala].R;
  cor[GREEN] = escala[valor_escala].G;
  cor[BLUE]  = escala[valor_escala].B;



}
/*************************************************************************/
/*************************************************************************/

string section::infoLane( int lane_id, laneset *laneset_ptr )
{

  ostringstream cout; 
  nodo *laneset_start_node, *laneset_end_node;

  if (laneset_ptr -> arrow_direction == FORWARD_ARROW) {
    laneset_start_node = nodo_inicio;
    laneset_end_node = nodo_fim;
  }
  else {
    laneset_start_node = nodo_fim;
    laneset_end_node = nodo_inicio;
  }

  cout << "Street: '" << street_name << "'" << endl;
  cout << "Section: (" << laneset_start_node->node_name << ") <-> (" << laneset_end_node->node_name << ")" << endl;
  cout << "Laneset: (" << laneset_start_node->node_name << ") -> (" << laneset_end_node->node_name << ")" << endl;
  cout << "Lane: " << lane_id << endl;

  return cout.str();


}


/*************************************************************************/
/*************************************************************************/

string section::infoLaneSet( int laneset_id, laneset *laneset_ptr )
{

  ostringstream cout; 
  nodo *laneset_start_node, *laneset_end_node;

  if (laneset_ptr -> arrow_direction == FORWARD_ARROW) {
    laneset_start_node = nodo_inicio;
    laneset_end_node = nodo_fim;
  }
  else {
    laneset_start_node = nodo_fim;
    laneset_end_node = nodo_inicio;
  }

  cout << "Street: '" << street_name << "'" << endl;
  cout << "Section: (" << laneset_start_node->node_name << ") <-> (" << laneset_end_node->node_name << ")" << endl;
  cout << "Laneset: (" << laneset_start_node->node_name << ") -> (" << laneset_end_node->node_name << ")" << endl;

  return cout.str();


}


/*************************************************************************/
/*************************************************************************/



section::~section() 
{
}
