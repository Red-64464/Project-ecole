"""Graphiques de comparaison RLMinimax vs Minimax vs AlphaBeta.

Genere des graphiques visuels qui montrent les resultats sur les 10 layouts :
  1. Score moyen par layout (3 agents compares)
  2. Taux de victoire de RLMinimax par layout
  3. Gain de RLMinimax par rapport a Minimax

Les valeurs proviennent de RESULTATS_FINAUX_RL_MINIMAX.md (resultats deja
reproduits avec compare.py --seed 0).

Usage :
    python3 plot_comparison.py
"""

import os
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np

OUTPUT_DIR = "output"

# ── Donnees issues de RESULTATS_FINAUX_RL_MINIMAX.md ────────────────────────
# (layout, minimax, alphabeta, rlminimax, win_rl_percent, depth, n_parties)
DATA = [
    ("testClassic",     548.5,  548.5,   552.5,  100, 1,  200),
    ("smallClassic",   -149.8, -149.8,   196.6,   34, 2,  100),
    ("capsuleClassic", -228.9, -228.9,  -135.2,   12, 2,  100),
    ("mediumClassic",    14.5,   14.5,   515.5,   28, 1,   50),
    ("originalClassic", 348.6,  348.6,   664.4,    2, 1,   50),
    ("trickyClassic",   101.3,  101.3,   293.0,    2, 1,   50),
    ("contestClassic",   11.3,   11.3,   244.4,   28, 1,   50),
    ("minimaxClassic", -243.5, -243.5,  -153.1,   34, 3,  100),
    ("trappedClassic", -392.3, -392.3,  -391.4,   11, 1, 1000),
    ("openClassic",    -419.5, -745.4,  1057.0,   90, 1,   10),
]

COLOR_MINIMAX  = "#90A4AE"   # gris-bleu
COLOR_ALPHABETA = "#42A5F5"  # bleu
COLOR_RL       = "#43A047"   # vert
COLOR_GAIN     = "#FB8C00"   # orange


def fig_scores_comparison():
    """Score moyen par layout : 3 barres groupees par carte."""
    layouts = [d[0] for d in DATA]
    mm   = [d[1] for d in DATA]
    ab   = [d[2] for d in DATA]
    rl   = [d[3] for d in DATA]

    x = np.arange(len(layouts))
    width = 0.27

    fig, ax = plt.subplots(figsize=(13, 6.5))
    b1 = ax.bar(x - width, mm, width, label="Minimax",   color=COLOR_MINIMAX,  edgecolor="white", linewidth=0.7)
    b2 = ax.bar(x,         ab, width, label="AlphaBeta", color=COLOR_ALPHABETA, edgecolor="white", linewidth=0.7)
    b3 = ax.bar(x + width, rl, width, label="RLMinimax", color=COLOR_RL,       edgecolor="white", linewidth=0.7)

    ax.axhline(y=0, color="black", linewidth=0.6)
    ax.set_xticks(x)
    ax.set_xticklabels(layouts, rotation=30, ha="right", fontsize=10)
    ax.set_ylabel("Score moyen", fontsize=11)
    ax.set_title("Score moyen par layout - RLMinimax vs Minimax vs AlphaBeta",
                 fontsize=13, fontweight="bold", pad=14)
    ax.legend(loc="upper left", fontsize=10, framealpha=0.95)
    ax.grid(True, axis="y", alpha=0.3, linestyle="--")
    ax.set_axisbelow(True)

    # Annoter la barre RLMinimax au-dessus
    for bar, val in zip(b3, rl):
        h = bar.get_height()
        ax.annotate(f"{val:.0f}",
                    xy=(bar.get_x() + bar.get_width()/2, h),
                    xytext=(0, 4 if h >= 0 else -12),
                    textcoords="offset points",
                    ha="center", fontsize=8, color=COLOR_RL, fontweight="bold")

    plt.tight_layout()
    out = os.path.join(OUTPUT_DIR, "comparaison_scores.png")
    plt.savefig(out, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"  -> {out}")


