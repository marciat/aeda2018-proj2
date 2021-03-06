#include "CadeiaFarmacias.h"
#include "Data.h"
#include "Farmacia.h"
#include "Pessoa.h"
#include "Produto.h"
#include "Receita.h"
#include "util.h"
#include "Venda.h"
#include "validacao.h"

#include <Windows.h>
#include <iostream>
#include <fstream>

//Defini��o dos n�meros que correspondem a cada cor -> ser�o utilizados nas fun��es setcolor
#define CYAN 3
#define WHITE 15

using namespace std;

CadeiaFarmacias cadeia;

void setcolor(unsigned int color);

void logo_menu_principal();

void menuCriarCadeia();

void logo_cadeia_farmacias(string nome_da_cadeia);

void menuGerirCadeia(string nome_da_cadeia);

void logo_farmacia(string nome_da_farmacia);

void menuFarmacia(string nome_da_farmacia);

void logo_funcionario(string nome_do_funcionario);

void menuFuncionario(string nome_do_funcionario);

void logo_cliente(string nome_do_cliente);

void menuCliente(string nome_do_cliente);

int main() {
	unsigned int option;

	menuCriarCadeia();
	do {
		cin >> option;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Opcao Invalida. Tente outra vez: ";
		}
	} while (cin.fail());
	string nome;

	switch (option) {
	case 1:
		cout << "Nome: ";
		cin.clear();
		cin.ignore(10000, '\n');
		getline(cin, nome);
		cadeia.setNome(nome);
		break;
	case 2:
	{
		ifstream ficheiro;
		cout << "Nome da cadeia: ";
		cin.clear();
		cin.ignore(10000, '\n');
		getline(cin, nome);
		string nome_fich = nome + ".txt";
		ficheiro.open(nome_fich);
		while (!ficheiro.is_open())
		{
			cout << "Ficheiro " << nome << ".txt nao encontrado!" << endl;
			cout << "Quer tentar outra vez ou sair? (S = sair)" << endl;
			getline(cin, nome);
			if (toupperstring(nome) == "S")
				goto sairDoPrograma;
			ficheiro.open(nome + ".txt");
		}
		cadeia.setNome(nome);
		import(ficheiro, cadeia);
		break;
	}
	case 3:
		goto sairDoPrograma;
		break;
	default:
		cout << "Opcao Invalida" << endl;
	}

	do {
		menu_cadeia:

		menuGerirCadeia(cadeia.getNome());
		do {
			cin >> option;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Opcao Invalida. Tente outra vez: ";
			}
		} while (cin.fail());
		switch (option) {
		case 1: {
			cout << "Nome da Farmacia: ";
			cin.clear();
			cin.ignore(10000, '\n');
			getline(cin, nome);
			string morada;
			cout << "Morada da Farmacia: ";
			getline(cin, morada);
			Farmacia* f = new Farmacia(nome, morada);
			cadeia.addFarmacia(f);
			break;
		}
		case 2: {
			cout << "Como pretende ordenar as farmacias? " << endl;
			cout << "	1-Por nome;" << endl;
			cout << "	2-Por numero de produtos;" << endl;
			cout << "	3-Por numero de vendas;" << endl;
			do {
				cin >> option;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Opcao Invalida. Tente outra vez: ";
				}
			} while (cin.fail());
			char crescente;
			if (option >= 1 && option <= 3) {
				cout << "Pretende fazer esta ordenacao por ordem crescente(c) ou decrescente(d)? ";
				do {
					cin >> crescente;
					if (cin.fail()) {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Opcao Invalida. Tente outra vez: ";
					}
				} while ((tolower(crescente) != 'c' && tolower(crescente) != 'd') || cin.fail());
			}

			switch (option) {
			case 1:
				if (crescente == 'c')
					cadeia.sortFarmacias(NOME, true);
				else
					cadeia.sortFarmacias(NOME, false);
				break;
			case 2:
				if (crescente == 'c')
					cadeia.sortFarmacias(NUM_PROD, true);
				else
					cadeia.sortFarmacias(NUM_PROD, false);
				break;
			case 3:
				if (crescente == 'c')
					cadeia.sortFarmacias(NUM_VENDA, true);
				else
					cadeia.sortFarmacias(NUM_VENDA, false);
				break;
			default:
				cadeia.sortFarmacias(DEFAULT, true);
				break;
			}

			break;
		}
		case 3: {
			cout << "Nome da Farmacia a eliminar: ";
			cin.clear();
			cin.ignore(10000, '\n');
			getline(cin, nome);
			try {
				Farmacia *f;
				f = cadeia.removeFarmacia(nome);
				cout << "Farmacia " << f->getNome() << " eliminada" << endl;
			}
			catch (FarmaciaInexistente &e) {
				cout << "Farmacia " << e.getNome() << " inexistente" << endl;
			}
			break;
		}
		case 4: {
			vector<Farmacia *> v = cadeia.getFarmacias();
			for (unsigned int i = 0; i < v.size(); i++) {
				v.at(i)->imprimeDados();
			}
			break;
		}
		case 5: {
			cout << "Nome do Cliente: ";
			cin.clear();
			cin.ignore(10000, '\n');
			getline(cin, nome);
			string morada;
			cout << "Morada do Cliente: ";
			getline(cin, morada);
			cout << "Contribuinte do Cliente: ";
			unsigned long contribuinte;
			do {
				cin >> contribuinte;
				if (cin.fail() || !valido_contribuinte(contribuinte)) {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Contribuinte Invalido. Tente outra vez: ";
				}
			} while (cin.fail() || !valido_contribuinte(contribuinte));
			string distrito;
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Distrito: ";
			getline(cin, distrito);
			Cliente *c;
			do{
				if(isDistrito(distrito)){
					c = new Cliente(nome, morada, contribuinte, distrito);
					cadeia.addCliente(c);
					break;
				}
				cout << "Distrito invalido" << endl;
				string novo_distrito = distritoCorrecao(distrito);
				cout << "Sera que quis dizer " << novo_distrito << "? (S/N) ";

				char opcao_distrito;
				cin >> opcao_distrito;
				if(toupper(opcao_distrito) == 'S'){
					c = new Cliente(nome, morada, contribuinte, novo_distrito);
					cadeia.addCliente(c);
					break;
				}
				else{
					cout << "Insira novamente o distrito: ";
					cin.ignore();
					getline(cin, distrito);
				}
			}while(true);

			break;
		}
		case 6: {
			cout << "Nome do Cliente a eliminar: ";
			cin.clear();
			cin.ignore(10000, '\n');
			getline(cin, nome);
			try {
				Cliente *c;
				c = cadeia.removeCliente(nome);
				cout << "Cliente " << c->getNome() << " eliminado" << endl;
			}
			catch (ClienteInexistente &e) {
				cout << "Cliente " << e.getNome() << " inexistente" << endl;
			}
			break;
		}
		case 7: {
			vector<Cliente *>clientes1 = cadeia.getClientes();
			for (unsigned int i = 0; i < clientes1.size(); i++) {
				(clientes1.at(i))->showInfo();
				cout << endl;
			}
			break;
		}
		case 8: {
			cout << "Nome do Funcionario: ";
			cin.clear();
			cin.ignore(10000, '\n');
			getline(cin, nome);
			string morada;
			cout << "Morada do Funcionario: ";
			getline(cin, morada);
			cout << "Numero de contribuinte do Funcionario: ";
			unsigned long contribuinte;
			do {
				cin >> contribuinte;
				if (cin.fail() || !valido_contribuinte(contribuinte)) {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Contribuinte Invalido. Tente outra vez: ";
				}
			} while (cin.fail() || !valido_contribuinte(contribuinte));
			Funcionario *f = new Funcionario(nome, morada, contribuinte);
			if(!cadeia.addFuncionario(f,true)){
				cout << "Funcionario com contribuinte " << contribuinte << " ja existe." << endl;
			}
			else
				cout << "Funcionario adicionado com sucesso." << endl;
			break;
		}
		case 9: {
			cout << "Numero de contribuinte do Funcionario a contratar: ";
			unsigned long contribuinte;
			do {
				cin >> contribuinte;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Contribuinte Invalido. Tente outra vez: ";
				}
			} while (cin.fail() );

			int n = cadeia.contratarFuncAntigo(contribuinte);

			if(n==0)
				cout << "Funcionario contratado com sucesso." << endl;
			else if(n==1)
				cout << "Funcionario ja trabalha na cadeia." << endl;
			else
				cout << "Funcionario " << contribuinte << " nao encontrado no registo." << endl;
			break;
		}
		case 10: {
			cout << "Numero de contribuinte do Funcionario a despedir: ";
			cin.clear();
			cin.ignore(10000, '\n');
			unsigned long contribuinte;
			do {
				cin >> contribuinte;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Contribuinte Invalido. Tente outra vez: ";
				}
			} while (cin.fail());
			try {
				Funcionario *f;
				f = cadeia.despedeFuncionario(contribuinte);
				cout << "Despedimento de " << f->getNome() << endl;
			}
			catch (FuncionarioInexistente &e) {
				cout << "Funcionario " << e.getCont() << " inexistente" << endl;
			}
			break;
		}
		case 11: {
			vector<FuncPtr> v = cadeia.getFuncionarios();
			for (size_t i = 0; i < v.size(); i++) {
				v.at(i).func->showInfo();
				if(v.at(i).atual_funcionario)
					cout << "Atualmente trabalha" << endl;
				else
					cout << "Despedido" << endl;
				cout << endl;
			}
			break;
		}
		case 12:{
			vector<Funcionario*> v = cadeia.getFuncionariosAtuais();
			for (size_t i = 0; i < cadeia.getFuncionariosAtuais().size(); i++) {
				v.at(i)->showInfo();
				cout << endl;
			}
			break;
		}
		case 13:{
			cadeia.mostrarFuncionariosAntigos();
			cout << endl;
			break;
		}
		case 14: {
			cout << "Cadeia de Farmacias " << cadeia.getNome() << endl;
			cout << "	Numero de Farmacias: " << cadeia.getNumFarmacias() << endl;
			cout << "	Numero de Funcionarios: " << cadeia.getNumFuncionarios() << endl;
			cout << "	Numero de Clientes: " << cadeia.getNumClientes() << endl << endl;
			break;
		}
		case 15: {
			cout << endl << "Nome da Farmacia: ";
			cin.clear();
			cin.ignore(10000, '\n');
			getline(cin, nome);
			vector<Farmacia *> v = cadeia.getFarmacias();
			vector<Farmacia *>::iterator frm;
			for (frm = v.begin(); frm != v.end(); frm++) {
				if ((*frm)->getNome() == nome)
					break;
			}

			if (frm == v.end()) {
				cout << "Farmacia " << nome << " inexistente" << endl;
				break;
			}
			do {
				menu_farmacia:

				menuFarmacia(nome);
				do {
					cin >> option;
					if (cin.fail()) {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Opcao Invalida. Tente outra vez: ";
					}
				} while (cin.fail());
				switch (option) {
				case 1: {
					(*frm)->imprimeDados();
					cout << endl;
					break;
				}
				case 2: {
					string num;
					string frm_nome = (*frm)->getNome();
					cout << "Numero de contribuinte do novo gerente: ";
					cin.clear();
					cin.ignore(10000, '\n');
					getline(cin, num);
					vector<FuncPtr>::iterator f;
					for (f = cadeia.getFuncionarios().begin(); f != cadeia.getFuncionarios().end(); f++) {
						if (f->func->getNoContribuinte() == stoul(num)) {
							if (f->func->getFarmacia() != NULL) {
								if (f->func->getFarmacia()->getNome() == frm_nome) {
									(*frm)->setGerente(f->func);
									string func_nome = f->func->getNome();
									cout << "Novo gerente: " << func_nome;
									cout << endl;
								}
							}
							else {
								cout << "Funcionario nao trabalha na farmacia " << frm_nome;
								cout << endl;
							}
							break;
						}
					}
					if (f == cadeia.getFuncionarios().end()) {
						cout << "Funcionario nao encontrado" << endl;
					}
					break;
				}
				case 3: {
					string num;
					string frm_nome = (*frm)->getNome();
					cout << "Numero de contribuinte do novo diretor tecnico: ";
					cin.clear();
					cin.ignore(10000, '\n');
					getline(cin, num);
					vector<FuncPtr> func = cadeia.getFuncionarios();
					vector<FuncPtr>::iterator f;
					for (f = func.begin(); f != func.end(); f++) {
						if (f->func->getNoContribuinte() == stoul(num)) {
							if (f->func->getFarmacia() != NULL) {
								if (f->func->getFarmacia()->getNome() == frm_nome) {
									(*frm)->setDiretorTecnico(f->func);
									string func_nome = f->func->getNome();
									cout << "Novo diretor tecnico: " << func_nome << endl;
								}
							}
							else
								cout << "Funcionario nao trabalha na farmacia " << frm_nome << endl;
						}
						if (f == func.end()) {
							cout << "Funcionario nao encontrado" << endl;
						}
					}
					break;
				}
				case 4: {
					vector<ProdutoStock> p = (*frm)->getStock();
					vector<ProdutoStock>::const_iterator prods;
					for (prods = p.begin(); prods != p.end(); prods++) {
						cout << prods->getProd()->getCodigo() << "  ";
						cout << prods->getProd()->getNome() << "   ";
						cout << prods->getProd()->getPreco() << " euros   ";
						cout << prods->getQuant() << " unidades";
						cout << endl;
					}
					cout << endl;
					break;
				}
				case 5: {
					string nome;
					cout << "Nome do produto: ";
					cin.clear();
					cin.ignore(10000, '\n');
					getline(cin, nome);
					nome = toupperstring(nome);
					bool existe = (*frm)->existeProduto(nome);
					if (!existe) {
						unsigned long cod;
						float preco;
						string desc;
						char pass;
						Produto* p = NULL;
						cout << "Codigo do produto: ";
						do {
							cin >> cod;
							if (cin.fail()) {
								cin.clear();
								cin.ignore(10000, '\n');
								cout << "Codigo Invalido. Tente outra vez: ";
							}
						} while (cin.fail());
						cout << "Preco do produto: ";
						do {
							cin >> preco;
							if (cin.fail()) {
								cin.clear();
								cin.ignore(10000, '\n');
								cout << "Preco Invalido. Tente outra vez: ";
							}
						} while (cin.fail());
						cout << endl << "Descricao do produto: ";
						cin.clear();
						cin.ignore(10000, '\n');
						getline(cin, desc);
						cout << endl << "Produto passivel de receita? (S / N) ";
						do {
							cin >> pass;
							if (cin.fail()) {
								cin.clear();
								cin.ignore(10000, '\n');
								cout << "Opcao Invalida. Tente outra vez: ";
							}
						} while (cin.fail());
						if (toupper(pass) == 'S') {
							float comp;
							char sR;
							bool semR;
							cout << endl << "Comparticipacao (em percentagem): ";
							do {
								cin >> comp;
								if (cin.fail()) {
									cin.clear();
									cin.ignore(10000, '\n');
									cout << "Comparticipacao Invalida. Tente outra vez: ";
								}
							} while (cin.fail());
							comp = comp / 100;
							cout << endl << "Produto pode ser vendido sem receita? (S / N) ";
							do {
								cin >> sR;
								if (cin.fail()) {
									cin.clear();
									cin.ignore(10000, '\n');
									cout << "Opcao Invalida. Tente outra vez: ";
								}
							} while (cin.fail());
							semR = (toupper(sR) == 'S');
							p = new Produto(cod, nome, preco, desc, true, comp, semR);
							(*frm)->addProdutoVender(p);
						}
						else {
							p = new Produto(cod, nome, preco, desc, false, 0, false);
							(*frm)->addProdutoVender(p);
						}
					}
					unsigned quant;
					cout << endl << "Quantidade do produto: ";
					do {
						cin >> quant;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Quantidade Invalida. Tente outra vez: ";
						}
					} while (cin.fail());
					(*frm)->setQuantidade(nome, quant);
					cout << endl << "Quantidade alterada." << endl;
					break;
				}
				case 6: {
					string code;
					cout << "Codigo do produto: ";
					cin.clear();
					cin.ignore(10000, '\n');
					getline(cin, code);

					try {
						(*frm)->removeProduto(stoul(code));
						cout << "Produto " << code << " eliminado" << endl;
					}
					catch (ProdutoInexistente &e) {
						cout << "Produto " << code << " inexistente" << endl;
					}
					break;
				}
				case 7: {
					vector<Venda*> v = (*frm)->getVendas();
					for (unsigned int i = 0; i < v.size(); i++) {
						cout << "Venda " << v.at(i)->getCodigo() << endl;
						v.at(i)->imprimeFatura();
						cout << endl << endl;
					}
					break;
				}
				case 8: {
					int ano;
					unsigned short mes;
					unsigned short dia;
					cout << "Data inferior:" << endl;
					cout << "Dia: " << endl;
					do {
						cin >> dia;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Dia Invalido. Tente outra vez: ";
						}
					} while (cin.fail() || dia < 1 || dia>31);
					cout << "Mes: " << endl;
					do {
						cin >> mes;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Mes Invalido. Tente outra vez: ";
						}
					} while (cin.fail() || mes < 1 || mes>12);
					cout << "Ano: " << endl;
					do {
						cin >> ano;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Ano Invalido. Tente outra vez: ";
						}
					} while (cin.fail());
					Data d1(dia, mes, ano);
					cout << "Dia: " << endl;
					do {
						cin >> dia;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Dia Invalido. Tente outra vez: ";
						}
					} while (cin.fail() || dia < 1 || dia>31);
					cout << "Mes: " << endl;
					do {
						cin >> mes;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Mes Invalido. Tente outra vez: ";
						}
					} while (cin.fail() || mes < 1 || mes>12);
					cout << "Ano: " << endl;
					do {
						cin >> ano;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Ano Invalido. Tente outra vez: ";
						}
					} while (cin.fail());
					Data d2(dia, mes, ano);
					vector<Venda*> v = (*frm)->getVendasTempo(d1, d2);
					for (unsigned int i = 0; i < v.size(); i++) {
						cout << "Venda " << v.at(i)->getCodigo() << endl;
						v.at(i)->imprimeFatura();
						cout << endl << endl;
					}
					break;
				}
				case 9: {
					Venda *v1 = new Venda();
					unsigned long no_con;
					cout << "VENDA " << v1->getCodigo() << endl;
					cout << "Numero de contribuinte do cliente: ";
					do {
						cin >> no_con;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Contribuinte Invalido. Tente outra vez: ";
						}
					} while (cin.fail());
					vector<Cliente *> v = cadeia.getClientes();
					int existe_cliente = -1;
					for (unsigned int i = 0; i < v.size(); i++) {
						if (v.at(i)->getNoContribuinte() == no_con) {
							existe_cliente = i;
							break;
						}
					}
					if (existe_cliente == -1) {
						cout << "Cliente nao existe na cadeia" << endl << endl;
						goto menu_farmacia;
					}
					v1->setCliente(v.at(existe_cliente));
					cout << endl << "Tem receita?(S/N) ";
					char op;
					do {
						cin >> op;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Opcao Invalida. Tente outra vez: ";
						}
					} while (cin.fail());

					if (toupper(op) == 'S') {
						cout << endl << "ADICIONAR RECEITA";
						cout << endl << "Numero da receita: ";
						unsigned long numero_r;
						do {
							cin >> numero_r;
							if (cin.fail()) {
								cin.clear();
								cin.ignore(10000, '\n');
								cout << "Numero de receita Invalido. Tente outra vez: ";
							}
						} while (cin.fail());

						cout << "Numero de contribuinte do utente: ";
						unsigned long numero_c;
						do {
							cin >> numero_c;
							if (cin.fail()) {
								cin.clear();
								cin.ignore(10000, '\n');
								cout << "Contribuinte Invalido. Tente outra vez: ";
							}
						} while (cin.fail());
						existe_cliente = -1;
						for (unsigned int i = 0; i < v.size(); i++) {
							if (v.at(i)->getNoContribuinte() == no_con) {
								existe_cliente = i;
								break;
							}
						}
						if (existe_cliente == -1) {
							cout << "Cliente nao existe na cadeia" << endl << endl;
							goto menu_farmacia;
						}
						string nome_medico;
						cout << "Nome do medico: ";
						cin.clear();
						cin.ignore(10000, '\n');
						getline(cin, nome_medico);
						Receita *r1 = new Receita(numero_r, nome_medico, v.at(existe_cliente));
						Produto *p1;
						string nome_prod = "";
						unsigned long quant;
						cout << "Adicionar medicamentos a receita (para parar escreva SAIR no nome do produto):" << endl;
						while (true) {
							cout << "Nome do Produto: ";
							cin.sync();
							getline(cin, nome_prod);
							nome_prod = toupperstring(nome_prod);
							if (nome_prod == "SAIR")
								break;
							p1 = new Produto(0, nome_prod, 0, " ", true, 0, true);
							cout << endl << "Quantidade: ";
							do {
								cin >> quant;
								if (cin.fail()) {
									cin.clear();
									cin.ignore(10000, '\n');
									cout << "Quantidade Invalida. Tente outra vez: ";
								}
							} while (cin.fail());
							r1->addProduto(p1, quant);
						}
						cout << endl << "Fim da receita" << endl;
						v1->setReceita(r1);
					}
					cout << "Adicionar produtos a venda (para parar escreva SAIR no nome do produto): " << endl;
					while (true) {
						string nome_prod;
						float quant;
						float iva;
						cout << "Nome do Produto: ";
						cin.sync();
						getline(cin, nome_prod);
						nome_prod = toupperstring(nome_prod);
						if (nome_prod == "SAIR")
							break;
						bool existe_prod = (*frm)->existeProduto(nome_prod);
						if (!existe_prod) {
							cout << endl << "Produto nao existe na farmacia" << endl;
							continue;
						}
						vector<ProdutoStock>::iterator prod;
						vector<ProdutoStock> st = (*frm)->getStock();
						for (prod = st.begin(); prod != st.end(); prod++) {
							if (prod->getProd()->getNome() == nome_prod)
								break;
						}
						Produto p1 = *(prod->getProd());
						cout << endl << "Quantidade: ";
						do {
							cin >> quant;
							if (cin.fail()) {
								cin.clear();
								cin.ignore(10000, '\n');
								cout << "Quantidade Invalida. Tente outra vez: ";
							}
						} while (cin.fail());
						cout << endl << "IVA (em percentagem): ";
						do {
							cin >> iva;
							if (cin.fail()) {
								cin.clear();
								cin.ignore(10000, '\n');
								cout << "IVA Invalido. Tente outra vez: ";
							}
						} while (cin.fail());
						iva = iva / 100;
						if (!(v1->addProduto(&p1, quant, iva))) {
							cout << "Nao foi possivel adicionar o produto. Necessita de receita medica" << endl;
						}
					}
					bool venda_sucesso = (*frm)->addVenda(v1);
					if (venda_sucesso) {
						cout << endl << "Venda realizada com sucesso. Fatura: " << endl;
						(*frm)->imprimeFatura(v1);
					}
					else {
						cout << endl << "Nao foi possivel realizar a venda. Nao existe stock suficiente." << endl;
					}
					break;
				}
				case 10: {
					cout << "Como pretende ordenar as vendas? " << endl;
					cout << "	1-Por numero de produtos vendidos;" << endl;
					cout << "	2-Por data;" << endl;
					cout << "	3-Por hora;" << endl;
					cout << "   4-Por valor total." << endl;
					do {
						cin >> option;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Opcao Invalida. Tente outra vez: ";
						}
					} while (cin.fail());
					char crescente;
					if (option >= 1 && option <= 3) {
						cout << "Pretende fazer esta ordenacao por ordem crescente(c) ou decrescente(d)? ";
						do {
							cin >> crescente;
							if (cin.fail()) {
								cin.clear();
								cin.ignore(10000, '\n');
								cout << "Opcao Invalida. Tente outra vez: ";
							}
						} while ((tolower(crescente) != 'c' && tolower(crescente) != 'd') || cin.fail());
					}

					switch (option) {
					case 1:
						if (crescente == 'c')
							(*frm)->sortVendas(NUM_PROD, true);
						else
							(*frm)->sortVendas(NUM_PROD, false);
						break;
					case 2:
						if (crescente == 'c')
							(*frm)->sortVendas(DATA, true);
						else
							(*frm)->sortVendas(DATA, false);
						break;
					case 3:
						if (crescente == 'c')
							(*frm)->sortVendas(HORA, true);
						else
							(*frm)->sortVendas(HORA, false);
						break;
					default:
						(*frm)->sortVendas(DEFAULT, true);
						break;
					}
					break;
				}
				case 11:{
					cout << endl << "Qual a quantidade maxima disponivel em stock dos produtos a encomendar? ";
					cin.clear();
					cin.ignore(10000, '\n');
					unsigned n;
					do {
						cin >> n;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Quantidade invalida. Tente outra vez: ";
						}
					} while (cin.fail());

					cout << "Quantas unidades de cada produto pretende encomendar? ";
					cin.clear();
					cin.ignore(10000, '\n');
					unsigned q;
					do {
						cin >> q;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Quantidade invalida. Tente outra vez: ";
						}
					} while (cin.fail());

					cout << "Nome do fornecedor: ";
					string nomefor;
					cin.clear();
					cin.ignore(10000, '\n');
					getline(cin, nomefor);
					(*frm)->criaEncomenda(nomefor, n, q);
					cout  << endl << "Encomenda realizada com sucesso." << endl << endl;
					break;
				}
				case 12:{
					cout << endl << "Qual o codigo do produto a encomendar? ";
					cin.clear();
					cin.ignore(10000, '\n');
					unsigned n;
					do {
						cin >> n;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Codigo invalido. Tente outra vez: ";
						}
					} while (cin.fail());

					cout << "Quantas unidades do produto pretende encomendar? ";
					unsigned q;
					do {
						cin >> q;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Quantidade invalida. Tente outra vez: ";
						}
					} while (cin.fail());

					cout << "Nome do fornecedor: ";
					string nomefor;
					cin.clear();
					cin.ignore(10000, '\n');
					getline(cin, nomefor);
					try{
						ProdutoStock p = (*frm)->criaEncomendaDe1Produto(nomefor, n, q);
						cout  << endl << "Encomenda de " << p.getProd()->getNome() << " realizada com sucesso." << endl << endl;
					}
					catch(ProdutoInexistente &p1){
						cout << endl << "Produto com codigo " << p1.getCodigo() << " nao existe." << endl << endl;
					}
					break;
				}
				case 13:{
					cout << "Quantas encomendas recebeu? ";
					cin.clear();
					cin.ignore(10000, '\n');
					unsigned i;
					do {
						cin >> i;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Quantidade invalida. Tente outra vez: ";
						}
					} while (cin.fail());

					(*frm)->entregaEncomendas(i);
					cout << "Encomendas recebidas com sucesso." << endl << endl;
					break;
				}
				case 14:{
					cout << "Qual o codigo do produto que recebeu? ";
					cin.clear();
					cin.ignore(10000, '\n');
					unsigned codigo;
					do {
						cin >> codigo;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Quantidade invalida. Tente outra vez: ";
						}
					} while (cin.fail());

					try{
						ProdutoStock ps = (*frm)->entregaEncomendaProduto(codigo);
						cout << "Encomenda de " << ps.getProd()->getNome() << " recebida com sucesso." << endl << endl;
					}
					catch(ProdutoInexistente &p){
						cout << endl << "Produto com codigo " << p.getCodigo() << " nao existe." << endl << endl;
					}
					catch(EncomendaInexistente &e){
						cout << endl << "Nao foi realizada nenhuma encomenda do produto com codigo " << e.getCodigo() << "." << endl << endl;
					}
					break;
				}
				case 15:{
					vector<Encomenda> v= (*frm)->getEncomendas();
					for(size_t i = 0; i<v.size(); i++){
						v.at(i).showInfo();
						cout << endl;
					}
					break;
				}
				case 16:
					goto menu_cadeia;
					break;
				}
			} while (true);
			break;
		}
		case 16: {
			cout << endl << "Numero de Contribuinte do Funcionario: ";
			cin.clear();
			cin.ignore(10000, '\n');
			unsigned long cont;
			do {
				cin >> cont;
				if (cin.fail() || !valido_contribuinte(cont)) {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Contribuinte Invalido. Tente outra vez: ";
				}
			} while (cin.fail());
			vector<FuncPtr> v = cadeia.getFuncionarios();
			vector<FuncPtr>::iterator func;
			for (func = v.begin(); func != v.end(); func++) {
				if (func->func->getNoContribuinte() == cont)
					break;
			}

			if (func == v.end()) {
				cout << "Funcionario " << cont << " inexistente." << endl;
				break;
			}
			if(!func->atual_funcionario){
				cout << "Funcionario " << cont << " nao trabalha atualmente na cadeia." << endl;
			}

			do {

				menuFuncionario(func->func->getNome());
				do {
					cin >> option;
					if (cin.fail()) {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Opcao Invalida. Tente outra vez: ";
					}
				} while (cin.fail());
				switch (option) {
				case 1: {
					func->func->showInfo();
					cout << endl;
					break;
				}
				case 2: {
					string nome_farmacia_nova;
					cout << "Nome da farmacia onde vai trabalhar: ";
					cin.clear();
					cin.ignore(10000, '\n');
					getline(cin, nome_farmacia_nova);
					vector<Farmacia*> farmacias = cadeia.getFarmacias();
					vector<Farmacia*>::iterator frm;
					for (frm = farmacias.begin(); frm != farmacias.end(); frm++) {
						if (toupperstring((*frm)->getNome()) == toupperstring(nome_farmacia_nova)) {
							func->func->setFarmacia((*frm));
							string nome_funcionario = func->func->getNome();
							cout << nome_funcionario << " trabalha agora na farmacia " << nome_farmacia_nova << endl;
							break;
						}
					}
					if (frm == farmacias.end()) {
						cout << "Farmacia nao encontrada" << endl;
					}
					break;
				}
				case 3: {
					string novo_cargo;
					cout << "Novo Cargo: ";
					cin.clear();
					cin.ignore(10000, '\n');
					getline(cin, novo_cargo);
					func->func->setCargo(novo_cargo);
					break;
				}
				case 4: {
					double novo_salario;
					cout << "Novo salario: ";
					do {
						cin >> novo_salario;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << "Salario Invalido. Tente outra vez: ";
						}
					} while (cin.fail());
					func->func->changeSalario(novo_salario);
					break;
				}
				case 5: {
					string nova_morada;
					cout << "Nova morada: ";
					cin.clear();
					cin.ignore(10000, '\n');
					getline(cin, nova_morada);
					func->func->setMorada(nova_morada);
					break;
				}
				case 6: {
					goto menu_cadeia;
					break;
				}
				}

			} while (true);

			break;
		}
		case 17: {
			cout << endl << "Nome do Cliente: ";
			cin.clear();
			cin.ignore(10000, '\n');
			getline(cin, nome);
			vector<Cliente *> v = cadeia.getClientes();
			vector<Cliente *>::iterator cl;
			for (cl = v.begin(); cl != v.end(); cl++) {
				if ((*cl)->getNome() == nome)
					break;
			}

			if (cl == v.end()) {
				cout << "Cliente " << nome << " inexistente" << endl;
				break;
			}
			do {

				menuCliente(nome);
				do {
					cin >> option;
					if (cin.fail()) {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Opcao Invalida. Tente outra vez: ";
					}
				} while (cin.fail());
				switch (option) {
				case 1: {
					(*cl)->showInfo();
					cout << endl;
					break;
				}
				case 2: {
					vector<Venda*> v = (*cl)->getHistorialCompras();
					for (unsigned int i = 0; i < v.size(); i++) {
						cout << "Venda " << v.at(i)->getCodigo() << endl;
						v.at(i)->imprimeFatura();
						cout << endl << endl;
					}
					break;
				}
				case 3: {
					string nova_morada;
					cout << "Nova morada: ";
					cin.clear();
					cin.ignore(10000, '\n');
					getline(cin, nova_morada);
					(*cl)->setMorada(nova_morada);
					break;
				}
				case 4:{
					string novo_distrito;
					cout << "Novo distrito: ";
					cin.clear();
					cin.ignore(10000, '\n');

					do{
						getline(cin, novo_distrito);
						if(isDistrito(novo_distrito))
							break;
						else{
							cout << "Distrito invalido." << endl;
							string distrito = distritoCorrecao(novo_distrito);
							cout << "Sera que quis dizer " << distrito << "? (S/N) ";
							char opcao_distrito;
							cin >> opcao_distrito;

							if(toupper(opcao_distrito) == 'S'){
								novo_distrito = distrito;
								break;
							}
							else{
								cout << "Insira novamente o distrito: ";
								cin.ignore();
							}
						}
					}while(!isDistrito(novo_distrito));

					(*cl)->setDistrito(novo_distrito);
					break;
				}
				case 5: {
					goto menu_cadeia;
					break;
				}
				}
			} while (true);
			break;
		}
		case 18: {
			string op;
			cout << endl << "Deseja guardar as alteracoes? (S / N) ";
			cin.clear();
			cin.ignore(10000, '\n');
			getline(cin, op);
			if (toupperstring(op) == "S") {
				string nome_fich = cadeia.getNome();
				nome_fich += ".txt";
				ofstream exportfile;
				exportfile.open(nome_fich);
				cout << endl << "A guardar altera�oes..." << endl;
				exportfile << cadeia;
				cout << "Cadeia guardada com sucesso" << endl;
			}
			goto sairDoPrograma;
			break;
		}
		}
	} while (true);



	sairDoPrograma:
	return 0;
}

