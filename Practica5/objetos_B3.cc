//**************************************************************************
// Práctica 3
//**************************************************************************

#include "objetos_B3.h"
#include "file_ply_stl.hpp"
#include <time.h>

//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
  //**** usando vertex_array ****
  glPointSize(grosor);
  glColor3f(r, g, b);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
  glDrawArrays(GL_POINTS, 0, vertices.size());

  /*int i;
  glPointSize(grosor);
  glColor3f(r,g,b);
  glBegin(GL_POINTS);
  for (i=0;i<vertices.size();i++){
    glVertex3fv((GLfloat *) &vertices[i]);
    }
  glEnd();*/
}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
  ambiente_difusa = _vertex4f(0.61424f, 0.04136f, 0.04136f, 0.55f);
  especular = _vertex4f(0.727811f, 0.626959f, 0.626959f, 0.55f);
  brillo = 76.8f;
  normales_caras_calculadas = false, normales_vertices_calculadas = false;
}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
  //**** usando vertex_array ****
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(grosor);
  glColor3f(r, g, b);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
  glDrawElements(GL_TRIANGLES, caras.size() * 3, GL_UNSIGNED_INT, &caras[0]);

  /*int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glLineWidth(grosor);
  glColor3f(r,g,b);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++){
    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
    }
  glEnd();*/
}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
  int i;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3f(r, g, b);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++)
  {
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada cara
//*************************************************************************

void _triangulos3D::draw_solido_colores()
{
  int i;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++)
  {
    glColor3f(colores_caras[i].r, colores_caras[i].g, colores_caras[i].b);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r, float g, float b, float grosor)
{
  switch (modo)
  {
  case POINTS:
    draw_puntos(r, g, b, grosor);
    break;
  case EDGES:
    draw_aristas(r, g, b, grosor);
    break;
  case SOLID:
    draw_solido(r, g, b);
    break;
  case SOLID_COLORS:
    draw_solido_colores();
    break;
  case SOLID_FLAT:
    draw_plano();
    break;
  case SOLID_SMOOTH:
    draw_suave();
    break;
  case SELECT:
    draw_seleccion(r, g, b);
    break;
  }
}

//*************************************************************************
// asignación colores
//*************************************************************************

void _triangulos3D::colors_random()
{
  int i, n_c;
  n_c = caras.size();
  colores_caras.resize(n_c);
  srand(time(NULL));
  for (i = 0; i < n_c; i++)
  {

    colores_caras[i].r = rand() % 1000 / 1000.0;
    colores_caras[i].g = rand() % 1000 / 1000.0;
    colores_caras[i].b = rand() % 1000 / 1000.0;
  }
}
//*************************************************************************

void _triangulos3D::colors_chess(float r1, float g1, float b1, float r2, float g2, float b2)
{
  int i, n_c;

  n_c = caras.size();
  colores_caras.resize(n_c);
  for (i = 0; i < n_c; i++)
  {
    if (i % 2 == 0)
    {
      colores_caras[i].r = r1;
      colores_caras[i].g = g1;
      colores_caras[i].b = b1;
    }
    else
    {
      colores_caras[i].r = r2;
      colores_caras[i].g = g2;
      colores_caras[i].b = b2;
    }
  }
}
void _triangulos3D::colorear(float r, float g, float b)
{

  int i, n_c;
  n_c = caras.size();
  colores_caras.resize(n_c);
  srand(time(NULL));
  for (i = 0; i < n_c; i++)
  {

    colores_caras[i].r = r;
    colores_caras[i].g = g;
    colores_caras[i].b = b;
  }
}

// ASIGNACION NORMALES CARAS
void _triangulos3D::calcular_normales_caras()
{

  int i, n_c;
  n_c = caras.size();
  normales_caras.resize(n_c);
  _vertex3f vec_a, vec_b;
  float norma;

  for (i = 0; i < n_c; i++)
  {
    vec_a = vertices[caras[i]._1] - vertices[caras[i]._0]; // CALCULO VECTORES TRIANGULOS
    vec_b = vertices[caras[i]._2] - vertices[caras[i]._0];

    normales_caras[i].x = vec_a.y * vec_b.z - vec_a.z * vec_b.y;
    normales_caras[i].y = vec_a.z * vec_b.x - vec_a.x * vec_b.z;
    normales_caras[i].z = vec_a.x * vec_b.y - vec_a.y * vec_b.x;

    norma = sqrt(pow(normales_caras[i].x, 2) + pow(normales_caras[i].y, 2) + pow(normales_caras[i].z, 2));

    normales_caras[i].x = normales_caras[i].x / norma;
    normales_caras[i].y = normales_caras[i].y / norma;
    normales_caras[i].z = normales_caras[i].z / norma;

    // normales_caras[i].normalize();
  }
  normales_caras_calculadas = true;
}

void _triangulos3D::calcular_normales_vertices()
{
  if (normales_caras_calculadas == false)
    calcular_normales_caras();
  int i, n_v;
  n_v = vertices.size();
  normales_vertices.resize(n_v);
  float norma = 0.0;

  for (int i = 0; i < n_v; i++)
  {
    normales_vertices[i].x = 0.0;
    normales_vertices[i].y = 0.0;
    normales_vertices[i].z = 0.0;
  }

  for (int i = 0; i < caras.size(); i++)
  {
    normales_vertices[caras[i]._0] += normales_caras[i];
    normales_vertices[caras[i]._1] += normales_caras[i];
    normales_vertices[caras[i]._2] += normales_caras[i];
  }

  for (int i = 0; i < n_v; i++)
  {

    norma = sqrt(pow(normales_vertices[i].x, 2) + pow(normales_vertices[i].y, 2) + pow(normales_vertices[i].z, 2));
    normales_vertices[i].x = normales_vertices[i].x / norma;
    normales_vertices[i].y = normales_vertices[i].y / norma;
    normales_vertices[i].z = normales_vertices[i].z / norma;

    // normales_vertices[i].normalize();
  }
  normales_vertices_calculadas = true;
}

void _triangulos3D::colores_plano_caras(float r, float g, float b, float l_x, float l_y, float l_z)
{
  if (normales_caras_calculadas == false)
    calcular_normales_caras();
  int i, n_c;
  n_c = caras.size();
  normales_caras.resize(n_c);

  _vertex3f vec_l;
  float norma, escalar;

  for (i = 0; i < n_c; i++)
  {

    vec_l.x = l_x - vertices[caras[i]._0].x;
    vec_l.y = l_y - vertices[caras[i]._0].y;
    vec_l.z = l_z - vertices[caras[i]._0].z;

    norma = sqrt(pow(vec_l.x, 2) + pow(vec_l.y, 2) + pow(vec_l.z, 2));
    vec_l.x = vec_l.x / norma;
    vec_l.y = vec_l.y / norma;
    vec_l.z = vec_l.z / norma;

    escalar = vec_l.x * normales_caras[i].x + vec_l.y * normales_caras[i].y + vec_l.z * normales_caras[i].z;
    if (escalar < 0)
    {
      escalar = 0.1;
    }

    colores_caras[i].r = r * escalar; // SE MULTIPLICARIA POR OTRO VALOR MAS SI LA LUZ NO FUESE BLANCA PQ ES *1
    colores_caras[i].g = g * escalar;
    colores_caras[i].b = b * escalar;
  }
}

void _triangulos3D::draw_plano()
{
  if (normales_caras_calculadas == false)
    calcular_normales_caras();
  int i;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_LIGHTING);
  glShadeModel(GL_FLAT); // GL_SMOOTH
  glEnable(GL_NORMALIZE);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&ambiente_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++)
  {
    glNormal3f(normales_caras[i].x, normales_caras[i].y, normales_caras[i].z);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_LIGHTING);
}