def fig_gains_vs_minimax():
    """Gain absolu de RLMinimax par rapport a Minimax (toujours positif)."""
    layouts = [d[0] for d in DATA]
    gains   = [d[3] - d[1] for d in DATA]

    # Tri decroissant pour mieux voir les ecarts
    order   = sorted(range(len(DATA)), key=lambda i: gains[i], reverse=True)
    layouts = [layouts[i] for i in order]
    gains   = [gains[i]   for i in order]

    fig, ax = plt.subplots(figsize=(12, 5.5))
    bars = ax.barh(layouts, gains, color=COLOR_GAIN, edgecolor="white", linewidth=0.7)
    ax.invert_yaxis()
    ax.axvline(x=0, color="black", linewidth=0.6)
    ax.set_xlabel("Gain de score moyen (RLMinimax - Minimax)", fontsize=11)
    ax.set_title("Avantage de RLMinimax par layout (toujours positif)",
                 fontsize=13, fontweight="bold", pad=14)
    ax.grid(True, axis="x", alpha=0.3, linestyle="--")
    ax.set_axisbelow(True)

    for bar, val in zip(bars, gains):
        w = bar.get_width()
        ax.annotate(f"+{val:.1f}",
                    xy=(w, bar.get_y() + bar.get_height()/2),
                    xytext=(6, 0),
                    textcoords="offset points",
                    ha="left", va="center", fontsize=9, fontweight="bold", color=COLOR_GAIN)

    plt.tight_layout()
    out = os.path.join(OUTPUT_DIR, "comparaison_gains.png")
    plt.savefig(out, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"  -> {out}")


def fig_winrate():
    """Taux de victoire de RLMinimax par layout."""
    layouts = [d[0] for d in DATA]
    wins    = [d[4] for d in DATA]

    # Colorer selon le winrate : rouge -> orange -> jaune -> vert
    def color_for(w):
        if w >= 80: return "#2E7D32"   # vert fonce
        if w >= 50: return "#7CB342"   # vert clair
        if w >= 30: return "#FFA726"   # orange
        if w >= 10: return "#FB8C00"   # orange fonce
        return "#E53935"               # rouge
    bar_colors = [color_for(w) for w in wins]

    fig, ax = plt.subplots(figsize=(12, 5.5))
    bars = ax.bar(layouts, wins, color=bar_colors, edgecolor="white", linewidth=0.7)
    ax.axhline(y=50, color="gray", linestyle="--", linewidth=0.8, label="50%")
    ax.set_ylabel("Taux de victoire (%)", fontsize=11)
    ax.set_title("Taux de victoire de RLMinimax par layout",
                 fontsize=13, fontweight="bold", pad=14)
    ax.set_xticks(range(len(layouts)))
    ax.set_xticklabels(layouts, rotation=30, ha="right", fontsize=10)
    ax.set_ylim(0, 110)
    ax.legend(loc="upper right", fontsize=9)
    ax.grid(True, axis="y", alpha=0.3, linestyle="--")
    ax.set_axisbelow(True)

    for bar, val in zip(bars, wins):
        ax.annotate(f"{val}%",
                    xy=(bar.get_x() + bar.get_width()/2, val),
                    xytext=(0, 4),
                    textcoords="offset points",
                    ha="center", fontsize=9, fontweight="bold", color="black")

    plt.tight_layout()
    out = os.path.join(OUTPUT_DIR, "comparaison_winrate.png")
    plt.savefig(out, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"  -> {out}")


