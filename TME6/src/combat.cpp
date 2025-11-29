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
sigaction(SIGUSR1, &sa , NULL);
pr::mtrand(300,1000);

}

void combat(pid_t adversaire){

while(PV > 0){
    defence();
    attaque(adversaire);
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
combat(getppid());

}else{
//pere -> Vador
std::cout<<"Vador pret au combat.PID="<<getpid()<<std::endl;
combat(pid);

}







    return 0;
}