//**************************************************************************
// Práctica 2
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>

const float AXIS_SIZE = 15000;
typedef enum
{
       POINTS,
       EDGES,
       SOLID,
       SOLID_COLORS,
       SOLID_FLAT,
       SOLID_SMOOTH,
       SELECT
} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:
       _puntos3D();
       void draw_puntos(float r, float g, float b, int grosor);

       vector<_vertex3f> vertices;
       vector<_vertex3f> colores_vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D : public _puntos3D
{
public:
       _triangulos3D();
       void draw_aristas(float r, float g, float b, int grosor);
       void draw_solido(float r, float g, float b);
       void draw_solido_colores();
       void draw(_modo modo, float r, float g, float b, float grosor);

       /* asignación de colores */
       void colors_random();
       void colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);
       void colorear(float r1, float g1, float b1);
       vector<_vertex3i> caras;
       vector<_vertex3f> colores_caras;

       // PRACTICA 4
       vector<_vertex3f> normales_caras;
       vector<_vertex3f> normales_vertices;

       void calcular_normales_caras();
       void calcular_normales_vertices();

       void draw_plano();
       void draw_suave();

       void colores_plano_caras(float r, float g, float b, float l_x, float l_y, float l_z); // ULT PARAM INTENSIDAD LUZ
       void colores_suave_caras(float r, float g, float b, float l_x, float l_y, float l_z); // ULT PARAM INTENSIDAD LUZ

       void colores_plano_vertices();
       void colores_suave_vertices();

       // MATERIAL
       _vertex4f ambiente_difusa; // COEF AMBIENTE Y DIFUSO (SUPONEMOS IGUALES)
       _vertex4f especular;       // COEF ESPECULAR
       float brillo;              // EXP BRILLO+
       bool normales_caras_calculadas, normales_vertices_calculadas;

       // PRACTICA 5
       void draw_seleccion(int r, int g, int b);
};

//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo : public _triangulos3D
{
public:
       _cubo(float tam = 0.5);
};

