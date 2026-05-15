## 14. Epsilon : exploration 🧭

`epsilon` dit :

```text
parfois, pendant l'entraînement, Pac-Man teste une action différente
```

Pourquoi ?

```text
Sinon Pac-Man répète toujours les mêmes actions
et ne découvre jamais de nouvelles stratégies.
```

Important :

```text
epsilon sert pendant l'entraînement.
En évaluation finale, on veut epsilon = 0.
```

Phrase simple :

```text
Pendant l'entraînement, Pac-Man ose tester.
Pendant l'examen, Pac-Man joue sérieusement.
```

---

## 15. Pourquoi entraîner par blocs ? 🧱

On n'entraîne pas au hasard pendant 10000 parties directement.

On fait :

```text
entraîner un peu
tester
regarder les résultats
garder ou rejeter les poids
```

Pourquoi ?

```text
Un entraînement peut améliorer une carte mais casser une autre.
```

Exemple :

```text
Pac-Man devient très bon sur capsuleClassic
mais devient moins bon sur smallClassic.
```

Donc on garde les meilleurs poids globaux.

---

## 16. Pourquoi une carte peut en casser une autre ? 🗺️

Chaque layout a une logique différente.

Exemples :

```text
testClassic = petite carte simple
smallClassic = carte plus ouverte
capsuleClassic = capsules importantes
mediumClassic = carte plus grande
originalClassic = plus long et plus dur
```

Si l'entraînement renforce trop une stratégie :

```text
chercher les capsules
fuir les fantômes
chercher vite la nourriture
survivre longtemps
```

Alors cette stratégie peut être bonne sur une carte,
mais moins bonne sur une autre.

Phrase simple :

```text
Un poids est une habitude.
Une bonne habitude sur une carte peut devenir une mauvaise habitude ailleurs.
```

---

## 17. C'est quoi `weights.json` ? 💾

`weights.json` est le fichier qui garde les poids appris.

Exemple :

```json
{
  "weights": [0.6891, -1.4093, 1.4061, 0.4036, 0.0252, 1.8572, -1.2624],
  "bias": -0.0834
}
```

Signification :

```text
weights = les 7 coefficients a1..a7
bias = la constante C
```

Quand on lance :

```bash
python pacman.py -p RLMinimaxAgent ...
```

L'agent charge ces poids.

---
