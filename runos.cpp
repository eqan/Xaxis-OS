#include<unistd.h>
#include<sys/wait.h>
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<pthread.h>
#include<stdlib.h>
#include"dictionary.h" //dict
// * Globals 
int disk=0,memory=0,cores=0;
std::string pwd="";
std::string userin;
dict* commands;
pthread_t last_thread_id;
int count=0; //Commands Count
// * -------

// * Function Prototypes
bool userLogin();
void readspecs();
void readCommands();
void* processCommand(void* args);
// * -------


int main(){
    readspecs();
    readCommands();
    while(!userLogin());
    
    while(1){
        std::cout<<"root@root-xaxis:";
        std::cin>>userin;
        if(pthread_create(&last_thread_id,NULL,processCommand,NULL)){
            std::cout<<"Thread Creation Failed, Couldn't Process Command.\n";
        }
        pthread_join(last_thread_id,NULL);
    }
}
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
        myFile_Handler>>commands[i].value;
    }
    myFile_Handler.close();
}
bool userLogin(){
    system("clear");
    std::string input;
    std::cout<<"\t\tXaxis OS\n";
    std::cout<<"User:Root\nPassword:";
    std::cin>>input;
    if(!pwd.compare(input)){
        std::cout<<"\n\tSuccessfully Logged In.\n";
        sleep(2);
        system("clear");
        return true;
    }
    return false;
}
void* processCommand(void* args){
    std::string arg;
    char* comm=new char[15];
    for(int i=0;userin[i]!=' ' || userin[i]!='\0';i++){
        if(userin[i]=='\0'){
            comm[i]='\0';
            std::string toCompare(comm);
            for(int j=0;j<count;j++){
                if(!toCompare.compare(commands[j].key)){
                    std::cout<<commands[j].value<<std::endl;
                    pthread_exit(NULL);
                }
            }
            system(comm);
            break;
        }
        comm[i]=userin[i];
    }
    pthread_exit(NULL);
}