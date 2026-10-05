import { calcularPascoa, adicionarDias } from './pascoa.js';

export interface CicloPascal {
  ano: number;
  pascoa: Date;
  domingoRamos: Date;
  quintaSanta: Date;
  sextaSanta: Date;
  sabadoSanto: Date;
  domingoDePascoa: Date;
}

export function obterCicloPascal(ano: number): CicloPascal {
  const pascoa = calcularPascoa(ano);

  return {
    ano,
    pascoa,
    domingoRamos: adicionarDias(pascoa, -7),
    quintaSanta: adicionarDias(pascoa, -3),
    sextaSanta: adicionarDias(pascoa, -2),
    sabadoSanto: adicionarDias(pascoa, -1),
    domingoDePascoa: pascoa,
  };
}
