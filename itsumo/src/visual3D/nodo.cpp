#include "nodo.h"
#include "section.h"

int nodo::_paint_traffic_lights = 0;
int nodo::_paint_sources = 0;
float nodo::diagonal_media = 0;


extern GLuint number_texture[MAX_NUMBER_TEXTURE];
extern current_signalplan mapping_trafficlight_to_current_signalplan;
extern current_phase mapping_signalplan_to_current_phase;

/*************************************************************************/
/*************************************************************************/


void nodo::desenha()
{
  glColor3f(0.66, 0.078, 0.11);  // vermelho
  glPointSize(8.0);
  glBegin(GL_POINTS);
  glVertex3f(localizacao.x, localizacao.y, 0);
  glEnd();
}


/*************************************************************************/
/*************************************************************************/

void nodo::adiciona_section(section *r)
{
  nodo *nodo_oposto;

  if (r->nodo_fim == this)
    nodo_oposto = r->nodo_inicio;
  else
    nodo_oposto = r->nodo_fim;

  float a = (nodo_oposto->localizacao).y - (localizacao).y;
  float b = (nodo_oposto->localizacao).x - (localizacao).x;
  float inclinacao = atan2(a, b)*180/PI;
  

  sections_conectadas.push_back(make_pair(inclinacao, r));
}


/*************************************************************************/
/*************************************************************************/

nodo::nodo(GLfloat x, GLfloat y, int _objects, int _id, char *_node_name, int _tlight_id, 
	   vector < int > _sources, vector < int > _sinks)
{
  id = _id;
  objects = _objects;
  localizacao.x = x;
  localizacao.y = y;

  tlight_id = _tlight_id;
  sources = _sources;
  sinks = _sinks;

  node_name = _node_name;

//   cout << "Criando nodo" << id << " (" << node_name << ")" << endl;
//   cout << "\tTlight " << tlight_id << endl;
//   for (unsigned int i=0; i<sources.size(); i++)
//     cout << "\tSource " << sources[i] << endl;
//   for (unsigned int i=0; i<sinks.size(); i++)
//     cout << "\tSink " << sinks[i] << endl;

  xmin=999999; xmax=-999999; ymin=999999; ymax=-999999;
}

/*************************************************************************/
/*************************************************************************/

nodo::~nodo()
{
}

/*************************************************************************/
/*************************************************************************/


inline bool compareStreetsByInclination(const section_e_inclinacao& r1, const section_e_inclinacao& r2)
{

  if (r1.first <= r2.first) {
    return false;
  }
  else {
    return true;
  }
}

/*************************************************************************/
/*************************************************************************/


inline bool comparePointsByInclination(const ponto_do_poligono_e_inclinacao &p1, 
				       const ponto_do_poligono_e_inclinacao &p2)
{

  if (p1.first <= p2.first) {
    return false;
  }
  else {
    return true;
  }
}


/*************************************************************************/
/*************************************************************************/


void nodo::ordena_sections()
{
  //  printf("ordenando nodo %d\n", id);
  sort(sections_conectadas.begin(), sections_conectadas.end(), compareStreetsByInclination);
}


/*************************************************************************/
/*************************************************************************/


