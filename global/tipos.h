#ifndef TIPOS_H
#define TIPOS_H

// define tipo das categorias dos alimentos
typedef enum {
    CEREAIS_DERIVADOS,
    VERDURAS_HORTALICAS_DERIVADOS,
    FRUTAS_DERIVADOS,
    GORDURAS_OLEOS,
    PESCADOS_FRUTOS_MAR,
    CARNES_DERIVADOS,
    LEITE_DERIVADOS,
    BEBIDAS,
    OVOS_DERIVADOS,
    PRODUTOS_ACUCARADOS,
    MISCELANEAS,
    OUTROS_INDUSTRIALIZADOS,
    ALIMENTOS_PREPARADOS,
    LEGUMINOSAS_DERIVADOS,
    NOZES_SEMENTES,
    NUM_CATEGORIAS
} CategoriaAlimento;

// define modelo das info do alimento
typedef struct {
    int numero;
    char descricao[200];
    float energia;
    float proteina;
    CategoriaAlimento categoria;
} Alimento;

#endif