void _triangulos3D::draw_suave()
{
  if (normales_vertices_calculadas == false)
    calcular_normales_vertices();
  int i;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH); // GL_SMOOTH
  glEnable(GL_NORMALIZE);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&ambiente_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++)
  {
    glNormal3f(normales_vertices[caras[i]._0].x, normales_vertices[caras[i]._0].y, normales_vertices[caras[i]._0].z);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);

    glNormal3f(normales_vertices[caras[i]._1].x, normales_vertices[caras[i]._1].y, normales_vertices[caras[i]._1].z);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);

    glNormal3f(normales_vertices[caras[i]._2].x, normales_vertices[caras[i]._2].y, normales_vertices[caras[i]._2].z);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_LIGHTING);
}
void _triangulos3D::draw_seleccion(int r, int g, int b)
{
  int i;

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3ub(r, g, b);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++)
  {
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
}
//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
  // vertices
  vertices.resize(8);
  vertices[0].x = -tam;
  vertices[0].y = -tam;
  vertices[0].z = tam;
  vertices[1].x = tam;
  vertices[1].y = -tam;
  vertices[1].z = tam;
  vertices[2].x = tam;
  vertices[2].y = tam;
  vertices[2].z = tam;
  vertices[3].x = -tam;
  vertices[3].y = tam;
  vertices[3].z = tam;
  vertices[4].x = -tam;
  vertices[4].y = -tam;
  vertices[4].z = -tam;
  vertices[5].x = tam;
  vertices[5].y = -tam;
  vertices[5].z = -tam;
  vertices[6].x = tam;
  vertices[6].y = tam;
  vertices[6].z = -tam;
  vertices[7].x = -tam;
  vertices[7].y = tam;
  vertices[7].z = -tam;

  // triangulos
  caras.resize(12);
  caras[0]._0 = 0;
  caras[0]._1 = 1;
  caras[0]._2 = 3;
  caras[1]._0 = 3;
  caras[1]._1 = 1;
  caras[1]._2 = 2;
  caras[2]._0 = 1;
  caras[2]._1 = 5;
  caras[2]._2 = 2;
  caras[3]._0 = 5;
  caras[3]._1 = 6;
  caras[3]._2 = 2;
  caras[4]._0 = 5;
  caras[4]._1 = 4;
  caras[4]._2 = 6;
  caras[5]._0 = 4;
  caras[5]._1 = 7;
  caras[5]._2 = 6;
  caras[6]._0 = 0;
  caras[6]._1 = 7;
  caras[6]._2 = 4;
  caras[7]._0 = 0;
  caras[7]._1 = 3;
  caras[7]._2 = 7;
  caras[8]._0 = 3;
  caras[8]._1 = 2;
  caras[8]._2 = 7;
  caras[9]._0 = 2;
  caras[9]._1 = 6;
  caras[9]._2 = 7;
  caras[10]._0 = 0;
  caras[10]._1 = 1;
  caras[10]._2 = 4;
  caras[11]._0 = 1;
  caras[11]._1 = 5;
  caras[11]._2 = 4;

  // PRACTICA 4 NORMALES

  // colores de las caras

  colors_random();
}

//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{
  int i;
  // vertices
  vertices.resize(5);
  vertices[0].x = -tam;
  vertices[0].y = 0;
  vertices[0].z = tam;
  vertices[1].x = tam;
  vertices[1].y = 0;
  vertices[1].z = tam;
  vertices[2].x = tam;
  vertices[2].y = 0;
  vertices[2].z = -tam;
  vertices[3].x = -tam;
  vertices[3].y = 0;
  vertices[3].z = -tam;
  vertices[4].x = 0;
  vertices[4].y = al;
  vertices[4].z = 0;

  caras.resize(6);
  caras[0]._0 = 0;
  caras[0]._1 = 1;
  caras[0]._2 = 4;
  caras[1]._0 = 1;
  caras[1]._1 = 2;
  caras[1]._2 = 4;
  caras[2]._0 = 2;
  caras[2]._1 = 3;
  caras[2]._2 = 4;
  caras[3]._0 = 3;
  caras[3]._1 = 0;
  caras[3]._2 = 4;
  caras[4]._0 = 3;
  caras[4]._1 = 1;
  caras[4]._2 = 0;
  caras[5]._0 = 3;
  caras[5]._1 = 2;
  caras[5]._2 = 1;

  // PRACTICA 4 NORMALES

  // colores de las caras
  colores_caras.resize(caras.size());
  colores_plano_caras(0.8, 0.7, 0, -20, 20, 10);
}

//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply()
{
  // leer lista de coordenadas de vértices y lista de indices de vértices
}

void _objeto_ply::parametros(char *archivo, float r, float g, float b, float r2, float g2, float b2, int num)
{
  int i, n_ver, n_car;

  vector<float> ver_ply;
  vector<int> car_ply;

  _file_ply::read(archivo, ver_ply, car_ply);

  n_ver = ver_ply.size() / 3;
  n_car = car_ply.size() / 3;

  printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

  vertices.resize(n_ver);
  caras.resize(n_car);
  // vértices

  for (i = 0; i < n_ver; i++)
  {
    vertices[i].x = ver_ply[3 * i];
    vertices[i].y = ver_ply[3 * i + 1];
    vertices[i].z = ver_ply[3 * i + 2];
  }

  // vértices
  for (i = 0; i < n_car; i++)
  {
    caras[i].x = car_ply[3 * i];
    caras[i].y = car_ply[3 * i + 1];
    caras[i].z = car_ply[3 * i + 2];
  }

  // PRACTICA 4 NORMALES
  colores_caras.resize(n_car);
  colores_plano_caras(0.8, 0.7, 0, -20, 20, 10);

  switch (num)
  {

  case 1:
    ambiente_difusa = _vertex4f(0.25f, 0.148f, 0.06475f, 1.0f);
    especular = _vertex4f(0.774597f, 0.458561f, 0.200621f, 1.0f);
    brillo = 76.8;
    break;

  case 2:
    ambiente_difusa = _vertex4f(0.714f, 0.4284f, 0.18144f, 1.0f);
    especular = _vertex4f(0.393548f, 0.271906f, 0.166721f, 1.0f);
    brillo = 25.6;
    break;

  case 3:
    ambiente_difusa = _vertex4f(0.714f, 0.4284f, 0.18144f, 1.0f);
    especular = _vertex4f(0.393548f, 0.271906f, 0.166721f, 1.0f);
    brillo = 25.6;
    break;

  case 4:
    ambiente_difusa = _vertex4f(0.25f, 0.148f, 0.06475f, 1.0f);
    especular = _vertex4f(0.774597f, 0.458561f, 0.200621f, 1.0f);
    brillo = 76.8;
    break;

  case 5:
    ambiente_difusa = _vertex4f(0.3f, 0.16f, 0.07f, 1.0f);
    especular = _vertex4f(0.774597f, 0.458561f, 0.200621f, 1.0f);
    brillo = 76.8;
    break;

  case 6:
    ambiente_difusa = _vertex4f(1.0f, 0.829f, 0.829f, 0.922f);
    especular = _vertex4f(0.296648f, 0.296648f, 0.296648f, 0.922f);
    brillo = 11.264f;
    break;

  case 7:
    ambiente_difusa = _vertex4f(1.0f, 0.829f, 0.829f, 0.922f);
    especular = _vertex4f(0.296648f, 0.296648f, 0.296648f, 0.922f);
    brillo = 10.0f;
    break;

  case 8:
    ambiente_difusa = _vertex4f(0.714f, 0.4284f, 0.18144f, 1.0f);
    especular = _vertex4f(0.393548f, 0.271906f, 0.166721f, 1.0f);
    brillo = 25.6;
    break;

  case 9:
    ambiente_difusa = _vertex4f(0.05f, 0.05f, 0.05f, 1.0f);
    especular = _vertex4f(0.7f, 0.7f, 0.7f, 1.0f);
    brillo = 10.0f;
    break;

  case 10:
    ambiente_difusa = _vertex4f(0.05f, 0.05f, 0.05f, 1.0f);
    especular = _vertex4f(0.7f, 0.7f, 0.7f, 1.0f);
    brillo = 10.0f;
    break;
  default:

    break;
  }

  /*
    colores_caras.resize(n_car);
    srand(10);

    // colores


    float sum;
    int n;
    colores_caras.resize(caras.size());
    if(!(r==0.0f && r2==0.0f && g==0.0f&& g2==0.0f&& b==0.0f&& b2==0.0f)){
      colors_chess( r,  g,  b,  r2,  g2,  b2);
    }else{
      colors_random();
    }

    */
}

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{
}

