#ifndef CONTROLADOR_RELES_H
#define CONTROLADOR_RELES_H

#include "TiposLiturgicos.h"

// Aplica uma única cor por vez nos relés ativos em nível baixo.
class ControladorReles {
public:
  void iniciar();
  void aplicarCor(CorLiturgica cor);
  void desligarTodos();

private:
  void definirEstadoDoRele(uint8_t pino, bool deveLigar);
};

#endif