void nodo::calcula_poligono_do_cruzamento()
{

  float a, b, inclinacao;

  vector < section_e_inclinacao >::iterator r;

  // percorre todas as sections conectadas ao nodo, adicionando os pontos que formam as esquinas
  // dessas sections no poligono que irah formar o cruzamento (nodo)
  for (r = sections_conectadas.begin(); r != sections_conectadas.end(); r++) {

    // verifica se o nodo atual eh o inicial ou o final da section. Isso eh importante
    // para saber se devemos usar o campo esquina_.._nodo_fim ou esquina_.._nodo_inicio
    // e tambem para sabermos a ordem que se deve inserir os pontos no poligono (se primeiro
    // a esquina da esquerda ou a esquina da direita)
    if (((*r).second)->nodo_fim == this) {

      // adiciona esquina da direita da section, juntamente com sua inclinacao em relacao
      // a posicao do nodo
      a = ((*r).second)->esquina_direita_nodo_fim.y - (localizacao).y;
      b = ((*r).second)->esquina_direita_nodo_fim.x - (localizacao).x;
      inclinacao = atan2(a, b)*180/PI;
//       cruzamento.push_back(make_pair(inclinacao,
// 				     &(((*r).second)->esquina_direita_nodo_fim)));
      cruzamento.push_back(make_pair((*r).second,
				     &(((*r).second)->esquina_direita_nodo_fim)));


      // adiciona esquina da esquerda da section, juntamente com sua inclinacao em relacao
      // a posicao do nodo
      a = ((*r).second)->esquina_esquerda_nodo_fim.y - (localizacao).y;
      b = ((*r).second)->esquina_esquerda_nodo_fim.x - (localizacao).x;
      inclinacao = atan2(a, b)*180/PI;
      cruzamento.push_back(make_pair((*r).second, 
				     &(((*r).second)->esquina_esquerda_nodo_fim)));
//       cruzamento.push_back(make_pair(inclinacao, 
// 				     &(((*r).second)->esquina_esquerda_nodo_fim)));


    }
    else {

      // codigo idem ao acima, exceto por tratar do caso onde o nodo atual
      // eh o inicio da section, e nao o fim (this==nodo_inicio)
      a = ((*r).second)->esquina_esquerda_nodo_inicio.y - (localizacao).y;
      b = ((*r).second)->esquina_esquerda_nodo_inicio.x - (localizacao).x;
      inclinacao = atan2(a, b)*180/PI;
//       cruzamento.push_back(make_pair(inclinacao, 
// 				     &(((*r).second)->esquina_esquerda_nodo_inicio)));
      cruzamento.push_back(make_pair((*r).second, 
				     &(((*r).second)->esquina_esquerda_nodo_inicio)));

      a = ((*r).second)->esquina_direita_nodo_inicio.y - (localizacao).y;
      b = ((*r).second)->esquina_direita_nodo_inicio.x - (localizacao).x;
      inclinacao = atan2(a, b)*180/PI;
//       cruzamento.push_back(make_pair(inclinacao, 
// 				     &(((*r).second)->esquina_direita_nodo_inicio)));
      cruzamento.push_back(make_pair((*r).second, 
				     &(((*r).second)->esquina_direita_nodo_inicio)));


    }
  }
 
  // depois de adicionar todos os pontos no poligono, vamos ordena-los em sentido
  // anti-horario para que possamos desenhar um poligono corretamente, passando os
  // pontos em ordem para o chamada GL_POLYGON
  //   sort(cruzamento.begin(), cruzamento.end(), comparePointsByInclination);

  // atualiza a bounding box


  xmin=999999; xmax=-999999; ymin=999999; ymax=-999999;
  for (poligono_e_inclinacoes::iterator p = cruzamento.begin(); p != cruzamento.end(); p++) {
    if (((*p).second)->x > xmax)
      xmax = ((*p).second)->x;
    if (((*p).second)->x < xmin)
      xmin = ((*p).second)->x;
    if (((*p).second)->y > ymax)
      ymax = ((*p).second)->y;
    if (((*p).second)->y < ymin)
      ymin = ((*p).second)->y;
  }



}

/*************************************************************************/
/*************************************************************************/



// returns the length of the diagonal of the node's bounding box
float nodo::diagonal_length()
{
  
  return sqrt(pow((xmax-xmin), 2) + pow((ymax-ymin), 2));
}


/*************************************************************************/
/*************************************************************************/


ponto nodo::ponto_medio()
{

  ponto ponto_medio;

  ponto_medio.x = (xmax+xmin)/2;
  ponto_medio.y = (ymax+ymin)/2;

  return ponto_medio;
  
  
}




/*************************************************************************/
/*************************************************************************/


