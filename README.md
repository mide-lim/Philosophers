# Documentação Completa - Philosophers

## 📋 Índice

1. [Visão Geral](#visão-geral)
2. [O Problema dos Filósofos](#o-problema-dos-filósofos)
3. [Estrutura do Projeto](#estrutura-do-projeto)
4. [Estruturas de Dados](#estruturas-de-dados)
5. [Arquivos e Funções](#arquivos-e-funções)
6. [Fluxo de Execução](#fluxo-de-execução)
7. [Sincronização e Threads](#sincronização-e-threads)
8. [Prevenção de Deadlock](#prevenção-de-deadlock)
9. [Compilação e Uso](#compilação-e-uso)
10. [Casos Especiais](#casos-especiais)

---

## 🎯 Visão Geral

Este projeto implementa o clássico **Dining Philosophers Problem** (Problema dos Filósofos Jantando) utilizando:
- **Threads POSIX** (`pthread`)
- **Mutexes** para sincronização
- **C** como linguagem de programação

O objetivo é simular filósofos sentados em uma mesa circular que alternam entre pensar, comer e dormir, compartilhando garfos (recursos) sem criar deadlocks ou morrer de fome.

---

## 🍝 O Problema dos Filósofos

### Descrição

N filósofos estão sentados em uma mesa circular. Entre cada par de filósofos há um garfo. Para comer, um filósofo precisa pegar **dois garfos** (o da esquerda e o da direita).

### Regras

1. **Filósofos não se comunicam** entre si
2. **Não sabem** quando outro filósofo vai morrer
3. Devem **alternar** entre: pensar → comer → dormir
4. Um filósofo **morre** se passar muito tempo sem comer (`time_to_die`)
5. A simulação **termina** quando:
   - Um filósofo morre
   - Todos comeram o número mínimo de refeições (opcional)

### Desafios

- **Deadlock**: Todos pegam um garfo ao mesmo tempo
- **Race Conditions**: Acesso concorrente a recursos compartilhados
- **Sincronização**: Coordenar múltiplas threads sem travar o programa

---

## 📁 Estrutura do Projeto

```
philo/
├── includes/
│   └── philo.h          # Cabeçalho com estruturas e protótipos
├── src/
│   ├── main.c           # Ponto de entrada do programa
│   ├── parse.c          # Validação e parsing de argumentos
│   ├── init.c           # Inicialização de estruturas
│   ├── routine.c        # Rotina principal dos filósofos
│   ├── monitor.c        # Monitoramento de morte e conclusão
│   ├── print.c          # Funções de impressão sincronizada
│   ├── utils.c          # Funções auxiliares (tempo, conversão)
│   ├── dead.c           # Gerenciamento do estado de morte
│   └── cleanup.c        # Limpeza de recursos
├── Makefile             # Compilação automatizada
└── README.md            # Informações básicas
```

---

## 🏗️ Estruturas de Dados

### `t_rules` - Regras da Simulação

```c
typedef struct s_rules
{
    int             n_philos;       // Número de filósofos
    long long       t_die;          // Tempo para morrer (ms)
    long long       t_eat;          // Tempo para comer (ms)
    long long       t_sleep;        // Tempo para dormir (ms)
    int             must_eat;       // Refeições obrigatórias (-1 se não definido)
    long long       start_time;     // Timestamp do início da simulação
    int             someone_died;   // Flag: alguém morreu?
    pthread_mutex_t write_mutex;    // Protege impressões no terminal
    pthread_mutex_t dead_mutex;     // Protege acesso a someone_died
    pthread_mutex_t meal_mutex;     // Protege last_meal e meals_eaten
} t_rules;
```

**Função**: Armazena parâmetros globais da simulação e mutexes para sincronização.

### `t_philo` - Filósofo

```c
typedef struct s_philo
{
    int             id;             // Identificador único (1, 2, 3...)
    int             meals_eaten;    // Contador de refeições
    long long       last_meal;      // Timestamp da última refeição
    pthread_t       thread;         // Thread associada ao filósofo
    pthread_mutex_t *left_fork;     // Ponteiro para garfo esquerdo
    pthread_mutex_t *right_fork;    // Ponteiro para garfo direito
    t_rules         *rules;         // Ponteiro para regras globais
} t_philo;
```

**Função**: Representa cada filósofo com seu estado e recursos.

---

## 📄 Arquivos e Funções

### **1. philo.h**

Arquivo de cabeçalho contendo:
- Inclusões de bibliotecas (`pthread.h`, `sys/time.h`, etc.)
- Definições das estruturas `t_rules` e `t_philo`
- Protótipos de todas as funções

---

### **2. main.c**

**Ponto de entrada** do programa.

#### Funções

##### `init_rules_mutexes(t_rules *rules)`
```c
static int init_rules_mutexes(t_rules *rules)
```
- Inicializa os 3 mutexes globais: `write_mutex`, `dead_mutex`, `meal_mutex`
- Define `someone_died = 0`
- **Retorna**: 0 (sucesso) ou 1 (erro)

##### `start_threads(t_philo *philos, t_rules *rules)`
```c
static int start_threads(t_philo *philos, t_rules *rules)
```
- Define `start_time` como timestamp atual
- Cria threads para todos os filósofos
- Inicializa `last_meal` de cada filósofo
- **Retorna**: 0 (sucesso) ou 1 (erro)

##### `join_threads(t_philo *philos, t_rules *rules)`
```c
static void join_threads(t_philo *philos, t_rules *rules)
```
- Aguarda o término de todas as threads com `pthread_join()`

##### `main(int ac, char **av)`
```c
int main(int ac, char **av)
```
**Fluxo**:
1. Valida argumentos com `parse_args()`
2. Inicializa mutexes com `init_rules_mutexes()`
3. Inicializa filósofos e garfos com `init_all()`
4. **Caso especial**: Se há apenas 1 filósofo:
   - Pega um garfo
   - Espera `t_die` 
   - Imprime morte
5. **Caso normal** (2+ filósofos):
   - Inicia threads
   - Monitora estado com `monitor_loop()`
   - Aguarda threads
6. Limpa recursos com `cleanup_all()`

---

### **3. parse.c**

**Valida e processa** argumentos da linha de comando.

#### Funções

##### `set_rules(int ac, char **av, t_rules *rules)`
```c
static int set_rules(int ac, char **av, t_rules *rules)
```
- Converte argumentos para inteiros e preenche `t_rules`
- Valida se valores são positivos
- **Argumentos esperados**:
  ```
  ./philo [n_philos] [t_die] [t_eat] [t_sleep] [must_eat (opcional)]
  ```

##### `parse_args(int ac, char **av, t_rules *rules)`
```c
int parse_args(int ac, char **av, t_rules *rules)
```
- Verifica número de argumentos (5 ou 6)
- Valida se todos são numéricos com `ft_isdigit_str()`
- Chama `set_rules()` para processar valores
- **Retorna**: 0 (sucesso) ou 1 (erro)

**Exemplo de uso**:
```bash
./philo 5 800 200 200 7
# 5 filósofos, morrem em 800ms, comem em 200ms, dormem em 200ms, 7 refeições mínimas
```

---

### **4. init.c**

**Inicializa** estruturas de dados e recursos.

#### Funções

##### `init_forks(pthread_mutex_t **forks, t_rules *rules)`
```c
static int init_forks(pthread_mutex_t **forks, t_rules *rules)
```
- Aloca array de `n_philos` mutexes (garfos)
- Inicializa cada mutex
- **Retorna**: 0 (sucesso) ou 1 (erro)

##### `init_one_philo(t_philo *p, int i, pthread_mutex_t *forks, t_rules *rules)`
```c
static void init_one_philo(t_philo *p, int i, pthread_mutex_t *forks, t_rules *rules)
```
- Define ID do filósofo (índice + 1)
- Zera `meals_eaten` e `last_meal`
- Atribui ponteiros para garfos:
  - `left_fork`: garfo no índice `i`
  - `right_fork`: garfo no índice `(i + 1) % n_philos` (circular)

##### `init_philos(t_philo **philos, pthread_mutex_t *forks, t_rules *rules)`
```c
static int init_philos(t_philo **philos, pthread_mutex_t *forks, t_rules *rules)
```
- Aloca array de `n_philos` filósofos
- Chama `init_one_philo()` para cada um
- **Retorna**: 0 (sucesso) ou 1 (erro)

##### `init_all(t_philo **philos, pthread_mutex_t **forks, t_rules *rules)`
```c
int init_all(t_philo **philos, pthread_mutex_t **forks, t_rules *rules)
```
- Função pública que inicializa tudo
- Chama `init_forks()` e `init_philos()`

---

### **5. routine.c**

**Rotina principal** executada por cada thread (filósofo).

#### Funções

##### `take_forks(t_philo *p)`
```c
static void take_forks(t_philo *p)
```
- **Estratégia anti-deadlock**: 
  - Filósofos **pares** pegam garfo direito → esquerdo
  - Filósofos **ímpares** pegam garfo esquerdo → direito
- Trava mutexes na ordem definida
- Imprime "has taken a fork" duas vezes

##### `drop_forks(t_philo *p)`
```c
static void drop_forks(t_philo *p)
```
- Destrava ambos os mutexes (garfos)

##### `philo_eat(t_philo *p)`
```c
static void philo_eat(t_philo *p)
```
1. **Trava `meal_mutex`** (protege estado crítico)
2. Atualiza `last_meal` com timestamp atual
3. Incrementa `meals_eaten`
4. **Destrava `meal_mutex`**
5. Imprime "is eating"
6. Dorme por `t_eat` ms

##### `philo_sleep_think(t_philo *p)`
```c
static void philo_sleep_think(t_philo *p)
```
1. Imprime "is sleeping"
2. Dorme por `t_sleep` ms
3. Imprime "is thinking"
4. **Para número ímpar de filósofos**: adiciona tempo de pensamento extra
   - `think_time = (t_eat * 2) - t_sleep`
   - Previne starvation em mesas ímpares

##### `philo_routine(void *arg)`
```c
void *philo_routine(void *arg)
```
**Rotina da thread**:
1. Filósofos pares esperam 1ms (dessincroniza início)
2. **Loop principal**:
   ```
   while (!is_dead(rules)):
       take_forks()
       if (is_dead(rules)):
           drop_forks()
           break
       philo_eat()
       drop_forks()
       if (comeu suficiente):
           break
       philo_sleep_think()
   ```

---

### **6. monitor.c**

**Monitora** filósofos para detectar morte ou conclusão.

#### Funções

##### `all_ate_enough(t_philo *philos, t_rules *rules)`
```c
static int all_ate_enough(t_philo *philos, t_rules *rules)
```
- Verifica se todos comeram `must_eat` refeições
- **Trava `meal_mutex`** para ler `meals_eaten` de forma segura
- **Retorna**: 1 se todos terminaram, 0 caso contrário

##### `monitor_loop(t_philo *philos, t_rules *rules)`
```c
void monitor_loop(t_philo *philos, t_rules *rules)
```
**Loop de monitoramento** (thread principal):
1. Verifica se todos comeram suficiente → **termina**
2. Para cada filósofo:
   - **Trava `meal_mutex`**
   - Calcula `tempo_desde_ultima_refeicao = now - last_meal`
   - Se `>= t_die`:
     - Imprime morte
     - Define `someone_died = 1`
     - **Para monitoramento**
   - **Destrava `meal_mutex`**
3. Dorme 500 µs entre verificações (evita busy-waiting excessivo)

---

### **7. print.c**

**Funções de impressão sincronizada**.

#### Funções

##### `print_state(t_philo *philo, const char *msg)`
```c
void print_state(t_philo *philo, const char *msg)
```
- **Trava `write_mutex`** (evita output embaralhado)
- Verifica se ninguém morreu
- Imprime: `[timestamp_ms] [philo_id] [mensagem]`
- **Destrava `write_mutex`**

**Exemplo**:
```
142 1 has taken a fork
142 1 has taken a fork
142 1 is eating
```

##### `print_death(t_philo *philo)`
```c
void print_death(t_philo *philo)
```
- Similar a `print_state()`, mas para morte
- Imprime: `[timestamp_ms] [philo_id] died`

---

### **8. utils.c**

**Funções auxiliares** de uso geral.

#### Funções

##### `get_time_ms(void)`
```c
long long get_time_ms(void)
```
- Obtém timestamp atual em **milissegundos**
- Usa `gettimeofday()`
- **Fórmula**: `(segundos * 1000) + (microsegundos / 1000)`

##### `elapsed_ms(long long since)`
```c
long long elapsed_ms(long long since)
```
- Calcula tempo decorrido desde `since`
- **Retorna**: `get_time_ms() - since`

##### `precise_sleep(long long ms, t_rules *rules)`
```c
void precise_sleep(long long ms, t_rules *rules)
```
- **Sleep preciso** que verifica morte durante espera
- Loop de `usleep(500)` até completar `ms` milissegundos
- **Para imediatamente** se `is_dead(rules)` retorna 1

##### `ft_atoi(const char *s)`
```c
int ft_atoi(const char *s)
```
- Converte string para inteiro
- Protege contra overflow (retorna -1 se > `INT_MAX`)

##### `ft_isdigit_str(const char *s)`
```c
int ft_isdigit_str(const char *s)
```
- Valida se string contém apenas dígitos
- **Retorna**: 1 (válido) ou 0 (inválido)

---

### **9. dead.c**

**Gerencia estado de morte** (variável compartilhada).

#### Funções

##### `is_dead(t_rules *rules)`
```c
int is_dead(t_rules *rules)
```
- **Lê** `someone_died` de forma thread-safe
- **Trava `dead_mutex`** durante leitura
- **Retorna**: 1 (alguém morreu) ou 0 (todos vivos)

##### `set_dead(t_rules *rules)`
```c
void set_dead(t_rules *rules)
```
- **Define** `someone_died = 1` de forma thread-safe
- **Trava `dead_mutex`** durante escrita

**Por que usar mutexes aqui?**
- Evita **race condition** ao acessar variável compartilhada
- Garante **visibilidade** da mudança entre threads

---

### **10. cleanup.c**

**Libera recursos** ao final da simulação.

#### Funções

##### `destroy_forks(pthread_mutex_t *forks, t_rules *rules)`
```c
static void destroy_forks(pthread_mutex_t *forks, t_rules *rules)
```
- Destrói todos os mutexes dos garfos com `pthread_mutex_destroy()`

##### `destroy_rules_mutexes(t_rules *rules)`
```c
static void destroy_rules_mutexes(t_rules *rules)
```
- Destrói os 3 mutexes globais: `write_mutex`, `dead_mutex`, `meal_mutex`

##### `cleanup_all(t_philo *philos, pthread_mutex_t *forks, t_rules *rules)`
```c
void cleanup_all(t_philo *philos, pthread_mutex_t *forks, t_rules *rules)
```
- Destrói garfos
- Libera memória de garfos e filósofos
- Destrói mutexes globais

---

## 🔄 Fluxo de Execução

### 1. **Inicialização**

```
main()
  ↓
parse_args()         → Valida argumentos
  ↓
init_rules_mutexes() → Cria mutexes globais
  ↓
init_all()           → Cria filósofos e garfos
  ├─ init_forks()    → Aloca e inicializa mutexes
  └─ init_philos()   → Aloca e configura filósofos
```

### 2. **Execução (caso normal: 2+ filósofos)**

```
start_threads()
  ↓
[Thread principal]           [Threads de filósofos]
      ↓                              ↓
monitor_loop()              philo_routine() (cada thread)
  ├─ Verifica morte              ├─ take_forks()
  ├─ Verifica refeições          ├─ philo_eat()
  └─ Aguarda 500µs               ├─ drop_forks()
                                  └─ philo_sleep_think()
      ↓                              ↓
join_threads()              [Thread termina]
```

### 3. **Finalização**

```
cleanup_all()
  ├─ destroy_forks()       → Destrói mutexes dos garfos
  ├─ free(forks)           → Libera memória
  ├─ free(philos)          → Libera memória
  └─ destroy_rules_mutexes() → Destrói mutexes globais
```

---

## 🔒 Sincronização e Threads

### **Mutexes Utilizados**

#### 1. **Garfos (N mutexes)**
- **Tipo**: `pthread_mutex_t forks[n_philos]`
- **Propósito**: Garantir acesso exclusivo aos garfos
- **Uso**: Travados em `take_forks()`, destravados em `drop_forks()`

#### 2. **write_mutex**
- **Propósito**: Serializar impressões no terminal
- **Problema sem mutex**: Saídas embaralhadas
- **Uso**: Travado em `print_state()` e `print_death()`

#### 3. **dead_mutex**
- **Propósito**: Proteger variável compartilhada `someone_died`
- **Problema sem mutex**: Race condition ao verificar/definir morte
- **Uso**: Travado em `is_dead()` e `set_dead()`

#### 4. **meal_mutex**
- **Propósito**: Proteger `last_meal` e `meals_eaten`
- **Problema sem mutex**: Monitor pode ler valor desatualizado
- **Uso**: Travado em:
  - `philo_eat()` (atualiza valores)
  - `monitor_loop()` (lê valores)
  - `all_ate_enough()` (lê valores)

### **Threads**

| Thread | Função | Responsabilidade |
|--------|--------|------------------|
| **Principal** | `monitor_loop()` | Detectar morte e conclusão |
| **Filósofos** (N) | `philo_routine()` | Executar ciclo pensar-comer-dormir |

---

## 🚫 Prevenção de Deadlock

### **Problema**

Se todos os filósofos pegarem o garfo esquerdo simultaneamente, ninguém consegue pegar o direito → **deadlock**.

### **Solução Implementada**

**Ordem de aquisição diferente por paridade**:

```c
// Filósofos PARES (ID 2, 4, 6...)
pthread_mutex_lock(right_fork);  // Pega direito primeiro
pthread_mutex_lock(left_fork);   // Depois esquerdo

// Filósofos ÍMPARES (ID 1, 3, 5...)
pthread_mutex_lock(left_fork);   // Pega esquerdo primeiro
pthread_mutex_lock(right_fork);  // Depois direito
```

**Por que funciona?**
- Quebra a **espera circular** (condição necessária para deadlock)
- Pelo menos um filósofo sempre pode progredir

### **Otimizações Adicionais**

#### 1. **Delay inicial para filósofos pares**
```c
if (p->id % 2 == 0)
    usleep(1000);  // 1ms de atraso
```
- Dessincroniza início das threads
- Reduz contenção inicial pelos garfos

#### 2. **Tempo de pensamento para mesa ímpar**
```c
if (n_philos % 2 != 0)
{
    think_time = (t_eat * 2) - t_sleep;
    precise_sleep(think_time);
}
```
- **Problema**: Com N ímpar, pode haver starvation
- **Solução**: Ajusta ritmo para equilibrar acesso aos garfos

---

## 🛠️ Compilação e Uso

### **Compilar**

```bash
make
```

**Gera**: Executável `philo`

**Flags utilizadas**:
- `-Wall -Wextra -Werror`: Avisos rigorosos
- `-pthread`: Suporte a threads POSIX
- `-g`: Símbolos de debug

### **Executar**

```bash
./philo [n_philos] [t_die] [t_eat] [t_sleep] [must_eat (opcional)]
```

**Parâmetros**:
- `n_philos`: Número de filósofos (e garfos)
- `t_die`: Tempo em ms até morte sem comer
- `t_eat`: Tempo em ms para comer
- `t_sleep`: Tempo em ms para dormir
- `must_eat`: (Opcional) Número mínimo de refeições

### **Exemplos**

#### 1. **Ninguém deve morrer**
```bash
./philo 5 800 200 200
```
- 5 filósofos
- Morrem em 800ms sem comer
- Comem em 200ms, dormem em 200ms

#### 2. **Com limite de refeições**
```bash
./philo 4 410 200 200 10
```
- Simula até todos comerem 10 vezes

#### 3. **Caso extremo (alta pressão)**
```bash
./philo 4 310 200 100
```
- Tempo apertado: `t_die = 310ms`, mas `t_eat + t_sleep = 300ms`

---

## ⚠️ Casos Especiais

### **1. Um único filósofo**

```c
if (rules.n_philos == 1)
{
    print_state(&philos[0], "has taken a fork");
    precise_sleep(rules.t_die, &rules);
    print_death(&philos[0]);
}
```

**Problema**: Apenas 1 garfo disponível, impossível comer  
**Solução**: Tratamento especial sem criar threads

**Saída esperada**:
```
0 1 has taken a fork
800 1 died
```

### **2. Valores inválidos**

```bash
./philo 5 -100 200 200  # Tempo negativo
./philo 0 800 200 200   # Zero filósofos
./philo 5 800 200 abc   # Não numérico
```

**Tratamento**:
- `parse_args()` valida e retorna erro
- Mensagens descritivas: "Error: invalid args value"

---

## 📊 Exemplo de Saída

```bash
./philo 3 800 200 200
```

**Saída** (timestamps em ms):
```
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
0 3 has taken a fork
0 3 has taken a fork
0 3 is eating
200 1 is sleeping
200 2 has taken a fork
200 2 has taken a fork
200 2 is eating
200 3 is sleeping
400 1 is thinking
400 1 has taken a fork
400 1 has taken a fork
400 1 is eating
400 2 is sleeping
400 3 is thinking
...
```

**Padrão observado**:
- IDs não aparecem em ordem sequencial (concorrência real)
- Timestamps mostram paralelismo
- Sem mensagens após morte

---

## 🧵 Conceitos de Threading

### **Race Condition**

**Problema**:
```c
// Sem mutex
philo->meals_eaten++;  // Thread A lê 5, escreve 6
                        // Thread B lê 5 (valor antigo), escreve 6
```

**Solução**:
```c
pthread_mutex_lock(&meal_mutex);
philo->meals_eaten++;
pthread_mutex_unlock(&meal_mutex);
```

### **Data Race vs Race Condition**

- **Data Race**: Acesso simultâneo não sincronizado a memória
- **Race Condition**: Resultado depende de timing (mais amplo)

Este projeto previne ambos com mutexes.

---

## 🎓 Lições Aprendidas

### **1. Deadlock é evitável**
- Ordem de aquisição de recursos resolve
- Não precisa de algoritmos complexos

### **2. Busy-waiting é caro**
```c
// ❌ RUIM
while (get_time_ms() - start < ms);  // 100% CPU

// ✅ BOM
while (get_time_ms() - start < ms)
    usleep(500);  // Libera CPU periodicamente
```

### **3. Verificação de morte é crítica**
- Todas as operações longas checam `is_dead()`
- Permite terminação rápida após morte

### **4. Mutexes têm custo**
- Não travar por muito tempo
- Região crítica deve ser mínima

---

## 🐛 Debugging

### **Data Races (Helgrind)**

```bash
valgrind --tool=helgrind ./philo 5 800 200 200
```

### **Memory Leaks (Valgrind)**

```bash
valgrind --leak-check=full ./philo 5 800 200 200
```

### **Deadlock Debugging**

Se programa trava:
1. Verificar ordem de mutexes
2. Garantir que todos os `lock()` têm `unlock()`
3. Checar se `is_dead()` está sendo verificado

---

## 📚 Referências

- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- [Dining Philosophers Problem (Wikipedia)](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [pthread_mutex_lock man page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)

---

## ✅ Checklist de Implementação

- [x] Parse e validação de argumentos
- [x] Inicialização de estruturas
- [x] Criação de threads
- [x] Rotina dos filósofos
- [x] Monitoramento de morte
- [x] Prevenção de deadlock (ordem de mutexes)
- [x] Sincronização de impressão
- [x] Proteção de dados compartilhados
- [x] Caso especial: 1 filósofo
- [x] Limpeza de recursos
- [x] Tratamento de mesa ímpar

---

## 🏁 Conclusão

Este projeto demonstra:
- **Sincronização de threads** com mutexes
- **Prevenção de deadlock** com ordenação de recursos
- **Monitoramento concorrente** de múltiplas threads
- **Gerenciamento seguro** de recursos compartilhados

A solução é **robusta**, **eficiente** e **livre de data races**, seguindo as melhores práticas de programação concorrente.

