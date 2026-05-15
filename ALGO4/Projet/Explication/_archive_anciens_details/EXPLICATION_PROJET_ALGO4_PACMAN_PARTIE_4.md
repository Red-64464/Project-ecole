## 18. C’est quoi alpha ?

`alpha` est le taux d'apprentissage.

Image simple :

```text
alpha grand  -> l'agent change beaucoup d'avis
alpha petit  -> l'agent change doucement
```

Si `alpha` est trop grand, les poids peuvent exploser.  
Le projet utilise par défaut :

```text
alpha = 0.001
```

C'est cohérent avec la conversation du groupe.

## 19. C’est quoi epsilon ?

`epsilon` est la probabilité d'explorer.

Exemple :

```text
epsilon = 0.05
```

Cela veut dire :

```text
5% du temps, Pac-Man teste une action aléatoire pendant l'entraînement.
95% du temps, il suit son alpha-bêta.
```

Important :

```text
epsilon sert pendant l'entraînement.
En évaluation, on veut epsilon = 0.
```

## 20. C’est quoi discount/gamma ?

`gamma` dit à quel point le futur compte.

```text
gamma proche de 0 -> l'agent pense surtout au présent
gamma proche de 1 -> l'agent pense beaucoup au futur
```

Dans ton projet :

```text
gamma = 0.9
```

Donc le futur compte beaucoup.

## 21. C’est quoi numTraining ?

`numTraining` est souvent utilisé dans les projets Berkeley pour dire :

```text
les X premières parties servent à apprendre
les parties suivantes servent à évaluer
```

Dans ton projet, l'entraînement se fait surtout avec :

```bash
python train.py --num-games 1000
```

Donc ici, `--num-games` joue le rôle principal pour le nombre d'épisodes d'entraînement.

## 22. C’est quoi un feature extractor ?

Un feature extractor transforme un état compliqué en une liste de nombres simples.

Exemple :

```text
État complet du jeu :
- murs
- fantômes
- nourriture
- capsules
- score

Features :
[nourriture_proche, fantômes_proches, capsules_proches, score_normalisé, ...]
```

Dans ton projet :

```python
extract_features(game_state)
```

retourne 7 nombres.

## 23. C’est quoi un poids ?

Un poids dit l'importance d'une feature.

Exemple :

```text
feature = fantôme dangereux proche
poids = négatif
```

Alors si un fantôme est proche, la note baisse.

Exemple :

```text
feature = nourriture proche
poids = positif
```

Alors si la nourriture est proche, la note monte.