void nodo::desenha_cruzamento_apenas_tlights_com_textura(int modo)
{

  poligono_e_inclinacoes::iterator p;

  glDisable(GL_TEXTURE_2D);			// Enable Texture Mapping
  glLineWidth(2.0);

  if (modo==SELECT)
    glPushName(id);

  glBegin(GL_POLYGON);
  p = cruzamento.begin();
     for (p++; p != cruzamento.end(); p++) {
       glColor4f(((*p).first)->cor[RED], ((*p).first)->cor[GREEN], ((*p).first)->cor[BLUE], ALPHA);
       glVertex3f(((*p).second)->x, ((*p).second)->y, 0.0);
     } 
     p = cruzamento.begin();
     glColor4f(((*p).first)->cor[RED], ((*p).first)->cor[GREEN], ((*p).first)->cor[BLUE], ALPHA);
     glVertex3f(((*p).second)->x, ((*p).second)->y, 0.0);
  glEnd();

  if (modo==SELECT)
    glPopName();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[objects]);
  
  if (objects!=0) {
    float delta = (0.7071*nodo::diagonal_media)/2;   // 0.7071 = sin(45).. isso vem da lei dos senos
    ponto ponto_med = ponto_medio();
    glColor4f(1.0, 1.0, 1.0, 1.0);

    if (modo==SELECT)
      glPushName(id);

    glBegin(GL_POLYGON);
       glTexCoord2f(1.0f, 0.0f); glVertex3f(ponto_med.x - delta, ponto_med.y - delta, -1.0);
       glTexCoord2f(0.0f, 0.0f); glVertex3f(ponto_med.x + delta, ponto_med.y - delta, -1.0);
       glTexCoord2f(0.0f, 1.0f); glVertex3f(ponto_med.x + delta, ponto_med.y + delta, -1.0);
       glTexCoord2f(1.0f, 1.0f); glVertex3f(ponto_med.x - delta, ponto_med.y + delta, -1.0);     
    glEnd();

    if (modo==SELECT)
      glPopName();
  }


}


/*************************************************************************/
/*************************************************************************/


void nodo::desenha_numero_plano_semaforico_atual(/*int tlight_id, */int cur_signalplan, int modo)
{

  float delta = (0.7071*nodo::diagonal_media)/2;   // 0.7071 = sin(45).. isso vem da lei dos senos
  ponto ponto_med = ponto_medio();
  
  if (modo==SELECT)
    glPushName(id);
 
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, number_texture[cur_signalplan]);

  glColor4f(1.0, 1.0, 1.0, 1.0);
 
  glBegin(GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f(ponto_med.x - delta, ponto_med.y - delta, -0.3);     
       glTexCoord2f(0.0f, 0.0f); glVertex3f(ponto_med.x - delta, ponto_med.y + delta, -0.3);
       glTexCoord2f(1.0f, 0.0f); glVertex3f(ponto_med.x + delta, ponto_med.y + delta, -0.3);
       glTexCoord2f(1.0f, 1.0f); glVertex3f(ponto_med.x + delta, ponto_med.y - delta, -0.3);
  glEnd();

  glDisable(GL_TEXTURE_2D);			// Disable Texture Mapping

  if (modo==SELECT)
    glPopName();

}

/*************************************************************************/
/*************************************************************************/


void nodo::desenha_numero_fase_atual(/*int tlight_id, */int cur_phase, int modo)
{

  float pos = (0.7071*nodo::diagonal_media)/2;   // 0.7071 = sin(45).. isso vem da lei dos senos
  float delta = pos*0.4;
  pos = pos*0.7;
  ponto ponto_med = ponto_medio();
  
  
  if (modo==SELECT)
    glPushName(id);
 
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, number_texture[cur_phase]);

  glColor4f(1.0, 1.0, 1.0, 1.0);
 
  glBegin(GL_POLYGON);
       glTexCoord2f(0.0f, 1.0f); glVertex3f( ponto_med.x - delta + pos, ponto_med.y - delta - pos, -0.2);
       glTexCoord2f(0.0f, 0.0f); glVertex3f( ponto_med.x - delta + pos, ponto_med.y + delta - pos, -0.2);
       glTexCoord2f(1.0f, 0.0f); glVertex3f( ponto_med.x + delta + pos, ponto_med.y + delta - pos, -0.2);
       glTexCoord2f(1.0f, 1.0f); glVertex3f( ponto_med.x + delta + pos, ponto_med.y - delta - pos, -0.2);
  glEnd();
  
  glDisable(GL_TEXTURE_2D);			// Disable Texture Mapping

  if (modo==SELECT)
    glPopName();

}

/*************************************************************************/
/*************************************************************************/


// void nodo::desenha_cruzamento_com_textura(int modo)
// {