//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide : public _triangulos3D
{
public:
       int tipo;
       _piramide(float tam = 0.5, float al = 1.0);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply : public _triangulos3D
{
public:
       _objeto_ply();

       void parametros(char *archivo, float r = 0.0f, float g = 0.0f, float b = 0.0f, float r2 = 0.0f, float g2 = 0.0f, float b2 = 0.0f, int num = -1);
};

//************************************************************************
// objeto por revolución
//************************************************************************
// tipo indica si es una figura por revolución normal o bien un cono o una esfera
// tipo=0 normal, tipo=1 cono, tipo=2 esfera
// tapa_in=0 sin tapa, tapa_in=1 con tapa
// tapa_su=0 sin tapa, tapa_su=1 con tapa

class _rotacion : public _triangulos3D
{
public:
       _rotacion();

       void parametros(vector<_vertex3f> perfil, int num, int tipo, int tapa_in, int tapa_su);
};

//************************************************************************
// cilindro
//************************************************************************

class _cilindro : public _rotacion
{
public:
       _cilindro(float radio = 1.0, float altura = 2.0, int num = 12);
};

//************************************************************************
// cono
//************************************************************************

class _cono : public _rotacion
{
public:
       _cono(float radio = 1.0, float altura = 2.0, int num = 12);
};

//************************************************************************
// esfera
//************************************************************************

class _esfera : public _rotacion
{
public:
       _esfera(float radio = 1.0, int num1 = 6, int num2 = 6);
};

//************************************************************************
// rotacion archivo PLY
//************************************************************************

class _rotacion_PLY : public _rotacion
{
public:
       _rotacion_PLY();
       void parametros_PLY(char *archivo, int num);
};

//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion : public _triangulos3D
{
public:
       _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};

//************************************************************************
// práctica 3, objeto jerárquico articulado excavadora
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

class _pala : public _triangulos3D
{
public:
       _pala(float radio = 1.0, float ancho = 2.0, int num = 8);
};

//************************************************************************
// brazo
//************************************************************************

class _brazo : public _triangulos3D
{
public:
       _brazo();
       void draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

protected:
       _cubo cubo;
};

//************************************************************************
// cabina
//************************************************************************

class _cabina : public _triangulos3D
{
public:
       _cabina();
       void draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

protected:
       _cubo cubo;
};

//************************************************************************
// sustentación
//************************************************************************

class _sustentacion : public _triangulos3D
{
public:
       _sustentacion();
       void draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

       float radio;

protected:
       _cilindro rueda;
       _cubo base;
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

class _excavadora : public _triangulos3D
{
public:
       _excavadora();

       void draw(_modo modo, float r, float g, float b, float grosor);

       float giro_cabina;
       float giro_primer_brazo;
       float giro_segundo_brazo;
       float giro_pala;

       float giro_primer_brazo_max;
       float giro_primer_brazo_min;
       float giro_segundo_brazo_max;
       float giro_segundo_brazo_min;
       float giro_pala_max;
       float giro_pala_min;

       float tamanio_pala;

protected:
       _pala pala;
       _brazo brazo;
       _cabina cabina;
       _sustentacion sustentacion;
};

// PRACTICA 3

//************************************************************************
// base
//************************************************************************

class _base : public _triangulos3D
{
public:
       _base(float tam = 2.0, float al = 1.0);

protected:
       _cubo base_timon;
};

//************************************************************************
// vela
//************************************************************************

class _vela : public _triangulos3D
{
public:
       _vela(float tam = 0.75, float al = 0.75, float radio = 1.0, float ancho = 0.75, int num = 30);
};

//************************************************************************
// mastil
//************************************************************************

class _mastil : public _triangulos3D
{
public:
       _mastil();
       void draw(_modo modo, float r, float g, float b, float grosor, float giro_vela, float cerrar_vela, float cerrar_vela2, float fondo, float ancho, _objeto_ply obj, _objeto_ply obj2, _objeto_ply obj3);

       float radio;
       float alto;

protected:
       _cilindro mastil;
       _cono mastil_tope;
       _vela mastil_vela;
};

//************************************************************************
// remo
//************************************************************************

class _remo : public _triangulos3D
{
public:
       _remo();
       void draw(_modo modo, float r, float g, float b, float grosor);

       float radio;
       float alto;
       float ancho, fondo;

protected:
       _cilindro base_remo;
       _cubo remo_final;
};

//************************************************************************
// timon
//************************************************************************

class _timon : public _triangulos3D
{
public:
       _timon();
       void draw(_modo modo, float r, float g, float b, float grosor, float giro_timon);
       float radio;
       float alto;
       float ancho, fondo;

protected:
       _cilindro base_timon;
       _esfera esfera;
};

//************************************************************************
// ancla
//************************************************************************

class _ancla : public _triangulos3D
{
public:
       _ancla();
       void draw(_modo modo, float r, float g, float b, float grosor, _objeto_ply obj, _objeto_ply obj2, float amplia_ancla);
       float radio;
       float alto;
       float ancho, fondo;
};

//************************************************************************
// barco
//************************************************************************
class _barco : public _triangulos3D
{
public:
       _barco();
       void setPly(char *archivo, int num);
       void draw(_modo modo, float r, float g, float b, float grosor);

       char *archivo_barco;

       float ancho;
       float alto;
       float fondo;

       float giro_mastil;
       float giro_mastil_max;
       float cerrar_vela;
       float cerrar_vela2;
       float cerrar_vela_aux;

       float giro_timon;
       float giro_timon_aspa;
       float giro_timon_aspa_max;

       float giro_remo;
       float giro_remo_max;
       float giro_remo_min;

       float amplia_ancla;
       float amplia_ancla_max;

       bool sigue;

       float radio, alto_timon;

       // PRACTICA 5

       // atributos para la selección

       _vertex3f color_pick;
       vector<_vertex3i> color_select;
       vector<int> activo;
       int piezas;
       int grosor_select;

       void seleccion();

protected:
       _base base_barco;
       _remo remo_barco;
       _mastil mastil_barco;
       _timon timon_barco;
       _cubo timon_final;
       _cilindro escudo_barco;
       vector<_objeto_ply> vector_objetos_barco;
       _ancla ancla_barco;
};

class _texturas
{

public:
       _texturas();
       void draw(GLuint textura_id, GLuint textura_id2, GLuint textura_id3, GLuint textura_id4, GLuint textura_id5, GLuint textura_id6, GLuint textura_id7, GLuint textura_id8);

protected:
       // OBJETO A DIBUJAR
};
