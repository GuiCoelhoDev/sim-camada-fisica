#include "CamadaEnlace.hpp"
#include "CamadaFisica.hpp"

//Camadas
void AplicacaoTransmissora(void)
{
    //Pede ao usuário uma mensagem (string) , tipo de codificação (int) e tipo de enquadramento (int)
    std::string mensagem;
    int codificacao = 0;
    int enquadramento = 0;
    int controleDeErro = 0;
    std::cout << "Digite uma mensagem:" << std::endl;
    getline(cin, mensagem);

    while (codificacao < 1 || codificacao > 3)
    {
        cout << "Escolha uma codificação" << endl;
        cout << "(1) Binária" << endl;
        cout << "(2) Manchester" << endl;
        cout << "(3) Manchester diferencial" << endl;
        cout << " >> ";
        std::cin >> codificacao;
    }
    while (enquadramento < 1 || enquadramento > 4)
    {
        cout << "Escolha o tipo de enquadramento" << endl;
        cout << "(1) Contagem de Caracteres" << endl;
        cout << "(2) Insercao de Bytes" << endl;
        cout << "(3) Inserção de Bits" << endl;
        cout << "(4) Violação da camada" << endl;
        cout << " >> ";
        std::cin >> enquadramento;
    }

    while (controleDeErro < 1 || controleDeErro > 4)
    {
        cout << "Escolha o tipo de controleDeErro" << endl;
        cout << "(1) Bit paridade par" << endl;
        cout << "(2) Bit paridade impar" << endl;
        cout << "(3) CRC" << endl;
        cout << "(4) Codigo de Hamming" << endl;
        cout << " >> ";
        std::cin >> controleDeErro;
    }

    CamadaDeAplicacaoTransmissora(mensagem, codificacao, enquadramento, controleDeErro);
}

void CamadaDeAplicacaoTransmissora(std::string mensagem, int codificacao, int enquadramento, int controleDeErro)
{
    //Transforma mensagem (string) em trem de bits (int)

    vector<int> quadro;     //mensagem transformada em vetor de bits (int 0/1)
    for (auto c : mensagem) //itera em cada char da mensagem
    {
        std::string set = bitset<8>(c).to_string(); //transforma o char em 8 bits

        for (auto &set_elem : set)
        {
            set_elem = set_elem - '0'; // converte elemento de char 48/49 para o inteiro 0/1
        }
        vector<int> bits(set.begin(), set.end()); // constroi um vetor com os 8 bits dessa iteração

        quadro.insert(quadro.end(), bits.begin(), bits.end()); //quadro recebe o vetor "bits" de 8 elementos em cada it
    }

    CamadaEnlaceDadosTransmissora(quadro, codificacao, enquadramento, controleDeErro);
}

void CamadaFisicaTransmissora(vector<int> quadro, int codificacao, int enquadramento, int controleDeErro)
{
    cout << "\nEntrou na Camada Física Transmissora!" << endl;

    cout << "\nTamanho do quadro enviado: " << quadro.size() << " bits" << endl;
    // int tipoDeCodificacao = codificacao; //alterar de acordo o teste
    vector<int> fluxoBrutoDeBits; //ATENÇÃO: trabalhar com BITS!!!
    switch (codificacao)
    {
    case 1: //codificao binaria
        fluxoBrutoDeBits = quadro;
        break;
    case 2: //codificacao manchester
        fluxoBrutoDeBits = CamadaFisicaTransmissoraCodificacaoManchester(quadro);
        break;
    case 3: //codificacao manchester diferencial
        fluxoBrutoDeBits = CamadaFisicaTransmissoraCodificacaoManchesterDiferencial(quadro);
        break;
    default:
        fluxoBrutoDeBits = quadro;
        break;
    } //fim do switch/case
    cout << "\nTamanho da mensagem codificada: " << fluxoBrutoDeBits.size() << " bits" << endl;
    MeioDeComunicacao(fluxoBrutoDeBits, codificacao, enquadramento, controleDeErro); //fluxo de bits codificado
}

