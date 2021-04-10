
#include "CamadaEnlace.hpp"
#include "CamadaFisica.hpp"
#include <sstream>
#include <iterator>
#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

//Camadas

void CamadaEnlaceDadosTransmissora(vector<int> quadro, int codificacao, int enquadramento, int controleDeErro)
{
    CamadaEnlaceDadosTransmissoraEnquadramento(quadro, enquadramento);
    CamadaDeEnlaceTransmissoraControleDeErro(quadro, controleDeErro);

    //chama proxima camada
    CamadaFisicaTransmissora(quadro, codificacao, enquadramento, controleDeErro);
}

void CamadaEnlaceDadosTransmissoraEnquadramento(vector<int> &quadro, int enquadramento)
{
    //Multiplexa tipos de verificaçao de inicio, fim de quadros e tamanhos

    vector<int> quadroEnquadrado;
    switch (enquadramento)
    {
    case 1: //contagem de caracteres
        quadroEnquadrado =
            CamadaDeEnlaceTransmissoraEnquadramentoContagemDeCaracteres(quadro);
        break;
    case 2: //insercao de bytes

        quadroEnquadrado = CamadaDeEnlaceTransmissoraEnquadramentoInsercaoDeBytes(quadro);
        break;
    case 3: //insercao de bits
        quadroEnquadrado =
            CamadaDeEnlaceTransmissoraEnquadramentoInsercaoDeBits(quadro);
        break;
    }
    quadro.assign(quadroEnquadrado.begin(), quadroEnquadrado.end());
}

void CamadaDeEnlaceTransmissoraControleDeErro(vector<int> &quadro, int controleDeErro)
{
    cout << "Controle de Erro transmissora" << endl;
    switch (controleDeErro)
    {
    case 1:
        CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(quadro); //Bit de paridade par
        break;
    case 2:
        CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(quadro); //bit de paridade impar
        break;
    case 3:
        CamadaEnlaceDadosTransmissoraControleDeErroControleDeErroCRC(quadro);
        break;
    case 4:
        //Hamming
        CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(quadro);
        break;
    default:
        break;
    }
}

//Enquadradores_BEGIN
vector<int> CamadaDeEnlaceTransmissoraEnquadramentoContagemDeCaracteres(vector<int> quadro)
{

    //Transforma quadro (vector<int>) em quadro com contagem de caracters
    vector<int> new_quadro;
    // tamanho do quadro: 4 bytes
    size_t tam_quadro = 4 * 8;

    for (auto q = quadro.begin(); q < quadro.end(); q += tam_quadro)
    {
        auto end_part = q + tam_quadro;
        if (end_part > quadro.end())
        {
            end_part = quadro.end();
        }
        auto insert_tam = end_part - q;
        auto contagem_entry = bitset_str_to_vector(bitset<8>(insert_tam).to_string());
        // insere no quadro o tamanho do quadro em valor binário
        new_quadro.insert(new_quadro.end(), contagem_entry.begin(), contagem_entry.end());
        // insere no quadro o pedaço do tamanho calculado
        new_quadro.insert(new_quadro.end(), q, end_part);
    }

    return new_quadro;
}

