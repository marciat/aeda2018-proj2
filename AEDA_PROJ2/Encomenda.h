/**
 * Header File que contem a classe Encomenda
 *
 * @author Marcia e Pedro
 * @date Dezembro, 2018
 */

#ifndef ENCOMENDA_H
#define ENCOMENDA_H

#include "Produto.h"
#include <string>

class Encomenda{
	std::string fornecedor; //Nome do fornecedor da encomenda
	unsigned long codigo; //Codigo da encomenda: atribuido sequencialmente
	unsigned long prod; //Codigo do produto a ser encomendado
	static unsigned long codigo_sequencial; //Variavel que permite atribuir o codigo da encomenda sequencialmente
	unsigned quant; //Quantidade do produto a encomendar
	unsigned quant_stock; //Quantidade do produto em stock

public:
	/**
	 * Construtor da classe Encomenda
	 */
	Encomenda();

	/**
	 * Construtor da classe Encomenda
	 * @param fornecedor Nome do fornecedor
	 * @param prod Codigo do produto encomendado
	 * @param quant Numero de unidades de produto encomendadas
	 */
	Encomenda(std::string fornecedor, unsigned long prod, unsigned quant, unsigned quant_stock);

	/**
	 * Metodo que permite obter o nome do fornecedor
	 * @return Nome do fornecedor
	 */
	std::string getFornecedor() const;

	/**
	 * Metodo que permite obter o codigo da encomenda
	 * @return Codigo da encomenda
	 */
	unsigned long getCodigo() const;

	/**
	 * Metodo que permite obter o codigo do produto encomendado
	 * @return Codigo do produto
	 */
	unsigned long getProd() const;

	/**
	 * Metodo que permite obter a quantidade de produto encomendado
	 * @return Quantidade de produto
	 */
	unsigned getQuant() const;

	/**
	 * Metodo que permite alterar o fornecedor
	 * @param f Nome do fornecedor
	 */
	void setFornecedor(std::string f);

	/**
	 * Metodo que permite alterar o codigo do produto encomendado
	 * @param p Codigo do produto
	 */
	void setProd(unsigned long p);

	/**
	 * Metodo que permite alterar a quantidade de produto encomendado
	 * @param q Quantidade de produto
	 */
	void setQuant(unsigned q);

	/**
	 * Metodo que permite alterar o codigo da encomenda
	 * @param c Codigo da encomenda
	 */
	void setCodigo(unsigned long c);

	/**
	 * Metodo que mostra no ecra os dados da encomenda
	 */
	void showInfo();

	/**
	 * Metodo que compara duas encomendas
	 * @param e Encomenda a ser comparada
	 * @return Retorna true se o stock do produto respetivo for superior. Retorna false, caso contrario
	 */
	bool operator< (const Encomenda &e) const;

	/**
	 * Overload do operador << que permite exportar uma encomenda
	 * @param output Variavel para onde se quer exportar(ecra, ficheiro, ...)
	 * @param e1 Encomenda que se quer exportar
	 * @return Retorna referencia para a variavel para onde se quer exportar
	 */
	friend std::ostream& operator<<(std::ostream &output, const Encomenda &e1);
};

#endif
