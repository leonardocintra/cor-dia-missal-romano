import { CorLiturgica } from '../tipos/liturgico.js';
import type { PeriodoLiturgico } from '../tipos/liturgico.js';
import { adicionarDias, dataUtc, obterDomingoMaisProximo } from './regras.js';
import { calcularPascoa } from './pascoa.js';

export function calcularDomingoAdvento(ano: number): Date {
  const natal = dataUtc(ano, 11, 25);
  const domingoAntesDoNatal = obterDomingoMaisProximo(natal);
  return adicionarDias(domingoAntesDoNatal, -21);
}

export function calcularQuartaCinzas(ano: number): Date {
  return adicionarDias(calcularPascoa(ano), -46);
}

export function determinarPeriodoLiturgico(data: Date): PeriodoLiturgico {
  const ano = data.getUTCFullYear();
  const pascoa = calcularPascoa(ano);
  const domingoAdvento = calcularDomingoAdvento(ano);
  const quartaCinzas = calcularQuartaCinzas(ano);
  const domingoRamos = adicionarDias(pascoa, -7);
  const sabadoSanto = adicionarDias(pascoa, -1);
  const tempoComumInicio = adicionarDias(pascoa, 50);
  const inicioNatalAtual = dataUtc(ano, 11, 25);
  const fimNatalAtual = dataUtc(ano + 1, 0, 6);
  const inicioNatalAnterior = dataUtc(ano - 1, 11, 25);
  const fimNatalAnterior = dataUtc(ano, 0, 6);
  const emNatal = (data >= inicioNatalAtual && data <= fimNatalAtual)
    || (data >= inicioNatalAnterior && data <= fimNatalAnterior);

  if (data >= domingoAdvento && data <= dataUtc(ano, 11, 24)) {
    return {
      nome: 'Advento',
      codigo: 'advento',
      corPadrao: CorLiturgica.VIOLETA,
      inicio: domingoAdvento,
      fim: dataUtc(ano, 11, 24),
    };
  }

  if (emNatal) {
    return {
      nome: 'Natal',
      codigo: 'natal',
      corPadrao: CorLiturgica.BRANCO,
      inicio: data >= inicioNatalAtual ? inicioNatalAtual : inicioNatalAnterior,
      fim: data <= fimNatalAnterior ? fimNatalAnterior : fimNatalAtual,
    };
  }

  if (data >= quartaCinzas && data < domingoRamos) {
    return {
      nome: 'Quaresma',
      codigo: 'quaresma',
      corPadrao: CorLiturgica.VIOLETA,
      inicio: quartaCinzas,
      fim: adicionarDias(domingoRamos, -1),
    };
  }

  if (data >= domingoRamos && data <= sabadoSanto) {
    return {
      nome: 'Semana Santa',
      codigo: 'semana-santa',
      corPadrao: CorLiturgica.VERMELHO,
      inicio: domingoRamos,
      fim: sabadoSanto,
    };
  }

  if (data.getTime() === pascoa.getTime()) {
    return {
      nome: 'Páscoa',
      codigo: 'pascoa',
      corPadrao: CorLiturgica.BRANCO,
      inicio: pascoa,
      fim: pascoa,
    };
  }

  if (data > pascoa && data < tempoComumInicio) {
    return {
      nome: 'Tempo Pascal',
      codigo: 'tempo-pascal',
      corPadrao: CorLiturgica.BRANCO,
      inicio: adicionarDias(pascoa, 1),
      fim: adicionarDias(tempoComumInicio, -1),
    };
  }

  if (data >= tempoComumInicio && data < domingoAdvento) {
    return {
      nome: 'Tempo Comum',
      codigo: 'tempo-comum',
      corPadrao: CorLiturgica.VERDE,
      inicio: tempoComumInicio,
      fim: adicionarDias(domingoAdvento, -1),
    };
  }

  return {
    nome: 'Tempo Comum',
    codigo: 'tempo-comum',
    corPadrao: CorLiturgica.VERDE,
    inicio: dataUtc(ano, 0, 1),
    fim: dataUtc(ano, 11, 31),
  };
}

export function determinarPeriodoLiturgicoPorAno(data: Date): PeriodoLiturgico {
  return determinarPeriodoLiturgico(data);
}