vector<int>
CamadaDeEnlaceTransmissoraEnquadramentoInsercaoDeBits(vector<int> quadro)
{
    vector<int> new_quadro;
    vector<int> flag{0, 1, 1, 1, 1, 1, 1, 0};
    // tamanho do quadro: 32 bytes
    size_t tam_quadro = 32 * 8;
    // verificador de sequencia de bits e contador de bits da mensagem
    int verifier = 0, i = 0;
    //Percorre a mensagem recebida como parametro
    for (auto q = quadro.begin(); q <= quadro.end(); q++)
    {
        //verifica sequencia de 5 bits 1
        if ((*q) == 1)
        {
            verifier++;
        }
        //Adiciona flag no inicio do quadro recebido como parametro
        if (q == quadro.begin())
        {
            // insere no quadro a flag no quadro em valor binário
            new_quadro.insert(new_quadro.end(), flag.begin(), flag.end());
        }
        //Adiciona flag no fim do quadro e no inicio do proximo
        else if (i % (int)tam_quadro == 0 && q != quadro.end())
        {
            // insere no quadro a flag no quadro em valor binário
            new_quadro.insert(new_quadro.end(), flag.begin(), flag.end());
            new_quadro.insert(new_quadro.end(), flag.begin(), flag.end());
        }
        //Adiciona flag no fim do quadro recebido como parametro
        else if (q == quadro.end())
        {
            // insere no quadro a flag no quadro em valor binário
            new_quadro.insert(new_quadro.end(), flag.begin(), flag.end());
        }
        // insere o bit de mensagem no novo quadro
        else
        {
            // verifica sequencia de bits e insere o bit no quadro da mensagem
            if (verifier == 5)
            {
                new_quadro.push_back(0);    //insere um zero no quadro
                new_quadro.push_back((*q)); //insere o elemento q no quadro
                verifier = 0;
            }
            //insere o bit no quadro da mensagem
            else
            {
                new_quadro.push_back(*q); //insere o elemento q no quadro
            }
        }
        i++;
    }
    return new_quadro;
}

vector<int> CamadaDeEnlaceTransmissoraEnquadramentoInsercaoDeBytes(vector<int> quadro) //32 bits (4 bytes)
{
    cout << "Inserção de Bytes Recomeço" << endl;
    vector<int> flag = {0, 0, 0, 0, 1, 1, 1, 1};        // 1 byte (esc) 0x1b
    vector<int> escape_flag = {0, 0, 0, 1, 1, 0, 1, 1}; // 1 byte (esc) 0x1b
    int tamanho_quadro_bytes = 4;
    int tamanho_quadro_bits = tamanho_quadro_bytes * 8;
    vector<int> new_quadro;
    vector<int> new_quadro_aux;
    vector<int>::iterator it_quadro;
    it_quadro = quadro.begin();
    int increment;

    for (auto it_quadro = quadro.begin(); it_quadro != quadro.end(); advance(it_quadro, increment))
    {
        vector<int> new_quadro_aux(it_quadro, quadro.end());
        auto sizeAux = new_quadro_aux.size();
        if (sizeAux < 32)
        {
            increment = sizeAux;
        }
        else
        {
            increment = 32;
        }
        new_quadro.insert(new_quadro.end(), flag.begin(), flag.end());
        new_quadro.insert(new_quadro.end(), it_quadro, it_quadro + increment); //Adiciona primeiros 4 bytes
        new_quadro.insert(new_quadro.end(), flag.begin(), flag.end());
    }

    return new_quadro;
}
//Enquadradores_END

//ControleDeErroTransmissao_BEGIN
typedef uint32_t CRC;

unsigned int crc32_table[256];

void PreCalcReminders()
{
    // convert get bitsize from CRC type
    const uint width = 8 * sizeof(CRC);

    // Most significant bit for this crc size
    const uint firstBit = (1 << (width - 1));

    // define polynomial
    const uint32_t CRC32_Polynomial = 0xEDB88320;

    uint32_t crc;

    for (auto div = 0; div < 256; ++div)
    {

        crc = div;
        for (int j = 0; j < 8; j++)
        {

            if (crc & 0x00000001L)
                crc = (crc >> 1) ^ CRC32_Polynomial;
            else
                crc = crc >> 1;
        }
        crc32_table[div] = crc;
    }
}

