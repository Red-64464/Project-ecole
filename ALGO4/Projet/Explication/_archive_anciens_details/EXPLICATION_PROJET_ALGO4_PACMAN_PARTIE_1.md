# EXPLICATION PROJET ALGO4 PAC-MAN IA

Ce document explique le projet avec des mots simples 🙂  
Le but est que tu puisses relire le projet avant l'examen et expliquer ce que fait ton code.

## 1. À quoi sert ce projet ?

Le projet sert à créer une IA qui joue à Pac-Man.

Elle doit combiner deux idées :

- Minimax avec élagage alpha-bêta : Pac-Man réfléchit aux coups futurs.
- Apprentissage par renforcement : Pac-Man apprend les poids de sa fonction d'évaluation.

Le PDF ne demande pas un simple agent Q-Learning qui joue directement.  
Il demande un agent `RLMinimaxAgent` : un Minimax alpha-bêta dont la fonction d'évaluation utilise des poids appris.

Formule principale :

```text
f(s) = a1*x1(s) + a2*x2(s) + ... + ak*xk(s) + C
```

- `s` = état du jeu.
- `xi(s)` = une feature, donc une information calculée sur l'état.
- `ai` = un poids appris.
- `C` = une constante, appelée aussi biais.

## 2. C’est quoi un agent IA ?

Un agent IA est un programme qui observe une situation et choisit une action.

Exemple de vraie vie :

```text
Tu es à un passage piéton.
Tu regardes la route.
Si une voiture arrive, tu attends.
Sinon, tu traverses.
```

Tu es l'agent. La route est l'état. Traverser ou attendre sont les actions.

Exemple Pac-Man :

```text
Pac-Man voit :
- où il est
- où sont les fantômes
- où est la nourriture
- où sont les capsules

Puis il choisit :
North, South, East, West ou Stop
```

## 3. C’est quoi un état du jeu ?

Un état du jeu, appelé `gameState`, est une photo complète de la partie à un moment précis.

Il contient par exemple :

- la position de Pac-Man
- la position des fantômes
- la nourriture restante
- les capsules restantes
- le score actuel
- si la partie est gagnée ou perdue

Dans le code, on utilise souvent :

```python
game_state.getPacmanPosition()
game_state.getGhostStates()
game_state.getFood()
game_state.getCapsules()
game_state.getScore()
game_state.isWin()
game_state.isLose()
```

## 4. C’est quoi une action ?

Une action est un mouvement possible.

Pac-Man peut choisir :

```text
North  = aller vers le haut
South  = aller vers le bas
East   = aller à droite
West   = aller à gauche
Stop   = ne pas bouger
```

Le code récupère les actions possibles avec :

```python
actions = game_state.getLegalActions(0)
```

`0` veut dire Pac-Man.

## 5. C’est quoi une récompense ?

Une récompense est ce que l'agent gagne ou perd après une action.

Exemples :

- manger une gomme donne des points
- manger un fantôme donne beaucoup de points
- perdre la partie donne un gros mauvais résultat
- faire un pas coûte souvent un petit point

Dans `train.py`, la récompense est calculée simplement :

```text
reward = score_suivant - score_actuel
```

Si le score monte, la récompense est positive.  
Si le score baisse, elle est négative.
