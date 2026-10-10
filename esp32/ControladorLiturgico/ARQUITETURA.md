# Arquitetura do Controlador Litúrgico

## Objetivo

O firmware calcula o dia litúrgico a partir da data do DS3231, mostra as
informações no OLED e seleciona a combinação correspondente nos quatro relés.

As regras do calendário ficam isoladas do ESP32. Portanto, uma alteração nas
regras litúrgicas não deve exigir mudanças no código do display, do RTC, dos
botões ou dos relés.

## Fonte de verdade das regras

O comportamento a ser preservado está na prova de conceito TypeScript, em
`../../poc/`, especialmente em:

- `src/calendario/pascoa.ts`;
- `src/calendario/periodos.ts`;
- `src/calendario/calendario-liturgico.ts`;
- `tests/`.

O primeiro port manterá exatamente as celebrações, cores e regras de
precedência cobertas por esses testes. Inclusões futuras no calendário romano
geral serão feitas como dados e regras adicionais, sem alterar o hardware.

## Organização planejada

```text
ControladorLiturgico/
  ControladorLiturgico.ino       Ponto de entrada: setup() e loop().
  ARQUITETURA.md                 Esta decisão de arquitetura.
  TiposLiturgicos.h              Tipos e enumerações do domínio.
  DataCivil.h/.cpp               Datas, comparação, dia da semana e soma de dias.
  CalculadoraPascoa.h/.cpp       Cálculo da Páscoa gregoriana.
  CalendarioLiturgico.h/.cpp     Períodos, celebrações e precedência.
  ConfiguracaoHardware.h         Pinos, endereço I2C e opções do aparelho.
  MapeamentoCores.h              Combinação de relés para cada cor.
  RelogioDs3231.h/.cpp           Adaptador do RTC DS3231.
  TelaOled.h/.cpp                Adaptador do SSD1306.
  LeitorBotoes.h/.cpp            Leitura com debounce e geração de eventos.
  ControladorReles.h/.cpp        Escrita segura nos quatro relés.
  ControladorLiturgico.h/.cpp    Orquestra a interação entre os módulos.
```

Os arquivos ficarão inicialmente na raiz do sketch. Essa organização é a mais
compatível com a Arduino IDE: ela compila o `.ino` e todos os `.cpp` presentes
na mesma pasta sem configuração extra.

## Responsabilidades dos módulos

### Domínio

O domínio será C++ puro: não incluirá `Arduino.h`, `Wire.h`, bibliotecas do
OLED nem `DateTime` do DS3231. Sua entrada será uma `DataCivil` e sua saída,
um `DiaLiturgico`.

```cpp
class CalendarioLiturgico {
public:
  DiaLiturgico obterDia(const DataCivil& data) const;
};
```

`DiaLiturgico` conterá a data, o período, a celebração de maior precedência e
a cor final. As celebrações simultâneas podem ser mantidas internamente para
diagnóstico, mas a tela e os relés precisam somente do resultado efetivo.

### Infraestrutura

Cada classe protege o restante do firmware dos detalhes de uma biblioteca ou
de uma ligação física:

- `RelogioDs3231` converte `DateTime` em `DataCivil`.
- `TelaOled` recebe dados prontos para exibir; não calcula liturgia.
- `LeitorBotoes` entrega eventos, como pressionamento curto e longo; não muda
  data nem escreve em relés.
- `ControladorReles` recebe uma `CorLiturgica` e aplica o mapa elétrico.

O estado ativo dos relés (ativo em nível alto ou baixo) e seus pinos ficarão
somente em `ConfiguracaoHardware.h` e `MapeamentoCores.h`.

### Aplicação

`ControladorLiturgico` será o único módulo que conhece domínio e infraestrutura.
Ele lerá a data do RTC, consultará o calendário, atualizará os relés e pedirá
à tela uma nova renderização quando algo visível tiver mudado.

```cpp
class ControladorLiturgico {
public:
  void iniciar();
  void atualizar();
};
```

`atualizar()` não deverá usar `delay()`. Assim, a leitura dos botões continua
responsiva e a troca de tela não bloqueia o controlador.

## Fluxo de funcionamento

```text
DS3231 -> DataCivil -> CalendarioLiturgico -> DiaLiturgico
                                             |          |
                                             v          v
                                       ControladorReles  TelaOled
                                                     ^
                                                     |
                                                LeitorBotoes
```

Há dois modos previstos: **data atual** e **consulta manual**. A consulta
manual altera somente a data usada para mostrar e simular o resultado. A opção
de fazer os relés seguirem a consulta manual será uma decisão explícita da
interface, e não um efeito colateral da navegação.

## Layout definido para o OLED

O layout principal será limpo e priorizará a cor, que é a informação visível à
distância e usada pelos relés:

```text
05/04/2026 - DOM

--------------------  faixa amarela (16 pixels)

Domingo de Páscoa
Páscoa

      BRANCO
```

Datas e dia da semana aparecem no topo, dentro da faixa amarela. A celebração
começa na linha 20, já na região azul. Quando ultrapassar a largura do OLED,
ela desliza horizontalmente de forma contínua nessa linha. O período fica
abaixo dela. A cor aparece em tamanho maior no rodapé.

O OLED SSD1306 com a fonte padrão não representa caracteres acentuados em
UTF-8. A tela normaliza somente a exibição para ASCII (por exemplo, `Páscoa`
aparece como `Pascoa`); o nome original com acentos continua preservado no
núcleo litúrgico e no Monitor Serial.

## Critérios para o primeiro marco de implementação

1. O firmware compila na Arduino IDE para ESP32.
2. O DS3231 fornece a data atual ao domínio.
3. As datas cobertas pelos testes TypeScript retornam a mesma cor e celebração
   no C++.
4. O OLED exibe data, período, celebração e cor.
5. Os relés recebem o mapeamento configurado para a cor resultante.
6. Nenhuma rotina principal bloqueia com `delay()`.

## Decisões ainda necessárias antes do hardware entrar no código

As definições de hardware ficam centralizadas em `ConfiguracaoHardware.h`:

- OLED e DS3231: SDA no GPIO 21 e SCL no GPIO 22;
- botões: roxo 25, vermelho 26, branco 27 e verde 32;
- relés: roxo 4, vermelho 16, branco 17 e verde 18;
- relés ativos em nível baixo.

Um botão seleciona a sua cor e liga somente o relé correspondente, desligando
os demais. Isso reproduz o comportamento validado no protótipo. A seleção muda
o aparelho para modo manual e permanece assim até a data do DS3231 mudar; na
virada de data, o modo automático é restaurado e a cor litúrgica é reaplicada.
