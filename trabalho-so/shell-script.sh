#!/bin/bash

DIRETORIO_ALVO=${1:-"./arquivos"}

echo "Iniciando a varredura no diretório: $DIRETORIO_ALVO"

./guardiao cadastrar "$DIRETORIO_ALVO"

codigo_de_saida=$?

case "$codigo_de_saida" in
    0)
        echo "Varredura concluída com sucesso."
        ;;
    *)
        echo "Erro: Ocorreu um problema na execução."
        exit 2
        ;;
esac