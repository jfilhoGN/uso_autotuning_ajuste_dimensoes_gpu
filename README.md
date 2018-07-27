Uso de Ferramenta de *Autotunning* para ajuste nas dimensões de *kernels* em Dispositivos Aceleradores (GPUs)
---

### Abstract

*Accelerator devices have been widely used in Parallel Computing. In GPUs, one of the main difficulties is in the fact that the programming model leaves the definition of threads arrangement structure to solve the problem to the programmer. This may result in the use of generic definitions that are not adjusted to architectural resources and that can cause performance degradation, moreover the best configurations may be also device dependent. The aim of this work is to analyze the influence or impact of the dimensions of these thread arrays configurations for performance in GPUs. Therefore, our proposal is related to the adjustment of code to the architectural characteristics and the search in the set of valid configurations. Combinations of  grids, blocks and threads that are present the best performance on the overall or in specific components. For this, some benchmarks will be used, and their configurations and kernels will be tested using the OpenTuner tool. The best configuration is found based on metrics retrieved from GPUs using the nvprof profiling tool.*


### Resumo

Dispositivos aceleradores tem sido amplamente utilizados em Computação Paralela. Nas GPUs, uma das principais dificuldades está no fato do modelo de programação deixar a definição da estrutura do arranjo de *threads* para a solução do problema sob a responsabilidade do programador. O que pode resultar no uso de definições genéricas não ajustadas aos recursos arquiteturais e consequentemente à degradação do desempenho, além disso, as melhores configurações também podem depender do dispositivo.
O objetivo deste trabalho é analisar qual a influência ou o impacto da escolha das dimensões dessas configurações de arranjos de *threads* para o desempenho da execução em GPUs. Assim, nossa proposta está relacionada com o ajuste de código às características arquiteturais e a busca no conjunto de configurações válidas para *grids*, *blocos* e *threads* qual é a configuração que apresente o melhor desempenho de maneira global ou em componentes específicos. Para isso serão utilizados alguns *benchmarks*, sendo que suas configurações e os *kernels* serão testados utilizando-se a ferramenta OpenTuner. A melhor configuração é encontrada com base em métricas recuperadas das GPUs utilizando a ferramenta de perfilamento nvprof.

Autores:

1. João Martins Filho <joaomfilho1995@gmail.com>
2. Rogério Aparecido Gonçalves <rogerioag@utfpr.edu.br>
3. Alfredo Goldman <gold@ime.usp.br>

## Arquivos e Diretórios

- Arquivo ***dimensions.h***: biblioteca que contém todos os índices de *grids* e blocos, essa biblioteca é utilizada nos *kernels* CUDA.
- Arquivo ***dimensions_calc_instructions.pdf***: *pdf* explicativo contendo todos os valores de instrução *add* e *mult* de cada índice do *dimensions.h*.
- Diretório ***gen-configs***: contém todas as configurações geradas para um tamanho de *N* para o OpenTuner percorrer o espaço de buscas.
- Diretório ***opentuner***: contém o *brench* da ferramenta OpenTuner utilizada para os experimentos.
- Arquivo ***ResultadosWSCAD.pdf***: contém todos os resultados obtidos até o momento para os algoritmos soma de vetor e *sincos*.
- Diretório ***sincoscuda-opentuner***:contém os *scripts* criados para a ferramenta OpenTuner, bem como o algoritmo *sincos* implementado em CUDA.
- Diretório ***sumvectorcuda-opentuner***:contém os *scripts* criados para a ferramenta OpenTuner, bem como o algoritmo *sumvector* implementado em CUDA.
