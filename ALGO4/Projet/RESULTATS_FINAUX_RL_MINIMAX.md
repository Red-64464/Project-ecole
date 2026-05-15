# Resultats finaux RLMinimax

Ce fichier resume les comparaisons finales avec les poids actuels de `weights.json`.

Important : `compare.py` utilise `--seed 0` par defaut.
Chaque agent affronte donc les memes fantomes dans le meme ordre.

## Poids utilises

```json
{
  "weights": [0.6891, -1.4093, 1.4061, 0.4036, 0.0252, 1.8572, -1.2624],
  "bias": -0.0834
}
```

## Tableau principal

| Layout | Depth | Parties | Minimax score | AlphaBeta score | RLMinimax score | Diff vs Minimax | Diff vs AlphaBeta | Win RL | Stop RL |
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

## Lecture simple

- RLMinimax bat Minimax et AlphaBeta en score moyen sur tous les layouts testes.
- Minimax et AlphaBeta donnent les memes scores dans ces tests : c'est attendu, car AlphaBeta optimise Minimax sans changer le resultat.
- RLMinimax n'a pas choisi `Stop` en moyenne dans ces tests.
- Sur `openClassic`, les agents de reference depassent le temps limite utilise pour le test, alors que RLMinimax termine et gagne souvent.
- Le winrate n'est pas toujours eleve, mais le score moyen est meilleur. C'est important car les fantomes gardent une part de hasard.

## Commandes reproductibles

```bash
python compare.py --num-games 200 --layout testClassic --depth 1 --baseline both
python compare.py --num-games 100 --layout smallClassic --depth 2 --baseline both
python compare.py --num-games 100 --layout capsuleClassic --depth 2 --baseline both
python compare.py --num-games 50 --layout mediumClassic --depth 1 --baseline both
python compare.py --num-games 50 --layout originalClassic --depth 1 --baseline both
python compare.py --num-games 50 --layout trickyClassic --depth 1 --baseline both
python compare.py --num-games 50 --layout contestClassic --depth 1 --baseline both
python compare.py --num-games 100 --layout minimaxClassic --depth 3 --baseline both
python compare.py --num-games 1000 --layout trappedClassic --depth 1 --baseline both
python compare.py --num-games 10 --layout openClassic --depth 1 --baseline both --timeout 1 --catch-exceptions
```

## Conclusion courte

Le projet respecte l'objectif principal : `RLMinimaxAgent` combine Alpha-Beta avec une fonction d'evaluation affine dont les poids sont appris par renforcement.
