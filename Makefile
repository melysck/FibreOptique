all: ChaineMain ReconstitueReseau timeCal testReorganiseReseau

ChaineMain: ChaineMain.o SVGwriter.o Chaine.o
	gcc -o ChaineMain ChaineMain.o SVGwriter.o Chaine.o -lm

ReconstitueReseau: ReconstitueReseau.o SVGwriter.o Chaine.o Reseau.o Hachage.o ArbreQuat.o
	gcc -Wall -o ReconstitueReseau ReconstitueReseau.o SVGwriter.o Chaine.o Reseau.o Hachage.o ArbreQuat.o -lm

timeCal: timeCal.o Reseau.o Hachage.o ArbreQuat.o 
	gcc -Wall -o timeCal timeCal.o Reseau.o Hachage.o ArbreQuat.o Chaine.o SVGwriter.o -lm

testReorganiseReseau: testReorganiseReseau.o Graphe.o Reseau.o Hachage.o ArbreQuat.o Chaine.o SVGwriter.o Struct_File.o
	gcc -Wall -o testReorganiseReseau testReorganiseReseau.o Graphe.o Reseau.o Hachage.o ArbreQuat.o Chaine.o SVGwriter.o Struct_File.o -lm

ChaineMain.o: ChaineMain.c SVGwriter.h Chaine.h
	gcc -Wall -c ChaineMain.c

ReconstitueReseau.o:  ReconstitueReseau.c SVGwriter.h Chaine.h Reseau.h
	gcc -Wall -c ReconstitueReseau.c

timeCal.o: timeCal.c Reseau.h Hachage.h ArbreQuat.h Chaine.h SVGwriter.h
	gcc -Wall -c timeCal.c 

testReorganiseReseau.o: testReorganiseReseau.c Graphe.h Reseau.h Hachage.h ArbreQuat.h Chaine.h SVGwriter.h Struct_File.h
	gcc -Wall -c testReorganiseReseau.c 

ArbreQuat.o: ArbreQuat.c ArbreQuat.h 
	gcc -Wall -c ArbreQuat.c 

Hachage.o: Hachage.c Hachage.h 
	gcc -Wall -c Hachage.c

Chaine.o: Chaine.c Chaine.h SVGwriter.h
	gcc -Wall -c Chaine.c

Reseau.o: Reseau.c Reseau.h Chaine.h 
	gcc -Wall -c Reseau.c

Graphe.o: Graphe.c Graphe.h Reseau.h Struct_File.c Struct_File.h
	gcc -c Graphe.c Struct_File.c

Struct_File.o: Struct_File.c Struct_File.h
	gcc -c Struct_File.c

clean:
	rm -f *.o ChaineMain ReconstitueReseau timeCal testReorganiseReseau