void setcolor(unsigned int color) //Fun��o que muda a cor da letra, conforme o inteiro que lhe � passado
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

void logo_menu_principal(){
	setcolor(CYAN);
	cout << " ___________________ " << endl;
	cout << "|                   |" << endl;
	cout << "|Cadeia de Farmacias|" << endl;
	cout << "|___________________|" << endl;
	setcolor(WHITE);
}

void menuCriarCadeia() {
	logo_menu_principal();
	cout << endl;
	cout << "1. Criar nova cadeia. " << endl;
	cout << "2. Importar cadeia. " << endl;
	cout << "3. Sair. " << endl;
}

void logo_cadeia_farmacias(string nome_da_cadeia){
	setcolor(CYAN);
	cout << " __________________________";
	for(unsigned i = 0; i < nome_da_cadeia.length(); i++)
		cout << "_";
	cout << endl;
	cout <<"|                          ";
	for(unsigned i = 0; i < nome_da_cadeia.length(); i++)
		cout << " ";
	cout << "|" << endl;
	cout <<"|GERIR CADEIA DE FARMACIAS " << toupperstring(nome_da_cadeia) << "|" << endl;
	cout <<"|__________________________";
	for(unsigned i = 0; i < nome_da_cadeia.length(); i++)
		cout << "_";
	cout << "|" << endl;
	setcolor(WHITE);
}

