#ifndef __HAS_LANESET_H
#define __HAS_LANESET_H


extern GLuint texture[NTEXTURAS];

class laneset {


public:

  poligono_simples poligono_da_laneset;
  poligono_simples poligono_da_seta_direcional;

  void desenha_com_textura(int modo);
  void desenha_sem_textura(int modo);

  void update_color(float density);

  void calcula_poligono_seta_direcional();
  void calcula_poligonos_lanes();
  void desenha_lanes(int modo);
  void desenha_semaforos(int modo);
  void desenha_seta_direcional(int modo);
  void desenha_divisorias_lanes(int modo);

  // divides the laneset polygon into a grid and returns the point in the ith
  // position in x axis and jth position in y axis
  ponto* point_in_grid(double i, double j, double xdivisions, double ydivisions);

//   poligono_simples polygon_nth_lane(int nth_lane, int total_lanes);

  laneset(int _id, int _position_in_section, int _total_number_lanesets, 
	  vetor_de_lanes _lanes, int _arrow_direction);

  ~laneset();

  int id;
  int position_in_section;
  int total_number_lanesets;

  int arrow_direction;

  cor_rgb cor;
  float _current_density;
 
  vetor_de_lanes lanes;

  list < poligono_simples * > poligonos_lanes;
  vector < poligono_simples * > poligonos_semaphores;
  vector < ponto * > lane_start_points;
  vector < ponto * > lane_end_points;

  ponto laneset_start_point;

  ponto esquina_esquerda_nodo_inicio;
  ponto esquina_direita_nodo_inicio;

  ponto esquina_esquerda_nodo_fim;
  ponto esquina_direita_nodo_fim;


};



#endif
