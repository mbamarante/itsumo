#ifndef __HAS_NODO_H
#define __HAS_NODO_H


#include "definitions.h"

class nodo {


public:
  
  static int _paint_traffic_lights;
  static int _paint_sources;

  static float diagonal_media;

  void desenha();
  void ordena_sections();

  void static toggle_paint_traffic_lights() { _paint_traffic_lights = !_paint_traffic_lights; };
  int static should_paint_traffic_lights() { return _paint_traffic_lights; };
  void static toggle_paint_sources() { _paint_sources = !_paint_sources; };
  int static should_paint_sources() { return _paint_sources; };

  float xmin, xmax, ymin, ymax;

  int id;
  int objects;
  int tlight_id;
  vector < int > sources;
  vector < int > sinks;

  string node_name;

  poligono_e_inclinacoes cruzamento;
  ponto localizacao;

  vector < section_e_inclinacao > sections_conectadas;
  
  void adiciona_section(section *r);

  float diagonal_length();
  ponto ponto_medio();
  void calcula_poligono_do_cruzamento();

  void desenha_cruzamento_apenas_tlights_com_textura(int modo);
  void desenha_cruzamento_sem_textura(int modo);
  void desenha_numero_plano_semaforico_atual(/*int tlight_id,*/ int cur_signalplan, int modo);
  void desenha_numero_fase_atual(/*int tlight_id,*/ int cur_phase, int modo);
//   void desenha_cruzamento_com_textura(int modo);

  string info( int cur_iter );

  nodo(GLfloat x, GLfloat y, int _objects, int _id, char *_node_name, int _tlight_id, 
       vector < int > _sources, vector < int > _sinks);
  ~nodo();
};


#endif
