.PHONY: x clean tar

EJECUTABLE= practica_objetos_B3

MODULOS:= practica_objetos_B3.o\
         objetos_B3.o\
         file_ply_stl.o\
      
LIBS:= -L/usr/X11R6/lib/ -lX11 -lglut -lGLU -lGL -lsupc++ -lm -lc -lstdc++ -lpthread -L/usr/lib/nvidia-331
CPPFLAGS:= -Wall -fmax-errors=2 -g

CC= g++

x: $(EJECUTABLE)
	./$(EJECUTABLE) ./ply/beethoven ./ply/sirena ./ply/escudo2 ./ply/remo ./ply/timon ./ply/barco ./ply/mastil ./ply/vela ./ply/vikingoPersona ./ply/timon_abajo ./ply/cadena_ancla ./ply/ancla ./ply/bandera

$(EJECUTABLE): $(MODULOS)	
	$(CC) -o $(EJECUTABLE) $(LDFLAGS) $(MODULOS) $(LIBS) 

clean:
	rm -rf *.o $(EJECUTABLE)

