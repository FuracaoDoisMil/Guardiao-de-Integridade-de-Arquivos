#include <iostream>      
#include <string>        
#include <fstream>       
#include <unordered_map> 
#include <filesystem>    
#include <sstream>       
#include <iomanip>       
#include <openssl/sha.h> 

std::string calcularHash(std::string diretorio){
    
}

void cadastrar(std::string diretorio){
    std::unordered_map<std::string, std::string> tebela_hash;

    
}

int main(int argc, char *argv[]){
    if (argc < 3)
    {
        std::cout<<"Utilize ./guardiao <operação> <diretorio>\n";
        return -1;
    }
    
    std::string operacao = argv[1];
    std::string diretorio = argv[2];

    if (operacao == "cadastrar")
    {
        cadastrar(diretorio);    
    }
    
}
