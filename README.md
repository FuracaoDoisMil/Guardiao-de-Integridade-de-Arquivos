# Guardiao-de-Integridade-de-Arquivos

para compilar:

primeiro você deve instalar a biblioteca openssl:

sudo apt update

sudo apt install libssl-dev

segundo:

g++ guardiao.cpp -o guardiao -lcrypto

terceiro:

./guardiao <operação> <diretorio>

exemplo:

./guardiao cadastrar /home/gustavo-pc/Vídeos/
