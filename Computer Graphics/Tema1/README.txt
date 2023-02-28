Nume: Andronic Dragos Sebastian
Grupa: 336CC

Fisiere:
Tema1.cpp si Tema1.h - randez modele si se deruleaza jocul
Tema este conceputa ca un automat pe stari. Dupa ce initializez
modelele si le adaug in lista de Mesh-uri in functia Update
declar matricele model pentru fiecare Mesh. 

In starea 1 maresc pasul de translatie, reinitializez numarul
de gloante, recalculez unghiul initial si aleg o pozitie
de translatie pe axa X de unde va pleca rata.

In starea 2 rata se va misca pe ecran dupa directia
aleasa random in constructor. Calculez si coordonatele
dreptunghiului in care incadrez rata pentru functionalitatea
de impuscare a ratei (ma folosesc de coordonatele centrului
si calculez coordonatele colturilor, implementat in OnMouseBtnPress)
Daca numarul de gloante este 0 sau timpul alocat fiecarei
runde este scurs va trece in starea 3, iar daca rata
este lovita va trece in stara 4.
Rata este lovita in cazul in care coordonatele cursorului
se incadreaza in coordonatele colturilor. Astfel va creste
scorul, altfel doar va scadea numarul de gloante.

In starea 3 rata se va duce inspre partea de sus a 
ecranului, iar in starea 4 se va duce in partea de jos
Ambele stari se vor duce in starea 5 in care reinitializez
starea de lovit a ratei si pozitia sa in window, apoi ma reintorc
in starea 1.

Miscarea ratei se realizeaza dupa o directie dupa care translatez si
rotesc dupa un unghi. Aripile le rotesc dupa valorile a doi vectori pe
care ii inversez. 

Modelele sunt randate dupa calcularea si actualizarea matricelor model si
a unor variabile (numar_gloante, numar_vieti, scor, pozitie).

glText.h - header in care sunt create functionalitati pentru redarea unui text 
(preluat de pe github: https://github.com/vallentin/glText)
Utilizat doar pentru esteticitatea finalizarii jocului, nu reprezinta o functionalitate
bonus, ci doar pentru a semnala ca jocul este incheiat cu un anumit scor.

Transformari.h - functii pentru calculul translatiei, scalarii si rotatiei unei matrice
Preluate din laboratorul 3 pentru prelucrarea matricelor model (translatie, scalare si
rotatie). 

Duck.cpp si Duck.h - unde creez modelele utilizate in tema
Creez urmatoarele modele: corpul ratei (un triunghi => corp, un cerc => capul, 
un triunghi => ciocul), aripile ratei (doua triunghiuri), inima (un triunghi si 
doua semicercuri), glont (un dreptunghi si un semicerc), iarba (un dreptunghi),
scor si scorUnit (dreptunghi gol si dreptunghi plin).

Calcule.cpp si Calcule.h - calcule pentru directia si unghiul in urma reflexiei
In momentul in care rata se loveste de un perete voi recalcula vectorul de directie
prin formula directie_noua = directie_initiala - 2 * produs_scalar (normala_peretelui si directie_initiala) * normala_peretelui
Fiecare perete are o normala definita (de exemplu peretele din dreapta va avea 
vectorul normala (-1, 0, 0))

Pentru recalcularea unghiului am luat calculat dupa formula arccos(produs_scalar(directie_noua, normala_peretelui) / (lungime directie * lungime normala))


