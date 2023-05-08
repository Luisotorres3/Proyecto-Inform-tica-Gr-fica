//**************************************************************************
// Práctica 3
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include <chrono>
#include <thread>
#include <unistd.h>

#include "objetos_B3.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

const float AxisSize = 10000;
float tam = 0.5;
float al = -1;
// ----------------------------------------------------------------------
// NECESARIO PARA CARGAR LAS IMÁGENES DE DISCO
#include "CImg.h"
using namespace cimg_library;
// ----------------------------------------------------------------------

using namespace std;

// IDENTIFICADOR DE TEXTURA
GLuint textura_id, textura_id2, textura_id3, textura_id4, textura_id5, textura_id6, textura_id7, textura_id8;

// ----------------------------------------------------------------------
void prepara_textura(char *file, GLuint *tex_id)
{
     vector<unsigned char> data;
     CImg<unsigned char> image;

     image.load(file);

     // empaquetamos bien los datos
     for (long y = 0; y < image.height(); y++)
          for (long x = 0; x < image.width(); x++)
          {
               unsigned char *r = image.data(x, y, 0, 0);
               unsigned char *g = image.data(x, y, 0, 1);
               unsigned char *b = image.data(x, y, 0, 2);
               data.push_back(*r);
               data.push_back(*g);
               data.push_back(*b);
          }

     glGenTextures(1, tex_id);
     glBindTexture(GL_TEXTURE_2D, *tex_id);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

     // TRASFIERE LOS DATOS A GPU
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
                  0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
}

void libera_textura(GLuint *tex_id)
{
     glDeleteTextures(1, tex_id);
}
// tipos
typedef enum
{
     CUBO,
     PIRAMIDE,
     OBJETO_PLY,
     ROTACION,
     CILINDRO,
     CONO,
     ESFERA,
     EXTRUSION,
     EXCAVADORA,
     BARCO,
     TEXTURA
} _tipo_objeto;
_tipo_objeto t_objeto = OBJETO_PLY;
_modo modo = SOLID_FLAT;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x, Size_y, Front_plane, Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x = 50, Window_y = 50, Window_width = 900, Window_high = 900;

// objetos
_cubo cubo;
_piramide piramide(0.85, 1.3);
_objeto_ply ply;
_rotacion rotacion;
_cilindro cilindro(1, 2, 6);
_cono cono(1, 2, 6);
_esfera esfera(1, 20, 20);
_excavadora excavadora;
_extrusion *extrusion;

// PRACTICA 3
_barco barco;
float giro1 = 0, giro2 = 0;
int num = 0;
int pulsar = 0, flag2 = 0, aux = 0, auxiliar = 0, auxiliar2 = 0, auxiliar3 = 0, auxiliar4 = 0, auxiliar5 = 0;
bool reducir = false;
bool reducir_vela = false;
bool sigue = true;
bool fin = false;
bool aux2 = false;

// PRACTICA 4
float alfa = 0;
bool segunda_luz = false;
int pulsar2 = 0, pulsar7 = 0;
bool textura = false;

_texturas texturas;

// PRACTICA 5
int estadoRaton[3], xc, yc;
float factor=1.0;
int Ancho= Window_width;
int Alto= Window_high;
int camaraOrtogonal;
void vistaOrtogonal();



// _objeto_ply *ply;

//**************************************************************************
//
//***************************************************************************

