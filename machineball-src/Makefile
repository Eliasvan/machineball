# Set some custom CXXFLAGS

# Minimize probability of runtime errors
CXXFLAGS += -Wmaybe-uninitialized -Wunused-variable -Wunused-but-set-variable -Wunused-result -Wcomment

# Avoid a lot of needless Allegro 'warnings' emerge
CXXFLAGS += -Wno-deprecated-declarations


# 'make' will build "machineball"

all : machineball


machineball : mbdata.o main.o window.o intro.o menu.o game.o text.o timer.o controls.o ball.o glode.o camera.o render.o court.o machine.o powerup.o explosion.o models.o options.o
	g++ mbdata.o main.o window.o intro.o menu.o game.o text.o timer.o controls.o ball.o glode.o camera.o render.o court.o machine.o powerup.o explosion.o models.o options.o -o machineball -laldmb -ldumb -lalleggl `allegro-config --libs` -lGL -lGLU -lode -lm


# Machineball's code components

main.o : mbdata.h main.cpp

window.o : mbdata.h window.cpp

intro.o : mbdata.h intro.cpp

menu.o : mbdata.h menu.cpp

game.o : mbdata.h game.cpp

text.o : mbdata.h text.cpp

timer.o : timer.cpp

controls.o : controls.cpp

ball.o : ball.cpp

glode.o : glode.cpp

camera.o : camera.cpp

render.o : mbdata.h render.cpp

court.o : court.cpp

machine.o : mbdata.h machine.cpp

powerup.o : mbdata.h powerup.cpp

explosion.o : mbdata.h explosion.cpp

models.o : models.cpp

options.o : options.cpp


# Machineball's data components

mbdata.h : mbdata_c_h

mbdata.o : mbdata.c

mbdata.c : mbdata_c_h

mbdata_c_h : mbdata.dat
	dat2c mbdata.dat -o mbdata.c -h mbdata.h -p mb -g

mbdata.dat :
	dat mbdata.dat -f -c2 -a data/*


# 'make clean' will clean up the build-directory

clean :
	rm -rf machineball main.o window.o intro.o menu.o game.o text.o timer.o controls.o ball.o glode.o camera.o render.o court.o machine.o powerup.o explosion.o models.o options.o mbdata.o mbdata.c mbdata.h mbdata.dat
