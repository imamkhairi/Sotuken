PRG = glut-0 glut-1 glut-2 RCcar
DIR = gl

all:	$(PRG)

glut-0:	glut-0.c
	cc glut-0.c -lglut -lGL -o glut-0

glut-1:	glut-1.c
	cc glut-1.c -lglut -lGLU -lGL -o glut-1

glut-2:	glut-2.c
	cc glut-2.c -lglut -lGLU -lGL -o glut-2

RCcar:	RCcar.c
	cc RCcar.c -lglut -lGLU -lGL -lm -o RCcar

clean:
	-rm $(PRG)

dist:	clean
	( cd ..; tar zcvf $(DIR).tgz $(DIR) )