//   poligono_e_inclinacoes::iterator p;

//   glEnable(GL_TEXTURE_2D);			// Enable Texture Mapping
//   glBindTexture(GL_TEXTURE_2D, texture[CROSSING]);

//   if (modo==SELECT)
//     glPushName(id);

//   glBegin(GL_POLYGON);
//   p = cruzamento.begin();
//      for (p++; p != cruzamento.end(); p++) {
//        glColor4f(((*p).first)->cor[RED], ((*p).first)->cor[GREEN], ((*p).first)->cor[BLUE], ALPHA);
//        glTexCoord2f( (((*p).second)->x - xmin)/(xmax-xmin), (((*p).second)->y - ymin)/(ymax-ymin));
//        glVertex3f(((*p).second)->x, ((*p).second)->y, 0.0);
//      } 
//      p = cruzamento.begin();
//      glColor4f(((*p).first)->cor[RED], ((*p).first)->cor[GREEN], ((*p).first)->cor[BLUE], ALPHA);
//      glTexCoord2f( (((*p).second)->x - xmin)/(xmax-xmin), (((*p).second)->y - ymin)/(ymax-ymin));
//      glVertex3f(((*p).second)->x, ((*p).second)->y, 0.0);
//   glEnd();

//   if (modo==SELECT)
//     glPopName();


//   glBindTexture(GL_TEXTURE_2D, texture[objects]);
  
//   if (objects!=0) {
//     float delta = (0.7071*nodo::diagonal_media)/2;   // 0.7071 = sin(45).. isso vem da lei dos senos
//     ponto ponto_med = ponto_medio();
//     glColor4f(1.0, 1.0, 1.0, 1.0);

//     if (modo==SELECT)
//       glPushName(id);

//     glBegin(GL_POLYGON);
//        glTexCoord2f(1.0f, 0.0f); glVertex3f(ponto_med.x - delta, ponto_med.y - delta, -1.0);
//        glTexCoord2f(0.0f, 0.0f); glVertex3f(ponto_med.x + delta, ponto_med.y - delta, -1.0);
//        glTexCoord2f(0.0f, 1.0f); glVertex3f(ponto_med.x + delta, ponto_med.y + delta, -1.0);
//        glTexCoord2f(1.0f, 1.0f); glVertex3f(ponto_med.x - delta, ponto_med.y + delta, -1.0);     
//     glEnd();

//     if (modo==SELECT)
//       glPopName();
//   }


// }


/*************************************************************************/
/*************************************************************************/



