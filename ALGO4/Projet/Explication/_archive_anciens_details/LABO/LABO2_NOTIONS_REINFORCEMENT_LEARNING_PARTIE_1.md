# Labo 2 - Reinforcement Learning et poids appris 🎓🤖

Ce fichier explique uniquement les notions du labo 2 qui servent dans notre projet.

Objectif :

```text
Comprendre comment Pac-Man apprend les poids de sa fonction d'évaluation.
```

Dans notre projet, ces notions servent surtout dans :

```text
features.py
rlMinimaxAgents.py
train.py
weights.json
compare.py
```

---

## 1. Idée générale du reinforcement learning 🧠

Reinforcement Learning veut dire :

```text
apprentissage par renforcement
```

Idée simple :

```text
Pac-Man joue.
Il reçoit des récompenses.
Il ajuste sa manière de juger les situations.
Petit à petit, il devient meilleur.
```

Comme un enfant qui apprend :

```text
bonne action -> récompense 🙂
mauvaise action -> punition 😬
```

---

## 2. Dans notre projet, qu'est-ce que Pac-Man apprend ? 🎯

Pac-Man n'apprend pas directement une table géante de toutes les situations.

Il apprend des poids :

```text
a1, a2, a3, a4, a5, a6, a7
```

Ces poids servent dans la formule :

```text
f(s) = a1*x1(s) + a2*x2(s) + ... + a7*x7(s) + C
```

Traduction :

```text
x_i = une information sur le jeu
a_i = l'importance de cette information
C = constante
```

---

## 3. C'est quoi une feature ? 🧩

Une feature est une information numérique sur le jeu.

Exemple :

```text
Est-ce que la nourriture est proche ?
Est-ce qu'un fantôme dangereux est proche ?
Est-ce qu'une capsule est proche ?
Combien reste-t-il de nourriture ?
```

Dans notre projet, les features sont calculées dans :

```text
features.py
```

Les 7 features :

| Feature | Signification simple |
|---|---|
| `x1` | nourriture proche |
| `x2` | fantômes dangereux proches |
| `x3` | fantômes effrayés proches |
| `x4` | nourriture restante |
| `x5` | capsule proche |
| `x6` | score actuel |
| `x7` | fantôme dangereux le plus proche |

Phrase simple :

```text
Les features sont les yeux de Pac-Man.
```

---

## 4. C'est quoi un poids ? ⚖️

Un poids dit si une feature est importante.

Exemple :

```text
fantôme dangereux proche = très important
nourriture proche = important
score actuel = important
```

Si un poids est positif :

```text
Pac-Man aime cette chose ✅
```

Si un poids est négatif :

```text
Pac-Man évite cette chose ❌
```

Exemple :

```text
poids nourriture proche = positif
=> Pac-Man cherche la nourriture

poids fantôme dangereux proche = négatif
=> Pac-Man fuit le danger
```

---
