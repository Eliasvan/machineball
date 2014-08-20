all : machineball

machineball : main.o window.o intro.o menu.o game.o text.o timer.o controls.o ball.o glode.o camera.o render.o court.o machine.o powerup.o explosion.o models.o mbdata.o options.o mbdata.o
	g++ main.o window.o intro.o menu.o game.o text.o timer.o controls.o ball.o glode.o camera.o render.o court.o machine.o powerup.o explosion.o models.o mbdata.o options.o -o machineball -laldmb -ldumb -lalleggl `allegro-config --libs` -lGL -lGLU -lode -lm

main.o : main.cpp

window.o : window.cpp

intro.o : intro.cpp

menu.o : menu.cpp

game.o : game.cpp

text.o : text.cpp

timer.o : timer.cpp

controls.o : controls.cpp

ball.o : ball.cpp

glode.o : glode.cpp

camera.o : camera.cpp

render.o : render.cpp

court.o : court.cpp

machine.o : machine.cpp

powerup.o : powerup.cpp

explosion.o : explosion.cpp

models.o : models.cpp

options.o : options.cpp

mbdata.o : mbdata.c

mbdata.c : mbdata.dat
	dat2c mbdata.dat -o mbdata.c -h mbdata.h -p mb -g

clean :
	rm machineball main.o window.o intro.o menu.o game.o text.o timer.o controls.o ball.o glode.o camera.o render.o court.o machine.o powerup.o explosion.o models.o options.o mbdata.o

