import { CorLiturgica, TipoCelebracao } from '../tipos/liturgico.js';
import type { CelebracaoLiturgica, DiaLiturgico } from '../tipos/liturgico.js';
import { determinarPeriodoLiturgico } from './periodos.js';
import { adicionarDias, calcularPascoa } from './pascoa.js';
import { dataUtc } from './regras.js';

type CelebracaoDefinicao = {
  nome: string;
  tipo: TipoCelebracao;
  cor: CorLiturgica;
  dataBase: Date;
  dataEfetiva?: Date;
  calendario: 'romano-geral';
};

const precedenciaPorTipo: Record<TipoCelebracao, number> = {
  [TipoCelebracao.SOLENIDADE]: 5,
  [TipoCelebracao.FESTA]: 4,
  [TipoCelebracao.DOMINGO]: 3,
  [TipoCelebracao.MEMORIA]: 2,
  [TipoCelebracao.SEMANA]: 1,
};

export function criarCelebracao(nome: string, tipo: TipoCelebracao, cor?: CorLiturgica): CelebracaoLiturgica {
  return {
    nome,
    tipo,
    ...(cor !== undefined ? { cor } : {}),
  };
}

function nomeDoDomingo(periodo: { nome: string }): string {
  switch (periodo.nome) {
    case 'Advento':
      return 'Domingo do Advento';
    case 'Quaresma':
      return 'Domingo da Quaresma';
    case 'Semana Santa':
      return 'Domingo da Semana Santa';
    case 'Páscoa':
      return 'Domingo de Páscoa';
    case 'Tempo Pascal':
      return 'Domingo do Tempo Pascal';
    case 'Natal':
      return 'Domingo do Tempo de Natal';
    case 'Tempo Comum':
      return 'Domingo do Tempo Comum';
    default:
      return 'Domingo';
  }
}

function construirCelebracoesDefinidas(ano: number): CelebracaoDefinicao[] {
  const pascoa = calcularPascoa(ano);
  const ascensaoDoSenhor = adicionarDias(pascoa, 39);
  const corpusChristi = adicionarDias(pascoa, 60);
  const epifania = dataUtc(ano, 0, 6);
  const batismoDoSenhor = adicionarDias(epifania, (7 - epifania.getUTCDay()) % 7);

  return [
    { nome: 'Quarta-feira de Cinzas', tipo: TipoCelebracao.SEMANA, cor: CorLiturgica.VIOLETA, dataBase: adicionarDias(pascoa, -46), calendario: 'romano-geral' },
    { nome: 'Domingo de Ramos', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO, dataBase: adicionarDias(pascoa, -7), calendario: 'romano-geral' },
    { nome: 'Quinta-feira Santa', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: adicionarDias(pascoa, -3), calendario: 'romano-geral' },
    { nome: 'Sexta-feira Santa', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO, dataBase: adicionarDias(pascoa, -2), calendario: 'romano-geral' },
    { nome: 'Sábado Santo', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO, dataBase: adicionarDias(pascoa, -1), calendario: 'romano-geral' },
    { nome: 'Domingo de Páscoa', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: pascoa, calendario: 'romano-geral' },
    { nome: 'Ascensão do Senhor', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: ascensaoDoSenhor, calendario: 'romano-geral' },
    { nome: 'Pentecostes', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO, dataBase: adicionarDias(pascoa, 49), calendario: 'romano-geral' },
    { nome: 'Santíssimo Corpo e Sangue de Cristo', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: corpusChristi, calendario: 'romano-geral' },
    { nome: 'Conversão de São Paulo', tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO, dataBase: dataUtc(ano, 0, 25), calendario: 'romano-geral' },
    { nome: 'Apresentação do Senhor', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: dataUtc(ano, 1, 2), calendario: 'romano-geral' },
    { nome: 'Cátedra de São Pedro', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: dataUtc(ano, 1, 22), calendario: 'romano-geral' },
    { nome: 'Natividade de São João Batista', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: dataUtc(ano, 5, 24), calendario: 'romano-geral' },
    { nome: 'São Pedro e São Paulo', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO, dataBase: dataUtc(ano, 5, 29), calendario: 'romano-geral' },
    { nome: 'Assunção da Bem-Aventurada Virgem Maria', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: dataUtc(ano, 7, 15), calendario: 'romano-geral' },
    { nome: 'Todos os Santos', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: dataUtc(ano, 10, 1), calendario: 'romano-geral' },
    { nome: 'Santa Maria, Mãe de Deus', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: dataUtc(ano, 0, 1), calendario: 'romano-geral' },
    { nome: 'Epifania do Senhor', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: dataUtc(ano, 0, 6), calendario: 'romano-geral' },
    { nome: 'Batismo do Senhor', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: batismoDoSenhor, calendario: 'romano-geral' },
    { nome: 'Santo Estêvão', tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO, dataBase: dataUtc(ano, 11, 26), calendario: 'romano-geral' },
    { nome: 'São João Evangelista', tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO, dataBase: dataUtc(ano, 11, 27), calendario: 'romano-geral' },
    { nome: 'Santos Inocentes', tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO, dataBase: dataUtc(ano, 11, 28), calendario: 'romano-geral' },
    { nome: 'Natal do Senhor', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: dataUtc(ano, 11, 25), calendario: 'romano-geral' },
  ];
}

export function calcularDiaLiturgico(data: Date): DiaLiturgico {
  const periodo = determinarPeriodoLiturgico(data);
  const ano = data.getUTCFullYear();
  const celebracoesAplicaveis: CelebracaoLiturgica[] = [];

  const definicoes = construirCelebracoesDefinidas(ano);

  for (const definicao of definicoes) {
    const dataEfetiva = definicao.dataEfetiva ?? definicao.dataBase;
    if (dataEfetiva.getTime() === data.getTime()) {
      celebracoesAplicaveis.push(criarCelebracao(definicao.nome, definicao.tipo, definicao.cor));
    }
  }

  if (data.getUTCDay() === 0 && periodo.nome !== 'Semana Santa') {
    celebracoesAplicaveis.push(criarCelebracao(nomeDoDomingo(periodo), TipoCelebracao.DOMINGO, periodo.corPadrao));
  }

  if (celebracoesAplicaveis.length === 0) {
    const nomePadrao = periodo.nome === 'Natal' ? 'Tempo de Natal' : periodo.nome;
    celebracoesAplicaveis.push(criarCelebracao(nomePadrao, TipoCelebracao.SEMANA, periodo.corPadrao));
  }

  const celebracoesOrdenadas = [...celebracoesAplicaveis].sort(
    (a, b) => (precedenciaPorTipo[b.tipo] ?? 0) - (precedenciaPorTipo[a.tipo] ?? 0),
  );

  const celebracaoEfetiva = celebracoesOrdenadas[0] ?? criarCelebracao('Tempo Comum', TipoCelebracao.SEMANA, periodo.corPadrao);
  const cor = celebracaoEfetiva.cor ?? periodo.corPadrao;

  return { data, periodo, celebracoes: celebracoesOrdenadas, cor };
}

export function calcularCorDoDia(data: Date): CorLiturgica {
  return calcularDiaLiturgico(data).cor;
}