void clean_window()
{

     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     // formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
     //  plano_delantero>0  plano_trasero>PlanoDelantero)
     glFrustum(-Size_x, Size_x, -Size_y, Size_y, Front_plane, Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

     // posicion del observador
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     glTranslatef(0, 0, -Observer_distance);
     glRotatef(Observer_angle_x, 1, 0, 0);
     glRotatef(Observer_angle_y, 0, 1, 0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{

     glDisable(GL_LIGHTING);
     glLineWidth(2);
     glBegin(GL_LINES);
     // eje X, color rojo
     glColor3f(1, 0, 0);
     glVertex3f(-AXIS_SIZE, 0, 0);
     glVertex3f(AXIS_SIZE, 0, 0);
     // eje Y, color verde
     glColor3f(0, 1, 0);
     glVertex3f(0, -AXIS_SIZE, 0);
     glVertex3f(0, AXIS_SIZE, 0);
     // eje Z, color azul
     glColor3f(0, 0, 1);
     glVertex3f(0, 0, -AXIS_SIZE);
     glVertex3f(0, 0, AXIS_SIZE);
     glEnd();
}

// LUCES
void luces(float alfa)
{

     GLfloat luz_ambiente1[] = {0.2, 0.2, 0.2, 1.0},
             luz_difusa1[] = {1, 1, 1, 1},
             luz_especular1[] = {0.8, 0.8, 0.8, 1},
             luz_posicion1[] = {0.0, 0.0, 40.0, 1.0};

     GLfloat luz_ambiente2[] = {0.30, 0.70, 0.0, 1.0},
             luz_difusa2[] = {1.0, 0.2, 0.2, 1},
             luz_especular2[] = {0.5, 0.1, 0.1, 1.0},
             luz_posicion2[] = {-20.0, 20.0, 0.0, 1.0};

     glLightfv(GL_LIGHT1, GL_AMBIENT, luz_ambiente1);
     glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_difusa1);
     glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular1);
     glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion1);

     glLightfv(GL_LIGHT2, GL_AMBIENT, luz_ambiente2);
     glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_difusa2);
     glLightfv(GL_LIGHT2, GL_SPECULAR, luz_especular2);

     glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_ambiente1);
     glLightfv(GL_LIGHT1, GL_SPECULAR, luz_ambiente1);
     glLightfv(GL_LIGHT1, GL_POSITION, luz_posicion1);

     glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_ambiente2);
     glLightfv(GL_LIGHT2, GL_SPECULAR, luz_ambiente2);

     glPushMatrix();

     glRotatef(alfa, 0, 1, 0);
     glLightfv(GL_LIGHT2, GL_POSITION, luz_posicion2);

     glPopMatrix();

     glDisable(GL_LIGHT0);
     glEnable(GL_LIGHT1);
     if (segunda_luz == true)
          glEnable(GL_LIGHT2);
     else
          glDisable(GL_LIGHT2);
}

//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects()
{

     switch (t_objeto)
     {
     case CUBO:
          cubo.draw(modo, 1.0, 0.0, 0.0, 5);
          break;
     case PIRAMIDE:
          piramide.draw(modo, 1.0, 0.0, 0.0, 5);
          break;
     case OBJETO_PLY:
          ply.draw(modo, 1.0, 0.6, 0.0, 5);
          break;
     case ROTACION:
          rotacion.draw(modo, 1.0, 0.0, 0.0, 5);
          break;
     case CILINDRO:
          cilindro.draw(modo, 1.0, 0.0, 0.0, 5);
          break;
     case CONO:
          cono.draw(modo, 1.0, 0.0, 0.0, 5);
          break;
     case ESFERA:
          esfera.draw(modo, 1.0, 0.0, 0.0, 5);
          break;
     case EXCAVADORA:
          excavadora.draw(modo, 1.0, 0.0, 0.0, 5);
          break;
     case EXTRUSION:
          extrusion->draw(modo, 1.0, 0.0, 0.0, 5);
          break;

     // PRACTICA 3
     case BARCO:
          barco.draw(modo, 1.0, 0.0, 0.0, 5);
          break;

     case TEXTURA:
          texturas.draw(textura_id, textura_id2, textura_id3, textura_id4, textura_id5, textura_id6, textura_id7, textura_id8);
          break;
     }
}

//**************************************************************************
//
//***************************************************************************

void draw(void)
{
     
     
     //glDrawBuffer(GL_FRONT);
     clean_window();
     // PRACTICA 4

     
     if(camaraOrtogonal==0){
          glViewport(0,0,Window_width,Window_high);
          change_projection();
          change_observer();
          // PRACTICA 4
          luces(alfa);
          draw_axis();
          draw_objects();
          

     }else{
          glViewport(0,0,Window_width,Window_high);
          vistaOrtogonal();
          luces(alfa);
          glutPostRedisplay();
     }
     glutSwapBuffers();
     if (t_objeto == BARCO)
     {
          glDrawBuffer(GL_BACK);
          clean_window();
          change_observer();
          barco.seleccion();
          
     }
     

     glFlush();
}

