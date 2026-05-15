## 24. C’est quoi Approximate Q-Learning ?

Dans Q-Learning classique, on stocke toutes les valeurs `Q(s,a)`.

Problème :

```text
Pac-Man a énormément d'états possibles.
Impossible de tout stocker proprement.
```

Approximate Q-Learning utilise des features et des poids.

Idée :

```text
Q(s,a) ≈ poids1*feature1 + poids2*feature2 + ...
```

Ton projet utilise une idée proche : des features et des poids.  
Mais attention : le PDF demande une fonction d'évaluation pour Minimax, pas un Q-agent pur.

## 25. Différence entre agent de recherche et agent d’apprentissage

| Type | Exemple | Il apprend ? | Il regarde le futur ? | Rôle dans le projet |
|---|---|---:|---:|---|
| Recherche | Minimax | non | oui | baseline du Lab 1 |
| Recherche optimisée | Alpha-Beta | non | oui | base de RLMinimax |
| Apprentissage | Q-Learning | oui | indirectement | notion du Lab 2 |
| Projet | RLMinimax | oui pour les poids | oui avec alpha-bêta | agent final |

## 26. Pourquoi un agent peut devoir s’entraîner longtemps ?

Parce qu'au début, les poids ne sont pas bons.

L'agent doit jouer beaucoup de parties pour voir :

- des situations gagnantes
- des situations dangereuses
- des fantômes proches
- des capsules utiles
- des fins de parties

La conversation parle de 500 à 2000 parties.  
Le PDF dit aussi que l'entraînement peut prendre un temps considérable.

## 27. Pourquoi mon projet semblait se terminer vite ?

Diagnostic simple :

- Si tu lances `pacman.py -p RLMinimaxAgent`, tu n'entraînes pas l'agent.
- Tu joues seulement avec les poids déjà sauvegardés dans `weights.json`.
- Si tu lances `train.py --num-games 10`, l'entraînement est volontairement court.
- Si Pac-Man perd vite, une partie d'entraînement se termine vite aussi.

Donc un lancement rapide ne veut pas forcément dire que le code est faux.  
Cela peut juste vouloir dire que tu n'as pas lancé un entraînement long.

## 28. Comment lancer le projet ?

Commande rapide :

```bash
python pacman.py -p RLMinimaxAgent -l testClassic -n 1 -q
```

Commande visuelle :

```bash
python pacman.py -p RLMinimaxAgent -l smallClassic -a depth=2
```

## 29. Comment entraîner l’agent ?

Petit test d'entraînement :

```bash
python train.py --num-games 10 --layout testClassic --depth 1 --eval-games 2
```

Entraînement plus sérieux :

```bash
python train.py --resume --num-games 1000 --layout smallClassic --depth 2 --alpha 0.001 --epsilon 0.05 --eval-games 50
```

Continuer sur une autre carte :

```bash
python train.py --resume --num-games 500 --layout capsuleClassic --depth 2 --alpha 0.001 --epsilon 0.05 --eval-games 30
```

## 30. Comment évaluer l’agent ?

Comparer RLMinimax avec le Lab 1 :

```bash
python compare.py --num-games 20 --layout smallClassic --depth 2
```

Tester directement RLMinimax :

```bash
python pacman.py -p RLMinimaxAgent -l smallClassic -a depth=2 -n 10 -q
```

## 31. Comment savoir si l’agent marche bien ?

Regarde plusieurs choses :

- il ne crashe pas
- il gagne parfois
- son score moyen est correct
- il ne choisit pas `Stop` tout le temps
- il évite les fantômes dangereux
- il mange de la nourriture
- il utilise parfois les capsules
- il reste correct sur plusieurs layouts

Un seul test ne suffit pas.  
Il faut regarder des moyennes.