void nodo::desenha_cruzamento_sem_textura(int modo)
{

  poligono_e_inclinacoes::iterator p;


  glDisable(GL_TEXTURE_2D);			// Enable Texture Mapping
  glColor4f(0.8, 1.0, 0.8, ALPHA);
  glLineWidth(2.0);


  if (modo==SELECT) {
    // desenha todo o contorno
    glPushName(id);

    glBegin(GL_POLYGON);
       for (p = cruzamento.begin(); p != cruzamento.end(); p++) {
         glColor4f(((*p).first)->cor[RED], ((*p).first)->cor[GREEN], ((*p).first)->cor[BLUE], ALPHA);
         glVertex3f(((*p).second)->x, ((*p).second)->y, 0.0);
       }
       p = cruzamento.begin();
       glColor4f(((*p).first)->cor[RED], ((*p).first)->cor[GREEN], ((*p).first)->cor[BLUE], ALPHA);
       glVertex3f(((*p).second)->x, ((*p).second)->y, 0.0);
    glEnd();
    glPopName();

  }

  else {

    // desenha somente ligacao entre-ruas
    glBegin(GL_LINES);
       p = cruzamento.begin();
       for (p++; p != cruzamento.end(); p++) {
         glColor4f(((*p).first)->cor[RED], ((*p).first)->cor[GREEN], ((*p).first)->cor[BLUE], ALPHA);
         glVertex3f(((*p).second)->x, ((*p).second)->y, 0.0);
       }
       p = cruzamento.begin();
       glColor4f(((*p).first)->cor[RED], ((*p).first)->cor[GREEN], ((*p).first)->cor[BLUE], ALPHA);
       glVertex3f(((*p).second)->x, ((*p).second)->y, 0.0);
    glEnd();
  }
  
  

  // 0 -> no traffic light, no source
  // 1 -> traffic light only
  // 2 -> source only
  // 3 -> source + traffic light

  //   cor_rgb cor_preenchimento;
  //   switch (objects) {
  //   case 0:
  //     cor_preenchimento[RED] = PRETO_R;
  //     cor_preenchimento[GREEN] = PRETO_G;
  //     cor_preenchimento[BLUE] = PRETO_B;
  //     break;

  //   case 1:
  //     glBindTexture(GL_TEXTURE_2D, texture[TRAFFIC_LIGHT]);
  
  //     if (should_paint_traffic_lights()) {
  //       cor_preenchimento[RED] = VERDE_LIMAO_R;
  //       cor_preenchimento[GREEN] = VERDE_LIMAO_G;
  //       cor_preenchimento[BLUE] = VERDE_LIMAO_B;
  //     } else {
  //       cor_preenchimento[RED] = AZUL_ESCURO_R;
  //       cor_preenchimento[GREEN] = AZUL_ESCURO_G;
  //       cor_preenchimento[BLUE] = AZUL_ESCURO_B;
  //     }
  //     break;

  //   case 2:
  //     glBindTexture(GL_TEXTURE_2D, texture[SOURCE]);
  //     if (should_paint_sources()) {
  //       cor_preenchimento[RED] = VERDE_CLARO_R;
  //       cor_preenchimento[GREEN] = VERDE_CLARO_G;
  //       cor_preenchimento[BLUE] = VERDE_CLARO_B;
  //     } else {
  //       cor_preenchimento[RED] = AZUL_ESCURO_R;
  //       cor_preenchimento[GREEN] = AZUL_ESCURO_G;
  //       cor_preenchimento[BLUE] = AZUL_ESCURO_B;
  //     }
  //     break;

  //   case 3:
  //     glBindTexture(GL_TEXTURE_2D, texture[SOURCE_TRAFFICLIGHT]);
  //     if (should_paint_traffic_lights() || should_paint_sources()) {
  //       cor_preenchimento[RED] = VERMELHO_R;
  //       cor_preenchimento[GREEN] = VERMELHO_G;
  //       cor_preenchimento[BLUE] = VERMELHO_B;
  //     } else {
  //       cor_preenchimento[RED] = AZUL_ESCURO_R;
  //       cor_preenchimento[GREEN] = AZUL_ESCURO_G;
  //       cor_preenchimento[BLUE] = AZUL_ESCURO_B;
  //     }
  //     break;

  //   default:
  //     cout << "Erro! Nodo contem objetos desconhecidos (" << objects << ")" << endl;
  //     exit(1);
  //   }


}




/*************************************************************************/
/*************************************************************************/


string nodo::info( int cur_iter )
{

  ostringstream cout; 
  
  pair <int, int> current_phase;
  pair < int, nodo * > current_signalplan;
  current_signalplan.first = -1;
  current_signalplan.second = NULL;

  cout << "Node: " << node_name << endl;

  if (tlight_id != -1) {
    cout << "Traffic light: " << tlight_id << endl;
    if (cur_iter != SIMULATION_NOT_STARTED) {
      current_signalplan = mapping_trafficlight_to_current_signalplan[tlight_id];
      current_phase = mapping_signalplan_to_current_phase[tlight_id];
      //add 1 to start from 1 not 0
      cout << "Current signalplan: " << current_signalplan.first + 1 << endl;
      cout << "Current phase (duration): " << current_phase.first << " (" <<current_phase.second << ")" << endl;
    } else
      cout << "Current signalplan: not running" << endl;
  }
 
  if (sources.size() > 0) {
    cout << "Sources: ";
    for (unsigned int i=0; i < sources.size(); i++) {
      if (i == sources.size()-1)
	cout << sources[i];
      else
	cout << sources[i] << ", ";
    }
    cout << endl;
  }
  

  if (sinks.size() > 0) {
    cout << "Sinks: ";
    for (unsigned int i=0; i < sinks.size(); i++) {
      if (i == sinks.size()-1)
	cout << sinks[i];
      else
	cout << sinks[i] << ", ";
    }
    cout << endl;
  }

  return cout.str();


}