void vistaOrtogonal(){     
    

     //VISTA1

     glViewport(0, 0, Ancho/2, Alto/2);
     glMatrixMode(GL_PROJECTION); 
     glLoadIdentity();
     glOrtho(-5*factor,5*factor,-5*factor,5*factor,-100,100); 
     glRotatef(90,0,1,0);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     draw_axis();
     draw_objects();
     glFlush();
     //VISTA2     
     glViewport(0, Alto/2, Ancho/2, Alto/2);     
     glMatrixMode(GL_PROJECTION); 
     glLoadIdentity();
     glOrtho(-5*factor,5*factor,-5*factor,5*factor,-100,100);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     draw_axis();
     draw_objects();
     glFlush();

     //VISTA3
     glViewport(Ancho/2, 0, Ancho/2, Alto/2);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-5*factor,5*factor,-5*factor,5*factor,-100,100); 
     glRotatef(90,1,0,0);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     draw_axis();
     draw_objects();
     glFlush();

     //VISTA4    
     glViewport(Ancho/2, Alto/2, Ancho/2, Alto/2);
     glMatrixMode(GL_PROJECTION); 
     glLoadIdentity();
     glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
     change_projection();
     change_observer();
     draw_axis();
     draw_objects();

     glFlush();
}


void aumenta_vela()
{
     if (barco.cerrar_vela_aux != 0)
     {
          barco.cerrar_vela *= 1.25;
          barco.cerrar_vela2 += -barco.cerrar_vela / 1.25;
          barco.cerrar_vela_aux--;
          sigue = false;
     }
     else if (barco.cerrar_vela_aux == 0)
     {
          sigue = true;
     }
}

void aumenta_remo()
{
     if ((barco.giro_remo < barco.giro_remo_max) && (barco.sigue == true) && sigue == true && num < 2)
     {
          sigue = false;
          barco.giro_remo += 1;
     }
     else if (sigue == true)
     {
          if (num < 2)
          {
               sigue = false;
               if (barco.giro_remo > barco.giro_remo_min)
               {
                    barco.giro_remo -= 1;
                    barco.sigue = false;
               }
               else
               {
                    barco.giro_remo += 1;
                    barco.sigue = true;
                    num++;
               }
          }
          else if (num == 2)
          {
               sigue = true;
          }
     }
}

void aumenta_ancla()
{
     if (barco.amplia_ancla == barco.amplia_ancla_max)
     {

          barco.amplia_ancla += 0;
          sleep(2);
          reducir = true;
          sigue = true;
     }
     else if (num == 2 && reducir == false)
     {
          sigue = false;
          barco.amplia_ancla += 0.25;
     }
}
void reduce_ancla()
{

     if (barco.amplia_ancla == 0)
     {
          barco.amplia_ancla += 0;
          sigue = true;
          aux2 = true;
     }
     else if (reducir == true)
     {
          sigue = false;
          if (reducir == true)
               barco.amplia_ancla -= 0.25;
     }
}
void aumenta_timon()
{
     if (2 * barco.giro_timon_aspa_max == auxiliar5)
     {
          aux2 = false;
          sigue = true;
          barco.giro_timon -= 0;
          barco.giro_timon_aspa += 0;
          barco.giro_mastil -= 0;
     }
     else if (aux2 == true)
     {
          if (auxiliar5 < (barco.giro_timon_aspa_max) && auxiliar5 != -1)
          {
               sigue = false;
               auxiliar5++;
               barco.giro_timon -= 5;
               barco.giro_timon_aspa += 1;
               barco.giro_mastil -= 1;
          }
          else if (auxiliar5 >= (barco.giro_timon_aspa_max) && auxiliar5 != -1)
          {
               if (auxiliar5 == 2 * barco.giro_timon_aspa_max)
               {

                    auxiliar5 = -1;
                    sigue = true;
                    aux2 = false;
                    barco.giro_timon -= 0;
                    barco.giro_timon_aspa += 0;
                    barco.giro_mastil -= 0;
                    sleep(2);
               }
               else
               {
                    sigue = false;
                    auxiliar5++;
                    barco.giro_timon += 5;
                    barco.giro_timon_aspa -= 1;
                    barco.giro_mastil += 1;
               }
          }
     }
}