void menuGerirCadeia(string nome_da_cadeia) {
	cout << endl;
	logo_cadeia_farmacias(nome_da_cadeia);
	cout << endl;

	cout << "1. Adicionar Farmacia." << endl;
	cout << "2. Ordenar farmacias." << endl;
	cout << "3. Remover Farmacia." << endl;
	cout << "4. Ver farmacias." << endl;
	cout << "5. Adicionar Cliente." << endl;
	cout << "6. Remover Cliente." << endl;
	cout << "7. Ver clientes." << endl;
	cout << "8. Adicionar Funcionario." << endl;
	cout << "9. Contratar Funcionario antigo." << endl;
	cout << "10. Despedir Funcionario." << endl;
	cout << "11. Ver todos os funcionarios." << endl;
	cout << "12. Ver funcionarios atuais." << endl;
	cout << "13. Ver funcionarios antigos." << endl;
	cout << "14. Ver dados da cadeia." << endl;
	cout << "15. Gerir Farmacia." << endl;
	cout << "16. Gerir Funcionario." << endl;
	cout << "17. Gerir Cliente." << endl;
	cout << "18. Sair." << endl;
}

void logo_farmacia(string nome_da_farmacia){
	setcolor(CYAN);
	cout << " _______________";
	for(unsigned i = 0; i < nome_da_farmacia.length(); i++)
		cout << "_";
	cout << endl;
	cout <<"|               ";
	for(unsigned i = 0; i < nome_da_farmacia.length(); i++)
		cout << " ";
	cout << "|" << endl;
	cout <<"|GERIR FARMACIA " << toupperstring(nome_da_farmacia) << "|" << endl;
	cout <<"|_______________";
	for(unsigned i = 0; i < nome_da_farmacia.length(); i++)
		cout << "_";
	cout << "|" << endl;
	setcolor(WHITE);
}

