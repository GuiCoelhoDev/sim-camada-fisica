#ifndef __ENLACE__H__
#define __ENLACE__H__
#endif //!__ENLACE__H__

#include <iostream>
#include <vector>
#include <bitset>
#include <sstream>

using namespace std;

void CamadaEnlaceDadosTransmissora(vector<int> quadro, int codificacao, int enquadramento, int controleDeErro);
void CamadaEnlaceDadosTransmissoraEnquadramento(vector<int> &quadro, int enquadramento);
void CamadaDeEnlaceTransmissoraControleDeErro(vector<int> &quadro, int controleDeErro);

//Enquadradores
vector<int> CamadaDeEnlaceTransmissoraEnquadramentoContagemDeCaracteres(vector<int>);
vector<int> CamadaDeEnlaceTransmissoraEnquadramentoInsercaoDeBits(vector<int>);
vector<int> CamadaDeEnlaceTransmissoraEnquadramentoInsercaoDeBytes(vector<int> quadro);
//Enquadradores

//ControleDeErroTransmissao_BEGIN
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(vector<int> quadro);
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(vector<int> quadro);
void CamadaEnlaceDadosTransmissoraControleDeErroControleDeErroCRC(vector<int> &quadro);
void CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(vector<int> &quadro);
//ControleDeErroTransmissao_END

void CamadaDeEnlaceReceptoraEnquadramento(vector<int> &quadro, int enquadramento);
void CamadaDeEnlaceReceptoraControleDeErro(vector<int> &quadro, int controleDeErro);

//Desenquadradores
vector<int> CamadaDeEnlaceReceptoraEnquadramentoContagemDeCaracteres(vector<int>);
vector<int> CamadaDeEnlaceReceptoraEnquadramentoInsercaoDeBits(vector<int>);
vector<int> CamadaDeEnlaceReceptoraEnquadramentoInsercaoDeBytes(vector<int>);
//Desenquadradores

//ControleDeErroRecepcao_BEGIN
void CamadaEnlaceDadosReceptoraControleDeErroBitDeParidadePar(vector<int> quadro);
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(vector<int> quadro);
void CamadaEnlaceDadosReceptoraControleDeErroControleDeErroCRC(vector<int> &quadro);
void CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(vector<int> &quadro);
//ControleDeErroRecepcao_END

void CamadaEnlaceDadosReceptora(vector<int> quadro, int codificacao, int enquadramento, int controleDeErro);

//Métodos auxiliares
vector<int> bitset_str_to_vector(std::string set);
