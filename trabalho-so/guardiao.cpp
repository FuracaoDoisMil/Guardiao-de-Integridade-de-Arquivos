#include <iostream>      
#include <string>        
#include <fstream>       
#include <unordered_map> 
#include <filesystem>    
#include <sstream>       
#include <iomanip>       
#include <openssl/evp.h> 

std::string calcularHash(std::string diretorio){
    std::ifstream arquivo(diretorio, std::ios::binary);
    if (!arquivo.is_open())
    {
        std::cout<<"O arquivo do diretorio: "<<diretorio<<" Não foi aberto com sucesso!\n";
    }
    
    char buffer[4096];

    EVP_MD_CTX* contexto = EVP_MD_CTX_new();

    EVP_DigestInit_ex(contexto, EVP_sha256(), nullptr);
    
    while (arquivo.read(buffer, sizeof(buffer)) || arquivo.gcount() > 0)
    {
        EVP_DigestUpdate(contexto, buffer, arquivo.gcount());
    }
    
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int tamanhoHash;

    EVP_DigestFinal_ex(contexto, hash, &tamanhoHash);

    EVP_MD_CTX_free(contexto);


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
