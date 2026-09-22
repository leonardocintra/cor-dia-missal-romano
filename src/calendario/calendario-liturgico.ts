import { CorLiturgica, TipoCelebracao } from '../tipos/liturgico.js';
import type { CelebracaoLiturgica, DiaLiturgico } from '../tipos/liturgico.js';
import { determinarPeriodoLiturgico } from './periodos.js';
import { adicionarDias, calcularPascoa } from './pascoa.js';
import { dataUtc } from './regras.js';

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

export function calcularDiaLiturgico(data: Date): DiaLiturgico {
  const periodo = determinarPeriodoLiturgico(data);
  const ano = data.getUTCFullYear();
  const pascoa = calcularPascoa(ano);

  const celebracoes: CelebracaoLiturgica[] = [];

  const listaDeCelebracoes: Array<{ data: Date; nome: string; tipo: TipoCelebracao; cor?: CorLiturgica }> = [
    { data: adicionarDias(pascoa, -46), nome: 'Quarta-feira de Cinzas', tipo: TipoCelebracao.SEMANA, cor: CorLiturgica.VIOLETA },
    { data: adicionarDias(pascoa, -7), nome: 'Domingo de Ramos', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO },
    { data: adicionarDias(pascoa, -3), nome: 'Quinta-feira Santa', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
    { data: adicionarDias(pascoa, -2), nome: 'Sexta-feira Santa', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO },
    { data: adicionarDias(pascoa, -1), nome: 'Sábado Santo', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO },
    { data: pascoa, nome: 'Domingo de Páscoa', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
    { data: adicionarDias(pascoa, 49), nome: 'Pentecostes', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO },
    { data: dataUtc(ano, 11, 25), nome: 'Natal do Senhor', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
    { data: dataUtc(ano, 11, 26), nome: 'Santo Estêvão', tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
    { data: dataUtc(ano, 11, 27), nome: 'São João Evangelista', tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
    { data: dataUtc(ano, 11, 28), nome: 'Santos Inocentes', tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
    { data: dataUtc(ano, 0, 1), nome: 'Santa Maria, Mãe de Deus', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
    { data: dataUtc(ano, 0, 6), nome: 'Epifania do Senhor', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
    { data: dataUtc(ano, 0, 10), nome: 'Batismo do Senhor', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
  ];

  for (const celebracao of listaDeCelebracoes) {
    if (celebracao.data.getTime() === data.getTime()) {
      celebracoes.push(criarCelebracao(celebracao.nome, celebracao.tipo, celebracao.cor));
    }
  }

  if (celebracoes.length === 0) {
    if (periodo.nome === 'Natal') {
      celebracoes.push(criarCelebracao('Tempo de Natal', TipoCelebracao.SEMANA, periodo.corPadrao));
    } else {
      celebracoes.push(criarCelebracao(periodo.nome, TipoCelebracao.SEMANA, periodo.corPadrao));
    }
  }

  const celebracoesOrdenadas = [...celebracoes].sort(
    (a, b) => (precedenciaPorTipo[b.tipo] ?? 0) - (precedenciaPorTipo[a.tipo] ?? 0),
  );
  const cor = celebracoesOrdenadas[0]?.cor ?? periodo.corPadrao;

  return { data, periodo, celebracoes: celebracoesOrdenadas, cor };
}

export function calcularCorDoDia(data: Date): CorLiturgica {
  return calcularDiaLiturgico(data).cor;
}