def fig_overview():
    """Figure de synthese : 3 sous-graphiques en une image."""
    layouts = [d[0] for d in DATA]
    mm   = [d[1] for d in DATA]
    ab   = [d[2] for d in DATA]
    rl   = [d[3] for d in DATA]
    wins = [d[4] for d in DATA]
    gains = [r - m for r, m in zip(rl, mm)]

    fig, axes = plt.subplots(3, 1, figsize=(13, 14))
    fig.suptitle("RLMinimax vs Minimax vs AlphaBeta - Synthese sur 10 layouts",
                 fontsize=15, fontweight="bold", y=0.995)

    # Subplot 1 : scores
    x = np.arange(len(layouts))
    width = 0.27
    axes[0].bar(x - width, mm, width, label="Minimax",   color=COLOR_MINIMAX,   edgecolor="white", linewidth=0.7)
    axes[0].bar(x,         ab, width, label="AlphaBeta", color=COLOR_ALPHABETA, edgecolor="white", linewidth=0.7)
    bars = axes[0].bar(x + width, rl, width, label="RLMinimax", color=COLOR_RL, edgecolor="white", linewidth=0.7)
    for bar, val in zip(bars, rl):
        h = bar.get_height()
        axes[0].annotate(f"{val:.0f}", xy=(bar.get_x() + bar.get_width()/2, h),
                         xytext=(0, 4 if h >= 0 else -12), textcoords="offset points",
                         ha="center", fontsize=7.5, color=COLOR_RL, fontweight="bold")
    axes[0].axhline(y=0, color="black", linewidth=0.6)
    axes[0].set_xticks(x); axes[0].set_xticklabels(layouts, rotation=25, ha="right", fontsize=9)
    axes[0].set_ylabel("Score moyen", fontsize=10)
    axes[0].set_title("1. Score moyen par layout", fontsize=11, fontweight="bold", pad=10)
    axes[0].legend(loc="upper left", fontsize=9)
    axes[0].grid(True, axis="y", alpha=0.3, linestyle="--"); axes[0].set_axisbelow(True)

    # Subplot 2 : winrate
    def color_for(w):
        if w >= 80: return "#2E7D32"
        if w >= 50: return "#7CB342"
        if w >= 30: return "#FFA726"
        if w >= 10: return "#FB8C00"
        return "#E53935"
    bar_colors = [color_for(w) for w in wins]
    bars2 = axes[1].bar(layouts, wins, color=bar_colors, edgecolor="white", linewidth=0.7)
    for bar, val in zip(bars2, wins):
        axes[1].annotate(f"{val}%", xy=(bar.get_x() + bar.get_width()/2, val),
                         xytext=(0, 3), textcoords="offset points",
                         ha="center", fontsize=9, fontweight="bold")
    axes[1].axhline(y=50, color="gray", linestyle="--", linewidth=0.8)
    axes[1].set_xticklabels(layouts, rotation=25, ha="right", fontsize=9)
    axes[1].set_ylabel("Winrate (%)", fontsize=10)
    axes[1].set_ylim(0, 110)
    axes[1].set_title("2. Taux de victoire de RLMinimax", fontsize=11, fontweight="bold", pad=10)
    axes[1].grid(True, axis="y", alpha=0.3, linestyle="--"); axes[1].set_axisbelow(True)

    # Subplot 3 : gains (tri decroissant)
    order   = sorted(range(len(DATA)), key=lambda i: gains[i], reverse=True)
    layouts_s = [layouts[i] for i in order]
    gains_s   = [gains[i]   for i in order]
    bars3 = axes[2].barh(layouts_s, gains_s, color=COLOR_GAIN, edgecolor="white", linewidth=0.7)
    axes[2].invert_yaxis()
    axes[2].axvline(x=0, color="black", linewidth=0.6)
    for bar, val in zip(bars3, gains_s):
        axes[2].annotate(f"+{val:.1f}", xy=(bar.get_width(), bar.get_y() + bar.get_height()/2),
                         xytext=(6, 0), textcoords="offset points",
                         ha="left", va="center", fontsize=9, fontweight="bold", color=COLOR_GAIN)
    axes[2].set_xlabel("Gain de score moyen RLMinimax - Minimax", fontsize=10)
    axes[2].set_title("3. Avantage de RLMinimax par layout (toujours > 0)",
                      fontsize=11, fontweight="bold", pad=10)
    axes[2].grid(True, axis="x", alpha=0.3, linestyle="--"); axes[2].set_axisbelow(True)

    plt.tight_layout(rect=[0, 0, 1, 0.985])
    out = os.path.join(OUTPUT_DIR, "synthese_comparaison.png")
    plt.savefig(out, dpi=150, bbox_inches="tight")
    plt.close(fig)
    print(f"  -> {out}")


def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    print("Generation des graphiques de comparaison...")
    fig_scores_comparison()
    fig_winrate()
    fig_gains_vs_minimax()
    fig_overview()
    print("\nTous les graphiques sont dans le dossier output/.")
    print("Le graphique principal est : output/synthese_comparaison.png")


if __name__ == "__main__":
    main()
