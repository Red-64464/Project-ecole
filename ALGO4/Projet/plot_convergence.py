"""Graphique de convergence des poids RLMinimax.

Ce script lance un entrainement rapide depuis zero pour montrer
comment les poids et le score evoluent au fil des parties.
Les poids originaux (weights.json) ne sont PAS touches.

Usage :
    python3 plot_convergence.py
"""

import os
import random
import matplotlib
matplotlib.use("Agg")  # mode sans ecran
import matplotlib.pyplot as plt

import ghostAgents
import layout as layout_module
import pacman
import textDisplay

from features import FEATURE_NAMES, NUM_FEATURES, extract_features
from train import (
    TrainingRLMinimaxAgent,
    _td_update,
    _build_initial_weights,
    _weighted_value,
)

# ─── Parametres ────────────────────────────────────────────────────────────────
NUM_GAMES   = 150      # parties d'entrainement (court pour la demo)
DEPTH       = 2
LAYOUT      = "smallClassic"
ALPHA       = 0.001
GAMMA       = 0.9
ALPHA_DECAY = 0.99995
ALPHA_FLOOR = 0.0001
EPSILON     = 0.05
SEED        = 42
WINDOW      = 10       # taille de la fenetre de lissage
OUTPUT_FILE = "output/convergence_poids.png"
# ───────────────────────────────────────────────────────────────────────────────


def run_game(lay, weights, bias):
    """Joue une partie et renvoie (game, agent)."""
    agent   = TrainingRLMinimaxAgent(weights[:], bias, depth=DEPTH, epsilon=EPSILON)
    ghosts  = [ghostAgents.DirectionalGhost(i + 1) for i in range(lay.getNumGhosts())]
    display = textDisplay.NullGraphics()
    rules   = pacman.ClassicGameRules(timeout=30)
    game    = rules.newGame(lay, agent, ghosts, display, quiet=True, catchExceptions=False)
    game.run()
    return game, agent


def smooth(data, w):
    """Moyenne glissante sur une fenetre de taille w."""
    out = []
    for k in range(len(data)):
        start = max(0, k - w + 1)
        out.append(sum(data[start:k+1]) / (k - start + 1))
    return out


def main():
    os.makedirs("output", exist_ok=True)
    random.seed(SEED)

    lay     = layout_module.getLayout(LAYOUT)
    weights = _build_initial_weights()
    bias    = 0.0
    alpha   = ALPHA

    scores_hist  = []
    wins_hist    = []
    weights_hist = []

    print(f"Entrainement de convergence : {NUM_GAMES} parties sur {LAYOUT}")
    print(f"Poids initiaux : {[round(w, 4) for w in weights]}")

    for i in range(NUM_GAMES):
        game, agent = run_game(lay, weights, bias)

        score = game.state.getScore()
        won   = game.state.isWin()
        scores_hist.append(score)
        wins_hist.append(int(won))

        # Mise a jour TD avec l'etat final reel
        weights, bias = _td_update(
            weights, bias,
            agent.state_history,
            game.state,
            won, alpha, GAMMA
        )
        alpha = max(alpha * ALPHA_DECAY, ALPHA_FLOOR)
        weights_hist.append(weights[:])

        if (i + 1) % WINDOW == 0:
            avg_s = sum(scores_hist[-WINDOW:]) / WINDOW
            avg_w = sum(wins_hist[-WINDOW:]) / WINDOW
            print(f"  Partie {i+1:4d}/{NUM_GAMES} | score({WINDOW})={avg_s:7.1f} | winrate={avg_w:.0%}")

    # ── Courbes lissees ──────────────────────────────────────────────────────
    x          = list(range(1, NUM_GAMES + 1))
    avg_scores = smooth(scores_hist, WINDOW)
    avg_wins   = [v * 100 for v in smooth(wins_hist, WINDOW)]

    # 4 poids avec la plus grande amplitude de variation
    amplitudes = [max(w[i] for w in weights_hist) - min(w[i] for w in weights_hist)
                  for i in range(NUM_FEATURES)]
    top4 = sorted(range(NUM_FEATURES), key=lambda i: amplitudes[i], reverse=True)[:4]

    # ── Figure ──────────────────────────────────────────────────────────────
    fig, axes = plt.subplots(3, 1, figsize=(10, 12))
    fig.suptitle(
        f"Convergence de l'apprentissage — RLMinimax ({NUM_GAMES} parties, {LAYOUT})",
        fontsize=13, fontweight="bold"
    )

    # Graphe 1 : score moyen
    axes[0].plot(x, scores_hist, color="#BBDEFB", linewidth=0.6, alpha=0.5, label="Score brut")
    axes[0].plot(x, avg_scores,  color="#1565C0", linewidth=2.0,  label=f"Moyenne ({WINDOW} parties)")
    axes[0].axhline(y=0, color="gray", linestyle="--", linewidth=0.8)
    axes[0].set_ylabel("Score")
    axes[0].set_title("Score au cours de l'entrainement")
    axes[0].legend(fontsize=9)
    axes[0].grid(True, alpha=0.3)

    # Graphe 2 : winrate
    axes[1].fill_between(x, avg_wins, alpha=0.25, color="#388E3C")
    axes[1].plot(x, avg_wins, color="#2E7D32", linewidth=2.0, label=f"Winrate % (fenetre {WINDOW})")
    axes[1].axhline(y=50, color="gray", linestyle="--", linewidth=0.8, label="50%")
    axes[1].set_ylabel("Taux de victoire (%)")
    axes[1].set_title("Taux de victoire au cours de l'entrainement")
    axes[1].set_ylim(0, 105)
    axes[1].legend(fontsize=9)
    axes[1].grid(True, alpha=0.3)

    # Graphe 3 : evolution des 4 poids
    colors = ["#E91E63", "#FF6F00", "#6A1B9A", "#00838F"]
    for feat_idx, color in zip(top4, colors):
        vals  = [w[feat_idx] for w in weights_hist]
        label = FEATURE_NAMES[feat_idx] if feat_idx < len(FEATURE_NAMES) else f"w{feat_idx}"
        axes[2].plot(x, vals, color=color, linewidth=1.4, label=label)
    axes[2].axhline(y=0, color="gray", linestyle="--", linewidth=0.8)
    axes[2].set_ylabel("Valeur du poids")
    axes[2].set_xlabel("Numero de partie")
    axes[2].set_title("Evolution des 4 poids avec la plus grande variation")
    axes[2].legend(fontsize=8)
    axes[2].grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig(OUTPUT_FILE, dpi=150, bbox_inches="tight")
    print(f"\nGraphique sauvegarde dans : {OUTPUT_FILE}")

    # ── Resume ───────────────────────────────────────────────────────────────
    last10_score = sum(scores_hist[-10:]) / 10
    last10_wr    = sum(wins_hist[-10:]) / 10
    print(f"Score moyen (10 dernieres) : {last10_score:.1f}")
    print(f"Winrate     (10 dernieres) : {last10_wr:.0%}")
    print(f"Poids finaux : {[round(w, 4) for w in weights_hist[-1]]}")
    print("\nweights.json n'a pas ete modifie.")


if __name__ == "__main__":
    main()
