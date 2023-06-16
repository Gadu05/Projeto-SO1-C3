/*****************************************************
*	Projeto Prático de Sistemas Operacionais I       *
******************************************************/

#include <iostream>
#include <sstream>
#include <fstream>
#include <windows.h>

using namespace std;

int realizarAcao (string acao, stringstream& texto);
int inserirTexto(string arquivo, string texto);
int renomear(string caminho, string nome);
int mover(string caminho, string destino);
int apagarPastaEArquivos(string caminho);
int deletarItem(string tipo, string nome);
int criarItem(string tipo, string nome);
int listarItem(string pasta);
int dataSistema();
int horaSistema();

void ajuda();
void limparTela ();
void verVersaoSistema();
void finalizarPrograma ();



/***********************************************
*                  FUNÇÃO MAIN                 *
***********************************************/

int main(){

    cout << "para ajuda digite: AJUDA" << endl;

    string comando;


    while(true){

        cout << ">>> ";

        getline(cin, comando);

        if (comando == "")
            continue;

        stringstream sep(comando);

        string acao;
        sep >> acao;

        realizarAcao(acao, sep);

    }

    return 0;

}

int realizarAcao(string acao, stringstream& texto){

    if (acao.compare("CRIAR") == 0){
        string tipo, nome, aux;
        texto >> tipo;
        texto >> nome;

        while (texto >> aux)
            nome += " " + aux;

        return criarItem(tipo, nome);

    }

    if (acao.compare("INSERIR") == 0){

        string arquivo, str, aux;
        texto >> arquivo;

        while (texto >> aux && arquivo.find(".") == string::npos)
            arquivo += " " + aux;

        str = aux;
        while (texto >> aux)
            str += " " + aux;

        return inserirTexto(arquivo, str);

    }

    if (acao.compare("LISTAR") == 0){
        string pasta;
        texto >> pasta;
        return listarItem(pasta);
    }

    if (acao.compare("APAGAR") == 0){

        char c;

        cout << "Esse comando ira apagar todos os itens dentro da pasta!" << endl;
        cout << "Deseja continuar? s/n: ";

        cin >> c;
        cin.clear();

        if (c == 'n')
            return 0;

        string nome, aux;
        texto >> nome;

        while (texto >> aux)
            nome += " " + aux;

        return apagarPastaEArquivos(nome);

    }

    if (acao.compare("DELETAR") == 0){
        string tipo, nome, aux;
        texto >> tipo;
        texto >> nome;

        while (texto >> aux)
            nome += " " + aux;

        if (deletarItem(tipo, nome) == 1){
            cout << "Houve um erro ao deletar o arquivo/diretorio." << endl;
            return 1;
        }

        return 0;
    }

    if (acao.compare("RENOMEAR") == 0){

        string caminho, nome, aux;
        texto >> caminho;

        while (texto >> aux)
            caminho += " " + aux;

        int n = caminho.rfind("/ ");
        aux = caminho.substr(n+2);
        caminho.replace(n, string::npos, "");
        nome = caminho;
        n = caminho.rfind("\\\\");
        nome.replace(n+2, string::npos, aux);

        return renomear(caminho, nome);

    }

    if (acao.compare("MOVER") == 0){

        string caminho, nome, aux;
        texto >> caminho;

        while (texto >> aux)
            caminho += " " + aux;

        int n = caminho.rfind("/ ");
        aux = caminho.substr(n+2);
        caminho.replace(n, string::npos, "");
        n = caminho.rfind("\\\\");
        nome = aux;
        nome.append(caminho.substr(n));

        cout << nome << "\n" << caminho <<endl;

        return mover(caminho, nome);

    }

    if (acao.compare("DATA") == 0)
        return dataSistema();

    if (acao.compare("HORA") == 0)
        return horaSistema();

    if (acao.compare("VER") == 0){
        verVersaoSistema();
        return 0;
    }

    if (acao.compare("LIMPAR") == 0){
        limparTela();
        return 0;
    }

    if (acao.compare("AJUDA") == 0){
        ajuda();
        return 0;
    }

    if (acao.compare("SAIR") == 0)
        finalizarPrograma();

    cout << "Comando nao identificado!" << endl;
    return 1;

}

/*****
COMANDO CRIAR
*****/

int criarItem(string tipo, string nome){


    if (tipo.compare("-a") == 0){

        HANDLE Hfile = CreateFile (nome.c_str(),
                                   GENERIC_READ|GENERIC_WRITE, //Tipo de Acesso
                                   FILE_SHARE_READ, //Tipo de Compartilhamento
                                   NULL, //Atributos de Segurança
                                   CREATE_NEW, //Criar Novo
                                   FILE_ATTRIBUTE_NORMAL, //Tipo de Atributo
                                   NULL);

        if (Hfile == INVALID_HANDLE_VALUE){
            cout << "Falha na Criacao do Arquivo." << endl << "Erro: " << GetLastError() << endl;
            return 1;
        }
        else
            cout << "Criacao de Arquivo Realizada com Sucesso." << endl;

        return 0;

    }

    if (tipo.compare("-d") == 0){

        BOOL Bdir = CreateDirectory(nome.c_str(), NULL);

        if (Bdir == false){
            cout << "Falha na criacao do diretorio." << endl << "Erro: " << GetLastError() << endl;
            return 1;
        }
        else
            cout << "Diretorio criado com sucesso!" << endl;
        return 0;

    }

    return 1;

}

