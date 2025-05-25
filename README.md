# Árbol B\*

## Concepto

El Árbol B\* es una variante optimizada del Árbol B. Su principal objetivo es mejorar la utilización del espacio y reducir la cantidad de divisiones de nodos durante las operaciones de inserción. A diferencia del Árbol B, el Árbol B\* emplea técnicas como la redistribución y la división triple, lo que le permite mantener los nodos más llenos y, por ende, más eficientes.

## Características

- Altura balanceada.
- Todos los datos están en nodos hoja.
- Redistribuye claves antes de dividir.
- Utiliza una estrategia de división triple para optimizar la estructura.
- Mejor aprovechamiento del espacio (cercano al 2/3 del nodo lleno).

## Comparación con el Árbol B

| Característica       | Árbol B       | Árbol B\*         |
|----------------------|---------------|-------------------|
| Redistribución       | Opcional      | Obligatoria antes de dividir |
| División             | Binaria       | Triple            |
| Eficiencia del espacio | Menor       | Mayor (≥ 66%)     |

## Operaciones

### Inserción

1. Buscar la posición correcta.
2. Si el nodo está lleno:
   - Intentar redistribuir con un hermano.
   - Si no es posible, hacer una división triple.
3. Insertar la clave.

### Eliminación

1. Buscar la clave.
2. Eliminar la clave.
3. Si el nodo tiene menos del mínimo:
   - Intentar redistribuir con un hermano.
   - Si no es posible, fusionar nodos.

## Aplicaciones

- Sistemas de gestión de bases de datos.
- Sistemas de archivos.
- Índices de almacenamiento de gran volumen.

## Ventajas

- Menor profundidad que otros árboles balanceados.
- Menor número de accesos al disco.
- Mejor rendimiento en operaciones de inserción intensiva.

## Desventajas

- Más complejidad en la implementación.
- Poca documentación comparado con Árboles B o B+.

## Ejemplo (C++)

```cpp
struct Nodo {
    int claves[5];
    Nodo* hijos[6];
    int n;
    bool esHoja;
};

