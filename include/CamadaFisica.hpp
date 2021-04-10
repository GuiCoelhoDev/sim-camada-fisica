#ifndef __FISICA__H__
#define __FISICA__H__
#endif //!__FISICA__H__

#include <iostream>
#include <vector>
#include <bitset>
#include <sstream>
#include <iterator>
#include <unistd.h>
using namespace std;

void AplicacaoTransmissora(void);

void CamadaDeAplicacaoTransmissora(std::string mensagem, int codificacao, int enquadramento, int controleDeErro);

void CamadaFisicaTransmissora(vector<int> quadro, int codificacao, int enquadramento, int controleDeErro);

//Codificadores
vector<int> CamadaFisicaTransmissoraCodificacaoBinaria(vector<int> quadro);

vector<int> CamadaFisicaTransmissoraCodificacaoManchester(vector<int> quadro);

vector<int> CamadaFisicaTransmissoraCodificacaoManchesterDiferencial(vector<int> quadro);
//Codificadores

void MeioDeComunicacao(vector<int> fluxoBrutoDeBits, int codificacao, int enquadramento, int controleDeErro);
void CamadaFisicaReceptora(vector<int> quadro, int codificacao, int enquadramento, int controleDeErro);

//Decodificadores
vector<int> CamadaFisicaReceptoraDecodificacaoBinaria(vector<int> quadro);

vector<int> CamadaFisicaReceptoraDecodificacaoManchester(vector<int> quadro);

vector<int> CamadaFisicaReceptoraDecodificacaoManchesterDiferencial(vector<int> quadro);

void CamadaDeAplicacaoReceptora(vector<int> quadro);
void AplicacaoReceptora(void);

//Funções Auxiliares

void printEncoding(vector<int> encoding);