void _rotacion::parametros(vector<_vertex3f> perfil, int num, int tipo, int tapa_in, int tapa_su)
{
  int i, j;
  _vertex3f vertice_aux;
  _vertex3i cara_aux;
  int num_aux;
  float radio;

  // tratamiento de los vértice
  radio = sqrt(perfil[0].x * perfil[0].x + perfil[0].y * perfil[0].y);

  num_aux = perfil.size();
  if (tipo == 1)
    num_aux = num_aux - 1;
  vertices.resize(num_aux * num + 2);
  for (j = 0; j < num; j++)
  {
    for (i = 0; i < num_aux; i++)
    {
      vertice_aux.x = perfil[i].x * cos(2.0 * M_PI * j / (1.0 * num)) +
                      perfil[i].z * sin(2.0 * M_PI * j / (1.0 * num));
      vertice_aux.z = -perfil[i].x * sin(2.0 * M_PI * j / (1.0 * num)) +
                      perfil[i].z * cos(2.0 * M_PI * j / (1.0 * num));
      vertice_aux.y = perfil[i].y;
      vertices[i + j * num_aux] = vertice_aux;
    }
  }

  caras.resize(2 * (num_aux - 1) * num + 2 * num);
  int c = 0;
  for (j = 0; j < num; j++)
  {
    for (i = 0; i < num_aux - 1; i++)
    {
      caras[c]._0 = i + j * num_aux;
      caras[c]._1 = ((j + 1) % num) * num_aux + i;
      caras[c]._2 = 1 + i + j * num_aux;
      c += 1;
      caras[c]._0 = ((j + 1) % num) * num_aux + i;
      caras[c]._1 = ((j + 1) % num) * num_aux + 1 + i;
      caras[c]._2 = 1 + i + j * num_aux;
      c += 1;
    }
  }

  // tapa inferior
  int total = num_aux * num;
  vertices[total].x = 0.0;
  if (tipo == 2)
    vertices[total].y = -radio;
  else
    vertices[total].y = perfil[0].y;
  vertices[total].z = 0.0;

  for (j = 0; j < num; j++)
  {
    caras[c]._0 = j * num_aux;
    caras[c]._1 = ((j + 1) % num) * num_aux;
    caras[c]._2 = total;
    c += 1;
  }

  // tapa superior
  vertices[total + 1].x = 0.0;
  if (tipo == 1)
    vertices[total + 1].y = perfil[1].y;
  if (tipo == 0)
    vertices[total + 1].y = perfil[num_aux - 1].y;
  if (tipo == 2)
    vertices[total + 1].y = radio;
  vertices[total + 1].z = 0.0;

  for (j = 0; j < num; j++)
  {
    caras[c]._0 = total + 1;
    caras[c]._1 = ((j + 1) % num) * num_aux + num_aux - 1;
    caras[c]._2 = num_aux - 1 + j * num_aux;
    c += 1;
  }

  // colores de las caras
  // PRACTICA 4 NORMALES
  calcular_normales_caras();

  float norma;
  if (tipo == 2)
  {
    normales_vertices.resize(vertices.size());
    for (int i = 0; i < vertices.size(); i++)
    {
      norma = sqrt(pow(vertices[i].x, 2) + pow(vertices[i].y, 2) + pow(vertices[i].z, 2));
      normales_vertices[i].x = vertices[i].x / norma;
      normales_vertices[i].y = vertices[i].y / norma;
      normales_vertices[i].z = vertices[i].z / norma;
    }
  }
  else
  {
    calcular_normales_vertices();
  }
  colores_caras.resize(caras.size());
  colores_plano_caras(0.8, 0.7, 0, -20, 20, 10);
}

//************************************************************************
// objeto cilindro (caso especial de rotacion)
//************************************************************************

_cilindro::_cilindro(float radio, float altura, int num)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;

  aux.x = radio;
  aux.y = -altura / 2.0;
  aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = radio;
  aux.y = altura / 2.0;
  aux.z = 0.0;
  perfil.push_back(aux);
  parametros(perfil, num, 0, 1, 1);
}

//************************************************************************
// objeto cono (caso especial de rotacion)
//************************************************************************

_cono::_cono(float radio, float altura, int num)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;

  aux.x = radio;
  aux.y = 0;
  aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = 0.0;
  aux.y = altura;
  aux.z = 0.0;
  perfil.push_back(aux);
  parametros(perfil, num, 1, 1, 1);
}

//************************************************************************
// objeto esfera (caso especial de rotacion)
//************************************************************************

_esfera::_esfera(float radio, int num1, int num2)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;
  for (int i = 1; i < num1; i++)
  {
    aux.x = radio * cos(M_PI * i / num1 - M_PI / 2.0);
    aux.y = radio * sin(M_PI * i / num1 - M_PI / 2.0);
    aux.z = 0.0;

    perfil.push_back(aux);
  }

  parametros(perfil, num2, 2, 1, 1);
}

//************************************************************************
// rotacion archivo PLY (caso especial de rotacion)
//************************************************************************

_rotacion_PLY::_rotacion_PLY()
{
}

void _rotacion_PLY::parametros_PLY(char *archivo, int num)
{
}

//************************************************************************

//************************************************************************
// objeto por extrusión
//************************************************************************

_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
  int i;
  _vertex3f vertice_aux;
  _vertex3i cara_aux;
  int num_aux;

  // tratamiento de los vértice

  num_aux = poligono.size();
  vertices.resize(num_aux * 2);
  for (i = 0; i < num_aux; i++)
  {
    vertices[2 * i] = poligono[i];
    vertices[2 * i + 1].x = poligono[i].x + x;
    vertices[2 * i + 1].y = poligono[i].y + y;
    vertices[2 * i + 1].z = poligono[i].z + z;
  }

  // tratamiento de las caras

  caras.resize(num_aux * 2);
  int c = 0;
  for (i = 0; i < num_aux; i++)
  {
    caras[c]._0 = i * 2;
    caras[c]._1 = (i * 2 + 2) % (num_aux * 2);
    caras[c]._2 = i * 2 + 1;
    c = c + 1;
    caras[c]._0 = (i * 2 + 2) % (num_aux * 2);
    caras[c]._1 = (i * 2 + 2) % (num_aux * 2) + 1;
    caras[c]._2 = i * 2 + 1;
    c = c + 1;
  }

  // colores de las caras

  colors_random();
}

