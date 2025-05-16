# 🎮 | (A)void Game 
Jogo de Aventura em Terminal, desenvolvido para a disciplina de **Programação Imperativa e Funcional 2025.1** na **CESAR School**, sob orientação do docente **Diego de Freitas Bezerra**.

---

### 📄 | Índice  
1. [Sobre o Projeto](#-sobre-o-projeto)  
2. [Características do Jogo](#-características-do-jogo)  
3. [Arquitetura do Código](#-arquitetura-do-código)  
4. [Como Jogar](#-como-jogar)  
5. [Equipe](#-equipe)
6. [Licença](#licenca)

***

### 📌 | Sobre o Projeto  
Este projeto implementa um jogo de aventura interativo no terminal, escrito em linguagem C, onde o jogador controla um personagem que deve **evitar obstáculos**, **sobreviver a diferentes fases** e **derrotar um chefão final**.

— 

**Recursos do Jogo:**

> Sistema de fases progressivas

> Sistema de pontuação persistente

> Menu interativo com navegação via teclado

> Mecânicas de movimentação e combate com tiros  


***

### 🎮 | Características do Jogo  

**1. Sistema de Menu Interativo**

Menu inicial com 4 opções

» Novo Jogo

» Pontuação

» Configuração

» Fechar Jogo  

- Navegação por teclas ↑/↓

- Feedback visual com seta indicadora da seleção atual


**2. Mecânica do Jogo**

- Movimentação em 4 direções (↑ ↓ ← →)  
- Sistema de fases com obstáculos em diferentes padrões:
  - **Fase 1:** Direita → Esquerda  
  - **Fase 2:** Esquerda → Direita  
  - **Fase 3:** Cima → Baixo  
  - **Fase 4:** Baixo → Cima  
  - **Fase 5:** Batalha contra o chefão final  

**3. Sistema de Combate**

- Tiros (barra de espaço) para enfrentar o chefão  
- Chefão também atira contra o jogador  
- Barra de vida do chefão com 10 pontos  
- Detecção de colisão:
  - Tiro do jogador vs. chefão  
  - Tiro do chefão vs. jogador  

**4. Elementos Visuais**

- Sprites ASCII para personagem, obstáculos e chefão  
- Efeitos visuais: tiros e explosões  
- Mensagens de Game Over e transições de fase  

**5. Sistema de Pontuação** 
- Pontuação por tempo de sobrevivência  
- +100 pontos por acerto no chefão  
- Sistema de recordes salvo entre sessões  
- Visualização de Top 20 pontuações

***

### 🧠 | Arquitetura do Código  

**Estruturas de Dados Principais**
- **Personagem:** Sprite ASCII do jogador  
- **Obstáculos:** Tamanho e posição  
- **Tiro:** Projéteis do jogador e do chefão  
- **Pontuação:** Dados com timestamp  

**1. Funções-Chave**
- **Renderização:**  
  `printPersonagem()`, `printObj()`, `printBossFinal()`, `printTiros()`  
- **Lógica de jogo:**  
  `verificarColisaoFases()`, `verificarColisaoTiroComBoss()`, `moverObsFase1()`, `moverObsFase2()`...  
- **Gerenciamento de Estado:**  
  `salvar_pontuacao()`, `ler_pontuacoes()`  

**2. Bibliotecas Utilizadas**
- `windows.h`: Funcionalidades do Windows  
- `time.h`: Registro de tempo e pontuações  
- `screen.h`, `keyboard.h`, `timer.h`: Bibliotecas customizadas  

**3. Fluxo do Jogo**

Menu Inicial → Novo Jogo → Fases 1 a 4 (obstáculos) → Fase 5 (chefão) → Game Over → Menu ou Pontuação  

---

### 🎯 | Como Jogar  

**Compilação e Execução**

1. Compile o código:
   ```bash
   gcc ./src/*.c -I ./include/

2. Execute o jogo:

   ```bash
   gcc ./src/*.c -I ./include/ -o game

3. Execute o jogo:

   ```bash
   ./game.exe

> Certifique-se de que todas as bibliotecas customizadas estejam no mesmo diretório do projeto.

***

### 👥 | Equipe

- Aline de Albuquerque Henriques - @aline-henriques
- Allan Ronald - @AllanVasconceL0s
- Bruno Felipe de Castilhos Gomes Rego - @Bruno-cod
- Thyalles Araújo Campos - @Thyalles

***

### ©️ | Licença MIT 

Este projeto é de uso educacional e não possui fins comerciais.
Todos os direitos reservados aos autores e à CESAR School.
