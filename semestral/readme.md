Ako semestralnu pracu som si vybral hru Bomberman. 

Hra bude obsahovat zaciatocne menu, kde si uzivatel moze vybrat hru PvP s ovladanim pre oboch hracov na klavesnici, a SinglePlayer,
kde bude hrac hrat proti AI. Na zaciatocnom menu sa bude taktiez nachadzat kolonka Options, z ktorej sa budu dat konfigurovat nasledovne veci:
- Mapa ( z predconfigurovanych suborou )
- Pritomnost Baloons ( autonomne objekty, ktore sa nahodne pohybuju, pri zrazke s hracom vybuchuju a beru zivot, mozno nicia ja steny, daju sa znicit bombami )
- Mnozstvo znicitelnych stien
- Nahodnost dropov pre Buffs ( bonusy, tiez zo suboru )
- Rychlost vybuchu bomb, pocet odpalitelnych bomb za minutu
- vypnutie Debuffov
- nastavenie zivotov / hra bez zivotou ( na skore )
... co ma este pri implementacii napadne

Hru zacinaju obe strany v protilahlych rohoch mapy, ich cielom je znizit mnostvo protihracovych zivotou na 0 ( ak sa hra na zivoty ).
V pribliznom strede mapy sa v stenach budu vyskytovat Buffs, teda castejsie nez pri okrajovych stenach
Medzi buffs patria : 
- docastne/trvale vylepsenie bomb, celkovo su 3 leveli, cim vyssi level, tym vykonnejsia je odpalovana bomba.
- zvysenie/znizenie hracovej rychlosti
- docasna mozost prechadzat cez bomby, hracov a baloons ( cez baloons sa zivot neznizi, pri vybuchu bomby vsak ano )
- docasna moznost prechadzat cez vsetky steny ( nie ale objekty ako buff vyssie )
- docasna nesmrtelnost
- Holy Hand Granade, 1-razova bomba s nepredvidatelnymi nasledkami odpalu

Dedenie a polymorfizmus
V dokumentacii sa nachadza subor classStructure.png, z kadiel by malo byt jasne, ako dedenie bude fungovat.

GameObject bude reprezentovat vsetko co sa da zobrazit na mape, to jest napr. hrac, stena, bomba, buff..
Budu obsahovat premenne pozicia, viditelnost, rigidity ( da sa s objektom hybat ), mapova reprezentacia, getters, setters, metodu vykreslenia na mape..

Player bude reprezentovat hraca, AI a tiez autonomne objekty baloons, budu mat moveset, co je trieda na spracovanie pohybu po mape

Z Bomb budu delene rozne typy bomb a to iste plati aj pre triedu Buff.

Dalej bude existovat trieda Interface, ktora bude reprezentovat okna vykreslovane pomocou konzole, budu mat metody ako napr. 
vykresli, zavri a budu uchovavat sirku, vysku, farbu pozadia, velkost fontov ...
Z Interface bude dedena ScoreTable na vypisanie najlespich skor, tabulka options a zaciatocna obrazovka,
mozno aj obrazovka oznamujuca vitaza hry.

Vsetky objekty budu riadene cez Game triedu, ktora bude spustana cez main. Game trieda bude obsahovat zaciatocne menu, inicializaciu hry, main game loop
a bude sa starat o vsetky dynamicke alokacie a dealokacie objektov.

Graficke rozhranie:
Konzolovy vystup, mozno ncurses
