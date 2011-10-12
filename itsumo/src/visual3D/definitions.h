#ifndef __HAS_DEFINITIONS_H
#define __HAS_DEFINITIONS_H

// A note on using textures:
//    Specify mapping of texture coordinates to object coordinates in
//    the following order:
//            texture(left_up)      object(left_bottom)
//            texture(left_bottom)  object(left_up)
//            texture(right_bottom) object(right_up)
//            texture(right_up)     object(right_bottom)


#include <GL/glut.h>
#include <GL/glx.h>     // Header file fot the glx libraries.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "zpr.h"

#include <unistd.h>

#include <cstring>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <iostream>

#include <sstream>

#include <ext/hash_map>   // hash table!
using namespace __gnu_cxx;

using namespace std;
using std::list;
using std::vector;
using std::cout;

typedef struct color {
  double R;
  double G;
  double B;
};


#define FRAME_TIME 100
#define INFO_BOX_TIME 4000

#define PI 3.14159265

#define ALPHA 1.0

#define NTEXTURAS 16
#define MAX_NUMBER_TEXTURE 12

#define SIMULATION_NOT_STARTED (-2)
#define SIMULATION_OVER (-1)

#define CAR 1
#define CTRL_BAR_BACKG 2
#define FROM_START 3
#define PLAY 4
#define STOP 5
#define STEP 6
#define INCREASE_SCALE 7
#define DECREASE_SCALE 8
#define NAVIGATION_BAR 9
#define ZOOM_IN 10
#define ZOOM_OUT 11
#define ZOOM_ALL 12
#define CAR_SPECIAL 13
#define INFO_BOX_BACKG 14

#define CLICKED_FROM_START -10
#define CLICKED_PLAY -11
#define CLICKED_STOP -12
#define CLICKED_STEP -13
#define CLICKED_INCREASE_SCALE -14
#define CLICKED_DECREASE_SCALE -15
#define CLICKED_PAN_UP -16
#define CLICKED_PAN_DOWN -17
#define CLICKED_PAN_LEFT -18
#define CLICKED_PAN_RIGHT -19
#define CLICKED_ZOOM_ALL -20
#define CLICKED_ZOOM_IN -21
#define CLICKED_ZOOM_OUT -22

#define PLAYING 0
#define STOPPED 1

#define SOURCE 3
#define SOURCE_TRAFFICLIGHT 4
#define SINK 5
#define SINK_TRAFFICLIGHT 6
#define SOURCE_SINK 7
#define TRAFFIC_LIGHT 8
#define SOURCE_SINK_TRAFFICLIGHT 9
#define NO_OBJECTS 0

#define RENDER 0
#define SELECT 1

#define SHOW_SECTIONS_INFO 0
#define SHOW_LANESETS_INFO 1
#define SHOW_LANES_INFO 2

#define FORWARD_ARROW 0
#define BACKWARDS_ARROW 1

#define ESQUERDA 0
#define DIREITA 1
#define INSIDE_BOTH 0
#define INSIDE_FIRST 1
#define INSIDE_SECOND 2
#define INSIDE_NONE 3
#define PARALLEL 5
#define DESLOCAMENTO_DEFAULT 0.005
#define LARGURA_DEFAULT_DE_CRUZAMENTO 0.05
#define FP_ERROR 0.0001

#define RED 0
#define GREEN 1
#define BLUE 2

#define CHAO_R 0.15
#define CHAO_G 0.15
#define CHAO_B 0.15

#define INICIAL_R 0.35
#define INICIAL_G 0.35
#define INICIAL_B 0.35

#define PRETO_R 0.0
#define PRETO_G 0.0
#define PRETO_B 0.0

#define VERDE_CLARO_R 0.0
#define VERDE_CLARO_G 0.74
#define VERDE_CLARO_B 0.74

#define BRANCO_R 1.0
#define BRANCO_G 1.0
#define BRANCO_B 1.0

#define VERDE_LIMAO_R 0.0
#define VERDE_LIMAO_G 0.74
#define VERDE_LIMAO_B 0.0

#define VERMELHO_R 0.74
#define VERMELHO_G 0.0
#define VERMELHO_B 0.0

#define AZUL_ESCURO_R 0.16
#define AZUL_ESCURO_G 0.16
#define AZUL_ESCURO_B 0.32


class nodo;
class section;
class laneset;


typedef struct PONTOXY {
  GLfloat x;
  GLfloat y;
} ponto;

typedef struct SEGMENTO {
  ponto ponto1;
  ponto ponto2;
} segmento;


typedef struct SEGMENTO_PTR {
  ponto *ponto1;
  ponto *ponto2;
} segmento_ptr_pontos;

typedef GLfloat cor_rgb[3];

typedef vector < int > vetor_de_lanes;
typedef vector < pair < int, vetor_de_lanes > > vetor_de_lanesets;

typedef vector < ponto * > poligono_simples;
typedef pair < section *, ponto * > ponto_do_poligono_e_inclinacao;
typedef vector < ponto_do_poligono_e_inclinacao > poligono_e_inclinacoes;

typedef vector < pair < int, nodo * > > nodos_da_topologia;
typedef vector < pair < int, section * > > sections_da_topologia;
typedef vector < pair < section *, laneset * > > lanesets_da_topologia;

typedef hash_map < int, ponto * > lane_end_point;
typedef hash_map < int, poligono_simples * > lane_semaphore;
typedef hash_map < int, ponto * > laneset_start_point;
typedef hash_map < int, poligono_simples * > lane_to_lane_polygon;
typedef hash_map < int, pair < int, nodo * > > current_signalplan;

typedef hash_map < int, pair<int, int> > current_phase;

typedef hash_map < int, pair < section *, laneset * > > lane_id_to_parent_section_ptr;
typedef hash_map < int, pair < section *, laneset * > > laneset_id_to_parent_section_ptr;

typedef hash_map < int, string > car_id_to_driver_name;

// typedef std::map < int, ponto * > lane_end_point;
// typedef std::map < int, poligono_simples * > lane_semaphore;
// typedef std::map < int, ponto * > laneset_start_point;


typedef vector < pair < int, int > > open_directions_vector;

// vector of    'car_id [lane_where_it_is_located, cell_in_that_lane, total_cells_in_lane]'
typedef vector < pair < int, pair < int, pair < int, pair < int, string > > > > > cars_and_locations;


// haverah um par desses para cada nodo. o 1o elemento do par eh o angulo 
// formado entre o ponto do nodo e o nodo oposto da section
typedef pair < float, section * > section_e_inclinacao;

typedef struct camera {
  GLdouble atx;
  GLdouble aty;
  GLdouble atz;
  GLdouble tox;
  GLdouble toy;
  GLdouble toz;
} Observador;


#endif
