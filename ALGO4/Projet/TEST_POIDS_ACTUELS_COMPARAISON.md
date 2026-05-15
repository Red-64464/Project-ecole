# Test des poids actuels - comparaison finale

Ce fichier garde une trace courte des tests faits avec les poids actuels.
La version plus professionnelle pour le professeur est :

```text
RAPPORT_POIDS_ET_RESULTATS.md
```

## Poids actuels

```json
{
  "weights": [0.6891, -1.4093, 1.4061, 0.4036, 0.0252, 1.8572, -1.2624],
  "bias": -0.0834
}
```

## Tableau final

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

## Conclusion simple

RLMinimax bat Minimax et AlphaBeta en score moyen sur tous les layouts testes.
La comparaison est reproductible car `compare.py` fixe la graine aleatoire par defaut avec `--seed 0`.