CRC CalcCRC(vector<int> message)
{
    // prevent zero initial remainder zero
    CRC crc = 0xFFFFFFFF;
    // convert get bitsize from CRC type
    const uint width = 8 * sizeof(CRC);
    for (auto it = message.begin(); it < message.end(); it += 8)
    {
        auto end = it + 8;
        if (end > message.end())
        {
            end = message.end();
        }
        stringstream ss;
        copy(it, end, ostream_iterator<int>(ss, ""));
        uint8_t current_byte = static_cast<uint8_t>(bitset<8>(ss.str()).to_ulong());
        crc = (crc >> 8) ^ crc32_table[(crc ^ (uint32_t)current_byte) & 0x000000FFul];
    }

    // last remainder is the CRC irself
    // xor it by it's final xor value
    crc = crc ^ 0xFFFFFFFFul;

    return crc;
}
void CamadaEnlaceDadosTransmissoraControleDeErroControleDeErroCRC(vector<int> &quadro)
{

    PreCalcReminders();

    auto crc = CalcCRC(quadro);
    auto crc_vec = bitset_str_to_vector(bitset<32>(crc).to_string());

    quadro.insert(quadro.end(), crc_vec.begin(), crc_vec.end());
}
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadePar(vector<int> quadro)
{
    int parity_test = 0;
    int parity_bit = 1;
    for (auto q = quadro.begin(); q <= quadro.end(); q++)
    {
        if ((*q) == 1)
        {
            parity_test++;
        }
    }
    if (parity_test % 2 == 0)
    {
        parity_bit = 0;
    }
    quadro.push_back(parity_bit);

} //fim do metodo CamadaEnlaceDadosTransmissoraControledeErroBitParidadePar
void CamadaEnlaceDadosTransmissoraControleDeErroBitParidadeImpar(vector<int> quadro)
{
    int parity_test = 0;
    int parity_bit = 1;
    for (auto q = quadro.begin(); q <= quadro.end(); q++)
    {
        if ((*q) == 1)
        {
            parity_test++;
        }
    }
    if (parity_test % 2 != 0)
    {
        parity_bit = 0;
    }
    quadro.push_back(parity_bit);
} //fim do metodo CamadaEnlaceDadosTransmissoraControledeErroBitParidadeImpar

void CamadaEnlaceDadosTransmissoraControleDeErroCodigoDeHamming(vector<int> &quadro)
{
    cout << "Transmissora - Hamming" << endl;
    int hammingVector[7];
    int quadroSize = quadro.size();
    vector<int> new_quadro;

    for (int hammingIterator = 0; hammingIterator < quadroSize; hammingIterator += 4)
    {

        hammingVector[0] = quadro[hammingIterator + 0]; //M7
        hammingVector[1] = quadro[hammingIterator + 1]; //M6
        hammingVector[2] = quadro[hammingIterator + 2]; //M5
        hammingVector[4] = quadro[hammingIterator + 3]; //M3

        hammingVector[6] = hammingVector[0] ^ hammingVector[2] ^ hammingVector[4]; //P1
        hammingVector[5] = hammingVector[0] ^ hammingVector[1] ^ hammingVector[4]; //P2
        hammingVector[3] = hammingVector[0] ^ hammingVector[1] ^ hammingVector[2]; //P4

        vector<int> new_quadro_aux(hammingVector + 0, hammingVector + 7);
        new_quadro.insert(new_quadro.end(), new_quadro_aux.begin(), new_quadro_aux.end());
    }

    quadro.assign(new_quadro.begin(), new_quadro.end());
}

//ControleDeErroTransmissao_END

void CamadaEnlaceDadosReceptora(vector<int> quadro, int codificacao, int enquadramento, int controleDeErro)
{
    CamadaDeEnlaceReceptoraControleDeErro(quadro, controleDeErro);
    CamadaDeEnlaceReceptoraEnquadramento(quadro, enquadramento);
    //chama proxima camada
    CamadaDeAplicacaoReceptora(quadro);
}

