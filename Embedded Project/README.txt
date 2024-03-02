Nume: Andronic Dragos Sebastian
Grupa: 336CC

Prin sender ma refer la placuta Arduino ce gestioneaza input-ul provenit de la butoane si player, iar prin receiver ma refer la placuta Arduino ce transmite informatii catre LCD-uri.

sender:

setup(): dezactivez rezistentele de pullup pentru butoane si configurez player-ul
loop(): in cazul in care un buton este apasat trimit input-ul pe interfata seriala, redau melodia din Player si modific volumul in functie de input-ul de la potentiometru.

receiver:
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8) - initializare obiect de tip u8g2 pentru interactiunea cu LCD1

U8G2_ST7920_128X64_F_SW_SPI u8g2_1(U8G2_R0, /* clock=*/ 12, /* data=*/ 7, /* CS=*/ 9, /* reset=*/ 8) - initializare obiect de tip u8g2 pentru interactiunea cu LCD2

initMessage() - afisare mesaje de inceput

setup() - initializare interfata seriala si obiectele pentru interactiunea cu LCD-urile

u8g2_prepare() si u8g21_prepare() - setare font si culoare pentru afisarea pe LCD-uri

populateMatrix(byte matrix[8][8]) - populeaza o matricea cu 1 si 0 (1 pentru navele de lupta)

generateGame() - genereaza cele doua tabele si le afiseaza pe LCD-uri

checkEnd(byte matrix[8][8]) - verifica toate navele au fost eliminate

checkPosition(byte matrix[8][8]) - verifica daca pozitia de atac este valida

generateSmallGame() - genereaza un joc cu mai putine campuri pentru testarea functionalitatilor

loop() - exista doua stari, cea de inceput unde afisez un mesaj si generez un nou joc doar daca a fost primit input de la cealalta placuta Arduino si starea a doua in care jucatorii aleg pozitia de atac, iar cand un jucator ramane fara nave de lupta se revine la starea initiala.