void menuFarmacia(string nome_da_farmacia) {
	cout << endl;
	logo_farmacia(nome_da_farmacia);
	cout << endl;

	cout << "1. Ver dados da Farmacia." << endl;
	cout << "2. Mudar gerente." << endl;
	cout << "3. Mudar diretor tecnico." << endl;
	cout << "4. Consultar stock." << endl;
	cout << "5. Adicionar ou alterar quantidade de Produto." << endl;
	cout << "6. Remover Produto." << endl;
	cout << "7. Ver historial de vendas." << endl;
	cout << "8. Ver vendas entre duas datas." << endl;
	cout << "9. Realizar Venda." << endl;
	cout << "10. Ordenar Vendas." << endl;
	cout << "11. Encomendar produtos com pouca quantidade disponivel." << endl;
	cout << "12. Encomendar um produto a escolher." << endl;
	cout << "13. Receber Encomendas." << endl;
	cout << "14. Receber Encomenda de um produto a escolher." << endl;
	cout << "15. Ver encomendas pendentes." << endl;
	cout << "16. Voltar atras." << endl;
}

void logo_funcionario(string nome_do_funcionario){
	setcolor(CYAN);
	cout << " __________________";
	for(unsigned i = 0; i < nome_do_funcionario.length(); i++)
		cout << "_";
	cout << endl;
	cout <<"|                  ";
	for(unsigned i = 0; i < nome_do_funcionario.length(); i++)
		cout << " ";
	cout << "|" << endl;
	cout <<"|GERIR FUNCIONARIO " << toupperstring(nome_do_funcionario) << "|" << endl;
	cout <<"|__________________";
	for(unsigned i = 0; i < nome_do_funcionario.length(); i++)
		cout << "_";
	cout << "|" << endl;
	setcolor(WHITE);
}