void CamadaDeEnlaceReceptoraEnquadramento(vector<int> &quadro, int enquadramento)
{
    // int tipoDeEnquadramento = 0; //alterar de acordo com o teste
    vector<int> quadroDesenquadrado;
    switch (enquadramento)
    {
    case 1: //contagem de caracteres
        quadroDesenquadrado =
            CamadaDeEnlaceReceptoraEnquadramentoContagemDeCaracteres(quadro);
        break;
    case 2: //insercao de bytes
        quadroDesenquadrado =
            CamadaDeEnlaceReceptoraEnquadramentoInsercaoDeBytes(quadro);

        break;
    case 3: //insercao de bits
        quadroDesenquadrado =
            CamadaDeEnlaceTransmissoraEnquadramentoInsercaoDeBits(quadro);
        break;
    case 4: //violacao da camada fisica
        // quadroEnquadrado = CamadaDeEnlacCamadaFisicaTransmissora(quadro, codificacao);
        break;
    }
    quadro.assign(quadroDesenquadrado.begin(), quadroDesenquadrado.end());
}
void CamadaDeEnlaceReceptoraControleDeErro(vector<int> &quadro, int controleDeErro)
{

    cout << "Controle de Erro Receptora" << endl;
    switch (controleDeErro)
    {
    case 1:
        CamadaEnlaceDadosReceptoraControleDeErroBitDeParidadePar(quadro); //bit de paridade par
        break;
    case 2:
        CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(quadro); //bit de paridade impar
        break;
    case 3:
        CamadaEnlaceDadosReceptoraControleDeErroControleDeErroCRC(quadro);
        break;
    case 4:
        //Hamming
        CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(quadro);

        break;
    default:
        break;
    }
}

//Desenquadradores_BEGIN

vector<int> CamadaDeEnlaceReceptoraEnquadramentoContagemDeCaracteres(vector<int> quadro)
{
    vector<int> desenquadro;

    for (auto q = quadro.begin(); q < quadro.end();)
    {
        stringstream ss;
        copy(q, q + 8, ostream_iterator<int>(ss, ""));
        q += 8;
        auto quadro_tam = static_cast<uint8_t>(std::bitset<8>(ss.str()).to_ulong());
        auto end_part = q + quadro_tam;
        if (q > quadro.end())
        {
            end_part = quadro.end();
        }
        desenquadro.insert(desenquadro.end(), q, end_part);
        q = end_part;
    }

    return desenquadro;
}

vector<int> CamadaDeEnlaceReceptoraEnquadramentoInsercaoDeBytes(vector<int> quadro)
{

    cout << "CamadaReceptoraInsercaoDeBytes" << endl;
    //Variaveis

    //Verificando flag
    int increment;
    vector<int> desenquadro;
    vector<int>::iterator it_quadro;

    for (auto it_quadro = quadro.begin(); it_quadro < quadro.end(); advance(it_quadro, increment))
    {

        int dec = 0;
        int multiplier = 1;
        vector<int> quadro_verifica_flag1(it_quadro, it_quadro + 8); //Vetor armazena flag
        for (auto i = 7; i >= 0; --i)                                //Verifica se vetor é flag
        {
            dec += (multiplier * quadro_verifica_flag1[i]);
            multiplier *= 2;
        }
        if (dec == 15)
        //Flag verificada
        {
            advance(it_quadro, 8);
            if (it_quadro == quadro.end())
            {
                return desenquadro;
            }
            int dec = 0;
            int multiplier = 1;
            vector<int> quadro_verificaflag2(it_quadro, it_quadro + 8);
            for (auto i = 7; i >= 0; --i) //Verifica se é flag
            {
                dec += (multiplier * quadro_verificaflag2[i]);
                multiplier *= 2;
            }
            if (dec == 15)
            {
                advance(it_quadro, 8);
            }

            vector<int> quadro_size_restante(it_quadro, quadro.end()); //Ve tamanho

            auto sizeAux = quadro_size_restante.size();
            sizeAux = sizeAux - 8;
            if (sizeAux < 32)
            {
                increment = sizeAux;
            }
            else
            {
                increment = 32;
            }

            //Flag encontrada
            //Avanca para Campo útil
            vector<int> desenquadro_aux(it_quadro, it_quadro + increment);                         //Copia quadro todo
            desenquadro.insert(desenquadro.end(), desenquadro_aux.begin(), desenquadro_aux.end()); //Adiciona ao desenquadro
        }
    }

    return desenquadro;
}

