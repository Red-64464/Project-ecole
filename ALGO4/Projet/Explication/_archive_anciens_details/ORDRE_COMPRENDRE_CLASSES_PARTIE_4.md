## 6. Plan de révision rapide en 30 minutes ⏱️

Si tu as très peu de temps, fais ça :

### 5 minutes

Lis :

```text
game.py → Agent
pacman.py → GameState
```

But :

```text
Comprendre ce qu'est un agent et ce qu'est un état.
```

### 5 minutes

Lis :

```text
features.py → les 7 features
```

But :

```text
Comprendre ce que Pac-Man regarde.
```

### 7 minutes

Lis :

```text
multiAgents.py → MinimaxAgent puis AlphaBetaAgent
```

But :

```text
Comprendre MAX, MIN, profondeur et coupures alpha-bêta.
```

### 8 minutes

Lis :

```text
rlMinimaxAgents.py → rl_evaluation_function puis RLMinimaxAgent
```

But :

```text
Comprendre l'agent final et la formule du PDF.
```

### 5 minutes

Lis :

```text
train.py → TrainingRLMinimaxAgent puis train()
compare.py → run_games
```

But :

```text
Comprendre comment les poids sont appris et testés.
```

---

## 7. Plan de révision complet en 2 heures 📚

Si tu as plus de temps :

```text
1. game.py : Agent, Directions, Grid
2. pacman.py : GameState, runGames, loadAgent
3. ghostAgents.py : RandomGhost, DirectionalGhost
4. features.py : BFS + 7 features
5. multiAgents.py : ReflexAgent
6. multiAgents.py : MinimaxAgent
7. multiAgents.py : AlphaBetaAgent
8. rlMinimaxAgents.py : validation des poids
9. rlMinimaxAgents.py : fonction affine
10. rlMinimaxAgents.py : RLMinimaxAgent
11. train.py : TrainingRLMinimaxAgent
12. train.py : boucle train()
13. compare.py : comparaison statistique
14. EXPLICATION_PROJET_ALGO4_PACMAN.md : résumé oral
```

---

## 8. Ce qu'il faut vraiment savoir défendre à l'oral 🎓

Tu dois surtout savoir expliquer :

```text
1. Pourquoi RLMinimax n'est pas un simple Q-agent.
2. Pourquoi il utilise Minimax alpha-bêta.
3. Pourquoi la fonction d'évaluation est affine.
4. Ce que représentent les features.
5. Ce que représentent les poids.
6. Comment les poids sont appris.
7. Pourquoi weights.json est validé.
8. Pourquoi comparer avec AlphaBetaAgent.
9. Pourquoi tester plusieurs layouts.
10. Pourquoi depth plus grand est parfois trop lent.
```

Phrase finale à retenir :

```text
Le moteur Berkeley fait tourner le jeu.
Mon projet ajoute un agent RLMinimax :
il cherche avec AlphaBeta,
il évalue avec une formule affine,
et il utilise des poids appris par renforcement.
```