void aumenta_mastil()
{
     if (barco.giro_mastil == 25 * barco.giro_mastil_max)
     {
          barco.giro_mastil += 0;
          sigue = true;
          reducir_vela = true;
          fin = true;
     }
     else if (sigue == true)
     {

          if (auxiliar4 < (25 * barco.giro_mastil_max) / 2 && auxiliar4 != -1)
          {
               auxiliar4++;
               barco.giro_mastil++;
          }
          else if (auxiliar4 >= (25 * barco.giro_mastil_max) / 2 && auxiliar4 != -1)
          {
               if (auxiliar4 == 25 * barco.giro_mastil_max)
               {
                    barco.giro_mastil += 0;
                    auxiliar4 = -1;
                    sigue = true;
                    reducir_vela = true;
                    fin = true;
               }
               else
               {
                    auxiliar4++;
                    barco.giro_mastil--;
               }
          }
     }
}

bool gira_vista()
{

     if (auxiliar < 120)
     {
          Observer_distance *= 1.01;
          auxiliar++;
     }
     else
     {

          if (auxiliar2 < 400)
          {
               Observer_angle_y++;
               Observer_angle_x += 0.05;
               auxiliar2++;
          }
          else
          {
               return 0;
          }
     }
     return 1;

     return 0;
}
// PRACTICA 3
void function_idle()
{

     if (pulsar == 1)
     {
          if (gira_vista() == 0)
          {
               aumenta_vela();
               aumenta_remo();
               aumenta_ancla();
               reduce_ancla();
               aumenta_timon();
               aumenta_mastil();
          }
     }

     glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1, int Alto1)
{
     float Aspect_ratio;

     Aspect_ratio = (float)Alto1 / (float)Ancho1;
     Size_y = Size_x * Aspect_ratio;
     Ancho=Ancho1;
     Alto=Alto1;
     change_projection();
     glViewport(0, 0, Ancho1, Alto1);
     glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1, int x, int y)
{
     switch (toupper(Tecla1))
     {
     case 'Q':
          exit(0);
     case '1':
          modo = POINTS;
          break;
     case '2':
          modo = EDGES;
          break;
     case '3':
          modo = SOLID;
          break;
     case '4':
          modo = SOLID_COLORS;
          break;

     // PRACTICA 4
     case '5':
          modo = SOLID_FLAT;
          break;
     case '6':
          modo = SOLID_SMOOTH;
          break;
     case '7':
          t_objeto = TEXTURA;
          break;

     case 'P':
          t_objeto = PIRAMIDE;
          break;
     case 'C':
          t_objeto = CUBO;
          break;
     case 'O':
          t_objeto = OBJETO_PLY;
          break;
     case 'R':
          t_objeto = ROTACION;
          break;
     case 'L':
          t_objeto = CILINDRO;
          break;
     case 'N':
          t_objeto = CONO;
          break;
     case 'E':
          t_objeto = ESFERA;
          break;
     case 'X':
          t_objeto = EXTRUSION;
          break;

     // PRACTICA 4
     case 'I':
          if (pulsar2 == 1)
          {
               segunda_luz = true;
               pulsar2 = 0;
          }
          else
          {
               segunda_luz = false;
               pulsar2 = 1;
          }
          break;

     // practica 4
     case 'J':
          alfa += 45;
          break;
     case 'K':
          alfa -= 45;
          break;

     // PRACTICA 5
     case 'G':
          camaraOrtogonal=0;
          break;
     case 'H':
          camaraOrtogonal=1;
          break;

     // PRACTICA 3
     case 'A':
          t_objeto = BARCO;
          break;

     case 'D':
          t_objeto = BARCO;
          if (pulsar == 1)
          {
               if (fin == true)
               {
                    Observer_distance = 4 * Front_plane;
                    Observer_angle_x = 0;
                    Observer_angle_y = 0;
                    auxiliar = 0, auxiliar2 = 0, auxiliar3 = 0;
                    reducir = false;
                    reducir_vela = false;
                    sigue = true;
                    barco.cerrar_vela = 0.0351844;
                    barco.cerrar_vela2 = 3.85926;
                    barco.cerrar_vela_aux = 15;
                    barco.giro_mastil = 0.0;
                    aux2 = false;
               }
               giro1 += 5;
               giro2 += 1;
               pulsar = 0;
          }
          else
          {
               Observer_distance = 4 * Front_plane;
               Observer_angle_x = 0;
               Observer_angle_y = 0;
               auxiliar = 0, auxiliar2 = 0, auxiliar3 = 0, auxiliar5 = 0, auxiliar4 = 0;
               reducir = false;
               reducir_vela = false;
               sigue = true;
               barco.cerrar_vela = 0.0351844;
               barco.cerrar_vela2 = 3.85926;
               barco.cerrar_vela_aux = 15;
               barco.giro_mastil = 0.0;
               aux2 = false;
               giro1 = 0;
               giro2 = 0;
               num = 0;
               flag2 = 0, aux = 0;

               pulsar = 1;
          }
          break;
     }
     glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1, int x, int y)
{

     switch (Tecla1)
     {
     case GLUT_KEY_LEFT:
          Observer_angle_y--;
          break;
     case GLUT_KEY_RIGHT:
          Observer_angle_y++;
          break;
     case GLUT_KEY_UP:
          Observer_angle_x--;
          break;
     case GLUT_KEY_DOWN:
          Observer_angle_x++;
          break;
     case GLUT_KEY_PAGE_UP:
          Observer_distance *= 1.2;
          break;
     case GLUT_KEY_PAGE_DOWN:
          Observer_distance /= 1.2;

          break;

     case GLUT_KEY_F1:
          if ((barco.giro_remo < barco.giro_remo_max) && (barco.sigue == true))
          {
               barco.giro_remo += 5;
               break;
          }
          else
          {
               if (barco.giro_remo > barco.giro_remo_min)
               {
                    barco.giro_remo -= 5;
                    barco.sigue = false;
                    break;
               }
               else
               {
                    barco.giro_remo += 5;
                    barco.sigue = true;
                    break;
               }
          }

     case GLUT_KEY_F5:
          if (barco.cerrar_vela_aux < 15)
          {
               barco.cerrar_vela /= 1.25;
               barco.cerrar_vela2 += barco.cerrar_vela;
               barco.cerrar_vela_aux++;
          }

          break;

     case GLUT_KEY_F6:
          if (barco.cerrar_vela_aux > 0)
          {
               barco.cerrar_vela *= 1.25;
               barco.cerrar_vela2 += -barco.cerrar_vela / 1.25;
               barco.cerrar_vela_aux--;
          }
          break;

     case GLUT_KEY_F7:
          if (barco.giro_timon_aspa_max > barco.giro_timon_aspa && barco.giro_mastil < barco.giro_mastil_max)
          {
               barco.giro_timon -= 5;
               barco.giro_timon_aspa += 1;
               barco.giro_mastil -= 1;
               break;
          }
          break;
     case GLUT_KEY_F8:
          if (-barco.giro_timon_aspa_max < barco.giro_timon_aspa)
          {
               barco.giro_timon += 5;
               barco.giro_timon_aspa -= 1;
               barco.giro_mastil += 1;
               break;
          }
          break;

     case GLUT_KEY_F9:
          barco.giro_mastil -= 1;
          break;

     case GLUT_KEY_F10:
          barco.giro_mastil += 1;
          break;

     case GLUT_KEY_F3:
          if (barco.amplia_ancla < barco.amplia_ancla_max)
          {
               barco.amplia_ancla++;
               break;
          }
          break;

     case GLUT_KEY_F4:
          if (barco.amplia_ancla > 0)
          {
               barco.amplia_ancla--;
               break;
          }
          break;
     }
     glutPostRedisplay();
}