//************************************************************************
// práctica 3, objeto jerárquico articulado
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

_pala::_pala(float radio, float ancho, int num)
{
  vector<_vertex3f> perfil;
  _vertex3f vertice_aux;
  _vertex3i cara_aux;
  int i, j;

  vertice_aux.x = radio;
  vertice_aux.y = 0;
  vertice_aux.z = -ancho / 2.0;
  perfil.push_back(vertice_aux);
  vertice_aux.z = ancho / 2.0;
  perfil.push_back(vertice_aux);

  // tratamiento de los vértices

  for (j = 0; j <= num; j++)
  {
    for (i = 0; i < 2; i++)
    {
      vertice_aux.x = perfil[i].x * cos(M_PI * j / (1.0 * num)) -
                      perfil[i].y * sin(M_PI * j / (1.0 * num));
      vertice_aux.y = perfil[i].x * sin(M_PI * j / (1.0 * num)) +
                      perfil[i].y * cos(M_PI * j / (1.0 * num));
      vertice_aux.z = perfil[i].z;
      vertices.push_back(vertice_aux);
    }
  }

  // tratamiento de las caras

  for (j = 0; j < num; j++)
  {
    cara_aux._0 = j * 2;
    cara_aux._1 = (j + 1) * 2;
    cara_aux._2 = (j + 1) * 2 + 1;
    caras.push_back(cara_aux);

    cara_aux._0 = j * 2;
    cara_aux._1 = (j + 1) * 2 + 1;
    cara_aux._2 = j * 2 + 1;
    caras.push_back(cara_aux);
  }

  // tapa inferior
  vertice_aux.x = 0;
  vertice_aux.y = 0;
  vertice_aux.z = -ancho / 2.0;
  vertices.push_back(vertice_aux);

  for (j = 0; j < num; j++)
  {
    cara_aux._0 = j * 2;
    cara_aux._1 = (j + 1) * 2;
    cara_aux._2 = vertices.size() - 1;
    caras.push_back(cara_aux);
  }

  // tapa superior
  vertice_aux.x = 0;
  vertice_aux.y = 0;
  vertice_aux.z = ancho / 2.0;
  vertices.push_back(vertice_aux);

  for (j = 0; j < num; j++)
  {
    cara_aux._0 = j * 2 + 1;
    cara_aux._1 = (j + 1) * 2 + 1;
    cara_aux._2 = vertices.size() - 1;
    caras.push_back(cara_aux);
  }

  colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}

//************************************************************************
// brazo
//************************************************************************

_brazo::_brazo()
{
  ancho = 0.6;
  alto = 0.1;
  fondo = 0.1;
  colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
};

void _brazo::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
  glScalef(ancho, alto, fondo);
  glTranslatef(0.5, 0, 0);
  cubo.draw(modo, r, g, b, grosor);
  glPopMatrix();
};

//************************************************************************
// cabina
//************************************************************************

_cabina::_cabina()
{
  ancho = 0.4;
  alto = 0.6;
  fondo = 0.4;
  cubo.colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
};

void _cabina::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
  glScalef(ancho, alto, fondo);
  cubo.draw(modo, r, g, b, grosor);
  glPopMatrix();
};

//************************************************************************
// sustentación
//************************************************************************

_sustentacion::_sustentacion()
{
  ancho = 1.2;
  alto = 0.3;
  fondo = 0.8;
  radio = 0.15;
  base.colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
};

void _sustentacion::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
  glTranslatef(2 * ancho / 6, -alto / 2.0, 0);
  glRotatef(90, 1, 0, 0);
  glScalef(radio, fondo / 2.2, radio);
  rueda.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-2 * ancho / 6, -alto / 2.0, 0);
  glRotatef(90, 1, 0, 0);
  glScalef(radio, fondo / 2.2, radio);
  rueda.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();
  glScalef(ancho, alto, fondo);
  base.draw(modo, r, g, b, grosor);
  glPopMatrix();
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

_excavadora::_excavadora()
{
  giro_cabina = 0.0;
  giro_primer_brazo = 0.0;
  giro_primer_brazo_max = 0;
  giro_primer_brazo_min = -90;
  giro_segundo_brazo = 0.0;
  giro_segundo_brazo_max = 30;
  giro_segundo_brazo_min = 0;
  giro_pala = 0.0;
  giro_pala_max = 50.0;
  giro_pala_min = -90.0;

  tamanio_pala = 0.15;
};

void _excavadora::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();

  sustentacion.draw(modo, r, g, b, grosor);

  glTranslatef(0, (cabina.alto + sustentacion.alto) / 2.0, 0);
  glRotatef(giro_cabina, 0, 1, 0);
  cabina.draw(modo, r, g, b, grosor);

  glTranslatef(cabina.ancho / 2.0, 0, 0);
  glRotatef(giro_segundo_brazo, 0, 0, 1);
  brazo.draw(modo, r, g, b, grosor);

  glTranslatef(brazo.ancho, 0, 0);
  glRotatef(giro_primer_brazo, 0, 0, 1);
  brazo.draw(modo, r, g, b, grosor);

  glTranslatef(brazo.ancho, 0, 0);
  glRotatef(giro_pala, 0, 0, 1);
  glTranslatef(tamanio_pala, 0, 0);
  glScalef(tamanio_pala, tamanio_pala, tamanio_pala);
  pala.draw(modo, r, g, b, grosor);

  glPopMatrix();
};

