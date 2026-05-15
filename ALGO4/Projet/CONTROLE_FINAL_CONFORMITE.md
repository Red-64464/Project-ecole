# Controle final de conformite

Date de verification : 14 mai 2026.

Documents verifies :

```text
projet.pdf
evaluation_screenshots/evaluation_extracted_text.txt
evaluation_screenshots/evaluation/
```

Captures PNG generees :

```text
evaluation_screenshots/projet/
evaluation_screenshots/evaluation/
```

## 1. Checklist du sujet `projet.pdf`

| Exigence du sujet | Verification dans le projet | Statut |
|---|---|---|
| Choisir des facteurs `x_i(s)` lies a Pac-Man | 7 facteurs dans `features.py` | OK |
| Certains facteurs doivent utiliser de la recherche | BFS utilise pour nourriture, fantomes et capsules | OK |
| Utiliser une fonction affine | `rl_evaluation_function` calcule `sum(ai*xi) + C` | OK |
| Poids independants de l'etat | Poids stockes dans `weights.json` | OK |
| Concevoir un agent Minimax avec Alpha-Beta | `RLMinimaxAgent` dans `rlMinimaxAgents.py` | OK |
| Utiliser la fonction apprise dans RLMinimax | `RLMinimaxAgent` charge `weights.json` | OK |
| Apprendre les poids par renforcement | `train.py` ajuste les poids avec une mise a jour TD | OK |
| Ne pas apprendre une strategie directe | L'entrainement apprend les poids, pas une politique finale | OK |
| Comparer avec Minimax | `compare.py --baseline both` compare Minimax, AlphaBeta et RLMinimax | OK |
| Produire des statistiques | Score moyen, winrate, temps moyen, coups moyens, Stop moyen | OK |

## 2. Checklist de la grille d'evaluation (captures `evaluation_screenshots/`)

| Critere utile pour le projet | Verification | Statut |
|---|---|---|
| README present | `README.md` a la racine du projet | OK |
| Resultats reproductibles | Commandes donnees dans `RESULTATS_FINAUX_RL_MINIMAX.md` | OK |
| Code clair et modulaire | `features.py`, `train.py`, `compare.py`, `rlMinimaxAgents.py` separent les responsabilites | OK |
| Fonction d'evaluation pertinente | Features liees a nourriture, fantomes, capsules, score | OK |
| Structure affine imposee | Pas de termes croises comme `x1*x2` ou `x1^2` | OK |
| Poids appris | `train.py` produit `weights.json` | OK |
| Signification des poids documentee | `RAPPORT_POIDS_ET_RESULTATS.md` | OK |
| Comparaison statistique | 10 layouts testes | OK |
| Comparaison controlee | `compare.py` fixe `--seed 0` par defaut | OK |
| Analyse critique | Limites expliquees dans les rapports | OK |

## 3. Verification du code

Fichiers principaux controles :

| Fichier | Role | Resultat |
|---|---|---|
| `features.py` | Calcule les facteurs | OK |
| `rlMinimaxAgents.py` | Agent final Alpha-Beta + evaluation affine | OK |
| `train.py` | Apprentissage des poids | OK |
| `compare.py` | Comparaison des agents | OK |
| `multiAgents.py` | Baselines Minimax et AlphaBeta | OK |
| `weights.json` | Poids finaux | OK |
| `run.ps1` / `run.sh` | Scripts de lancement | OK, entrainement optionnel |

Correction faite pendant le controle :

```text
RLMinimax ne filtre plus les actions dangereuses avant Alpha-Beta.
Il enlève seulement Stop quand Pac-Man a au moins un vrai mouvement possible.
Toutes les autres actions legales sont comparees par Alpha-Beta.
```

Pourquoi c'est mieux :

```text
L'algorithme est plus proche d'un Minimax Alpha-Beta standard.
La logique est plus simple a defendre a l'oral.
```

## 4. Verification automatique

Commande executee :

```bash
python -m py_compile compare.py game.py pacman.py layout.py train.py rlMinimaxAgents.py features.py multiAgents.py ghostAgents.py textDisplay.py util.py
```

Resultat :

```text
Aucune erreur de syntaxe.
```

Tests rapides executes :

```bash
python compare.py --num-games 5 --layout testClassic --depth 1 --baseline both
python compare.py --num-games 5 --layout smallClassic --depth 1 --baseline both
python pacman.py -p RLMinimaxAgent -l testClassic -a depth=1 -q -n 1 -f
python compare.py --num-games 100 --layout smallClassic --depth 2 --baseline both
.\run.ps1 -CompareGames 1 -DemoGames 1 -Layout testClassic -Depth 1
bash run.sh
```

Resultat :

```text
Les commandes fonctionnent.
RLMinimax joue correctement.
La comparaison officielle smallClassic/100 parties reproduit le gain +346.4.
Les scripts de lancement sautent l'entrainement par defaut et gardent weights.json.
```

## 5. Verification large des resultats

Une verification large a ete lancee sans reentrainer les poids.

Resultat principal :

```text
RLMinimax bat Minimax et AlphaBeta en score moyen sur tous les layouts testes.
```

Details :

| Layout | Parties | Resultat RLMinimax |
|---|---:|---|
| testClassic | 200 | meilleur score moyen |
| smallClassic | 100 | meilleur score moyen |
| capsuleClassic | 100 | meilleur score moyen |
| mediumClassic | 50 | meilleur score moyen |
| originalClassic | 50 | meilleur score moyen |
| trickyClassic | 50 | meilleur score moyen |
| contestClassic | 50 | meilleur score moyen |
| minimaxClassic | 100 | meilleur score moyen |
| trappedClassic | 1000 | legerement meilleur score moyen |
| openClassic | 10 | meilleur score moyen, baselines en timeout |

Le tableau complet est dans :

```text
RAPPORT_POIDS_ET_RESULTATS.md
RESULTATS_FINAUX_RL_MINIMAX.md
```

## 6. Points a defendre a l'oral

Phrase courte :

```text
Notre agent utilise Alpha-Beta pour explorer les coups possibles, puis une fonction affine apprise par renforcement pour evaluer les etats.
```

Points importants :

1. Les features sont des informations simples du jeu.
2. Les poids disent l'importance de chaque feature.
3. Les poids sont stockes dans `weights.json`.
4. L'apprentissage ajuste les poids, pas une politique directe.
5. Alpha-Beta garde la logique Minimax mais evite des calculs inutiles.
6. Les resultats sont compares avec les memes graines aleatoires.

## 7. Limites honnetes

Le projet est conforme, mais il faut pouvoir expliquer ces limites :

1. RLMinimax ne gagne pas toujours, car les layouts et les fantomes restent difficiles.
2. Un meilleur score moyen ne veut pas toujours dire un winrate parfait.
3. Les poids sont bons pour les layouts testes, mais pas mathematiquement optimaux pour toutes les cartes possibles.
4. `openClassic` est particulier, car les agents de reference depassent le timeout choisi.

## 8. Points non verifiables ici

Le dossier local `Projet` n'est pas un depot Git.
Donc l'historique des commits et des pushs ne peut pas etre verifie depuis ce dossier.

Avant la remise, il faut verifier dans le vrai depot de l'ecole :

```text
1 commit/push regulier.
Structure attendue par l'enseignant.
README.md present dans le depot.
```

## 9. Conclusion finale

Pour la partie projet, les exigences principales sont respectees :

```text
fonction affine, facteurs Pac-Man, recherche BFS, Alpha-Beta, poids appris,
comparaison statistique, resultats documentes, commandes reproductibles.
```