vector<int> CamadaDeEnlaceReceptoraEnquadramentoInsercaoDeBits(vector<int> quadro)
{
    vector<int> desenquadro;                 // vetor de mensagem desenquadrada
    vector<int> tag{0, 1, 1, 1, 1, 1, 1, 0}; // tag de enquadramento
    vector<int> seq{1, 1, 1, 1, 1, 0};       // zero de manutencao de informacoes
    //percorre o quadro
    for (auto q = quadro.begin(); q < quadro.end(); q++)
    {
        vector<int> tag_test;                      // teste de tag
        vector<int> seq_test;                      //teste de sequencia de cinco bits 1
        tag_test.insert(tag_test.end(), q, q + 7); //recolhe o byte que começa com q
        seq_test.insert(seq_test.end(), q, q + 5); //recolhe a sequencia de bits que começa com q
        //compara o vetor de teste de tag com a tag
        if (tag_test == tag)
        {
            q += 8;
        }
        //compara o vetor de sequencia de 5 bits 1 com os proximos 6 bits do quadro
        else if (seq_test == seq)
        {
            desenquadro.insert(seq_test.end(), q, q + 4);
            q += 5;
        }
        // adiciona o bit q ao desenquadro
        else
        {
            desenquadro.push_back((*q));
        }
    }
    return desenquadro;
}

//Desenquadradores_END