/*****
COMANDO INSERIR
*****/

int inserirTexto(string arquivo, string texto){

    ofstream file;
    file.open(arquivo.c_str(), ofstream::app);

    file << texto;

    file.close();

    return 0;

}

/*****
COMANDO LISTAR
*****/

int listarItem(string pasta){

    if (pasta == "")
        pasta = ".\\";
    cout << pasta << endl;

    WIN32_FIND_DATA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    string searchPath = pasta + "\\*";
    hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE){

        do{
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0){
                    cout << "[DIR] " << findData.cFileName << endl;
                }
            }
            else
                cout << "[ARQ] " << findData.cFileName << endl;
        }
        while (FindNextFile(hFind, &findData) != 0);

        FindClose(hFind);
    }

    return 0;
}

/*****
COMANDO APAGAR
*****/

int apagarPastaEArquivos(string caminho){

    WIN32_FIND_DATA findData;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    string searchPath = caminho + "\\*";
    hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE){

        do{
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0){
                    string apagar = caminho + "\\\\" + findData.cFileName;
                    apagarPastaEArquivos(apagar);
                    deletarItem("-d", apagar);
                }
            }
            else
                deletarItem("-a", caminho+"\\"+findData.cFileName);
        }
        while (FindNextFile(hFind, &findData) != 0);

        FindClose(hFind);
    }

    return deletarItem("-d", caminho);

}

/*****
COMANDO RENOMEAR
*****/

int renomear(string caminho, string nome){

    BOOL Bdir = MoveFile(caminho.c_str(), nome.c_str());

    if (Bdir == false){
        cout << "Falha na renomeacao do arquivo/diretorio." << endl << "Erro: " << GetLastError() << endl;
        return 1;
    }

    return 0;

}

/*****
COMANDO MOVER
*****/

int mover(string caminho, string destino){

    BOOL Bdir = MoveFile(caminho.c_str(), destino.c_str());

    if (Bdir == false){
        cout << "Nao foi possivel mover o arquivo/diretorio." << endl << "Erro: " << GetLastError() << endl;
        return 1;
    }

    return 0;

}

/*****
COMANDO DELETAR
*****/

int deletarItem (string tipo, string nome){

    if (tipo.compare("-a") == 0){

        if (DeleteFile(nome.c_str()))
            return 0;
        else
            return 1;
    }

    if (tipo.compare("-d") == 0){

        if (RemoveDirectory(nome.c_str()))
            return 0;
        else
            return 1;
    }

    return 1;

}


/*****
COMANDO AJUDA
*****/

void ajuda(){

    cout << endl;
    cout << "AJUDA" << "       Lista todos os comandos." << endl;
    cout << "APAGAR" << "      Apagar uma pasta e seus arquivos." << endl;
    cout << "CRIAR" << "       Cria uma arquivo ou uma pasta." << endl;
    cout << "  [-a]" << "      Criar arquvio." << endl;
    cout << "  [-d]" << "      Criar pasta." << endl;
    cout << "DATA" << "        Mostra a data do sistema." << endl;
    cout << "DELETAR" << "     Apaga uma arquivo ou uma pasta vazia." << endl;
    cout << "  [-a]" << "      Apagar arquvio." << endl;
    cout << "  [-d]" << "      Apagar pasta vazia." << endl;
    cout << "HORA" << "        Mostra a hora do sistema." << endl;
    cout << "INSERIR" << "     Insere texto em um arquivo." << endl;
    cout << "LIMPAR" << "      Limpa a tela." << endl;
    cout << "LISTAR" << "      Mostra os arquivos e pastas da pasta." << endl;
    cout << "MOVER" << "       Move um arquivo ou uma pasta. OBS: Adicione '/' no final para separar as entradas." << endl;
    cout << "RENOMEAR" << "    Renomea um arquivo ou uma pasta. OBS: Adicione '/' no final para separar as entradas." << endl;
    cout << "SAIR" << "        Finaliza o programa." << endl;
    cout << "VER" << "         Mostra a versao do sistema operacional." << endl;

    return;

}

/*****
COMANDO VER
*****/

void verVersaoSistema(){

    printf("Versao do sistema operacional: %lu\n", GetVersion());
    return;

}

/*****
COMANDO DATA
*****/

int dataSistema(){

    SYSTEMTIME st;
    GetSystemTime(&st);

    string diaSemana;

    cout << "Data do sistema: " << st.wDay << "/" << st.wMonth << "/" << st.wYear << endl;

    return 0;

}

/*****
COMANDO HORA
*****/

int horaSistema(){

    SYSTEMTIME st;
    GetSystemTime(&st);

    string diaSemana;

    cout << "Hora do sistema: " << st.wHour << ":" << st.wMinute << ":" << st.wMilliseconds << endl;

    return 0;

}

/*****
COMANDO LIMPAR
*****/

void limparTela(){
    system("cls");
    return;
}

/*****
COMANDO SAIR
*****/

void finalizarPrograma(){
    exit(0);
}

