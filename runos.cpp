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
pthread_mutex_t sync_Process_io;
long mem_usage = 0;
// * -------

// * Function Prototypes
bool userLogin();
void readspecs();
void readCommands();
void* processCommand(void* args);
void creatingProcess(char* arg);
void writeProcesses();
void readProcesses();
void* keepProcessesUpdated(void* args);
void tot_mem_usage();
// * -------

int main(){
    pthread_mutex_init(&sync_Process_io,NULL);
    readspecs();
    readCommands();
    writeProcesses();
    if(pthread_create(&last_thread_id,NULL,keepProcessesUpdated,NULL)){
        std::cout<<"Process Log Init Failed.\nShutting Down Xaxis.\n";
        exit(0);
    }
    //while(!userLogin());
    //std::cin.ignore();
    while(1){
        std::cout<<"root@root-xaxis : ";
        std::cin.clear();
        std::getline(std::cin,userin);
        if(userin == "memusage"){
            std::cout<<"MEMORY IN USE : "<< mem_usage << " KBs"<<std::endl;
            continue;
        }    
        if(pthread_create(&last_thread_id,NULL,processCommand,NULL)){
            std::cout<<"Thread Creation Failed, Couldn't Process Command.\n";
        }
        pthread_join(last_thread_id,NULL);
        usleep(10000);
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
        if(i==0 || i==1){
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
            //std::cout<<"RUNNING NEW PROCESS: "<<std::endl;
            creatingProcess(comm);
            return NULL; 
        }
        else{
            if(comm[0]=='c' && comm[1]=='d'){
                chdir(&comm[3]);
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
        readProcesses();
        for(int i=0;i<processes.size();i++){
            if(processes[i]==getpid()){
                processes.erase(processes.begin()+i);
                break;
            }   
        }
        writeProcesses();
        exit(0);
    }
    else{
        processes.push_back(pid);
        writeProcesses();
    }
    return;
}

/*

*/

void writeProcesses(){
    pthread_mutex_lock(&sync_Process_io);
    std::ofstream writer;
    writer.open("/home/winepine/Desktop/oslabs/finalproject/processes",std::ofstream::trunc);
    for(int i=0;i<processes.size(); i++){
        writer << processes[i] << std::endl;
    }
    writer.close();
    std::string req;
    system("echo -n "" > /home/winepine/Desktop/oslabs/finalproject/processes_mem");
    for(int i=0;i<processes.size(); i++){
        req="/home/winepine/Desktop/oslabs/finalproject/./memusage.sh ";
        req+=std::to_string(processes[i]);
        system(&req[0]);
    }
    pthread_mutex_unlock(&sync_Process_io);
    return;
}

/*

*/


void readProcesses(){ 
    pthread_mutex_lock(&sync_Process_io);
    processes.clear();
    int tmp;
    std::ifstream reader;
    reader.open("/home/winepine/Desktop/oslabs/finalproject/processes");
    while(reader>>tmp){
        processes.push_back(tmp);
    }
    reader.close();
    pthread_mutex_unlock(&sync_Process_io);
    return;
}


/*

*/


void* keepProcessesUpdated(void* args){
    while(1){
        readProcesses();
        tot_mem_usage();
        usleep(10000);
    }
}

/*

*/


void tot_mem_usage(){
    mem_usage=0;
    std::string tmp;
    std::ifstream reader;
    reader.open("/home/winepine/Desktop/oslabs/finalproject/processes_mem");
    while(reader>>tmp){
        tmp[tmp.size()-1]='\0';
        mem_usage+=stoi(tmp);
    }
    reader.close();
}