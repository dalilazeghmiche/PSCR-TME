#include "util/rsleep.h"
#include "util/mtrand.h"
#include <time.h>

void pr::randsleep() {
  struct timespec tosleep;//declarer var de type struct timespec structure definie dans time.h
  tosleep.tv_sec = 0;//secondes
  // 300 millions de ns = 0.3 secondes
  // 1 milliard de ns = 1 seconde
  tosleep.tv_nsec = pr::mtrand(300000000, 1000000000);//nanosecondes
  struct timespec remain;
  while (nanosleep(&tosleep, &remain) != 0) {//tosleep combien dormir , remain combien reste si un signal interrompe la pause
    tosleep = remain;//si une pause termine normalement nanosleep return 0 si non -1
  }//boucle : sommeil peut etre interrumpu par n'importe quel signal
}



/*Avantages par rapport à sleep() et usleep()

précision nanoseconde

indépendant de la modification de l’horloge système

compatible signaux sans redémarrer automatiquement

permet de reprendre un sleep interrompu*/