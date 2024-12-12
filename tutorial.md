
# Tutorial

## Pré-requisitos

Certifique-se de que as seguintes ferramentas estão instaladas no seu sistema:

- GCC (com suporte a OpenMP)

## Configurando o Ambiente

1. **Instale o GCC**

   ```bash
   sudo apt-get install gcc
   ```

2. **Verifique o suporte ao OpenMP**

   ```bash
   gcc -fopenmp --version
   ```

## Instalando 

Clone o repositório

    ```bash
    git clone 
    cd csc64-exame
    ```

## Compilando o Projeto


- **`make all`**: Compila todos os executáveis necessários.
- **`make db`**: Executa o gerador de base de dados.
- **`make clean`**: Remove todos os arquivos gerados durante a compilação e execução.

## Executando o Programa


Compile os executáveis e gere a base de dados.

```bash
make all
make db
```

Antes de rodar o programa, escolha o número de threads:

```bash
ulimit -s unlimited
export OMP_NUM_THREADS=<NUMERO DE THREADS AQUI>
```

Rode o programa

```bash
./build/parallel.exe
```

Na raiz do projeto, há o script `Xmit.sh` que já realiza todos esses processos e roda o programa no supercomputador Santos Dumont.

Basta usá-lo:

```bash
./Xmit.sh <NUMERO DE THREADS>
```

## Analisando o output

Os resultados do programa são gerados no diretório output. Os principais arquivos são:

1. `sorted_X.csv`: resultado do processamento para X threads.
2. `Out_X_OMP`: tempo de execução para X threads.

Temos dois scripts:

1. `validate.py`: verifica se todos arquivos .csv são iguais.
2. `plot.py`: gera o gráfico com speedup e tempo de execução para cada thread.