export enum CorLiturgica {
  VIOLETA = 'violeta',
  BRANCO = 'branco',
  VERMELHO = 'vermelho',
  VERDE = 'verde',
  ROSA = 'rosa',
}

export enum TipoCelebracao {
  SOLENIDADE = 'solenidade',
  FESTA = 'festa',
  MEMORIA = 'memoria',
  DOMINGO = 'domingo',
  SEMANA = 'semana',
}

export interface CelebracaoLiturgica {
  nome: string;
  tipo: TipoCelebracao;
  cor?: CorLiturgica;
}

export interface PeriodoLiturgico {
  nome: string;
  codigo: string;
  corPadrao: CorLiturgica;
  inicio: Date;
  fim: Date;
}

export interface DiaLiturgico {
  data: Date;
  periodo: PeriodoLiturgico;
  celebracoes: CelebracaoLiturgica[];
  cor: CorLiturgica;
}
