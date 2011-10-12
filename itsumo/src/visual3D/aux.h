#ifndef __HAS_AUX_H
#define __HAS_AUX_H


#include "definitions.h"

#define isPerpendicular(a, b) equal(a, b)
#define isHorizontal(a, b) equal(a, b)

poligono_simples* cell_polygon(poligono_simples *pol, double cell, double cell_size);

void desenha_ponto(ponto p);
void desenha_ponto(nodo *p);
void desenha_segmento(nodo *inicio, nodo *fim);
void desenha_pontilhado_ptr(segmento_ptr_pontos seg, int qual);
int interseccao(ponto *p, segmento_ptr_pontos s1, segmento_ptr_pontos s2);
float max(float a, float b);
bool equal(float a, float b);
bool greaterThan(float a, float b);
bool lessThan(float a, float b);

void carrega_texturas();
void define_escala();
void desenha_escala();
void desenha_legendas();
void desenha_barra_de_controle();
void desenha_info_objeto(string info);

std::string intToString(int i);


#endif