//Codificadores_BEGIN

vector<int> CamadaFisicaTransmissoraCodificacaoManchester(vector<int> quadro)
{
    vector<int> clock = {0, 1};
    vector<int> encoding; //resultado da codificação manchester
    for (auto bit : quadro)
    {
        for (auto c : clock)
        {
            encoding.push_back((c xor bit));
        }
    }
    return encoding;
}

vector<int> CamadaFisicaTransmissoraCodificacaoManchesterDiferencial(vector<int> quadro)
{
    vector<int> encoding;
    int nextValue;
    int lastValue;
    encoding.push_back(1);
    for (auto a : quadro)
    {
        lastValue = encoding.back();
        nextValue = (a == 0) ? not(lastValue) : lastValue;

        encoding.push_back(nextValue);
        encoding.push_back(not(nextValue));
    }
    encoding.erase(encoding.begin());
    // cout << encoding.size() << endl;
    return encoding;
}

//Codificadores_END

void MeioDeComunicacao(vector<int> fluxoBrutoDeBits, int codificacao, int enquadramento, int controleDeErro)
{

    cout << "\nEntrou no protocolo Meio De Comunicação" << endl;

    switch (codificacao)
    {
    case 1:
        cout << " Binária" << endl;
        break;
    case 2:
        cout << " Manchester" << endl;
        break;
    case 3:
        cout << " Manchester Diferencial" << endl;
        break;
    default:
        cout << " Binária por default" << endl;
        break;
    }

    switch (enquadramento)
    {
    case 1:
        cout << " Contagem Caracters" << endl;
        break;
    case 2:
        cout << " Bytes" << endl;
        break;
    case 3:
        cout << "Bits" << endl;
        break;
    case 4:
        cout << "Violação" << endl;
        break;
    default:
        cout << " Contagem por default" << endl;
        break;
    }

    vector<int> fluxoBrutoDeBitsPontoA = fluxoBrutoDeBits;
    vector<int> fluxoBrutoDeBitsPontoB;

    cout << "\nBitsNoPontoA:" << endl;

    for (auto e = fluxoBrutoDeBitsPontoA.begin(); e < fluxoBrutoDeBitsPontoA.end(); e += 2)
    {
        cout << char(*e + '0') << char(*(e + 1) + '0') << " ";
    }

    cout << endl;
    cout << endl;
    int retry_times = 5;
    srand(time(nullptr));
    for (int i = retry_times; i > 0; --i)
    {
        fluxoBrutoDeBitsPontoB.clear();
        bool errorwillocurr = rand() % 2;
        if (errorwillocurr)
        {
            int bitRange = fluxoBrutoDeBitsPontoA.size();
            int errorPos[2] = {rand() % bitRange, rand() % bitRange};

            for (int i = 0; i < fluxoBrutoDeBitsPontoA.size(); ++i)
            {
                if (i == errorPos[0] || i == errorPos[1])
                {
                    cout << i << endl;
                    fluxoBrutoDeBitsPontoB.push_back(!fluxoBrutoDeBitsPontoA[i]);
                }
                else
                {
                    fluxoBrutoDeBitsPontoB.push_back(fluxoBrutoDeBitsPontoA[i]);
                }
            }
        }
        else
        {
            for (int c : fluxoBrutoDeBitsPontoA)
                fluxoBrutoDeBitsPontoB.push_back(c);
        }

        cout << "\nBitsNoPontoB:" << endl;

        for (auto e = fluxoBrutoDeBitsPontoB.begin(); e < fluxoBrutoDeBitsPontoB.end(); e += 2)
        {
            cout << char(*e + '0') << char(*(e + 1) + '0') << " ";
        }
        cout << endl;
        try
        {
            CamadaFisicaReceptora(fluxoBrutoDeBitsPontoB, codificacao, enquadramento, controleDeErro);
            // if message could be sent, stop
            break;
        }
        catch (const std::domain_error &e)
        {
            std::cout << "Erro no meio de comunicação: " << std::endl;
            std::cerr << e.what() << '\n';
            std::cerr << "Tentando novamente em 1s" << std::endl;
            sleep(1);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Erro ao decodificar a mensagem" << std::endl;
            std::cerr << "Tentando novamente em 1s" << std::endl;
            sleep(1);
        }
    }
    if (!retry_times)
    {
        std::cout << "Não foi possível enviar a mensagem ao receptor, saindo" << std::endl;
    }
}

