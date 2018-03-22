# set this variable to the director in which you saved the common files
commondir = common/

all : main_project

main_project : main_project.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
		gcc -Wall -o main_project -I$(commondir) -I common/Linux -DGL_GLEXT_PROTOTYPES main_project.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm


clean :
	rm main_project
