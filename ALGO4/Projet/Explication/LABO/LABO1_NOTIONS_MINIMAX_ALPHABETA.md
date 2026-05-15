# Labo 1 - Minimax et Alpha-Beta

## 1. Idee simple

Pac-Man veut gagner.

Les fantomes veulent l'empecher de gagner.

Minimax sert a choisir une action quand plusieurs personnages ont des objectifs opposes.

## 2. MAX et MIN

| Role | Personnage | Objectif |
|---|---|---|
| MAX | Pac-Man | Avoir le meilleur score |
| MIN | Fantomes | Faire baisser le score de Pac-Man |

## 3. Arbre de recherche

Un arbre de recherche represente les futurs possibles.

Exemple :

```text
Pac-Man choisit une action.
Puis le fantome repond.
Puis Pac-Man rejoue.
```

Plus on regarde loin, plus la decision est intelligente, mais plus le calcul est long.

## 4. Profondeur

La profondeur indique combien de coups on regarde dans le futur.

Profondeur faible :

```text
Rapide, mais parfois un peu aveugle.
```

Profondeur elevee :

```text
Plus intelligent, mais plus lent.
```

## 5. Fonction d'evaluation

Quand on ne peut pas tout calculer jusqu'a la fin, on donne une note a la situation.

Une bonne evaluation prefere :

1. La nourriture proche.
2. Les capsules utiles.
3. Le score eleve.
4. Les fantomes dangereux loin.

## 6. Minimax

Minimax dit :

```text
Pac-Man choisit le meilleur coup en supposant que les fantomes vont jouer contre lui.
```

## 7. Alpha-Beta

Alpha-Beta donne le meme resultat que Minimax, mais evite certains calculs inutiles.

Phrase simple :

```text
Si une branche ne peut plus devenir meilleure, on arrete de la regarder.
```

## 8. Lien avec le projet

Dans le projet :

1. Alpha-Beta sert a chercher les coups possibles.
2. La fonction d'evaluation sert a noter les positions.
3. RLMinimax utilise des poids appris pour rendre cette evaluation meilleure.

## 9. A savoir pour l'oral

Tu dois pouvoir dire :

```text
Minimax cherche le meilleur coup contre un adversaire.
Alpha-Beta optimise Minimax en coupant les branches inutiles.
Notre projet utilise Alpha-Beta avec une evaluation apprise.
```