void CamadaFisicaReceptora(vector<int> quadro, int codificacao, int enquadramento, int controleDeErro)
{
    cout << "\nEntrou na Camada Física Receptora!" << endl;
    cout << "\nTamanho do quadro recebido: " << quadro.size() << " bits" << endl; //Espero 79 bits aki
    int tipoDeCodificacao = codificacao;                                          //alterar de acordo o teste
    vector<int> fluxoBrutoDeBits;                                                 //ATENÇÃO: trabalhar com BITS!!!
    switch (codificacao)
    {
    case 1: //codificao binaria
        fluxoBrutoDeBits = CamadaFisicaReceptoraDecodificacaoBinaria(quadro);

        break;
    case 2: //codificacao manchester
        fluxoBrutoDeBits = CamadaFisicaReceptoraDecodificacaoManchester(quadro);
        break;
    case 3: //codificacao manchester diferencial
        fluxoBrutoDeBits = CamadaFisicaReceptoraDecodificacaoManchesterDiferencial(quadro);
        break;
    default:
        fluxoBrutoDeBits = quadro;
        break;
    }

    cout << "\nTamanho da mensagem decodificada: " << fluxoBrutoDeBits.size() << " bits" << endl;
    cout << "\nMensagem decodificada: ";
    printEncoding(fluxoBrutoDeBits);
    CamadaEnlaceDadosReceptora(fluxoBrutoDeBits, codificacao, enquadramento, controleDeErro);
}

//Decodificadores_BEGIN

vector<int> CamadaFisicaReceptoraDecodificacaoBinaria(vector<int> quadro)
{
    return quadro;
}

vector<int> CamadaFisicaReceptoraDecodificacaoManchester(vector<int> quadro)
{
    vector<int> clock = {0, 1};
    vector<int> decoding; //resultado da codificação manchester
    for (auto i = 0; i < quadro.size(); i += 2)
    {
        int clk_idx = i % 2;
        int result_xor = (clock[clk_idx] xor quadro[i]) && (clock[clk_idx + 1] xor quadro[i + 1]);
        decoding.push_back(result_xor);
    }
    return decoding;
}

vector<int> CamadaFisicaReceptoraDecodificacaoManchesterDiferencial(vector<int> quadro)
{
    // preappend 1 ao quadro
    quadro.insert(quadro.begin(), 1);
    vector<int> decoding;
    for (auto q = quadro.begin() + 1; q < quadro.end(); q += 2)
    {
        decoding.push_back(*q == *(q - 1));
    }

    return decoding;
}
//Decodificadores_END

void CamadaDeAplicacaoReceptora(vector<int> quadro)
{
    stringstream message;
    int contador = 0;
    for (auto q = quadro.begin(); q < quadro.end(); q += 8)
    {
        stringstream ss;
        // copia de 8 em 8 bits pra stream
        copy(q, q + 8, ostream_iterator<int>(ss, ""));
        // converte a cada 8 bits copiados acima pra um char
        // bitset(ss.str) -> converte pra int -> casta pra char
        message << static_cast<char>(std::bitset<8>(ss.str()).to_ulong());
    }
    cout << "Mensagem recebida: ";
    cout << message.str() << endl;
}

//Métodos auxiliares

void printEncoding(vector<int> encoding)
{
    for (auto e = encoding.begin(); e < encoding.end(); e += 2)
    {
        cout << char(*e + '0') << char(*(e + 1) + '0') << " ";
    }
    cout << endl;
}