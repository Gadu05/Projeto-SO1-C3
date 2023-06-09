#include <iostream>
#include <sstream>
#include <windows.h>

using namespace std;

int realizarAcao(string acao, stringstream& texto);
int criarItem(string tipo, string nome);
int dataSistema();
int horaSistema();

int main(){

    cout << ">> ";

    string comando;
    getline(cin, comando);
    stringstream sep(comando);

    string acao;
    sep >> acao;

    realizarAcao(acao, sep);

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

    if (acao.compare("DATA") == 0)
        return dataSistema();
    if (acao.compare("HORA") == 0)
        return horaSistema();

    cout << "Comando nao identificado!" << endl;

    return 1;

}

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
            cout << "Falha na Criacao do Arquivo. \nErro: " << GetLastError() << endl;
            return 1;
        }
        else
            cout << "Criacao de Arquivo Realizada com Sucesso." << endl;
        return 0;

    }

    if (tipo.compare("-p") == 0){

        BOOL Bdir = CreateDirectory(nome.c_str() , NULL);

        if (Bdir == false){
            cout << "Falha na criacao do diretorio. \nErro: " << GetLastError() << endl;
            return 1;
        }
        else
            cout << "Diretorio criado com sucesso!" << endl;
        return 0;

    }

    return 1;

}


int dataSistema(){

    SYSTEMTIME st;
    GetSystemTime(&st);

    string diaSemana;

    cout << "Data do sistema: " << st.wDay << "/" << st.wMonth << "/" << st.wYear << endl;

    return 0;

}

int horaSistema(){

    SYSTEMTIME st;
    GetSystemTime(&st);

    string diaSemana;

    cout << "Hora do sistema: " << st.wHour << ":" << st.wMinute << ":" << st.wMilliseconds << endl;

    return 0;

}