//***************************************************************************
// Funciones para la selección por color
//***************************************************************************

void procesar_color(unsigned char color[3])
{
     int i;
     for (i = 0; i < barco.piezas; i++)
     {
          if (color[0] == barco.color_select[i].r &&
              color[1] == barco.color_select[i].g &&
              color[2] == barco.color_select[i].b)
          {
               if (barco.activo[i] == 0)
               {
                    barco.activo[i] = 1;
               }
               else
               {
                    barco.activo[i] = 0;
               }
               glutPostRedisplay();
          }
     }
}

//***************************************************************************

void pick_color(int x, int y)
{
     GLint viewport[4];
     unsigned char pixel[3];

     glViewport(0, 0, Ancho, Alto);
     glGetIntegerv(GL_VIEWPORT, viewport);
     glReadBuffer(GL_BACK);
     glReadPixels(x, viewport[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte *)&pixel[0]);
     printf(" valor x %d, valor y %d, color %d, %d, %d \n", x, y, pixel[0], pixel[1], pixel[2]);

     procesar_color(pixel);
}

//***************************************************************************
// Funciones para manejo de eventos del ratón
//***************************************************************************

void clickRaton(int boton, int estado, int x, int y)
{
     if (boton == GLUT_RIGHT_BUTTON)
     {
          if (estado == GLUT_DOWN)
          {
               if(camaraOrtogonal==0){
                    estadoRaton[2] = 1;
                    xc = x;
                    yc = y;
               }else{
                    estadoRaton[2] = 0; 
               }
          }
          else
               estadoRaton[2] = 1;
               // 1 o 0
     }
     if (boton == GLUT_LEFT_BUTTON)
     {
          if (estado == GLUT_DOWN)
          {
               estadoRaton[2] = 2;
               xc = x;
               yc = y;
               pick_color(xc, yc);
          }
     }

     if (boton == 3)
     {
          if (estado == GLUT_UP)
          {
               estadoRaton[0] = 3;
               if(camaraOrtogonal==0)
                    Observer_distance *= 0.9;
               factor*=0.9;
          }else{
               estadoRaton[0] = 3;
          }
     }

     if (boton == 4)
     {
          if (estado == GLUT_UP)
          {
               estadoRaton[0] = 4;
               factor*=1.1;
               if(camaraOrtogonal==0)
                    Observer_distance *= 1.1;
          }else{
               estadoRaton[0] = 3;
          }
     }
}

