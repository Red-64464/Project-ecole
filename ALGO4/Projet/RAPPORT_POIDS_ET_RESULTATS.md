# Rapport des poids et resultats - RLMinimax

## 1. Objectif du projet

Le sujet demande de combiner deux idees :

1. Un agent Minimax avec elagage Alpha-Beta.
2. Une fonction d'evaluation affine dont les poids sont appris par renforcement.

L'agent final du projet est :

```text
RLMinimaxAgent
```

Il est implemente dans :

```text
rlMinimaxAgents.py
```

## 2. Fonction d'evaluation utilisee

La fonction respecte le modele du sujet :

```text
f(s) = a1*x1(s) + a2*x2(s) + a3*x3(s) + a4*x4(s)
     + a5*x5(s) + a6*x6(s) + a7*x7(s) + C
```

Les facteurs `x_i(s)` sont calcules dans `features.py`.
Les poids `a_i` et la constante `C` sont stockes dans `weights.json`.

## 3. Facteurs choisis

| Facteur | Fonction | Sens simple | Methode |
|---:|---|---|---|
| x1 | `nearest_food_bfs` | Nourriture proche | BFS |
| x2 | `ghosts_within_3` | Fantomes dangereux proches | BFS |
| x3 | `scared_ghosts_nearby` | Fantomes mangeables proches | BFS |
| x4 | `remaining_food` | Quantite de nourriture restante | Lecture de l'etat |
| x5 | `nearest_capsule_bfs` | Capsule proche | BFS |
| x6 | `current_score` | Score courant normalise | Lecture de l'etat |
| x7 | `nearest_dangerous_ghost_bfs` | Proximite du danger principal | BFS |

Ces facteurs sont lies au jeu Pac-Man et sont distincts :

- x1 aide a aller vers la nourriture.
- x2 et x7 aident a eviter le danger.
- x3 aide a profiter des fantomes effrayes.
- x4 encourage a finir la carte.
- x5 tient compte des capsules.
- x6 garde le lien avec le score officiel du jeu.

## 4. Poids finaux

```json
{
  "weights": [
    0.6891,
    -1.4093,
    1.4061,
    0.4036,
    0.0252,
    1.8572,
    -1.2624
  ],
  "bias": -0.0834
}
```

Lecture des poids :

| Facteur | Poids | Interpretation |
|---|---:|---|
| x1 nourriture proche | 0.6891 | Effet positif : chercher la nourriture est utile |
| x2 fantomes dangereux proches | -1.4093 | Effet negatif : eviter les fantomes dangereux |
| x3 fantomes effrayes proches | 1.4061 | Effet positif : profiter des fantomes mangeables |
| x4 nourriture restante negative | 0.4036 | Effet positif sur une valeur deja negative : moins de nourriture restante est mieux |
| x5 capsule proche | 0.0252 | Effet positif faible : utile, mais moins dominant |
| x6 score courant normalise | 1.8572 | Effet positif fort : respecter le score du jeu |
| x7 danger principal proche | -1.2624 | Effet negatif : rester loin du danger |
| C biais | -0.0834 | Correction globale independante de l'etat |

## 5. Entrainement

L'entrainement se fait dans `train.py`.

Le projet n'apprend pas une strategie directe.
Il apprend les poids de la fonction d'evaluation.
Ensuite, `RLMinimaxAgent` utilise ces poids dans Alpha-Beta.

Parametres importants disponibles :

| Parametre | Role |
|---|---|
| `--num-games` | Nombre de parties d'entrainement |
| `--alpha` | Vitesse d'apprentissage |
| `--gamma` | Importance du futur |
| `--epsilon` | Exploration aleatoire pendant l'entrainement |
| `--depth` | Profondeur de recherche pendant l'entrainement |
| `--resume` | Reprendre depuis `weights.json` |

## 6. Resultats finaux

Comparaison faite avec les poids actuels, sans reentrainement.

| Layout | Depth | Parties | Minimax | AlphaBeta | RLMinimax | RL vs Minimax | RL vs AlphaBeta | Win RL | Stop RL |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| testClassic | 1 | 200 | 548.5 | 548.5 | 552.5 | +4.0 | +4.0 | 100% | 0.0 |
| smallClassic | 2 | 100 | -149.8 | -149.8 | 196.6 | +346.4 | +346.4 | 34% | 0.0 |
| capsuleClassic | 2 | 100 | -228.9 | -228.9 | -135.2 | +93.7 | +93.7 | 12% | 0.0 |
| mediumClassic | 1 | 50 | 14.5 | 14.5 | 515.5 | +501.0 | +501.0 | 28% | 0.0 |
| originalClassic | 1 | 50 | 348.6 | 348.6 | 664.4 | +315.7 | +315.7 | 2% | 0.0 |
| trickyClassic | 1 | 50 | 101.3 | 101.3 | 293.0 | +191.7 | +191.7 | 2% | 0.0 |
| contestClassic | 1 | 50 | 11.3 | 11.3 | 244.4 | +233.1 | +233.1 | 28% | 0.0 |
| minimaxClassic | 3 | 100 | -243.5 | -243.5 | -153.1 | +90.4 | +90.4 | 34% | 0.0 |
| trappedClassic | 1 | 1000 | -392.3 | -392.3 | -391.4 | +0.9 | +0.9 | 11% | 0.0 |
| openClassic | 1 | 10 | -419.5 | -745.4 | 1057.0 | +1476.5 | +1802.4 | 90% | 0.0 |

## 7. Interpretation

Les resultats montrent que RLMinimax ameliore le score moyen sur tous les layouts testes.

Le taux de victoire n'est pas toujours eleve, mais ce n'est pas contradictoire :

- les fantomes ont une part de hasard ;
- certains layouts sont difficiles ;
- le score moyen mesure aussi la qualite du comportement avant la victoire ou la defaite.

Le point important est que la comparaison est reproductible et faite dans les memes conditions.

## 8. Commandes reproductibles

Exemple principal :

```bash
python compare.py --num-games 100 --layout smallClassic --depth 2 --baseline both
```

Toutes les commandes de verification sont listees dans :

```text
RESULTATS_FINAUX_RL_MINIMAX.md
```

## 9. Conclusion

Le projet respecte l'objectif demande :

```text
Alpha-Beta + fonction affine + poids appris par renforcement + comparaison statistique.
```