// PRACTICA 3
//************************************************************************
//  base
//************************************************************************
_base::_base(float tam, float al)
{
  int i;
  // vertices
  vertices.resize(15);
  vertices[0].x = 2 * tam;
  vertices[0].y = 0;
  vertices[0].z = 0;
  vertices[1].x = tam;
  vertices[1].y = -al;
  vertices[1].z = 0;
  vertices[2].x = tam;
  vertices[2].y = 0;
  vertices[2].z = -tam;
  vertices[3].x = tam;
  vertices[3].y = 0;
  vertices[3].z = tam;

  vertices[4].x = -2 * tam;
  vertices[4].y = 0;
  vertices[4].z = 0;
  vertices[5].x = -tam;
  vertices[5].y = -al;
  vertices[5].z = 0;
  vertices[6].x = -tam;
  vertices[6].y = 0;
  vertices[6].z = -tam;
  vertices[7].x = -tam;
  vertices[7].y = 0;
  vertices[7].z = tam;

  // PUNTO MEDIO

  vertices[8].x = 0;
  vertices[8].y = 0;
  vertices[8].z = 0;
  int aux = 9;
  for (int i = 0; i < 9; i++)
  {
    if (i != 1 && i != 5 && i != 8)
    {
      if (i == 0 || i == 4)
      {
        if (i == 0)
        {
          vertices[aux].y = vertices[i].y + 0.5;
        }
        else
        {
          vertices[aux].y = vertices[i].y + 0.75;
        }
        vertices[aux].x = vertices[i].x;
        vertices[aux].z = vertices[i].z;
        aux++;
      }
      else
      {
        vertices[aux].x = vertices[i].x;
        vertices[aux].y = vertices[i].y + 0.3;
        vertices[aux].z = vertices[i].z;
        aux++;
      }
    }
  }

  caras.resize(26);

  caras[0]._0 = 0;
  caras[0]._1 = 1;
  caras[0]._2 = 2;
  caras[1]._0 = 0;
  caras[1]._1 = 1;
  caras[1]._2 = 3;
  caras[2]._0 = 1;
  caras[2]._1 = 5;
  caras[2]._2 = 2; // z+
  caras[3]._0 = 1;
  caras[3]._1 = 5;
  caras[3]._2 = 3;

  caras[4]._0 = 4;
  caras[4]._1 = 5;
  caras[4]._2 = 6; // z+
  caras[5]._0 = 4;
  caras[5]._1 = 5;
  caras[5]._2 = 7;
  caras[6]._0 = 5;
  caras[6]._1 = 2;
  caras[6]._2 = 6; // z-
  caras[7]._0 = 5;
  caras[7]._1 = 3;
  caras[7]._2 = 7; // z-

  // tapa superior
  caras[8]._0 = 4;
  caras[8]._1 = 7;
  caras[8]._2 = 6;
  caras[9]._0 = 0;
  caras[9]._1 = 2;
  caras[9]._2 = 3;

  caras[10]._0 = 6;
  caras[10]._1 = 7;
  caras[10]._2 = 8;
  caras[11]._0 = 6;
  caras[11]._1 = 2;
  caras[11]._2 = 8;

  caras[12]._0 = 2;
  caras[12]._1 = 3;
  caras[12]._2 = 8;
  caras[13]._0 = 7;
  caras[13]._1 = 3;
  caras[13]._2 = 8;

  // barandilla
  caras[14]._0 = 0;
  caras[14]._1 = 2;
  caras[14]._2 = 10;
  caras[15]._0 = 0;
  caras[15]._1 = 9;
  caras[15]._2 = 10;
  caras[16]._0 = 0;
  caras[16]._1 = 3;
  caras[16]._2 = 11;
  caras[17]._0 = 0;
  caras[17]._1 = 9;
  caras[17]._2 = 11;
  caras[18]._0 = 2;
  caras[18]._1 = 6;
  caras[18]._2 = 13;
  caras[19]._0 = 2;
  caras[19]._1 = 10;
  caras[19]._2 = 13;
  caras[20]._0 = 3;
  caras[20]._1 = 7;
  caras[20]._2 = 14;
  caras[21]._0 = 3;
  caras[21]._1 = 11;
  caras[21]._2 = 14;
  caras[22]._0 = 4;
  caras[22]._1 = 6;
  caras[22]._2 = 13;
  caras[23]._0 = 4;
  caras[23]._1 = 12;
  caras[23]._2 = 13;
  caras[24]._0 = 4;
  caras[24]._1 = 12;
  caras[24]._2 = 14;
  caras[25]._0 = 4;
  caras[25]._1 = 7;
  caras[25]._2 = 14;

  colors_chess(0.379f, 0.278f, 0.241f, 0.472, 0.338, 0.263); // Bronze
}

//************************************************************************
// vela
//************************************************************************
_vela::_vela(float tam, float al, float radio, float ancho, int num)
{
  vector<_vertex3f> perfil;
  _vertex3f vertice_aux;
  _vertex3i cara_aux;
  int i, j;

  vertice_aux.x = radio / 2.5;
  vertice_aux.y = 0;
  vertice_aux.z = -ancho;
  perfil.push_back(vertice_aux);
  vertice_aux.z = ancho;
  perfil.push_back(vertice_aux);

  // tratamiento de los vértices

  for (j = 0; j <= num; j++)
  {
    for (i = 0; i < 2; i++)
    {
      vertice_aux.y = perfil[i].x * cos(M_PI * j / (1.0 * num)) -
                      perfil[i].y * sin(M_PI * j / (1.0 * num));
      vertice_aux.x = perfil[i].x * sin(M_PI * j / (1.0 * num)) +
                      perfil[i].y * cos(M_PI * j / (1.0 * num));
      vertice_aux.z = perfil[i].z;
      vertices.push_back(vertice_aux);
    }
  }

  // tratamiento de las caras

  for (j = 0; j < num; j++)
  {
    cara_aux._0 = j * 2;
    cara_aux._1 = (j + 1) * 2;
    cara_aux._2 = (j + 1) * 2 + 1;
    caras.push_back(cara_aux);

    cara_aux._0 = j * 2;
    cara_aux._1 = (j + 1) * 2 + 1;
    cara_aux._2 = j * 2 + 1;
    caras.push_back(cara_aux);
  }

  colors_chess(0.1f, 0.1f, 0.0f, 0.1f, 0.1f, 0.0f);
}

//************************************************************************
// mastil
//************************************************************************
_mastil::_mastil()
{
  radio = 0.05;
  alto = 0.75;
  mastil_vela.colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}
void _mastil::draw(_modo modo, float r, float g, float b, float grosor, float giro_vela, float cerrar_vela, float cerrar_vela2, float fondo, float ancho, _objeto_ply obj, _objeto_ply obj2, _objeto_ply obj3)
{
  /*
  glPushMatrix();
  glScalef(radio, alto, radio);
  mastil.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0,alto,0.0);
  glRotatef(90.0,0.0,0.0,1.0);
  glRotatef(90.0,1.0,0.0,0.0);
  glScalef(radio/2, alto/1.2, radio/2);
  //glTranslatef(giro_vela,1.0,0.0);
  mastil.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, alto/2, 0);
  //glRotatef(giro_vela,0.0,1.0,0.0);
  glScalef(1.0,0.5,1.0);
  glScalef(1.0,1,0.5);
  glScalef(0.5,1.0,1.0);
  glScalef(1.0,2,1.0);
  glScalef(1.0,1,alto*2.0);
  glRotatef(180,0.0,1.0,0.0);
  mastil_vela.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, alto, 0);
  glScalef(radio, radio / 2, radio);
  mastil_tope.draw(modo, r, g, b, grosor);
  glPopMatrix();

  */

  glPushMatrix();

  // glRotatef(giro_mastil,0.0,1.0,0.0);
  glTranslatef(0, 6.5 * ancho, 0);
  obj.draw(modo, r, g, b, grosor);
  glPopMatrix();

  // VELA
  glPushMatrix();
  glTranslatef(0, 0, 1.5 * alto);
  glRotatef(giro_vela, 0.0, 0.0, 1.0);
  glTranslatef(0, 0, cerrar_vela2);
  glScalef(1, cerrar_vela, 1);
  glScalef(1, 1, cerrar_vela);
  glRotatef(-10 * cerrar_vela, 0.0, 0.0, 1.0);
  obj2.draw(modo, r, g, b, grosor);
  glPopMatrix();

  // BANDERA
  glPushMatrix();
  glTranslatef(0, 0, 1.5 * alto);
  // obj3.draw(modo, r, g, b, grosor);
  glPopMatrix();
}

//************************************************************************
//  remo
//************************************************************************
_remo::_remo()
{
  ancho = 1;
  alto = 0.25;
  fondo = 0.5;
  radio = 0.01;
  base_remo.colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
  remo_final.colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}
void _remo::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
  glScalef(radio, alto, radio);
  base_remo.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glScalef(ancho / 5, alto / 10, fondo / 5);
  glTranslatef(fondo * 3, 0, 0);
  remo_final.draw(modo, r, g, b, grosor);
  glPopMatrix();
}

//************************************************************************
//  timon
//************************************************************************

_timon::_timon()
{
  ancho = 5;
  alto = 0.01;
  fondo = 0.5;
  radio = 0.15;
  colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}
