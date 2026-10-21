#include <iostream>
#include <string>
using namespace std;

// no usado por fila, pilha e lista
struct No {
    string nome;
    int    urgencia;
    No*    prox;  // proximo no
    No(const string& n, int u = 0) : nome(n), urgencia(u), prox(nullptr) {}
};

// fifo, primeiro que entra, primeiro que sai
struct FilaPacientes {
    No* frente = nullptr;
    No* tras   = nullptr;

    //Entra na fila
    void enqueue(const string& nome, int urgencia) {
        No* novo = new No(nome, urgencia);
        if (tras == nullptr) {   // fila vazia
            frente = novo;
            tras = novo;
        } else {
            tras->prox = novo;
            tras = novo;
        }
    }

    //Sair da fila (ser atendido)
    string dequeue() {
        if (isEmpty()) return "";
        No* temp = frente;
        string nome = temp->nome;
        frente = frente->prox;
        if (frente == nullptr) tras = nullptr;  // fila vazia
        delete temp;
        return nome;
    }

    // ve o primeiro sem remover
    string frente_peek() const {
        if (isEmpty()) return "";
        return frente->nome;
    }

    bool isEmpty() const { return frente == nullptr; }

    // conta os pacientes
    int tamanho() const {
        int cont = 0;
        No* p = frente;
        while (p != nullptr) {
            cont++;
            p = p->prox;
        }
        return cont;
    }

    ~FilaPacientes() {
        while (!isEmpty()) dequeue();
    }
};

// lifo,  ultimo que entra, primeiro que sai
struct PilhaHistorico {
    No* topo = nullptr;

    void push(const string& nome) {
        No* novo = new No(nome);
        novo->prox = topo;
        topo = novo;
    }

    string pop() {
        if (isEmpty()) return "";
        No* temp = topo;
        string nome = temp->nome;
        topo = topo->prox;
        delete temp;
        return nome;
    }

    // ve o topo sem remover
    string peek() const {
        if (isEmpty()) return "";
        return topo->nome;
    }

    bool isEmpty() const { return topo == nullptr; }

    ~PilhaHistorico() {
        while (!isEmpty()) pop();
    }
};

// lista dos medicos
struct ListaMedicos {
    No* head = nullptr;

    // adiciona no final
    void adicionarMedico(const string& nome) {
        No* novo = new No(nome);
        if (head == nullptr) {
            head = novo;
            return;
        }
        No* p = head;
        while (p->prox != nullptr) p = p->prox;  // vai ate o ultimo
        p->prox = novo;
    }

    // tira o primeiro da lista
    string chamarMedico() {
        if (head == nullptr) return "";
        No* temp = head;
        string nome = temp->nome;
        head = head->prox;
        delete temp;
        return nome;
    }

    // imprime a lista
    void percorrer(const string& prefixo = "") const {
        if (!prefixo.empty()) cout << prefixo << ": ";
        if (head == nullptr) {
            cout << "(lista vazia)\n";
            return;
        }
        No* p = head;
        while (p != nullptr) {
            cout << p->nome;
            if (p->prox != nullptr) cout << " -> ";
            p = p->prox;
        }
        cout << "\n";
    }

    bool temMedico() const { return head != nullptr; }

    ~ListaMedicos() {
        while (head) {
            No* t = head;
            head  = head->prox;
            delete t;
        }
    }
};

// imprime o historico usando uma pilha auxiliar pra nao perder os dados
void imprimirHistorico(PilhaHistorico& hist) {
    cout << "\n=== Relatório de Atendimentos ===\n";
    if (hist.isEmpty()) {
        cout << "(nenhum atendimento registrado)\n";
        return;
    }
    PilhaHistorico aux;
    int i = 1;
    // passa tudo pra aux imprimindo
    while (!hist.isEmpty()) {
        string item = hist.pop();
        cout << "  " << i++ << ". " << item << "\n";
        aux.push(item);
    }
    // devolve pro historico
    while (!aux.isEmpty()) {
        hist.push(aux.pop());
    }
}

int main() {
    cout << "╔═══════════════════════════════════════════════════╗\n";
    cout << "║  Sistema de Atendimento — Squad Branco loco       ║\n";
    cout << "╚═══════════════════════════════════════════════════╝\n\n";

    FilaPacientes  fila;
    PilhaHistorico historico;
    ListaMedicos   medicos;

    // cadastro dos medicos
    medicos.adicionarMedico("Dra. Ana Lima");
    medicos.adicionarMedico("Dr. Bruno Costa");
    medicos.adicionarMedico("Dra. Carla Ramos");
    medicos.percorrer("Médicos disponíveis");

    // chegada dos pacientes
    cout << "\nPacientes chegando:\n";
    fila.enqueue("Eduardo (urgencia=3)", 3);
    fila.enqueue("Fernanda (urgencia=1)", 1);
    fila.enqueue("Gabriel (urgencia=2)", 2);
    fila.enqueue("Helena (urgencia=1)", 1);

    cout << "Próximo a ser atendido: " << fila.frente_peek() << "\n";
    cout << "Total na fila: " << fila.tamanho() << "\n";

    // atendimentos
    cout << "\nSimulação de atendimentos:\n";
    int atend = 1;
    while (!fila.isEmpty() && medicos.temMedico()) {
        string paciente = fila.dequeue();
        string medico   = medicos.chamarMedico();

        cout << "  [" << atend++ << "] " << medico
             << " atende " << paciente << "\n";

        historico.push(medico + " → " + paciente);
        medicos.adicionarMedico(medico);  // volta pro final da lista
    }

    cout << "\nÚltimo atendimento registrado (peek): " << historico.peek() << "\n";

    imprimirHistorico(historico);
    cout << "\nHistórico preservado. Topo atual: " << historico.peek() << "\n";

    return 0;
}