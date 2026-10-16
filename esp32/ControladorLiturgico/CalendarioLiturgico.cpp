#include "CalendarioLiturgico.h"

namespace {

struct CelebracaoFixa {
  uint8_t mes;
  uint8_t dia;
  const char* nome;
  TipoCelebracao tipo;
  CorLiturgica cor;
};

// Espelho da tabela-fonte do poc (poc/src/calendario/calendario-liturgico.ts).
// Fonte dos dados: calendario/2026-pt-General-G.ics e calendario/2027-pt-General-G.ics.
// Apenas solenidades [S], festas [F] e memórias obrigatórias [M]; comemorações
// facultativas [m]/[m*] ficam de fora e mantêm a cor da estação.
const CelebracaoFixa CELEBRACOES_FIXAS[] = {
  { 1, 1, "Santa Maria, Mãe de Deus", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO },
  { 1, 2, "Santos Basilio Magno e Gregório de Nazianzo, bispos e doutores da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 1, 6, "Epifania do Senhor", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO },
  { 1, 17, "Santo Antão, abade", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 1, 21, "Santa Inês, virgem e mártir", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 1, 24, "São Francisco de Sales, bispo e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 1, 25, "Conversão de São Paulo, apóstolo", TipoCelebracao::FESTA, CorLiturgica::BRANCO },
  { 1, 26, "Santos Timóteo e Tito, bispos", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 1, 28, "São Tomás de Aquino, presbítero e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 1, 31, "São João Bosco, presbítero", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 2, 2, "Apresentação do Senhor", TipoCelebracao::FESTA, CorLiturgica::BRANCO },
  { 2, 5, "Santa Águeda, virgem e mártir", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 2, 6, "Santos Paulo Miki e Companheiros, mártires", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 2, 10, "Santa Escolástica, virgem", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 2, 14, "Santos Cirilo, monge, e Metódio, bispo", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 2, 22, "Cadeira de São Pedro, apóstolo", TipoCelebracao::FESTA, CorLiturgica::BRANCO },
  { 3, 19, "São José, esposo da Virgem Santa Maria", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO },
  { 3, 25, "Anunciação do Senhor", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO },
  { 4, 7, "São João Batista de La Salle, presbítero", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 4, 25, "São Marcos, evangelista", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 4, 29, "Santa Catarina de Siena, virgem e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 5, 2, "Santo Atanasio, bispo e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 5, 3, "Santos Filipe e Tiago, apóstolos", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 5, 14, "São Matias, apóstolo", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 5, 26, "São Filipe Néri, presbítero", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 5, 31, "Visitação da Nossa Senhora", TipoCelebracao::FESTA, CorLiturgica::BRANCO },
  { 6, 1, "São Justino, mártir", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 6, 3, "Santos Carlos Lwanga e Companheiros, mártires", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 6, 5, "São Bonifácio, bispo e mártir", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 6, 11, "São Barnabé, apóstolo", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 6, 21, "São Luís Gonzaga, religioso", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 6, 24, "Nascimento de São João Batista", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO },
  { 6, 28, "Santo Ireneu, bispo, mártir e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 6, 29, "Santos Pedro e Paulo, apóstolos", TipoCelebracao::SOLENIDADE, CorLiturgica::VERMELHO },
  { 7, 3, "São Tomé, apóstolo", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 7, 11, "São Bento, abade", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 7, 15, "São Boaventura, bispo e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 7, 22, "Santa Maria Madalena", TipoCelebracao::FESTA, CorLiturgica::BRANCO },
  { 7, 25, "São Tiago, apóstolo", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 7, 26, "Santos Joaquim e Ana, pais da Virgem Santa Maria", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 7, 29, "Santos Marta, Maria e Lázaro", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 7, 31, "Santo Inácio de Loiola, presbítero", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 8, 1, "Santo Afonso de Ligório, bispo e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 8, 4, "São João Maria Vianney, presbítero", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 8, 6, "Transfiguração do Senhor", TipoCelebracao::FESTA, CorLiturgica::BRANCO },
  { 8, 8, "São Domingos, presbítero", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 8, 10, "São Lourenço, diácono e mártir", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 8, 11, "Santa Clara, virgem", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 8, 14, "São Maximiliano Maria Kolbe, presbitero e mártir", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 8, 15, "Assunção da Virgem Santa Maria", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO },
  { 8, 20, "São Bernardo, abade e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 8, 21, "São Pio X, papa", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 8, 22, "Virgem Santa Maria, Rainha", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 8, 24, "São Bartolomeu, apóstolo", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 8, 27, "Santa Mónica", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 8, 28, "Santo Agostinho, bispo e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 8, 29, "Martírio de São João Batista", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 9, 3, "São Gregorio Magno, papa e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 9, 8, "Natividade da Virgem Santa Maria", TipoCelebracao::FESTA, CorLiturgica::BRANCO },
  { 9, 13, "São João Crisóstomo, bispo e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 9, 14, "Exaltação da Santa Cruz", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 9, 15, "Nossa Senhora das Dores", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 9, 16, "Santos Cornélio, papa, e Cipriano, bispo, mártires", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 9, 20, "Santos André Kim Taegon, presbítero, Paulo Chong Hasang e Companheiros, mártires", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 9, 21, "São Mateus, apóstolo e evangelista", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 9, 23, "São Pio de Pietrelcina, presbítero", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 9, 27, "São Vicente de Paulo, presbítero", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 9, 29, "Santos Miguel, Gabriel e Rafael, Arcanjos", TipoCelebracao::FESTA, CorLiturgica::BRANCO },
  { 9, 30, "São Jerônimo, presbítero e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 10, 1, "Santa Teresa do Menino Jesus, virgem e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 10, 2, "Santos Anjos da Guarda", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 10, 4, "São Francisco de Assis, religioso", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 10, 7, "Nossa Senhora do Rosário", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 10, 15, "Santa Teresa de Jesus, virgem e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 10, 17, "Santo Inácio de Antioquia, bispo e mártir", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 10, 18, "São Lucas, evangelista", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 10, 28, "Santos Simão e Judas, apóstolos", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 11, 1, "Todos os Santos", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO },
  { 11, 4, "São Carlos Borromeu, bispo", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 11, 9, "Dedicação da Basílica de Latrão", TipoCelebracao::FESTA, CorLiturgica::BRANCO },
  { 11, 10, "São Leão Magno, papa e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 11, 11, "São Martinho de Tours, bispo", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 11, 12, "São Josafá, bispo e mártir", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 11, 17, "Santa Isabel da Hungría, religiosa", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 11, 21, "Apresentação de Nossa Senhora", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 11, 22, "Santa Cecília, virgem e mártir", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 11, 24, "Santos André Dũng Lạc, presbítero, e Companheiros, mártires", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 11, 30, "Santo André, apóstolo", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 12, 3, "São Francisco Xavier, presbítero", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 12, 7, "Santo Ambrósio, bispo e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 12, 8, "Imaculada Conceição da Virgem Santa Maria", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO },
  { 12, 13, "Santa Luzia, virgem e mártir", TipoCelebracao::MEMORIA, CorLiturgica::VERMELHO },
  { 12, 14, "São João da Cruz, presbítero e doutor da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO },
  { 12, 25, "Natal do Senhor", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO },
  { 12, 26, "Santo Estêvão, primeiro mártir", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
  { 12, 27, "São João, apóstolo e evangelista", TipoCelebracao::FESTA, CorLiturgica::BRANCO },
  { 12, 28, "Santos Inocentes, mártires", TipoCelebracao::FESTA, CorLiturgica::VERMELHO },
};

}  // namespace

PeriodoLiturgico CalendarioLiturgico::obterPeriodoLiturgico(const DataCivil& data) const {
  const int16_t ano = data.ano;
  const DataCivil pascoa = CalculadoraPascoa::calcularDataDaPascoa(ano);
  const DataCivil inicioDoAdvento = calcularInicioDoAdvento(ano);
  const DataCivil quartaFeiraDeCinzas = calcularQuartaFeiraDeCinzas(ano);
  const DataCivil domingoDeRamos = pascoa.adicionarDias(-7);
  const DataCivil sabadoSanto = pascoa.adicionarDias(-1);
  const DataCivil inicioDoTempoComum = pascoa.adicionarDias(50);
  const DataCivil inicioDoNatalAtual(ano, 12, 25);
  const DataCivil fimDoNatalAtual(ano + 1, 1, 6);
  const DataCivil inicioDoNatalAnterior(ano - 1, 12, 25);
  const DataCivil fimDoNatalAnterior(ano, 1, 6);

  if (data.ehPosteriorOuIgualA(inicioDoAdvento)
    && data.ehAnteriorOuIgualA(DataCivil(ano, 12, 24))) {
    return criarPeriodo(
      TempoLiturgico::ADVENTO,
      CorLiturgica::VIOLETA,
      inicioDoAdvento,
      DataCivil(ano, 12, 24)
    );
  }

  const bool estaNoNatal = (data.ehPosteriorOuIgualA(inicioDoNatalAtual)
      && data.ehAnteriorOuIgualA(fimDoNatalAtual))
    || (data.ehPosteriorOuIgualA(inicioDoNatalAnterior)
      && data.ehAnteriorOuIgualA(fimDoNatalAnterior));

  if (estaNoNatal) {
    const DataCivil inicio = data.ehPosteriorOuIgualA(inicioDoNatalAtual)
      ? inicioDoNatalAtual
      : inicioDoNatalAnterior;
    const DataCivil fim = data.ehAnteriorOuIgualA(fimDoNatalAnterior)
      ? fimDoNatalAnterior
      : fimDoNatalAtual;

    return criarPeriodo(TempoLiturgico::NATAL, CorLiturgica::BRANCO, inicio, fim);
  }

  if (data.ehPosteriorOuIgualA(quartaFeiraDeCinzas) && data.ehAnteriorA(domingoDeRamos)) {
    return criarPeriodo(
      TempoLiturgico::QUARESMA,
      CorLiturgica::VIOLETA,
      quartaFeiraDeCinzas,
      domingoDeRamos.adicionarDias(-1)
    );
  }

  if (data.ehPosteriorOuIgualA(domingoDeRamos) && data.ehAnteriorOuIgualA(sabadoSanto)) {
    return criarPeriodo(
      TempoLiturgico::SEMANA_SANTA,
      CorLiturgica::VERMELHO,
      domingoDeRamos,
      sabadoSanto
    );
  }

  if (data.ehIgual(pascoa)) {
    return criarPeriodo(TempoLiturgico::PASCOA, CorLiturgica::BRANCO, pascoa, pascoa);
  }

  if (data.ehPosteriorA(pascoa) && data.ehAnteriorA(inicioDoTempoComum)) {
    return criarPeriodo(
      TempoLiturgico::TEMPO_PASCAL,
      CorLiturgica::BRANCO,
      pascoa.adicionarDias(1),
      inicioDoTempoComum.adicionarDias(-1)
    );
  }

  if (data.ehPosteriorOuIgualA(inicioDoTempoComum) && data.ehAnteriorA(inicioDoAdvento)) {
    return criarPeriodo(
      TempoLiturgico::TEMPO_COMUM,
      CorLiturgica::VERDE,
      inicioDoTempoComum,
      inicioDoAdvento.adicionarDias(-1)
    );
  }

  return criarPeriodo(
    TempoLiturgico::TEMPO_COMUM,
    CorLiturgica::VERDE,
    DataCivil(ano, 1, 1),
    DataCivil(ano, 12, 31)
  );
}

DiaLiturgico CalendarioLiturgico::obterDiaLiturgico(const DataCivil& data) const {
  DiaLiturgico dia = {};
  dia.data = data;
  dia.periodo = obterPeriodoLiturgico(data);
  dia.quantidadeDeCelebracoes = 0;

  const int16_t ano = data.ano;
  const DataCivil pascoa = CalculadoraPascoa::calcularDataDaPascoa(ano);
  const DataCivil epifania(ano, 1, 6);
  const DataCivil batismoDoSenhor = epifania.adicionarDias(
    (7 - static_cast<int8_t>(epifania.obterDiaDaSemana())) % 7
  );
  const bool ehDomingo = data.obterDiaDaSemana() == DiaDaSemana::DOMINGO;

  const DataCivil quartaFeiraDeCinzas = pascoa.adicionarDias(-46);
  adicionarMovelSeIgual(dia, data, quartaFeiraDeCinzas, "Quarta-feira de Cinzas", TipoCelebracao::SEMANA, CorLiturgica::VIOLETA);
  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(-7), "Domingo de Ramos", TipoCelebracao::SOLENIDADE, CorLiturgica::VERMELHO);
  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(-3), "Quinta-feira Santa", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(-2), "Sexta-feira Santa", TipoCelebracao::SOLENIDADE, CorLiturgica::VERMELHO);
  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(-1), "Sábado Santo", TipoCelebracao::SOLENIDADE, CorLiturgica::VERMELHO);
  adicionarMovelSeIgual(dia, data, pascoa, "Domingo de Páscoa", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);

  static const char* const oitava[] = {
    "Segunda-Feira na Oitava da Páscoa",
    "Terça-Feira na Oitava da Páscoa",
    "Quarta-Feira na Oitava da Páscoa",
    "Quinta-Feira na Oitava da Páscoa",
    "Sexta-Feira na Oitava da Páscoa",
    "Sábado na Oitava da Páscoa",
  };
  for (uint8_t diaOitava = 1; diaOitava <= 6; diaOitava++) {
    adicionarMovelSeIgual(
      dia,
      data,
      pascoa.adicionarDias(static_cast<int32_t>(diaOitava)),
      oitava[diaOitava - 1],
      TipoCelebracao::SOLENIDADE,
      CorLiturgica::BRANCO
    );
  }

  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(42), "Ascensão do Senhor", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(49), "Pentecostes", TipoCelebracao::SOLENIDADE, CorLiturgica::VERMELHO);
  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(50), "Bem-aventurada Virgem Maria, Mãe da Igreja", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO);
  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(56), "Santíssima Trindade", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(63), "Santíssimo Corpo e Sangue de Cristo", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(68), "Sagrado Coração de Jesus", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  adicionarMovelSeIgual(dia, data, pascoa.adicionarDias(69), "Imaculado Coração da Virgem Santa Maria", TipoCelebracao::MEMORIA, CorLiturgica::BRANCO);
  adicionarMovelSeIgual(dia, data, calcularInicioDoAdvento(ano).adicionarDias(-7), "Nosso Senhor Jesus Cristo, Rei do Universo", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  adicionarMovelSeIgual(dia, data, calcularSagradaFamilia(ano), "Sagrada Família de Jesus, Maria e José", TipoCelebracao::FESTA, CorLiturgica::BRANCO);
  adicionarMovelSeIgual(dia, data, batismoDoSenhor, "Batismo do Senhor", TipoCelebracao::FESTA, CorLiturgica::BRANCO);

  // Celebrações de data fixa, com supressão de festa/memória que cai num domingo.
  for (const CelebracaoFixa& fixa : CELEBRACOES_FIXAS) {
    if (data.mes != fixa.mes || data.dia != fixa.dia) continue;
    if (ehDomingo && (fixa.tipo == TipoCelebracao::FESTA || fixa.tipo == TipoCelebracao::MEMORIA)) {
      continue;
    }
    adicionarCelebracao(dia, fixa.nome, fixa.tipo, fixa.cor);
  }

  if (ehDomingo && dia.periodo.tempo != TempoLiturgico::SEMANA_SANTA) {
    adicionarCelebracao(
      dia,
      obterNomeDoDomingo(dia.periodo.tempo),
      TipoCelebracao::DOMINGO,
      dia.periodo.corPadrao
    );
  }

  if (dia.quantidadeDeCelebracoes == 0) {
    adicionarCelebracao(
      dia,
      obterNomePadraoDoPeriodo(dia.periodo.tempo),
      TipoCelebracao::SEMANA,
      dia.periodo.corPadrao
    );
  }

  ordenarCelebracoesPorPrecedencia(dia);
  dia.cor = dia.celebracoes[0].cor;
  return dia;
}

CorLiturgica CalendarioLiturgico::obterCorDoDia(const DataCivil& data) const {
  return obterDiaLiturgico(data).cor;
}

DataCivil CalendarioLiturgico::calcularInicioDoAdvento(int16_t ano) {
  const DataCivil natal(ano, 12, 25);
  const int8_t diasDesdeDomingo = static_cast<int8_t>(natal.obterDiaDaSemana());
  const DataCivil domingoAntesDoNatal = natal.adicionarDias(-diasDesdeDomingo);
  return domingoAntesDoNatal.adicionarDias(-21);
}

DataCivil CalendarioLiturgico::calcularQuartaFeiraDeCinzas(int16_t ano) {
  return CalculadoraPascoa::calcularDataDaPascoa(ano).adicionarDias(-46);
}

DataCivil CalendarioLiturgico::calcularSagradaFamilia(int16_t ano) {
  const DataCivil natal(ano, 12, 25);
  const int8_t diasAteDomingo = (7 - static_cast<int8_t>(natal.obterDiaDaSemana())) % 7;
  const DataCivil proximoDomingo = natal.adicionarDias(diasAteDomingo);
  if (proximoDomingo.ehIgual(natal)) {
    return DataCivil(ano, 12, 30);
  }
  return proximoDomingo;
}

PeriodoLiturgico CalendarioLiturgico::criarPeriodo(
  TempoLiturgico tempo,
  CorLiturgica corPadrao,
  const DataCivil& inicio,
  const DataCivil& fim
) {
  PeriodoLiturgico periodo = { tempo, corPadrao, inicio, fim };
  return periodo;
}

void CalendarioLiturgico::adicionarCelebracao(
  DiaLiturgico& dia,
  const char* nome,
  TipoCelebracao tipo,
  CorLiturgica cor
) {
  if (dia.quantidadeDeCelebracoes >= DiaLiturgico::QUANTIDADE_MAXIMA_DE_CELEBRACOES) {
    return;
  }

  CelebracaoLiturgica celebracao = { nome, tipo, cor };
  dia.celebracoes[dia.quantidadeDeCelebracoes] = celebracao;
  dia.quantidadeDeCelebracoes++;
}

void CalendarioLiturgico::adicionarMovelSeIgual(
  DiaLiturgico& dia,
  const DataCivil& data,
  const DataCivil& alvo,
  const char* nome,
  TipoCelebracao tipo,
  CorLiturgica cor
) {
  if (data.ehIgual(alvo)) {
    adicionarCelebracao(dia, nome, tipo, cor);
  }
}

void CalendarioLiturgico::ordenarCelebracoesPorPrecedencia(DiaLiturgico& dia) {
  for (uint8_t indice = 0; indice < dia.quantidadeDeCelebracoes; indice++) {
    for (uint8_t proximoIndice = indice + 1; proximoIndice < dia.quantidadeDeCelebracoes; proximoIndice++) {
      if (static_cast<uint8_t>(dia.celebracoes[proximoIndice].tipo)
        > static_cast<uint8_t>(dia.celebracoes[indice].tipo)) {
        const CelebracaoLiturgica temporaria = dia.celebracoes[indice];
        dia.celebracoes[indice] = dia.celebracoes[proximoIndice];
        dia.celebracoes[proximoIndice] = temporaria;
      }
    }
  }
}

const char* CalendarioLiturgico::obterNomeDoDomingo(TempoLiturgico tempo) {
  switch (tempo) {
    case TempoLiturgico::ADVENTO:
      return "Domingo do Advento";
    case TempoLiturgico::QUARESMA:
      return "Domingo da Quaresma";
    case TempoLiturgico::SEMANA_SANTA:
      return "Domingo da Semana Santa";
    case TempoLiturgico::PASCOA:
      return "Domingo de Páscoa";
    case TempoLiturgico::TEMPO_PASCAL:
      return "Domingo do Tempo Pascal";
    case TempoLiturgico::NATAL:
      return "Domingo do Tempo de Natal";
    case TempoLiturgico::TEMPO_COMUM:
      return "Domingo do Tempo Comum";
  }

  return "Domingo";
}

const char* CalendarioLiturgico::obterNomePadraoDoPeriodo(TempoLiturgico tempo) {
  if (tempo == TempoLiturgico::NATAL) {
    return "Tempo de Natal";
  }

  switch (tempo) {
    case TempoLiturgico::ADVENTO:
      return "Advento";
    case TempoLiturgico::TEMPO_COMUM:
      return "Tempo Comum";
    case TempoLiturgico::QUARESMA:
      return "Quaresma";
    case TempoLiturgico::SEMANA_SANTA:
      return "Semana Santa";
    case TempoLiturgico::PASCOA:
      return "Páscoa";
    case TempoLiturgico::TEMPO_PASCAL:
      return "Tempo Pascal";
    case TempoLiturgico::NATAL:
      return "Tempo de Natal";
  }

  return "Tempo Comum";
}