//ControleDeErroRecepcao_BEGIN
void CamadaEnlaceDadosReceptoraControleDeErroBitDeParidadePar(vector<int> quadro)
{
    int sum = 0;                                          // soma de bits 1 no quadro
    for (auto q = quadro.begin(); q <= quadro.end(); q++) //contabiliza todos os bits 1 do quadro
    {
        if ((*q) == 1)
        {
            sum++;
        }
    }
    if (sum % 2 == 0) // investiga paridade
    {
        cout << "mensagem sem erros" << endl;
    }
    else
    {
        throw std::domain_error("erro! retransmitir mensagem");
    }
    quadro.pop_back(); // retira o bit de deteccao de erros

} //fim do metodo CamadaEnlaceDadosReceptoraControleDeErroBitDeParidadePar
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(vector<int> quadro)
{
    int sum = 0;                                          // soma de bits 1 no quadro
    for (auto q = quadro.begin(); q <= quadro.end(); q++) //contabiliza todos os bits 1 do quadro
    {
        if ((*q) == 1)
        {
            sum++;
        }
    }
    if (sum % 2 != 0) // investiga paridade
    {
        cout << "mensagem sem erros" << endl;
    }
    else
    {
        throw std::domain_error("erro! retransmitir mensagem");
    }
    quadro.pop_back(); // retira o bit de deteccao de erros
} //fim do metodo CamadaEnlaceDadosReceptoraControleDeErroBitDeParidadeImpar
void CamadaEnlaceDadosReceptoraControleDeErroCodigoDeHamming(vector<int> &quadro)
{
    cout << "Receptora - Cod1igodeHamming" << endl;
    int hammingVector[7];
    int hammingData[4];
    int quadroSize = quadro.size();
    vector<int> new_quadro;
    int hammingIterator = 0;
    int c, c1, c2, c3;

    for (hammingIterator = 0; hammingIterator < quadroSize; hammingIterator += 7)
    {
        //Montando vetor de hamming 7 bits [P1,P2,M3,P4,M5,M6,M7]
        hammingVector[0] = quadro[hammingIterator + 0]; //M7
        hammingVector[1] = quadro[hammingIterator + 1]; //M6
        hammingVector[2] = quadro[hammingIterator + 2]; //M5
        hammingVector[3] = quadro[hammingIterator + 3]; //P4
        hammingVector[4] = quadro[hammingIterator + 4]; //M3
        hammingVector[5] = quadro[hammingIterator + 5]; //P2
        hammingVector[6] = quadro[hammingIterator + 6]; //P1

        //Fim_Montagem

        //Verificando erros
        c1 = hammingVector[6] ^ hammingVector[0] ^ hammingVector[2] ^ hammingVector[4]; //P1 + Mn 0
        c2 = hammingVector[5] ^ hammingVector[0] ^ hammingVector[1] ^ hammingVector[4]; //P2 + Mn
        c3 = hammingVector[3] ^ hammingVector[0] ^ hammingVector[1] ^ hammingVector[2]; //P4 + Mn
        c = c3 * 4 + c2 * 2 + c1;
        if (c == 0)
        {
            //Colocando apenas os dados "Carga útil"
            hammingData[0] = hammingVector[0];
            hammingData[1] = hammingVector[1];
            hammingData[2] = hammingVector[2];
            hammingData[3] = hammingVector[4];

            vector<int> new_quadro_aux(hammingData + 0, hammingData + 4);
            new_quadro.insert(new_quadro.end(), new_quadro_aux.begin(), new_quadro_aux.end());
        }
        else
        {

            cout << "\nErro na posição:" << (7 - c) + hammingIterator << endl;
            cout << "\nQuadro recebido : ";
            for (int i = 0; i < 7; i++)
            {
                cout << hammingVector[i];
            }
            //Corrige o erro do bit na posicao C
            if (hammingVector[7 - c] == 0)
            {
                hammingVector[7 - c] = 1;
            }
            else
            {
                hammingVector[7 - c] = 0;
            }
            //Quadro corrigido
            cout << "\nQuadro corrigido:" << endl;
            for (int i = 0; i < 7; i++)
            {
                cout << hammingVector[i];
            }

            cout << endl;

            hammingData[0] = hammingVector[0];
            hammingData[1] = hammingVector[1];
            hammingData[2] = hammingVector[2];
            hammingData[3] = hammingVector[4];

            vector<int> new_quadro_aux(hammingData + 0, hammingData + 4);
            new_quadro.insert(new_quadro.end(), new_quadro_aux.begin(), new_quadro_aux.end());
        }
    }

    quadro.assign(new_quadro.begin(), new_quadro.end());

    //Fim verificação
}

void CamadaEnlaceDadosReceptoraControleDeErroControleDeErroCRC(vector<int> &quadro)
{
    stringstream ss;
    const size_t src_size = 32;

    auto crc_start_pos = quadro.end() - 32;
    // last 8bits are the reminder

    copy(crc_start_pos, quadro.end(), ostream_iterator<int>(ss, ""));

    // calc crc for message
    auto message_crc = CalcCRC(vector<int>(quadro.begin(), crc_start_pos));

    CRC quadro_crc = static_cast<CRC>(bitset<32>(ss.str()).to_ulong());

    // remove crc from quadro
    quadro.erase(crc_start_pos, quadro.end());
    if (message_crc != quadro_crc)
    {
        char buff[100];
        snprintf(buff, sizeof(buff), "Mensagem recebida com erro, crc esperado: 0x%X, recebido: 0x%X", message_crc, quadro_crc);
        std::string buffAsStdStr = buff;
        throw ::domain_error(buffAsStdStr);
    }
}
//ControleDeErroRecepcao_END
//Métodos auxiliares

vector<int>
bitset_str_to_vector(string set)
{
    for (auto &set_elem : set)
    {
        set_elem = set_elem - '0'; // converte elemento de char 48/49 para o inteiro 0/1
    }

    return vector<int>(set.begin(), set.end()); // constroi um vetor com os 8 bits dessa iteração
}