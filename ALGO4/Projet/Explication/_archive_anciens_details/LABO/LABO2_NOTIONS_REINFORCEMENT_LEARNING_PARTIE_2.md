## 5. Exemple concret de formule affine 🧮

Imaginons seulement 3 features :

```text
x1 = nourriture proche = 0.8
x2 = fantôme dangereux proche = 1.0
x3 = score normalisé = 0.5
```

Et les poids :

```text
a1 = 2
a2 = -5
a3 = 1
C = 0
```

Calcul :

```text
f(s) = 2*0.8 + (-5)*1.0 + 1*0.5 + 0
f(s) = 1.6 - 5 + 0.5
f(s) = -2.9
```

La note est mauvaise 😬

Pourquoi ?

```text
Parce qu'un fantôme dangereux est proche.
```

---

## 6. C'est quoi une récompense ? 🎁

Une récompense dit si ce qui vient de se passer est bon ou mauvais.

Dans notre projet, on utilise :

```text
reward = score_après - score_avant
```

Exemples :

```text
Pac-Man mange une gomme -> score augmente -> récompense positive 🙂
Pac-Man perd du temps -> score baisse -> récompense négative 😐
Pac-Man meurt -> très mauvais 😬
Pac-Man gagne -> très bon 🎉
```

---

## 7. C'est quoi un état et une transition ? 🔁

Un état :

```text
s = situation actuelle du jeu
```

Une transition :

```text
s -> s'
```

Ça veut dire :

```text
on passe de l'état actuel au prochain état
```

Image :

```text
Etat s
  |
  | Pac-Man joue une action
  v
Etat s'
```

---

## 8. La valeur V(s) ⭐

Dans notre projet, la valeur d'un état est :

```text
V(s) = f(s)
```

Donc :

```text
V(s) = C + a1*x1(s) + a2*x2(s) + ... + a7*x7(s)
```

Phrase simple :

```text
V(s) est la note que Pac-Man donne à une situation.
```

---

## 9. TD-Learning expliqué simplement ⏳

TD signifie :

```text
Temporal Difference
```

Idée :

```text
Pac-Man compare ce qu'il pensait avec ce qui s'est vraiment passé.
```

Si Pac-Man pensait :

```text
cette situation vaut 10
```

Mais après un coup, il découvre :

```text
en fait, elle valait plutôt 15
```

Alors il corrige ses poids.

---
