#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>
#include <cstdlib>

// Custom mystrdup : allocates with new[], copies string (avoid strdup and free)
char* mystrdup(const char* src);
//garder en memoire le pid du processus fils en cours d'execution
pid_t current_child =-1;

void sigint_handler(int sig){
    const char * msg = "\n[mini-shell] SIGINT recu \n";
write(STDOUT_FILENO , msg , strlen(msg));



    if(current_child >0){
        //envoyer SIGINT au fils en cours (tuer)
        kill(current_child, SIGINT);
    }
    //ne pas tuer le shell!
    const char* prompt = "mini-shell>";
    //strlen(taille de msg)
    //STDOUT_FILENO (1)--> sur l'ecran
    write(STDOUT_FILENO, prompt, strlen(prompt));
}

//voir si il ya zombie avec (top)dans le terminal
//diagnostiquer (des messages d'affichage)
volatile sig_atomic_t timeout;
int handler_alarm(int sig){
    timeout=1;
}
//handler==gestionnaire de signal
//Q11-2: avec sec
volatile sig_atomic_t childdead;
int handler_chld(int sig){//un fils est mort
    chlddead=1;
}

int wait_till_pid(pid_t pid , int sec)
int status;
pid_t r;
signal(SIGALRM,handler_alarm);//sig lorsque alarm(5); --> 5--> SIGALRM
signal(SIGCHLD,handler_chld);

childdead=0;
timeout=0;
alarm(sec);//lance le coumpte a rebours


while(1){

pause();//attendre un signal soit SIGCHLD soit SIGALRM

if(timeout){
    return 0;
}

if(childdead){



r=wait(&status);
if(r==-1){
    alarm(0);//annuler alarm
    return -1;
}
if(r==pid){
    alarm (0);
    return pid;
}
//sinon autre fils est mort continue a attendre
childdead=0;
}
}





//Q11-1:avec wait

int wait_till_pid(pid_t pid){
int status;
pid_t r;

while(1){

r=wait(&status);//attendre n'importe fils
if(r==-1){//plus de fils ou error
    return -1;
}
if(r==pid){//bo fils
    return pid;
}

//autre fils se terminer -> continue

}





}






















wait_till_pid(pid_t pid){

int status;

while(1){
    //attend un signal arrive notamment (SIGCHLD)
//endort jusqu'a un signal non ignore 
pause();

pid_t r=waitpid(pid, &status,NULL);

if(r== pid){
    return pid;
}
else if(r == -1){
    return -1;
}




}


}























int main() {

struct sigaction sa;
sa.sa_handler=sigint_handler;
sigemptyset(&sa.sa_mask);
sa.sa_flags=SA_RESTART;//pour eviter que getline soit interrompu
sigaction(SIGINT, &sa, NULL);


    std::string line;

    while (true) {
        std::cout << "mini-shell> " << std::flush;
        if (!std::getline(std::cin, line)) {
            std::cout << "\nExiting on EOF (Ctrl-D)." << std::endl;
            break;
        }

        if (line.empty()) continue;

        // Simple parsing: split by spaces using istringstream
        std::istringstream iss(line);
        std::vector<std::string> args;
        std::string token;
        while (iss >> token) {
            args.push_back(token);
        }
        if (args.empty()) continue;

        // Prepare C-style argv: allocate and mystrdup
        char** argv = new char*[args.size()];
        for (size_t i = 0; i < args.size(); ++i) {
            argv[i] = mystrdup(args[i].c_str());
        }
       
        // Your code for fork/exec/wait/signals goes here...

if(args.empty()) continue;

//phrase de shell diviser en tokens stocker dans le vecteur args
if(args[0]=="exit"){
    std::cout<<"bye bye \n";
    break;
}







pid_t pid=fork();

if(pid <0){
    perror("fork");
}
else if(pid ==0){
    //SIGINT (Ctrl-C)--> par defaut , tue le processus
    signal(SIGINT, SIG_DFL/*comportement par defaut de signal*/);//si je recois CTRL-C applique l'action : tuer le processus 
//execvp remplace l'image de processus par une commande
//remplace le processsus par la commande ( ls , pwd ...)
    execvp(argv[0],argv);
perror("execvp");
exit(1);

}
else{
//pere(le shell) --> son handler attrape ctrl-C et reste vivant
current_child=pid;//pid de fils sup a 0
int status;//donne comment fils tue (signal , normal ,stope...)
//on bit POSIX donne un macros pour lire ce status
pid_t res=waitpid(pid,& status, 0);
if(res <0){
    perror("waitpid");
}else{
// 0 n'est pas terminer normalement , non 0 terminer normalement
if(WIFEXITED(status)){
std::cout<<"[diagnostic] Fils termine normalement avec  "
<<WEXITSTATUS(status)<<std::endl;
}
else if(WIFSIGNALED(status)){
    std::cout<< "[diagnostic] Fils tue par le signal"
    <<WTERMSIG(status)<<std::endl;
}



}






//apres la fin du fils, plus de fils actif
current_child=-1;
}































        // cleanup argv allocations
        for (size_t i = 0; i < args.size(); ++i) {
            delete[] argv[i];
        }
        delete[] argv;
    }
    return 0;
}



char* mystrdup(const char* src) {
    if (src == nullptr) return nullptr;
    size_t len = strlen(src) + 1;  // +1 for null terminator
    char* dest = new char[len];
    memcpy(dest, src, len);  // Or strcpy if preferred
    return dest;
}


// Q10 -- dans le mini-shell , les commande externes (comme ls , sleep,echo ...)
//--> executees dans un processus fils grace a fork(), execpv(),waitpid()
//mais commande interne comme exit doit pas lancer un processus --> avant fork()

//shell(commande interne , externe):
//interne -->fct de shell lui meme pas dans le systeme de fichier
// cd , exit , export, history , alias , set ...
//interne car modifie l'etat interne de shell
//--> ces commande doit s'executer sans fork
//commande externes--> des programmes stocker dans /bin , /usr/bin
//ls , echo, sleep, cat , grep
//shell --> cree fils (fork , execvp , waitpid)


//1.3) pseudo wait --> utilise un signal pour s'informer de la fin de processus fils 