void menuFuncionario(string nome_do_funcionario) {
	cout << endl;
	logo_funcionario(nome_do_funcionario);
	cout << endl;

	cout << "1. Ver dados do Funcionario." << endl;
	cout << "2. Alterar farmacia em que trabalha." << endl;
	cout << "3. Alterar cargo." << endl;
	cout << "4. Alterar salario." << endl;
	cout << "5. Alterar morada." << endl;
	cout << "6. Voltar atras." << endl;
}

void logo_cliente(string nome_do_cliente){
	setcolor(CYAN);
	cout << " ______________";
	for(unsigned i = 0; i < nome_do_cliente.length(); i++)
		cout << "_";
	cout << endl;
	cout <<"|              ";
	for(unsigned i = 0; i < nome_do_cliente.length(); i++)
		cout << " ";
	cout << "|" << endl;
	cout <<"|GERIR CLIENTE " << toupperstring(nome_do_cliente) << "|" << endl;
	cout <<"|______________";
	for(unsigned i = 0; i < nome_do_cliente.length(); i++)
		cout << "_";
	cout << "|" << endl;
	setcolor(WHITE);
}

void menuCliente(string nome_do_cliente) {
	cout << endl;
	logo_cliente(nome_do_cliente);
	cout << endl;
	cout << "1. Ver dados do Cliente." << endl;
	cout << "2. Ver historial de compras." << endl;
	cout << "3. Alterar morada." << endl;
	cout << "4. Alterar distrito." << endl;
	cout << "5. Voltar atras." << endl;
}


