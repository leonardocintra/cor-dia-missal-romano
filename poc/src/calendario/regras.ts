export function dataUtc(ano: number, mes: number, dia: number): Date {
  return new Date(Date.UTC(ano, mes, dia));
}

export function adicionarDias(data: Date, dias: number): Date {
  return new Date(Date.UTC(
    data.getUTCFullYear(),
    data.getUTCMonth(),
    data.getUTCDate() + dias,
  ));
}

export function mesmoDia(a: Date, b: Date): boolean {
  return a.getTime() === b.getTime();
}

export function obterDiaSemanaUTC(data: Date): number {
  return data.getUTCDay();
}

export function obterDomingoMaisProximo(data: Date): Date {
  const diaSemana = obterDiaSemanaUTC(data);
  const diasAteDomingo = (diaSemana + 7) % 7;
  return adicionarDias(data, -diasAteDomingo);
}
