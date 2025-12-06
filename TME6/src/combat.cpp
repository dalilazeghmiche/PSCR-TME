#include <iostream>
#include <csignal>
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include"util/rsleep.h"
#include"util/mtrand.h"
#include<sys/types.h>
volatile sig_atomic_t PV=3;

void handler(int sig){

PV= PV-1;
}


void handler_pare(int sig){
std::cout<<"coup pare!"<<std::endl;

}

void attaque(pid_t adversaire){

    struct sigaction sa;
    sa.sa_handler=handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=0;
    //sigaction defini ce que a faire quand un signal arrive
    sigaction(SIGUSR1 , &sa , NULL);

//kill envoie un signal a un processus ou groupe de processus
//renvoie -1 echec si 0 reussi 
if(kill(adversaire,SIGUSR1)==-1){
    std::cout<< "[Victoire] Mon adversaire est deja mort.\n";
    exit(0);
}
pr::mtrand(300,1000);//0.3 a 1s

}

void defence(){
    struct sigaction sa;
    sa.sa_handler=SIG_IGN;//ignorer
sigemptyset(&sa.sa_mask);
sa.sa_flags=0;
sigaction(SIGUSR1, &sa , NULL); //si signal SIGUSR1 arrive alors action c'est ignorer avec (handler:SIG_IGN)
pr::mtrand(300,1000);

}
void defence_Luke(){
    //1)installer un handler qui affiche coup pare
struct sigaction sa;
sa.sa_handler=handler_pare;
sigemptyset(&sa.sa_mask);
sa.sa_flags=0;
sigaction(SIGUSR1, &sa,NULL);
//2)bloquer SIGUSR1
sigset_t mask, oldmask;//mask --> liste de signaux bloquer --oldmask--> liste de signaux etait bloquer avant pour restaurer apres
sigemptyset(&mask); //initialise mask a vide
sigaddset(&mask,SIGUSR1);//ajout -> mask={SIGUSR1} <-- liste des signaux que Luke va bloquer dans cette phase
sigprocmask(SIG_BLOCK, &mask,&oldmask);//bloquer SIGUSR1 lorsque randsleep, pour eviter qu'un coup touche Luke pendant qu'il dort

//3) attendre une duree aleatoire
pr::randsleep();

//4)sigsuspend -> debloque temporairement SIGUSR1
sigset_t suspend_mask;
sigemptyset(&suspend_mask);
//masque vide = aucun signal bloque pendant sigsuspend
sigsuspend(&suspend_mask);



//5)restaurer le masque precedent
sigpromask(SIG_SETMASK, &oldmask,NULL);

}


void combat_Luke(pid_t pere){

while(PV > 0){
    defence_Luke();
    attaque(pere);
    std::cout<< "PV restants:"<<PV<< std::endl;
}
std::cout<<"[Defaite] Je n'ai plus de PV...\n";
exit(1);
}


void combat_Vador(pid_t fils){

while(PV > 0){
//waitpid > 0 (pid de fils) mort si ==0 fils en vie si -1 error
if(waitpid(fils,NULL,WNOHANG)>0){
    std::cout<<"[Victoire] Luke est mort"<<std::endl;
    exit(0);

}



    defence();//vador ignore les coups
    attaque(fils);// vador attaque
    std::cout<< "PV restants:"<<PV<< std::endl;
}
std::cout<<"[Defaite] Je n'ai plus de PV...\n";
exit(1);
}


















int main() {
    
pid_t pid= fork();

if(pid == 0){
//fils -> Luke
std::cout<< "Luke pret au combat . PID="<<getpid()<<std::endl;
combat_Luke(getppid());

}else{
//pere -> Vador
std::cout<<"Vador pret au combat.PID="<<getpid()<<std::endl;
combat_Vador(pid);




}







    return 0;
}







//Q3:
//un processus devient un zombie lorsque quand il se termine mais son pere ne recupere pas son code de retour avec wait ou waitpid
//quand processus se termine kernel garde une petite structure sont pid , exit , qlq infos
//tq le pere n'a pas fait wait or waitpid kernel ne supprime pas completement l'entree de processus donc devient zombie
//zombie mort mais existe encore dans la table de processus 
//----
//wait non bloquant --> tu demande l'etat de fils mais tu veux pas attendre si le fils n'est pas encore mort
//wait(null)-> bloquant :si le fils n'est pas encore mort le pere reste bloque et attend 
//waitpid(-1,NULL,WNOHANG)
//decliner faire variantes de meme idee

//sigpromask() --> controler quelles signaux bloquer temporairement par le processus
//restaurer le masque(Vide) avec sigpromask -->remettre le masque des signaux a sont etat normal --> debloquer tt les signaux bloquee




