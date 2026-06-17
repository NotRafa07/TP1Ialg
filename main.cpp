// -------------------------------------------------------------------
// Equipe: Rafael Teixeira Sá Fortes, Alvaro Pacheco Rocha, Matheus Da Silva Moreira
// Turma: 14B
// Tema: Naruto
// -------------------------------------------------------------------

#include <iostream>
#include "Src/logica_ninja.cpp" 

using namespace std;

int main() {
    cout << "INICIANDO O SISTEMA... TESTE DE EXECUCAO!" << endl; //mensagem de teste para confirmar que o programa iniciou sem problemas
    
    int tamanho = 40; 
    int contador = 0;
    Ninja* ninjas = new Ninja[tamanho];

    // Carrega o CSV...
    carregarArquivo(ninjas, tamanho, contador);
    cout << "ARQUIVO CARREGADO COM SUCESSO!" << endl; //mensagem de teste pra confirmar que o arquivo foi carregado sem problemas

    int opcao = -1;
    
    while (opcao != 0) {
        cout << "\n=========================================" << endl;
        cout << "       SISTEMA DE CADASTRO SHINOBI       " << endl;
        cout << "=========================================" << endl;
        cout << "1. Listar Ninjas (Todos ou Trecho)" << endl;
        cout << "2. Cadastrar Novo Ninja" << endl;
        cout << "3. Remover Ninja (Exclusao Logica)" << endl;
        cout << "4. Buscar Registro" << endl;
        cout << "5. Ordenar Banco de Dados" << endl;
        cout << "0. Salvar e Sair" << endl;
        cout << "=========================================" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1: {
                int tipoListagem;
                cout << "\n[1] Mostrar arquivo inteiro\n[2] Mostrar um trecho especifico\nEscolha: ";
                cin >> tipoListagem;
                
                int inicio = 0;
                int fim = contador;
                
                if (tipoListagem == 2) {
                    cout << "Digite a posicao inicial (0 a " << contador - 1 << "): ";
                    cin >> inicio;
                    cout << "Digite a posicao final: ";
                    cin >> fim;
                    fim++; 
                }

                cout << "\n--- LISTA DE NINJAS ---" << endl;
                for(int i = inicio; i < fim && i < contador; i++) {
                    if(!ninjas[i].removido) {
                        cout << "[ID: " << ninjas[i].id << "] Nome: " << ninjas[i].nome 
                             << " | Idade: " << ninjas[i].idade 
                             << " | Rank: " << ninjas[i].rank << endl;
                    }
                }
                break;
            }
            case 2: {
                cout << "\n--- CADASTRAR NOVO NINJA ---" << endl;
                if (contador == tamanho) {
                    redimensionar(ninjas, tamanho);
                }

                // ID é gerado automaticamente na RAM
                ninjas[contador].id = contador + 1;
                cout << "ID gerado pelo sistema: " << ninjas[contador].id << endl;
                
                cin.ignore(); 
                
                cout << "Nome do Ninja: ";
                cin.getline(ninjas[contador].nome, 100);
                
                cout << "Rank (Ex: Genin, Jounin): ";
                cin.getline(ninjas[contador].rank, 30);
                
                cout << "Possui Kekkei Genkai? (1 para SIM, 0 para NAO): ";
                int kg;
                cin >> kg;
                ninjas[contador].temKekkeiGenkai = (kg == 1);
                
                cout << "Idade: ";
                cin >> ninjas[contador].idade;
                
                cin.ignore(); 
                
                cout << "Especialidade Principal: ";
                cin.getline(ninjas[contador].especialidade, 50);
                
                ninjas[contador].removido = false;
                contador++;
                
                cout << "Ninja cadastrado com sucesso!" << endl;
                break;
            }
            case 3: {
                int idRemover;
                cout << "\n--- REMOVER NINJA ---" << endl;
                cout << "Digite o ID do ninja que deseja remover: ";
                cin >> idRemover;
                
                int index = buscaBinariaPorId(ninjas, contador, idRemover);
                if(index != -1) {
                    ninjas[index].removido = true; 
                    cout << "Ninja removido com sucesso!" << endl;
                } else {
                    cout << "ERRO: Ninja nao encontrado. Certifique-se de ordenar por ID primeiro!" << endl;
                }
                break;
            }
            case 4: {
                int tipoBusca;
                cout << "\n--- BUSCAR NINJA ---" << endl;
                cout << "[1] Buscar por ID" << endl;
                cout << "[2] Buscar Nome Exato" << endl;
                cout << "[3] Buscar Nome Parcial (Ex: 'Uchiha')" << endl;
                cout << "Escolha: ";
                cin >> tipoBusca;

                if (tipoBusca == 1) {
                    int idBusca;
                    cout << "Digite o ID: ";
                    cin >> idBusca;
                    int index = buscaBinariaPorId(ninjas, contador, idBusca);
                    if(index != -1) {
                        cout << "ENCONTRADO -> Nome: " << ninjas[index].nome << " | Especialidade: " << ninjas[index].especialidade << endl;
                    } else {
                        cout << "Nao encontrado! O vetor foi ordenado por ID antes da busca?" << endl;
                    }
                } else if (tipoBusca == 2) {
                    char nomeBusca[100];
                    cin.ignore();
                    cout << "Digite o Nome exato: ";
                    cin.getline(nomeBusca, 100);
                    int index = buscaPorNome(ninjas, contador, nomeBusca);
                    if(index != -1) {
                        cout << "ENCONTRADO -> ID: " << ninjas[index].id << " | Idade: " << ninjas[index].idade << endl;
                    } else {
                        cout << "Ninja nao encontrado." << endl;
                    }
                } else if (tipoBusca == 3) {
                    char nomeBusca[100];
                    cin.ignore();
                    cout << "Digite o trecho do nome: ";
                    cin.getline(nomeBusca, 100);
                    int index = buscaParcialPorNome(ninjas, contador, nomeBusca);
                    if(index != -1) {
                        cout << "ENCONTRADO -> Nome: " << ninjas[index].nome << " | ID: " << ninjas[index].id << endl;
                    } else {
                        cout << "Nenhum ninja encontrado com esse trecho." << endl;
                    }
                }
                break;
            }
            case 5: {
                int tipoOrdem;
                cout << "\n--- ORDENAR DADOS (Selection Sort) ---" << endl;
                cout << "[1] Ordenar por ID" << endl;
                cout << "[2] Ordenar por Idade" << endl;
                cout << "Escolha: ";
                cin >> tipoOrdem;

                if (tipoOrdem == 1) {
                    ordenarPorId(ninjas, contador);
                    cout << "Vetor ordenado por ID com sucesso!" << endl;
                } else if (tipoOrdem == 2) {
                    ordenarPorIdade(ninjas, contador);
                    cout << "Vetor ordenado por Idade com sucesso!" << endl;
                } else {
                    cout << "Opcao invalida." << endl;
                }
                break;
            }
            case 0: {
                cout << "\nSalvando arquivo..." << endl;
                salvarArquivo(ninjas, contador);
                break;
            }
            default:
                cout << "Opcao invalida." << endl;
                break;
        }
    }

    delete[] ninjas;
    return 0;
}