/*************************************************************************/

void RatonMovido(int x, int y)
{
     if (estadoRaton[2] == 1)
     {
          Observer_angle_y = Observer_angle_y - (x - xc);
          Observer_angle_x = Observer_angle_x + (y - yc);
          xc = x;
          yc = y;
          glutPostRedisplay();
     }
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
     prepara_textura("./img/q.tga", &textura_id);
     prepara_textura("./img/dado3.png", &textura_id2);
     prepara_textura("./img/parchis.png", &textura_id3);
     prepara_textura("./img/plastico.jpeg", &textura_id4);
     prepara_textura("./img/plastico.jpeg", &textura_id5);

     prepara_textura("./img/plasticoAzul.jpeg", &textura_id6);
     prepara_textura("./img/plasticoVerde.jpeg", &textura_id7);
     prepara_textura("./img/plasticoRojo.jpeg", &textura_id8);
     // se inicalizan la ventana y los planos de corte
     Size_x = 0.5;
     Size_y = 0.5;
     Front_plane = 0.8;
     Back_plane = 2000;

     // se incia la posicion del observador, en el eje z
     Observer_distance = 4 * Front_plane;
     Observer_angle_x = 0;
     Observer_angle_y = 0;

     // se indica el color para limpiar la ventana	(r,v,a,al)
     // blanco=(1,1,1,1) rojo=(1,0,0,1), negro=(0,0,0,0) ...
     glClearColor(1, 1, 1, 1);

     // se habilita el z-bufer
     glEnable(GL_DEPTH_TEST);
     change_projection();
     glViewport(0, 0, Window_width, Window_high);
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************
int main(int argc, char *argv[])
{

     // perfil

     vector<_vertex3f> perfil, poligono;
     _vertex3f aux;

     aux.x = 1.0;
     aux.y = -1.0;
     aux.z = 0.0;
     perfil.push_back(aux);
     aux.x = 1.5;
     aux.y = 0.0;
     aux.z = 0.0;
     perfil.push_back(aux);
     aux.x = 1.0;
     aux.y = 1.0;
     aux.z = 0.0;
     perfil.push_back(aux);
     aux.x = 1.5;
     aux.y = 1.2;
     aux.z = 0.0;
     perfil.push_back(aux);

     rotacion.parametros(perfil, 6, 0, 1, 1);

     aux.x = 1.0;
     aux.y = 0.0;
     aux.z = 1.0;
     poligono.push_back(aux);
     aux.x = 1.0;
     aux.y = 0.0;
     aux.z = -1.0;
     poligono.push_back(aux);
     aux.x = -1.2;
     aux.y = 0.0;
     aux.z = -1.0;
     poligono.push_back(aux);
     aux.x = -0.8;
     aux.y = 0.0;
     aux.z = 0.0;
     poligono.push_back(aux);
     aux.x = -1.2;
     aux.y = 0.0;
     aux.z = 1.0;
     poligono.push_back(aux);

     extrusion = new _extrusion(poligono, 0.25, 1.0, 0.25);

     // se llama a la inicialización de glut
     glutInit(&argc, argv);

     // se indica las caracteristicas que se desean para la visualización con OpenGL
     // Las posibilidades son:
     // GLUT_SIMPLE -> memoria de imagen simple
     // GLUT_DOUBLE -> memoria de imagen doble
     // GLUT_INDEX -> memoria de imagen con color indizado
     // GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
     // GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
     // GLUT_DEPTH -> memoria de profundidad o z-bufer
     // GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
     glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

     // posicion de la esquina inferior izquierdad de la ventana
     glutInitWindowPosition(Window_x, Window_y);

     // tamaño de la ventana (ancho y alto)
     glutInitWindowSize(Window_width, Window_high);

     // llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
     // al bucle de eventos)
     glutCreateWindow("PRACTICA - 5");

     // asignación de la funcion llamada "dibujar" al evento de dibujo
     glutDisplayFunc(draw);
     // asignación de la funcion llamada "change_window_size" al evento correspondiente
     glutReshapeFunc(change_window_size);
     // asignación de la funcion llamada "normal_key" al evento correspondiente
     glutKeyboardFunc(normal_key);
     // asignación de la funcion llamada "tecla_Especial" al evento correspondiente
     glutSpecialFunc(special_key);

     // PRACTICA 3
     // glutIdleFunc(function_idle);
     glutIdleFunc(function_idle);

     // funcion de inicialización
     initialize();

     // eventos ratón
     glutMouseFunc(clickRaton);
     glutMotionFunc(RatonMovido);

     // creación del objeto ply

     // remo sirena escudo2 remo timon barco mastil vela vikingoPersona
     ply.parametros(argv[1]);
     barco.setPly(argv[2], 0);
     barco.setPly(argv[3], 1);
     barco.setPly(argv[4], 2);
     barco.setPly(argv[5], 3);
     barco.setPly(argv[6], 4);
     barco.setPly(argv[7], 5);
     barco.setPly(argv[8], 6);
     barco.setPly(argv[9], 7);
     barco.setPly(argv[10], 8);
     barco.setPly(argv[11], 9);
     barco.setPly(argv[12], 10);
     barco.setPly(argv[13], 11);

     // ply = new _objeto_ply(argv[1]);

     cout << "--------------------------------------------" << endl;
     cout << "--------------------------------------------" << endl;
     cout << "FUNCIONALIDAD PRÁCTICA 3 - LUIS SOTO TORRES" << endl
          << endl;
     cout << "TECLA S - MOSTRAR EL BARCO" << endl;
     cout << "TECLA D - MOSTRAR LA ANIMACIÓN" << endl;
     cout << "TECLA F1 - MOVER LOS REMOS EN AMBOS SENTIDOS" << endl;
     cout << "TECLA F3 - BAJAR EL ANCLA" << endl;
     cout << "TECLA F4 - SUBIR EL ANCLA" << endl;
     cout << "TECLA F5 - PLEGAR LA VELA" << endl;
     cout << "TECLA F6 - AMPLIAR LA VELA" << endl;
     cout << "TECLA F7 - GIRAR EL TIMON A LA IZQUIERDA" << endl;
     cout << "TECLA F8 - GIRAR EL TIMON A LA DERECHA" << endl;
     cout << "TECLA F9 - GIRAR LA VELA A LA IZQUIERDA" << endl;
     cout << "TECLA F10 - GIRAR LA VELA A LA DERECHA" << endl;
     cout << "--------------------------------------------" << endl;
     // inicio del bucle de eventos
     glutMainLoop();
     libera_textura(&textura_id);
     libera_textura(&textura_id2);
     libera_textura(&textura_id3);
     libera_textura(&textura_id4);
     libera_textura(&textura_id5);
     libera_textura(&textura_id6);
     libera_textura(&textura_id7);
     libera_textura(&textura_id8);
     return 0;
}
