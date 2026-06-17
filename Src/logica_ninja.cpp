#include <iostream>
#include <fstream>

using namespace std;

struct Ninja {
    int id;                 
    char nome[100];         
    char rank[30];          
    bool temKekkeiGenkai;   
    int idade;              
    char especialidade[50]; 
    bool removido;          
};

// Redimensiona o vetor dinâmico quando atinge o limite
void redimensionar(Ninja* &v, int &tamanho) {
    int novoTamanho = tamanho + 10; 
    Ninja* reserva = new Ninja[novoTamanho];

    for (int i = 0; i < tamanho; i++) {
        reserva[i] = v[i];
    }

    delete[] v;
    v = reserva;
    tamanho = novoTamanho;
}

// Compara strings de char (necessário já que não usamos <string>)
bool comparaStrings(char str1[], const char str2[]) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

// Ordenação por ID (Selection Sort)
void ordenarPorId(Ninja* v, int contador) {
    for (int i = 0; i < contador - 1; i++) {
        int indiceMenor = i;
        for (int j = i + 1; j < contador; j++) {
            if (v[j].id < v[indiceMenor].id) {
                indiceMenor = j;
            }
        }
        if (indiceMenor != i) {
            Ninja aux = v[i];
            v[i] = v[indiceMenor];
            v[indiceMenor] = aux;
        }
    }
}

// Ordenação por Idade (Selection Sort)
void ordenarPorIdade(Ninja* v, int contador) {
    for (int i = 0; i < contador - 1; i++) {
        int indiceMenor = i;
        for (int j = i + 1; j < contador; j++) {
            if (v[j].idade < v[indiceMenor].idade) {
                indiceMenor = j;
            }
        }
        if (indiceMenor != i) {
            Ninja aux = v[i];
            v[i] = v[indiceMenor];
            v[indiceMenor] = aux;
        }
    }
}

// Função pra saber qual string vem antes no alfabeto (usando Tabela ASCII)
bool vemAntesAlfabetica(char str1[], char str2[]) {
    int i = 0;
    // Vai comparando letra por letra até achar uma diferente
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] < str2[i]) return true;  // str1 vem antes
        if (str1[i] > str2[i]) return false; // str2 vem antes
        i++;
    }
    // Se as letras foram iguais até aqui, a palavra mais curta vem primeiro
    return str1[i] == '\0' && str2[i] != '\0';
}

// Ordenação por Nome (Selection Sort)
void ordenarPorNome(Ninja* v, int contador) {
    for (int i = 0; i < contador - 1; i++) {
        int indiceMenor = i;
        for (int j = i + 1; j < contador; j++) {
            // Usa nossa função customizada pra ver quem vem antes no alfabeto
            if (vemAntesAlfabetica(v[j].nome, v[indiceMenor].nome)) {
                indiceMenor = j;
            }
        }
        if (indiceMenor != i) {
            Ninja aux = v[i];
            v[i] = v[indiceMenor];
            v[indiceMenor] = aux;
        }
    }
}

// Busca Binária por ID
int buscaBinariaPorId(Ninja* v, int contador, int idBuscado) {
    int inicio = 0;
    int fim = contador - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        if (v[meio].id == idBuscado && !v[meio].removido) {
            return meio; 
        }
        if (v[meio].id < idBuscado) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1; 
}

// Busca Sequencial por Nome
int buscaPorNome(Ninja* v, int contador, char nomeBuscado[]) {
    for (int i = 0; i < contador; i++) {
        if (!v[i].removido && comparaStrings(v[i].nome, nomeBuscado)) {
            return i;
        }
    }
    return -1; 
}

// Busca Parcial por Nome (verifica se o nomeBusca está contido no nomeNinja)
int buscaParcialPorNome(Ninja* v, int contador, char nomeBusca[]) {
    for (int i = 0; i < contador; i++) {
        if (!v[i].removido) {
            // Tenta achar o nomeBusca dentro do nome do ninja
            for (int j = 0; v[i].nome[j] != '\0'; j++) {
                int k = 0;
                // Verifica se a sequência bate a partir do índice j
                while (nomeBusca[k] != '\0' && v[i].nome[j + k] == nomeBusca[k]) {
                    k++;
                }
                // Se k chegou no final da string de busca, achamos uma parte!
                if (nomeBusca[k] == '\0') return i;
            }
        }
    }
    return -1;
}

// Carrega os dados separando por vírgula e lidando com true/false
void carregarArquivo(Ninja* &ninjas, int &tamanho, int &contador) {
    ifstream arquivo("Info/Ninjas.csv");
    if (!arquivo.is_open()) {
        cout << "Arquivo CSV nao encontrado." << endl;
        return;
    }

    char buffer[100];
    
    // Pula a primeira linha (cabeçalho)
    arquivo.getline(buffer, 100, '\n');

    // CHECAGEM DE SEGURANÇA 1: Garante que o vetor inicial não tá lotado antes de ler
    if (contador == tamanho) {
        redimensionar(ninjas, tamanho);
    }

    // Lê até a vírgula para pegar o nome
    while (arquivo.getline(ninjas[contador].nome, 100, ',')) {
        
        // Gera o ID automaticamente na memória (1, 2, 3...)
        ninjas[contador].id = contador + 1;

        // Rank
        arquivo.getline(ninjas[contador].rank, 30, ',');
        
        // Kekkei Genkai (Lê true/false e converte pra bool)
        arquivo.getline(buffer, 100, ',');
        ninjas[contador].temKekkeiGenkai = (buffer[0] == 't' || buffer[0] == 'T');
        
        // Idade (Converte char para int subtraindo '0')
        arquivo.getline(buffer, 100, ',');
        int idade = 0;
        for(int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] >= '0' && buffer[i] <= '9') {
                idade = idade * 10 + (buffer[i] - '0');
            }
        }
        ninjas[contador].idade = idade;

        // Especialidade (Lê até a quebra de linha)
        arquivo.getline(ninjas[contador].especialidade, 50, '\n');
        
        // Limpa o \r se o CSV foi salvo no Windows
        for(int i = 0; ninjas[contador].especialidade[i] != '\0'; i++) {
            if (ninjas[contador].especialidade[i] == '\r') {
                ninjas[contador].especialidade[i] = '\0';
            }
        }
        
        ninjas[contador].removido = false;
        contador++;

        // CHECAGEM DE SEGURANÇA 2: Redimensiona OBRIGATORIAMENTE antes da próxima volta do while!
        if (contador == tamanho) {
            redimensionar(ninjas, tamanho);
        }
    }
    arquivo.close();
}

// Salva mantendo o padrão exato do CSV original
void salvarArquivo(Ninja* ninjas, int contador) {
    ofstream arquivo("Info/Ninjas.csv");
    
    // Reescreve o cabeçalho
    arquivo << "Nome,Rank,Tem Kekkei Genkai,Idade,Especialidade\n";
    
    for(int i = 0; i < contador; i++) {
        if(!ninjas[i].removido) {
            arquivo << ninjas[i].nome << ","
                    << ninjas[i].rank << ","
                    << (ninjas[i].temKekkeiGenkai ? "true" : "false") << ","
                    << ninjas[i].idade << ","
                    << ninjas[i].especialidade << "\n";
        }
    }
    arquivo.close();
}