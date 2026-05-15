## 34. Pièges classiques

- Confondre entraînement et évaluation.
- Lancer `pacman.py` et croire que ça entraîne.
- Mettre `alpha` trop grand.
- Oublier de sauvegarder les poids.
- Écraser de bons poids avec un entraînement trop court.
- Utiliser une profondeur trop grande et attendre trop longtemps.
- Tester sur une seule carte.
- Croire qu'un score positif veut toujours dire victoire.
- Choisir `Stop` trop souvent.
- Croire que 100% de winrate est obligatoire.
- Copier le projet d'une autre personne sans comprendre.

## 35. Résumé final ultra simple

Imagine que Pac-Man doit choisir un chemin dans un labyrinthe.

Il fait deux choses :

```text
1. Il imagine plusieurs futurs possibles avec Minimax.
2. Pour juger ces futurs, il utilise une formule avec des poids appris.
```

Les features sont ses yeux :

```text
où est la nourriture ?
où sont les fantômes ?
où sont les capsules ?
quel est le score ?
```

Les poids sont son expérience :

```text
cette chose est importante
cette chose est dangereuse
cette chose vaut la peine
```

L'entraînement sert à améliorer ces poids.

Phrase à retenir pour l'examen :

```text
Mon agent RLMinimax utilise alpha-bêta pour chercher dans l'arbre de jeu,
et une fonction d'évaluation affine dont les poids sont appris par renforcement.
```

## 36. Audit final du 12 mai 2026 ✅

Le projet a été revérifié avec le PDF officiel `projet.pdf`.

Résumé très simple :

```text
Le PDF demande un agent RLMinimax.
Cet agent doit être un Minimax alpha-bêta.
Sa fonction d'évaluation doit être affine.
Les poids doivent être appris par renforcement.
Le projet doit être comparé au Minimax du labo 1.
```

Formule à défendre :

```text
f(s) = a1*x1(s) + a2*x2(s) + ... + ak*xk(s) + C
```

Résultats importants obtenus :

| Test | Résultat |
|---|---|
| Compilation Python | OK |
| Isolation du projet | OK, aucune dépendance trouvée vers les labos ou le dossier collègue |
| Poids extrêmes | OK, valeurs invalides refusées avec fallback propre |
| `testClassic` depth 1 | RLMinimax gagne 100% et bat AlphaBeta |
| `smallClassic` depth 2 | RLMinimax bat AlphaBeta |
| `capsuleClassic` depth 2 | RLMinimax bat AlphaBeta |
| `mediumClassic` depth 1 | RLMinimax bat AlphaBeta |
| `originalClassic` depth 1 | RLMinimax bat AlphaBeta |
| `Stop` | 0 en moyenne pour RLMinimax |

Décision sur l'entraînement :

```text
On ne relance pas un très gros entraînement si les poids actuels sont déjà bons.
Un entraînement long peut améliorer une carte mais casser une autre.
On garde donc les poids actuels comme meilleurs poids tant que les tests restent bons.
```

Commandes utiles :

```bash
python -m py_compile compare.py pacman.py layout.py train.py rlMinimaxAgents.py features.py multiAgents.py
python pacman.py -p RLMinimaxAgent -l testClassic -a depth=1 -n 3 -q
python compare.py --num-games 100 --layout smallClassic --depth 2
python compare.py --num-games 100 --layout capsuleClassic --depth 2
```

Phrase orale très simple :

```text
J'ai testé que mon agent ne dépend pas des dossiers extérieurs,
que ses poids invalides ne cassent pas le programme,
et qu'il bat l'AlphaBeta local sur plusieurs cartes.
```
