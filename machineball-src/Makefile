all : machineball

machineball : main.o window.o intro.o menu.o game.o text.o timer.o controls.o ball.o glode.o camera.o render.o court.o machine.o powerup.o explosion.o models.o mbdata.o options.o mbdata.o
	gcc main.o window.o intro.o menu.o game.o text.o timer.o controls.o ball.o glode.o camera.o render.o court.o machine.o powerup.o explosion.o models.o mbdata.o options.o -o machineball -laldmb -ldumb -lagl `allegro-config --libs` -lGL -lGLU -lode

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

mbdata.o : mbdata.s

mbdata.s : mbdata.dat
	dat2s mbdata.dat -o mbdata.s -h mbdata.h -p mb

clean :
	rm machineball main.o window.o intro.o menu.o game.o text.o timer.o controls.o ball.o glode.o camera.o render.o court.o machine.o powerup.o explosion.o models.o options.o mbdata.o

