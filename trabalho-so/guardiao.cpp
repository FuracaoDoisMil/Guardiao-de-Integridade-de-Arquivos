#include <iostream>      
#include <string>        
#include <fstream>       
#include <unordered_map> 
#include <filesystem>    
#include <sstream>       
#include <iomanip>       
#include <openssl/evp.h> 


std::string calcularHash(std::string diretorio){
    //abre o arquivo do diretorio informado em formato binario
    std::ifstream arquivo(diretorio, std::ios::binary);
    if (!arquivo.is_open())
    {
        std::cout<<"O arquivo do diretorio: "<<diretorio<<" Não foi aberto com sucesso!\n";
        return "";
    }
    //buffer de leitura
    char buffer[4096];
    
    //cria a variavel para fazer o calculo de hash
    EVP_MD_CTX* contexto = EVP_MD_CTX_new();
    
    //inicia o calculo da hash definindo que vai usar o calculo sha256
    EVP_DigestInit_ex(contexto, EVP_sha256(), nullptr);
    
    //lê o arquivo em 4096 em 4096 bytes até a leitura chegar a zero
    while (arquivo.read(buffer, sizeof(buffer)) || arquivo.gcount() > 0)
    {
        //adiciona os dados lidos ao calculo da hash
        EVP_DigestUpdate(contexto, buffer, arquivo.gcount());
    }
    
    //variavel que vai armazenar os caracteres binarios da hash
    unsigned char hash[EVP_MAX_MD_SIZE];

    //variavel que vai guardar o tamanho da hash (que no caso é de 32bytes)
    unsigned int tamanhoHash;

    //finaliza e entrega o resultado do calculo da hash
    EVP_DigestFinal_ex(contexto, hash, &tamanhoHash);
    
    //libera a memoria que foi reservada para fazer o calculo
    EVP_MD_CTX_free(contexto);

    std::stringstream resultado;

    //laço para converter os caracteres de binario pra hexadecimal
    for (int i = 0; i < tamanhoHash; i++)
    {
        resultado << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    
    return resultado.str();

}

void cadastrar(std::string diretorio){
    //verifica se o diretorio existe
    if (!std::filesystem::exists(diretorio) || !std::filesystem::is_directory(diretorio))
    {
        std::cout<<"ERRO: O diretorio: "<< diretorio << " não existe\n\n";
        return;
    }
    
    //cria a tabela hash
    std::unordered_map<std::string, std::string> tabela_hash;

    //abre o arquivo "referencias.txt"
    std::ifstream arquivo_referencia("referencias.txt");
    if (!arquivo_referencia.is_open())
    {
        //se o arquivo não existir, ele é criado
        std::ofstream criar_arquivo("referencias.txt");
        
        //depois que ele foi criado, ele é fechado para escrita e aberto para leitura
        criar_arquivo.close();
        arquivo_referencia.open("referencias.txt");
    }
    

    //Esse laço é responsavel por abrir todos os arquivos do diretorio informado recursivamente
    for (const auto &arquivo : std::filesystem::recursive_directory_iterator(diretorio))
    {
        if (arquivo.is_regular_file())
        {
            //pega o caminho completo do arquivo
            std::string caminho = std::filesystem::absolute(arquivo.path()).string();

            //faz o calculo da hash
            std::string hash = calcularHash(arquivo.path().string());

            //se a hash não foi calculada, ele passa para o proximo item e não cadastra o que deu errado
            if (hash.empty())
            {
                std::cout<<"Erro ao calcular a hash, do caminho: " <<caminho<<"\n\n";
                continue;
            }
            
            //guarda o nome do arquivo e sua hash dentro da tabela hash 
            tabela_hash[caminho] = hash;
            
        }
        
    }
    
    //tabela hash auxiliar que vai receber os itens já cadastrados de "referencias.txt"
    std::unordered_map<std::string, std::string> referencias_existentes;

    std::string caminho;
    std::string hash;

    //esse laço lê todas as palavras do arquivo e escreve nas variaveis caminho e hash
    while (arquivo_referencia >> std::quoted(caminho) >> hash)
    {
        //armazena os itens nessa tabela auxiliar
        referencias_existentes[caminho] = hash;

    }
    
    std::ofstream escrever_arquivo_referencias("referencias.txt", std::ios::app);

    //laço para percorrer a tabela hash
    for (const auto &par : tabela_hash)
    {
        //verifica se o nome do arquivo já está cadastrado em "referencias.txt"
        if (referencias_existentes.find(par.first) == referencias_existentes.end())
        {
            //se não estiver cadastrado ele insere no arquivo "referencias.txt"
            escrever_arquivo_referencias << std::quoted(par.first) <<" "<< par.second << "\n";

            std::cout<<"O arquivo: "<<par.first<<"\n"<<"Com a HASH: "<<par.second<<"\n"<<"Foi cadastrada com sucesso!!\n\n";
        }
        else
        {
            //se ele já estiver cadastrado ele não cadastra novamente para não gerar duplicidade 
            std::cout<<"O arquivo: "<<par.first<<"\n"<<"Com a HASH: "<<par.second<<"\n"<<"Já foi cadastrada!\n\n";
        }
        
    }

}

void verificar(std::string diretorio){
    //verifica se o diretorio existe
    if (!std::filesystem::exists(diretorio) || !std::filesystem::is_directory(diretorio))
    {
        std::cout<<"ERRO: O diretorio: "<< diretorio << " não existe\n\n";
        return;
    }

    std::unordered_map<std::string, std::string> tabela_hash;

    std::ifstream arquivo_referencia("referencias.txt");
    //verifica se o arquivo "referencias.txt" foi criado
    if (!arquivo_referencia.is_open())
    {
        std::cout<<"Erro ao abrir o arquivo referencias.txt\n\n";
        return;
    }
    //verifica se o arquivo "referencias.txt" está vazio
    if (arquivo_referencia.peek() == std::ifstream::traits_type::eof())
    {
        std::cout<<"O arquivo 'referencias.txt' não tem nenhum conteudo cadastrado!\n\n";
        return;
    }
    
    //Esse laço é responsavel por abrir todos os arquivos do diretorio informado recursivamente
    for (const auto &arquivo : std::filesystem::recursive_directory_iterator(diretorio))
    {
        if (arquivo.is_regular_file())
        {
            //pega o caminho completo do arquivo
            std::string caminho = std::filesystem::absolute(arquivo.path()).string();

            //faz o calculo da hash
            std::string hash = calcularHash(arquivo.path().string());

            //se a hash não foi calculada, ele passa para o proximo item e não cadastra o que deu errado
            if (hash.empty())
            {
                std::cout<<"Erro ao calcular a hash, do caminho: " <<caminho<<"\n\n";
                continue;
            }
            
            //guarda o nome do arquivo e sua hash dentro da tabela hash 
            tabela_hash[caminho] = hash;
            
        }
        
    }

    //tabela hash auxiliar que vai receber os itens já cadastrados de "referencias.txt"
    std::unordered_map<std::string, std::string> referencias_existentes;

    std::string caminho;
    std::string hash;

    //esse laço lê todas as palavras do arquivo e escreve nas variaveis caminho e hash
    while (arquivo_referencia >> std::quoted(caminho) >> hash)
    {
        //armazena os itens nessa tabela hash auxiliar
        referencias_existentes[caminho] = hash;

    }

    //percorre todos os arquivos atuais dentro da tabela hash
    for (const auto &par : referencias_existentes)
    {
        //verifica se os arquivo já cadastrados em "referencias.txt" foram removidos 
        if(tabela_hash.find(par.first) != tabela_hash.end()){

            //verifica se a hash do arquivo atual está diferente da hash original de "referencias.txt"
            if (tabela_hash[par.first] != par.second)
            {
                std::cout<<"O arquivo: "<<par.first<<" foi modificado\n"<<
                "hash original: "<<referencias_existentes[par.first]<<"\n"<<
                "hash atual:    "<<tabela_hash[par.first]<<"\n\n";
            }
            else
            {
                
                std::cout<<"O arquivo: "<<par.first<<" não foi modificado!\n\n";
            }
            
        }
        else
        {

            std::cout << "O arquivo: " << par.first
                      << " não foi encontrado no diretório especificado. "
                      << "Pode ter sido movido ou removido.\n\n";
        }
    }
    //esse laço verifica se o arquivo atual não está cadastrada em "referencias.txt"
    for (const auto & par : tabela_hash)
    {
        if (referencias_existentes.find(par.first) == referencias_existentes.end())
        {
            std::cout<<"O arquivo: "<<par.first<< " não foi cadastrado\n\n";
        }
        
    }
    
}

int main(int argc, char *argv[]){
    if (argc < 3)
    {
        std::cout<<"Utilize ./guardiao <operação> <diretorio>\n";
        std::cout<<"Exemplo ./guardiao cadastrar /home/gustavo-pc/Vídeos/\n\n";
        return -1;
    }
    
    std::string operacao = argv[1];
    std::string diretorio = argv[2];

    if (operacao == "cadastrar")
    {
        cadastrar(diretorio);    
    }
    
    if (operacao == "verificar")
    {
        verificar(diretorio);
    }

    return 0;
    
}
