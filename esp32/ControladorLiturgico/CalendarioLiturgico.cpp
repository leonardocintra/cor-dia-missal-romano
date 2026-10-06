#include "CalendarioLiturgico.h"

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
  const DataCivil quartaFeiraDeCinzas = pascoa.adicionarDias(-46);
  const DataCivil epifania(ano, 1, 6);
  const DataCivil batismoDoSenhor = epifania.adicionarDias(
    (7 - static_cast<int8_t>(epifania.obterDiaDaSemana())) % 7
  );

  if (data.ehIgual(quartaFeiraDeCinzas)) {
    adicionarCelebracao(dia, "Quarta-feira de Cinzas", TipoCelebracao::SEMANA, CorLiturgica::VIOLETA);
  } else if (data.ehIgual(pascoa.adicionarDias(-7))) {
    adicionarCelebracao(dia, "Domingo de Ramos", TipoCelebracao::SOLENIDADE, CorLiturgica::VERMELHO);
  } else if (data.ehIgual(pascoa.adicionarDias(-3))) {
    adicionarCelebracao(dia, "Quinta-feira Santa", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(pascoa.adicionarDias(-2))) {
    adicionarCelebracao(dia, "Sexta-feira Santa", TipoCelebracao::SOLENIDADE, CorLiturgica::VERMELHO);
  } else if (data.ehIgual(pascoa.adicionarDias(-1))) {
    adicionarCelebracao(dia, "Sábado Santo", TipoCelebracao::SOLENIDADE, CorLiturgica::VERMELHO);
  } else if (data.ehIgual(pascoa)) {
    adicionarCelebracao(dia, "Domingo de Páscoa", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(pascoa.adicionarDias(39))) {
    adicionarCelebracao(dia, "Ascensão do Senhor", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(pascoa.adicionarDias(49))) {
    adicionarCelebracao(dia, "Pentecostes", TipoCelebracao::SOLENIDADE, CorLiturgica::VERMELHO);
  } else if (data.ehIgual(pascoa.adicionarDias(60))) {
    adicionarCelebracao(dia, "Santíssimo Corpo e Sangue de Cristo", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(DataCivil(ano, 1, 25))) {
    adicionarCelebracao(dia, "Conversão de São Paulo", TipoCelebracao::FESTA, CorLiturgica::BRANCO);
  } else if (data.ehIgual(DataCivil(ano, 2, 2))) {
    adicionarCelebracao(dia, "Apresentação do Senhor", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(DataCivil(ano, 2, 22))) {
    adicionarCelebracao(dia, "Cátedra de São Pedro", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(DataCivil(ano, 6, 24))) {
    adicionarCelebracao(dia, "Natividade de São João Batista", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(DataCivil(ano, 6, 29))) {
    adicionarCelebracao(dia, "São Pedro e São Paulo", TipoCelebracao::SOLENIDADE, CorLiturgica::VERMELHO);
  } else if (data.ehIgual(DataCivil(ano, 8, 15))) {
    adicionarCelebracao(dia, "Assunção da Bem-Aventurada Virgem Maria", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(DataCivil(ano, 11, 1))) {
    adicionarCelebracao(dia, "Todos os Santos", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(DataCivil(ano, 1, 1))) {
    adicionarCelebracao(dia, "Santa Maria, Mãe de Deus", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(epifania)) {
    adicionarCelebracao(dia, "Epifania do Senhor", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(batismoDoSenhor)) {
    adicionarCelebracao(dia, "Batismo do Senhor", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  } else if (data.ehIgual(DataCivil(ano, 12, 26))) {
    adicionarCelebracao(dia, "Santo Estêvão", TipoCelebracao::FESTA, CorLiturgica::VERMELHO);
  } else if (data.ehIgual(DataCivil(ano, 12, 27))) {
    adicionarCelebracao(dia, "São João Evangelista", TipoCelebracao::FESTA, CorLiturgica::BRANCO);
  } else if (data.ehIgual(DataCivil(ano, 12, 28))) {
    adicionarCelebracao(dia, "Santos Inocentes", TipoCelebracao::FESTA, CorLiturgica::VERMELHO);
  } else if (data.ehIgual(DataCivil(ano, 12, 25))) {
    adicionarCelebracao(dia, "Natal do Senhor", TipoCelebracao::SOLENIDADE, CorLiturgica::BRANCO);
  }

  if (data.obterDiaDaSemana() == DiaDaSemana::DOMINGO
    && dia.periodo.tempo != TempoLiturgico::SEMANA_SANTA) {
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
