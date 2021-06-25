#include<unistd.h>
#include<sys/wait.h>
#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<vector>
#include<pthread.h>
#include<stdlib.h>
#include<algorithm>
#include<fstream>
#include"dictionary.h" //dict
// * Globals 
int disk=0,memory=0,cores=0;
std::string pwd="";
std::string userin;
dict* commands;
pthread_t last_thread_id;
int count=0; //Commands Count
std::vector<int>processes;
// * -------

// * Function Prototypes
bool userLogin();
void readspecs();
void readCommands();
void* processCommand(void* args);
void creatingProcess(char* arg);
// * -------


int main(){
    readspecs();
    readCommands();
    while(!userLogin());
    std::cin.ignore();
    
    while(1){
        std::cout<<"root@root-xaxis:";
        std::cin.ignore(-1);
        std::getline(std::cin,userin);
        if(pthread_create(&last_thread_id,NULL,processCommand,NULL)){
            std::cout<<"Thread Creation Failed, Couldn't Process Command.\n";
        }
        pthread_join(last_thread_id,NULL);
    }
}

/*

*/


void readspecs(){
    std::ifstream myFile_Handler;
    myFile_Handler.open("osspecs");
    if(myFile_Handler.is_open()){
        std::getline(myFile_Handler,pwd);
        disk = stoi(pwd);
        std::getline(myFile_Handler,pwd);
        memory = stoi(pwd);
        std::getline(myFile_Handler,pwd);
        cores = stoi(pwd);
        std::getline(myFile_Handler,pwd);
    myFile_Handler.close();
    }
    else{
        std::cout << "Unable to open the file!";
    }
}

/*

*/


void readCommands(){
    std::string comm_tmp;
    std::ifstream myFile_Handler;
    myFile_Handler.open("commands");
    //Count Commands
    while(getline(myFile_Handler,comm_tmp)){
        count++;
    }
    //------
    commands=new dict[count];
    myFile_Handler.close();
    myFile_Handler.open("commands");
    for(int i=0;i<count;i++){
        myFile_Handler>>commands[i].key;
        if(i==0){
            myFile_Handler>>commands[i].value;
            myFile_Handler>>comm_tmp;
            commands[i].value+=" ";
            commands[i].value+=comm_tmp;
        }else{
            myFile_Handler>>commands[i].value;
        }
    }
    myFile_Handler.close();
}

/*

*/


bool userLogin(){
    system("clear");
    std::string input;
    std::cout<<"\t\tXaxis OS\n";
    std::cout<<"User:Root\nPassword:";
    std::cin>>input;
    if(!pwd.compare(input)){
        std::cout<<"\n\tSuccessfully Logged In.\n";
        //sleep(2);
        system("clear");
        return true;
    }
    return false;
}


/*

*/


void* processCommand(void* args){
    std::string arg;
    char* comm=new char[25];

    int i=0;
    int k=0;
    for(i;userin[i]!=' ' || userin[i]!='\0';i++){
        if(userin[i]=='\0'){ //Command With No Argument
            comm[i]='\0';
            std::string toCompare(comm);
            for(int j=0;j<count;j++){
                if(!toCompare.compare(commands[j].key)){
                    if(commands[j].value[0]=='c' && commands[j].value[1]=='d'){
                        chdir(&commands[j].value[3]);
                    }else{
                        system(commands[j].value.c_str());
                    }
                    pthread_exit(NULL);
                }
            }
            std::cout<<userin<<": is not a command in here..\n";
            pthread_exit(NULL);
        }
        if(userin[i]==' ')break;
        comm[i]=userin[i];
    }
    //----
    comm[i++]='\0';
    std::string toCompare(comm);
    for(int j=0;j<count;j++){
        if(!toCompare.compare(commands[j].key)){
            for(k=0;k<commands[j].value.length();k++){
                comm[k]=commands[j].value[k];
            }
            if(commands[j].key.compare("run")){
                comm[k++]=' ';
            }
            for(i;userin[i]!='\0';i++,k++){
                comm[k]=userin[i];
            }
            break;
        }
    }
    if(!k){std::cout<<userin<<": Try Rechecking The Command.\n";}
    else{
        if(comm[0]=='.'){
            std::cout<<"RUNNING NEW PROCESS:";
            creatingProcess(comm);
            return NULL; 
        }
        else{
            if(comm[0]=='c' && comm[1]=='d'){
                std::cout<<chdir(&comm[3]);
            }else{
                system(comm);
            }
        }
    }
    pthread_exit(NULL);
}

/*

*/

void creatingProcess(char* arg){
    int pid = fork();
    if(pid == 0){
        system(arg);
        std::vector<int>::iterator forRemoval;
        forRemoval = remove(processes.begin(),processes.end(),getpid());
        for(int i=0;i<processes.size(); i++){
            std::cout << processes[i] << " ";
        }
        exit(0);
    }
    else{
        processes.push_back(pid);
        for(int i=0;i<processes.size(); i++){
            std::cout << processes[i] << " ";
        }
    }
    exit(0);
    return;
}