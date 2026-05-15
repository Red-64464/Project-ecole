# Labo 2 - Reinforcement Learning

## 1. Idee simple

Le Reinforcement Learning veut dire :

```text
Apprendre en essayant, en gagnant des recompenses ou en prenant des penalites.
```

Pac-Man joue des parties. Ensuite, on ajuste les poids pour mieux jouer.

## 2. Ce que Pac-Man apprend

Pac-Man n'apprend pas les regles du jeu.

Les regles existent deja dans Berkeley.

Pac-Man apprend surtout :

```text
Quelles informations sont importantes pour choisir une bonne action.
```

## 3. Features

Une feature est une information transformee en nombre.

Exemples :

| Feature | Sens |
|---|---|
| nourriture proche | Est-ce qu'une nourriture est facile a atteindre ? |
| fantome proche | Est-ce que Pac-Man est en danger ? |
| capsule proche | Est-ce qu'une capsule peut aider ? |
| score actuel | Est-ce que la partie avance bien ? |

## 4. Poids

Un poids dit l'importance d'une feature.

Exemple :

```text
fantome dangereux proche = poids negatif
nourriture proche = poids positif
```

Donc Pac-Man apprend a aimer ou eviter certaines situations.

## 5. Formule affine

La fonction d'evaluation ressemble a ceci :

```text
evaluation = poids_1 * feature_1 + poids_2 * feature_2 + ... + biais
```

Cette formule donne une note a une position.

## 6. Alpha, gamma et epsilon

| Mot | Sens simple |
|---|---|
| alpha | Vitesse d'apprentissage |
| gamma | Importance du futur |
| epsilon | Part de hasard pour explorer |

Epsilon est utile parce qu'au debut Pac-Man doit parfois essayer des actions differentes pour apprendre.

## 7. `weights.json`

`weights.json` garde les poids appris.

Il permet de relancer l'agent avec la memoire de l'entrainement.

## 8. Pourquoi tester plusieurs cartes

Un agent peut bien marcher sur une carte et moins bien sur une autre.

C'est normal, car les labyrinthes changent.

Il faut donc tester plusieurs situations pour savoir si l'agent est robuste.

## 9. Lien avec RLMinimax

RLMinimax combine deux idees :

1. Alpha-Beta cherche les futurs coups possibles.
2. Les poids appris evaluent les positions atteintes.

## 10. A savoir pour l'oral

```text
Le Reinforcement Learning sert ici a apprendre les poids de la fonction d'evaluation.
Ces poids aident RLMinimax a mieux juger les positions du jeu.
```
