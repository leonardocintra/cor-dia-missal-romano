import { describe, expect, it } from 'vitest';
import { calcularPascoa, calcularPascoaEmData } from '../src/calendario/pascoa.js';

describe('Cálculo da Páscoa', () => {
  it('calcula a Páscoa de 2026', () => {
    expect(calcularPascoa(2026)).toEqual(new Date(Date.UTC(2026, 3, 5)));
  });

  it('calcula a Páscoa de 2027', () => {
    expect(calcularPascoa(2027)).toEqual(new Date(Date.UTC(2027, 2, 28)));
  });

  it('calcula a Páscoa de 2033', () => {
    expect(calcularPascoa(2033)).toEqual(new Date(Date.UTC(2033, 3, 17)));
  });

  it('converte a data da Páscoa em objeto Date', () => {
    expect(calcularPascoaEmData(2028)).toEqual(new Date(Date.UTC(2028, 3, 16)));
  });
});
