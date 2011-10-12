#ifndef __HAS_SECTION_H
#define __HAS_SECTION_H


extern GLuint texture[NTEXTURAS];

class section {

private:
  static double _xmin, _xmax, _ymin, _ymax;

public:


  double static xmin() { return _xmin; };
  double static xmax() { return _xmax; };
  double static ymin() { return _ymin; };
  double static ymax() { return _ymax; };

  void static set_xmin(double x) { _xmin = x; };
  void static set_xmax(double x) { _xmax = x; };
  void static set_ymin(double y) { _ymin = y; };
  void static set_ymax(double y) { _ymax = y; };

//   void desenha_com_textura(int modo);
  void desenha_sem_textura(int modo);

  void calcula_esquinas();
  void empurra_esquina(nodo *nodo_referencia, ponto a_partir_de, float quanto);
  void calcula_poligono_da_section();
  static double bounding_box_diagonal();
  float comprimento();

  string infoLaneSet( int laneset_id, laneset *laneset_ptr );
  string infoLane( int lane_id, laneset *laneset_ptr );

  void update_color(float density);

  poligono_simples polygon_nth_laneset(int nth_laneset, int total_lanesets);

  section(nodo *inicio, nodo *fim, int _nlanesets, int _id, int _length, char *_name, char *_street_name);

  ~section();

  vetor_de_lanesets lanesets;

  int id;
  string name;
  string street_name;

  cor_rgb cor;
  float _current_density;

  poligono_simples poligono_da_section;

  nodo *nodo_inicio;
  nodo *nodo_fim;
  
  segmento_ptr_pontos seg_esquerda;
  segmento_ptr_pontos seg_direita;
  
  int total_number_of_lanes;
  int nlanesets;
  int length;

  ponto esquina_esquerda_nodo_inicio;
  ponto esquina_direita_nodo_inicio;

  ponto esquina_esquerda_nodo_fim;
  ponto esquina_direita_nodo_fim;


};



#endif
