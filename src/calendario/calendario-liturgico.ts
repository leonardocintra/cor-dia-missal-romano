import { CorLiturgica, TipoCelebracao } from '../tipos/liturgico.js';
import type { CelebracaoLiturgica, DiaLiturgico } from '../tipos/liturgico.js';
import { determinarPeriodoLiturgico } from './periodos.js';
import { adicionarDias, calcularPascoa } from './pascoa.js';

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
  const pascoa = calcularPascoa(data.getUTCFullYear());

  const celebracoes: CelebracaoLiturgica[] = [];

  if (data.getTime() === pascoa.getTime()) {
    celebracoes.push(criarCelebracao('Domingo de Páscoa', TipoCelebracao.SOLENIDADE, CorLiturgica.BRANCO));
  }

  if (data.getTime() === adicionarDias(pascoa, -7).getTime()) {
    celebracoes.push(criarCelebracao('Domingo de Ramos', TipoCelebracao.SOLENIDADE, CorLiturgica.VERMELHO));
  }

  if (data.getTime() === adicionarDias(pascoa, -3).getTime()) {
    celebracoes.push(criarCelebracao('Quinta-feira Santa', TipoCelebracao.SOLENIDADE, CorLiturgica.BRANCO));
  }

  if (data.getTime() === adicionarDias(pascoa, -2).getTime()) {
    celebracoes.push(criarCelebracao('Sexta-feira Santa', TipoCelebracao.SOLENIDADE, CorLiturgica.VERMELHO));
  }

  if (data.getTime() === adicionarDias(pascoa, -46).getTime()) {
    celebracoes.push(criarCelebracao('Quarta-feira de Cinzas', TipoCelebracao.SEMANA, CorLiturgica.VIOLETA));
  }

  if (celebracoes.length === 0) {
    if (periodo.nome === 'Advento' || periodo.nome === 'Quaresma') {
      celebracoes.push(criarCelebracao(periodo.nome, TipoCelebracao.SEMANA, periodo.corPadrao));
    } else if (periodo.nome === 'Natal') {
      celebracoes.push(criarCelebracao('Natal do Senhor', TipoCelebracao.SOLENIDADE, CorLiturgica.BRANCO));
    } else {
      celebracoes.push(criarCelebracao('Tempo Comum', TipoCelebracao.SEMANA, periodo.corPadrao));
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