void _timon::draw(_modo modo, float r, float g, float b, float grosor, float giro_timon)
{
  glPushMatrix();
  glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
  glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

  glScalef(radio, alto, radio);
  base_timon.draw(modo, r, g, b, grosor);
  glPopMatrix();

  double aux = 16.5 * alto;
  float aux2 = 0.0;
  glPushMatrix();
  glScalef(radio / 4, radio / 4, radio / 4);
  esfera.draw(modo, r, g, b, grosor);
  glPopMatrix();
  for (int i = 0; i < 8; i++)
  {
    glPushMatrix();
    glRotatef(aux2, 0.0f, 0.0f, 1.0f);
    glTranslatef(0, aux, 0);
    glScalef(radio / 10, radio / 5, radio / 10);
    esfera.draw(modo, r, g, b, grosor);
    glPopMatrix();
    aux *= -1;
    aux2 += 45.0;
  }
}

//************************************************************************
//  ancla
//************************************************************************

_ancla::_ancla()
{
  ancho = 5;
  alto = 0.01;
  fondo = 0.5;
  radio = 0.15;
  colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}
void _ancla::draw(_modo modo, float r, float g, float b, float grosor, _objeto_ply obj, _objeto_ply obj2, float amplia_ancla)
{
  double aux = 0;
  for (int i = 0; i < amplia_ancla; i++)
  {
    glPushMatrix();
    glTranslatef(0, aux, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.05, 0.05, 0.05);
    obj.draw(modo, r, g, b, grosor);
    glPopMatrix();
    aux -= 0.1;
  }
  glPushMatrix();
  glTranslatef(0, aux + 0.25, 0);
  glRotatef(-90, 1, 0, 0);
  glScalef(0.05, 0.05, 0.05);

  obj2.draw(modo, r, g, b, grosor);
  glPopMatrix();
}

// PRACTICA 3
//************************************************************************
//  barco
//************************************************************************
_barco::_barco()
{
  ancho = 0.5;
  alto = 0.5;
  fondo = 0.5;

  giro_mastil = 0.0;
  giro_mastil_max = 16;
  cerrar_vela = 0.0351844;
  cerrar_vela2 = 3.85926;
  cerrar_vela_aux = 15;

  giro_timon_aspa = 0.0;
  giro_timon_aspa_max = 15.0;
  amplia_ancla = 1;
  amplia_ancla_max = 14;

  giro_remo = 0.0;
  giro_remo_max = 40;
  giro_remo_min = -40;

  double aux1 = -1.0;

  radio = 0.15;

  alto_timon = 0.01;

  sigue = true;

  // PRACTICA 5
  int color = 50;
  piezas = 24;
  grosor_select = 0.5;
  color_pick = _vertex3f(0.0, 1.0, 0.0);
  color_select.resize(piezas);
  activo.resize(piezas);

  for (int i = 0; i < piezas; i++)
  {
    activo[i] = 0;
    color_select[i].r = color_select[i].g = color_select[i].b = color;
    color += 5;
  }
}
void _barco::draw(_modo modo, float r, float g, float b, float grosor)
{

  float r_p, g_p, b_p;  int tam = 2;

  r_p = color_pick.r;  g_p = color_pick.g;  b_p = color_pick.b;
//------------------------------------------------------------------------------------//
  // BASE BARCO
  glPushMatrix();
  glRotatef(-90, 1.0, 0.0, 0.0);  
  glRotatef(-90, 0.0, 0.0, 1.0);  
  glRotatef(-180, 0.0, 0.0, 1.0);  
  glTranslatef(0, 0, -2 * alto);  
  glScalef(ancho, alto, fondo);
  if (activo[0] == 1)
    vector_objetos_barco[4].draw(EDGES, r_p, g_p, b_p, grosor_select);
  else
    vector_objetos_barco[4].draw(modo, r, g, b, grosor);
  glPopMatrix();

//------------------------------------------------------------------------------------//
  // REMOS
  double aux1 = -1.0;
  double aux = -1;
  for (int i = 0; i < 7; i++)
  {
    glPushMatrix();
    glRotatef(100.0f, 1.0, 0.0, 0.0);
    glTranslatef(0.0, remo_barco.alto * 2, 0.0);
    glTranslatef(0.0, 0.0, remo_barco.alto - 0.1);
    glTranslatef(2 * aux, 0.0, 0.0);

    glRotatef(giro_remo, 0.0, 0.0, -1);
    glRotatef(90, 0.0, 0.0, 1.0);
    if (activo[i+1] == 1)
      vector_objetos_barco[2].draw(EDGES, r_p, g_p, b_p, grosor_select);
    else
      vector_objetos_barco[2].draw(modo, r, g, b, grosor);
    glPopMatrix();

    aux += 1.3 * remo_barco.alto;
  }

  aux = -1;
  for (int i = 0; i < 7; i++)
  {
    glPushMatrix();
    glRotatef(-100.0f, 1.0, 0.0, 0.0);
    glTranslatef(0.0, remo_barco.alto * 2, 0.0);
    glTranslatef(0.0, 0.0, -(remo_barco.alto - 0.1));
    glTranslatef(2 * aux, 0.0, 0.0);

    glRotatef(giro_remo, 0.0, 0.0, -1.0);
    // glScalef(0.5,0.5,0.5);
    glRotatef(90, 0.0, 0.0, 1.0);
    if (activo[i+8] == 1)
      vector_objetos_barco[2].draw(EDGES, r_p, g_p, b_p, grosor_select);
    else
      vector_objetos_barco[2].draw(modo, r, g, b, grosor);
    glPopMatrix();

    aux += 1.3 * remo_barco.alto;
  }

//------------------------------------------------------------------------------------//
  // MASTIL
  glPushMatrix();
  // glRotatef(giro_mastil,0.0,1.0,0.0);
  glRotatef(-90, 1, 0, 0);
  glRotatef(90, 0, 0, 1);
  glTranslatef(0, 0, -alto / 2);
  if (activo[15] == 1)
    mastil_barco.draw(EDGES, r_p, g_p, b_p, grosor_select, giro_mastil, cerrar_vela, cerrar_vela2, fondo, ancho, vector_objetos_barco[5], vector_objetos_barco[6], vector_objetos_barco[11]);
  else
    mastil_barco.draw(modo, r, g, b, grosor, giro_mastil, cerrar_vela, cerrar_vela2, fondo, ancho, vector_objetos_barco[5], vector_objetos_barco[6], vector_objetos_barco[11]);
  glPopMatrix();

//------------------------------------------------------------------------------------//
  // TIMON
  glPushMatrix();
  glRotatef(90.0f, 0.0, 1.0, 0.0);
  glRotatef(-90.0f, 1.0, 0.0, 0.0);
  glTranslatef(0.0, -5 * alto, alto / 2);
  glScalef(1.2 * radio, 1.2 * radio, 1.2 * radio);
  glRotatef(giro_timon, 0.0, 1.0, 0.0);
  if (activo[16] == 1)
    vector_objetos_barco[3].draw(EDGES, r_p, g_p, b_p, grosor_select);
    else
    vector_objetos_barco[3].draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();

  glTranslatef(0, -5.5 * alto, 0);
  glRotatef(90.0f, 0.0, 1.0, 0.0);
  glRotatef(-90.0f, 1.0, 0.0, 0.0);
  glRotatef(-20.0f, 1.0, 0.0, 0.0);
  glScalef(0.005, 0.005, 0.005);
  glRotatef(giro_timon_aspa, 0.0, 1.0, 0.0);
  if (activo[17] == 1)
    vector_objetos_barco[8].draw(EDGES, r_p, g_p, b_p, grosor_select);
    else
    vector_objetos_barco[8].draw(modo, r, g, b, grosor);
  glPopMatrix();

//------------------------------------------------------------------------------------//
  // VIKINGO
  aux = -2;
  for (int i = 0; i < 5; i++)
  {
    glPushMatrix();
    glTranslatef(ancho * 5.2, -alto / 2, -alto / 2);
    glTranslatef(2.5 * aux, 0, 0);
    glRotatef(-90, 1.0, 0.0, 0);
    glRotatef(90, 0.0, 0.0, 1);
    glScalef(0.001, 0.001, 0.001);
    if (activo[i + 18] == 1)
      vector_objetos_barco[7].draw(EDGES, r_p, g_p, b_p, grosor_select);
    else
      vector_objetos_barco[7].draw(modo, r, g, b, grosor);

    glPopMatrix();
    aux += 0.5;
  }

//------------------------------------------------------------------------------------//
  // ANCLA
  glPushMatrix();
  glTranslatef(-6 * ancho, -3.5 * alto, ancho * 0.75);
  glScalef(2, 2, 2);
  if (activo[23] == 1)
    ancla_barco.draw(EDGES, r_p, g_p, b_p, grosor_select, vector_objetos_barco[9], vector_objetos_barco[10], amplia_ancla);
    else
    ancla_barco.draw(modo, r, g, b, grosor, vector_objetos_barco[9], vector_objetos_barco[10], amplia_ancla);
  glPopMatrix();
}

