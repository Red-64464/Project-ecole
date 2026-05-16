"""
compare.py
----------
Compare deux agents en restant dans le dossier du projet.

Important :
    Ce fichier ne va pas chercher dans les dossiers de laboratoires.
    Le projet doit pouvoir etre pousse seul sur Git, donc tout vient d'ici.
"""

from __future__ import annotations

import argparse
import random
import sys
import time
from typing import Callable

import ghostAgents
import layout as layout_module
import pacman
import textDisplay
from game import Directions
from multiAgents import AlphaBetaAgent, MinimaxAgent
from rlMinimaxAgents import RLMinimaxAgent


def run_games(
    lay,
    agent_factory: Callable[[], object],
    num_games: int,
    agent_timeout: int = 30,
    catch_exceptions: bool = False,
    seed: int | None = 0,
):
    """
    Joue plusieurs parties et retourne les resultats.

    Chaque resultat contient :
    - score final
    - victoire ou defaite
    - duree
    - nombre total de coups
    - nombre de Stop choisis par Pac-Man
    """
    results = []
    num_ghosts = lay.getNumGhosts()

    for game_index in range(num_games):
        if seed is not None:
            random.seed(seed + game_index)

        agent = agent_factory()
        ghosts = [ghostAgents.DirectionalGhost(i + 1) for i in range(num_ghosts)]
        display = textDisplay.NullGraphics()
        rules = pacman.ClassicGameRules(timeout=agent_timeout)
        game = rules.newGame(
            lay, agent, ghosts, display, True, catchExceptions=catch_exceptions
        )

        start = time.time()
        game.run()
        duration = time.time() - start

        pacman_actions = [
            action for agent_index, action in game.moveHistory
            if agent_index == 0
        ]
        stop_count = sum(1 for action in pacman_actions if action == Directions.STOP)

        results.append(
            (
                game.state.getScore(),
                game.state.isWin(),
                duration,
                len(game.moveHistory),
                stop_count,
            )
        )

    return results


def summarize(results):
    """Calcule les statistiques simples d'une liste de parties."""
    scores = [r[0] for r in results]
    wins = [r[1] for r in results]
    durations = [r[2] for r in results]
    moves = [r[3] for r in results]
    stops = [r[4] for r in results]

    return {
        "games": len(results),
        "avg_score": sum(scores) / len(scores),
        "best_score": max(scores),
        "worst_score": min(scores),
        "wins": sum(wins),
        "win_rate": sum(wins) / len(wins),
        "avg_time": sum(durations) / len(durations),
        "avg_moves": sum(moves) / len(moves),
        "avg_stop": sum(stops) / len(stops),
    }


def print_stats(name: str, results):
    """Affiche les statistiques d'un agent."""
    stats = summarize(results)
    print(f"\n{'=' * 50}")
    print(f"  {name}")
    print(f"{'=' * 50}")
    print(f"  Parties jouees :       {stats['games']}")
    print(f"  Score moyen :          {stats['avg_score']:.1f}")
    print(f"  Meilleur score :       {stats['best_score']:.1f}")
    print(f"  Pire score :           {stats['worst_score']:.1f}")
    print(
        f"  Taux de victoire :     {stats['wins']}/{stats['games']} "
        f"({stats['win_rate']:.0%})"
    )
    print(f"  Temps moyen/partie :   {stats['avg_time']:.2f}s")
    print(f"  Coups moyens/partie :  {stats['avg_moves']:.1f}")
    print(f"  Stop moyens/partie :   {stats['avg_stop']:.1f}")
    print(f"{'=' * 50}")
    return stats


def main():
    """Point d'entree de la comparaison en ligne de commande.

    Cette fonction lit les options, lance les agents dans les memes conditions,
    puis affiche les statistiques utiles pour le rapport.
    """
    parser = argparse.ArgumentParser(
        description="Comparaison locale Minimax/AlphaBeta vs RLMinimaxAgent"
    )
    parser.add_argument("--num-games", type=int, default=20, help="Nombre de parties par agent")
    parser.add_argument("--layout", type=str, default="smallClassic", help="Layout")
    parser.add_argument("--depth", type=int, default=2, help="Profondeur minimax")
    parser.add_argument("--timeout", type=int, default=30, help="Temps max par agent")
    parser.add_argument(
        "--seed",
        type=int,
        default=0,
        help="Graine aleatoire pour rejouer les memes fantomes avec chaque agent",
    )
    parser.add_argument(
        "--catch-exceptions",
        action="store_true",
        help="Continue meme si un agent depasse le temps ou plante",
    )
    parser.add_argument(
        "--baseline",
        choices=("alphabeta", "minimax", "both"),
        default="alphabeta",
        help=(
            "Agent de reference : alphabeta pour la baseline la plus forte, "
            "minimax pour coller a la grille, both pour afficher les deux"
        ),
    )
    args = parser.parse_args()

    if args.num_games <= 0:
        print("Erreur : --num-games doit etre positif.")
        sys.exit(1)

    lay = layout_module.getLayout(args.layout)
    if lay is None:
        print(f"Erreur : layout '{args.layout}' introuvable.")
        sys.exit(1)

    print(
        f"Comparaison locale sur '{args.layout}' avec {args.num_games} parties "
        f"par agent et profondeur={args.depth}"
    )

    baseline_specs = []
    if args.baseline in ("minimax", "both"):
        baseline_specs.append(("MinimaxAgent local", MinimaxAgent))
    if args.baseline in ("alphabeta", "both"):
        baseline_specs.append(("AlphaBetaAgent local", AlphaBetaAgent))

    baseline_stats = []
    for name, agent_class in baseline_specs:
        print(f"\nLancement de {name}...")
        results = run_games(
            lay,
            lambda cls=agent_class: cls(depth=str(args.depth)),
            args.num_games,
            agent_timeout=args.timeout,
            catch_exceptions=args.catch_exceptions,
            seed=args.seed,
        )
        baseline_stats.append((name, print_stats(name, results)))

    print("\nLancement de RLMinimaxAgent...")
    rl_results = run_games(
        lay,
        lambda: RLMinimaxAgent(depth=str(args.depth)),
        args.num_games,
        agent_timeout=args.timeout,
        catch_exceptions=args.catch_exceptions,
        seed=args.seed,
    )
    rl_stats = print_stats("RLMinimaxAgent poids appris", rl_results)

    print(f"\n{'=' * 55}")
    print("  Resume de la comparaison")
    print(f"{'=' * 55}")
    print(
        f"  {'Reference':<22} {'Score ref':>12} {'Score RL':>12} "
        f"{'Diff RL':>10} {'Win ref':>9} {'Win RL':>9} {'Stop ref':>10} {'Stop RL':>9}"
    )
    print(f"  {'-' * 100}")
    for name, stats in baseline_stats:
        diff = rl_stats["avg_score"] - stats["avg_score"]
        print(
            f"  {name:<22} "
            f"{stats['avg_score']:>12.1f} {rl_stats['avg_score']:>12.1f} "
            f"{diff:>+10.1f} {stats['win_rate']:>9.0%} {rl_stats['win_rate']:>9.0%} "
            f"{stats['avg_stop']:>10.1f} {rl_stats['avg_stop']:>9.1f}"
        )
    print("  Tout est calcule uniquement avec les fichiers du dossier Projet.")
    print(f"{'=' * 55}")


if __name__ == "__main__":
    main()