void _barco::seleccion()
{
  _vertex3i color;
  
  //glDrawBuffer(GL_BACK);

//------------------------------------------------------------------------------------//
  // BASE BARCO
  glPushMatrix();
  glRotatef(-90, 1.0, 0.0, 0.0);
  glRotatef(-90, 0.0, 0.0, 1.0);
  glRotatef(-180, 0.0, 0.0, 1.0);
  glTranslatef(0, 0, -2 * alto);
  glScalef(ancho, alto, fondo);
  color = color_select[0];
  vector_objetos_barco[4].draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

//------------------------------------------------------------------------------------//
  // REMOS
  double aux1 = -1.0;
  double aux = -1;
  for (int i = 0; i < 7; i++)
  {
    glPushMatrix();
    glRotatef(100.0f, 1.0, 0.0, 0.0);
    glTranslatef(0.0, remo_barco.alto * 2, 0.0);
    glTranslatef(0.0, 0.0, remo_barco.alto - 0.1);
    glTranslatef(2 * aux, 0.0, 0.0);

    glRotatef(giro_remo, 0.0, 0.0, -1);
    glRotatef(90, 0.0, 0.0, 1.0);
    color = color_select[i + 1];
    vector_objetos_barco[2].draw(SELECT, color.r, color.g, color.b, 1);
    glPopMatrix();

    aux += 1.3 * remo_barco.alto;
  }
  aux = -1;
  for (int i = 0; i < 7; i++)
  {
    glPushMatrix();
    glRotatef(-100.0f, 1.0, 0.0, 0.0);
    glTranslatef(0.0, remo_barco.alto * 2, 0.0);
    glTranslatef(0.0, 0.0, -(remo_barco.alto - 0.1));
    glTranslatef(2 * aux, 0.0, 0.0);

    glRotatef(giro_remo, 0.0, 0.0, -1.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    color = color_select[i + 8];
    vector_objetos_barco[2].draw(SELECT, color.r, color.g, color.b, 1);
    glPopMatrix();

    aux += 1.3 * remo_barco.alto;
  }

//------------------------------------------------------------------------------------//
  // MASTIL
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  glRotatef(90, 0, 0, 1);
  glTranslatef(0, 0, -alto / 2);
  color = color_select[15];
  mastil_barco.draw(SELECT, color.r, color.g, color.b, 1, giro_mastil, cerrar_vela, cerrar_vela2, fondo, ancho, vector_objetos_barco[5], vector_objetos_barco[6], vector_objetos_barco[11]);
  glPopMatrix();

//------------------------------------------------------------------------------------//
  // TIMON
  glPushMatrix();
  glRotatef(90.0f, 0.0, 1.0, 0.0);
  glRotatef(-90.0f, 1.0, 0.0, 0.0);
  glTranslatef(0.0, -5 * alto, alto / 2);
  glScalef(1.2 * radio, 1.2 * radio, 1.2 * radio);
  glRotatef(giro_timon, 0.0, 1.0, 0.0);
  color = color_select[16];
  vector_objetos_barco[3].draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  glPushMatrix();

  glTranslatef(0, -5.5 * alto, 0);
  glRotatef(90.0f, 0.0, 1.0, 0.0);
  glRotatef(-90.0f, 1.0, 0.0, 0.0);
  glRotatef(-20.0f, 1.0, 0.0, 0.0);
  glScalef(0.005, 0.005, 0.005);
  glRotatef(giro_timon_aspa, 0.0, 1.0, 0.0);
  color = color_select[17];
  vector_objetos_barco[8].draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();


//------------------------------------------------------------------------------------//
  // VIKINGO
  aux = -2;
  for (int i = 0; i < 5; i++)
  {
    glPushMatrix();
    glTranslatef(ancho * 5.2, -alto / 2, -alto / 2);
    glTranslatef(2.5 * aux, 0, 0);
    glRotatef(-90, 1.0, 0.0, 0);
    glRotatef(90, 0.0, 0.0, 1);
    glScalef(0.001, 0.001, 0.001);
    color = color_select[i + 18];
    vector_objetos_barco[7].draw(SELECT, color.r, color.g, color.b, 1);
    glPopMatrix();
    aux += 0.5;
  }

//------------------------------------------------------------------------------------//
  // ANCLA
  glPushMatrix();
  glTranslatef(-6 * ancho, -3.5 * alto, ancho * 0.75);
  glScalef(2, 2, 2);
  color = color_select[23];
  ancla_barco.draw(SELECT, color.r, color.g, color.b, 1,vector_objetos_barco[9], vector_objetos_barco[10], amplia_ancla);
  glPopMatrix();
  
  //glDrawBuffer(GL_FRONT);
}


void _barco::setPly(char *archivo, int num)
{
  float r, g, b, r2, g2, b2;
  r = 0.0f, g = 0.0f, b = 0.0f, r2 = 0.0f, g2 = 0.0f, b2 = 0.0f;
  archivo_barco = archivo;
  _objeto_ply nuevo;
  switch (num)
  {
  case 1:
    r, g, b, r2, g2, b2;
    break;

  case 2:
    // 108, 52, 131
    // f = b / 255.0
    r = 149 / 256.0, g = 83 / 256.0, b = 0 / 256.0, r2 = 100 / 256.0, g2 = 60 / 256.0, b2 = 8 / 256.0;
    break;

  case 3:
    r = 62 / 256.0, g = 52 / 256.0, b = 39 / 256.0, r2 = 91 / 256.0, g2 = 63 / 256.0, b2 = 28 / 256.0;
    break;

  case 4:
    r = 0.472, g = 0.338, b = 0.263, r2 = 0.379, g2 = 0.278, b2 = 0.241;
    break;

  case 5:
    r = 108 / 256.0, g = 61 / 256.0, b = 1 / 256.0, r2 = 65 / 256.0, g2 = 40 / 256.0, b2 = 7 / 256.0;
    break;

  case 6:
    r = 183 / 256.0, g = 152 / 256.0, b = 112 / 256.0, r2 = 194 / 256.0, g2 = 171 / 256.0, b2 = 143 / 256.0;
    break;

  case 7:
    r = 230 / 256.0, g = 196 / 256.0, b = 152 / 256.0, r2 = 225 / 256.0, g2 = 174 / 256.0, b2 = 108 / 256.0;
    break;

  case 8:
    r = 62 / 256.0, g = 52 / 256.0, b = 39 / 256.0, r2 = 91 / 256.0, g2 = 63 / 256.0, b2 = 28 / 256.0;
    break;

  case 9:
    r = 0.0, g = 0.0, b = 0.0, r2 = 0.15, g2 = 0.15, b2 = 0.15;
    break;

  case 10:
    r = 0.0, g = 0.0, b = 0.0, r2 = 0.1, g2 = 0.1, b2 = 0.1;
    break;
  }
  // f = b / 255.0
  nuevo.parametros(archivo_barco, r, g, b, r2, g2, b2, num);
  vector_objetos_barco.push_back(nuevo);
}

_texturas::_texturas()
{
}

void _texturas::draw(GLuint textura_id, GLuint textura_id2, GLuint textura_id3, GLuint textura_id4, GLuint textura_id5, GLuint textura_id6, GLuint textura_id7, GLuint textura_id8)

{
  float tam = 0.5;
  float al = -1;
  // OBJETO A DIBUJAR
  GLfloat vertices[] = {
      // CARA FRONTAL
      0,
      0,
      1,
      1,
      0,
      1,
      1,
      1,
      1,
      0,
      1,
      1,
      // CARA DERECHA
      1,
      0,
      1,
      1,
      0,
      0,
      1,
      1,
      0,
      1,
      1,
      1,
      // CARA ARRIBA
      1,
      1,
      1,
      0,
      1,
      1,
      0,
      1,
      0,
      1,
      1,
      0,
      // CARA ABAJO
      0,
      0,
      1,
      1,
      0,
      1,
      1,
      0,
      0,
      0,
      0,
      0,
      // CARA IZQUIERDA
      0,
      0,
      1,
      0,
      0,
      0,
      0,
      1,
      0,
      0,
      1,
      1,
      // CARA DETRAS
      1,
      0,
      0,
      0,
      0,
      0,
      0,
      1,
      0,
      1,
      1,
      0,
  };
  GLfloat texVertices[] = {
      0.5, 0.33, 0.75, 0.33, 0.75, 0.66, 0.5, 0.66,
      0.75, 0.33, 0.75, 0.66, 1.0, 0.66, 1, 0.33,
      0.5, 0.33, 0.75, 0.33, 0.75, 0, 0.5, 0,
      0.5, 0.66, 0.75, 0.66, 0.75, 1, 0.5, 1,
      0.5, 0.66, 0.25, 0.66, 0.25, 0.33, 0.5, 0.33,
      0.0, 0.33, 0.25, 0.33, 0.23, 0.66, 0.0, 0.66

  };

  // OBJETO A DIBUJAR
  GLfloat vertices3[] = {
      // CARA FRONTAL
      0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0

  };
  GLfloat texVertices3[] = {
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0

  };
  GLfloat vertices4[] = {
      -tam, 0.0, tam,
      tam, 0.0, tam,
      0, al, 0,
      tam, 0.0, tam,
      tam, 0.0, -tam,
      0, al, 0,
      tam, 0.0, -tam,
      -tam, 0.0, -tam,
      0, al, 0,
      -tam, 0.0, -tam,
      -tam, 0.0, tam,
      0, al, 0};

  GLfloat texVertices4[] = {
      0.0,
      1.0,
      1.0,
      1.0,
      0.5,
      0.0,
      0.0,
      1.0,
      1.0,
      1.0,
      0.5,
      0.0,
      0.0,
      1.0,
      1.0,
      1.0,
      0.5,
      0.0,
      0.0,
      1.0,
      1.0,
      1.0,
      0.5,
      0.0,
  };

  GLfloat vertices5[] = {
      // CARA FRONTAL
      0,
      0,
      1,
      1,
      0,
      1,
      1,
      1,
      1,
      0,
      1,
      1,
      // CARA DERECHA
      1,
      0,
      1,
      1,
      0,
      0,
      1,
      1,
      0,
      1,
      1,
      1,
      // CARA ARRIBA
      1,
      1,
      1,
      0,
      1,
      1,
      0,
      1,
      0,
      1,
      1,
      0,
      // CARA ABAJO
      0,
      0,
      1,
      1,
      0,
      1,
      1,
      0,
      0,
      0,
      0,
      0,
      // CARA IZQUIERDA
      0,
      0,
      1,
      0,
      0,
      0,
      0,
      1,
      0,
      0,
      1,
      1,
      // CARA DETRAS
      1,
      0,
      0,
      0,
      0,
      0,
      0,
      1,
      0,
      1,
      1,
      0,
  };
  GLfloat texVertices5[] = {
      0.5, 0.33, 0.75, 0.33, 0.75, 0.66, 0.5, 0.66,
      0.75, 0.33, 0.75, 0.66, 1.0, 0.66, 1, 0.33,
      0.5, 0.33, 0.75, 0.33, 0.75, 0, 0.5, 0,
      0.5, 0.66, 0.75, 0.66, 0.75, 1, 0.5, 1,
      0.5, 0.66, 0.25, 0.66, 0.25, 0.33, 0.5, 0.33,
      0.0, 0.33, 0.25, 0.33, 0.23, 0.66, 0.0, 0.66

  };

  float latitud = 0.0, longitud = 0.0, radio = 24;
  glColor3f(1, 1, 1);
  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, textura_id2);

  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
  glDrawArrays(GL_QUADS, 0, 24);

  glBindTexture(GL_TEXTURE_2D, textura_id2);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
  glPushMatrix();
  glTranslatef(-1, 0, -1.2);
  glRotatef(90, 0, 0, 1);
  glRotatef(45, 1, 0, 0);
  glDrawArrays(GL_QUADS, 0, 24);
  glPopMatrix();

  glBindTexture(GL_TEXTURE_2D, textura_id3);
  glVertexPointer(3, GL_FLOAT, 0, vertices3);
  glTexCoordPointer(2, GL_FLOAT, 0, texVertices3);
  glPushMatrix();
  glScalef(10, 10, 10);
  glTranslatef(-0.5, 0, -0.5);

  glDrawArrays(GL_QUADS, 0, 4);
  glPopMatrix();
  float aux = -3.25, aux2 = -3.25;
  GLuint auxiliar = textura_id6;
  for (int i = 0; i < 4; i++)
  {
    if (i == 1)
    {
      auxiliar = textura_id8;
    }
    else if (i == 2)
    {
      auxiliar = textura_id5;
    }
    else if (i == 3)
    {
      auxiliar = textura_id7;
    }
    glBindTexture(GL_TEXTURE_2D, auxiliar);

    glVertexPointer(3, GL_FLOAT, 0, vertices4);
    glTexCoordPointer(2, GL_FLOAT, 0, texVertices4);
    glPushMatrix();
    glTranslatef(0, 1.5, 0);
    glTranslatef(aux, 0, aux2);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, auxiliar);
    glVertexPointer(3, GL_FLOAT, 0, vertices5);
    glTexCoordPointer(2, GL_FLOAT, 0, texVertices5);
    glPushMatrix();
    glTranslatef(-0.5, -0.01, -0.5);
    glTranslatef(aux, 0, aux2);
    glDrawArrays(GL_QUADS, 0, 24);
    glPopMatrix();
    aux *= -1;
    if (i == 1)
    {
      aux2 *= -1;
    }
  }

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
}