"""Entrainement des poids de RLMinimax.

Le projet n'apprend pas une politique directe. Il apprend les coefficients de
la fonction affine f(s)=somme(ai*xi(s))+C, puis RLMinimax utilise ces poids.
"""

import argparse
import random
import sys

import ghostAgents
import layout as layout_module
import pacman
import textDisplay

from features import FEATURE_NAMES, NUM_FEATURES, extract_features
from rlMinimaxAgents import RLMinimaxAgent, load_weights, save_weights

class TrainingRLMinimaxAgent(RLMinimaxAgent):
    """Version d'entrainement de RLMinimax.

    Cette classe garde l'historique des etats visites pendant une partie. Cet
    historique permet ensuite de modifier les poids selon les recompenses
    obtenues, sans apprendre une strategie directe.
    """

    def __init__(self, weights, bias, depth=2, epsilon=0.0):
        """Reçoit les poids courants et les paramètres d'exploration."""
        self.index = 0
        self.depth = int(depth)
        self.weights = weights
        self.bias = bias
        self.epsilon = float(epsilon)
        self.state_history = []

    def reset_state_history(self):
        """Vide l'historique avant une nouvelle partie d'entrainement."""
        self.state_history = []

    def get_action(self, game_state):
        """Choisit une action et memorise l'etat courant.

        Pendant l'entrainement, l'agent peut parfois explorer une action au
        hasard avec epsilon. Cela aide a tester des situations differentes.
        """
        self.state_history.append((extract_features(game_state), game_state.getScore()))

        actions = game_state.getLegalActions(0)
        if self.epsilon > 0 and actions and random.random() < self.epsilon:
            moving = [action for action in actions if action != "Stop"]
            return random.choice(moving or actions)

        return super().get_action(game_state)

def run_single_game(lay, agent, ghosts, quiet=True):
    """Lance une partie Pac-Man sans interface graphique.

    Elle recoit un layout, un agent Pac-Man et une liste de fantomes. Elle
    renvoie l'objet `game`, qui contient le score final et l'historique.
    """
    display = textDisplay.NullGraphics()
    rules = pacman.ClassicGameRules(timeout=30)
    game = rules.newGame(lay, agent, ghosts, display, quiet, catchExceptions=False)
    game.run()
    return game

def _build_initial_weights():
    """Cree des poids de depart coherents avec le sens des features.

    Les valeurs restent aleatoires, mais leur signe de depart est logique :
    positif pour les informations utiles et negatif pour les dangers.
    """
    return [
        random.uniform(0.5, 2.0),
        random.uniform(-2.0, -0.5),
        random.uniform(0.5, 2.0),
        random.uniform(0.1, 0.5),
        random.uniform(0.0, 1.0),
        random.uniform(1.0, 4.0),
        random.uniform(-2.0, -0.5),
    ]

def _weighted_value(weights, bias, features):
    """Calcule la valeur affine d'une liste de features."""
    return bias + sum(weight * feature for weight, feature in zip(weights, features))

def _save_weights_or_exit(weights, bias):
    """Sauvegarde les poids ou arrete proprement le programme en cas d'erreur."""
    try:
        save_weights(weights, bias)
    except (RuntimeError, ValueError, TypeError) as exc:
        print(f"Erreur : {exc}", file=sys.stderr)
        sys.exit(1)

def _load_layout_or_exit(layout_name):
    """Charge un layout Pac-Man ou affiche une erreur claire s'il manque."""
    lay = layout_module.getLayout(layout_name)
    if lay is None:
        print(f"Erreur : layout '{layout_name}' introuvable.", file=sys.stderr)
        sys.exit(1)
    return lay

def _validate_training_args(num_games, alpha, gamma, alpha_decay, alpha_floor, eval_games, epsilon):
    """Verifie les hyperparametres avant de lancer l'entrainement."""
    checks = [
        (num_games > 0, "le nombre de parties doit etre positif"),
        (alpha > 0, "alpha doit etre positif"),
        (0 <= gamma <= 1, "gamma doit etre dans [0, 1]"),
        (0 < alpha_decay <= 1, "alpha_decay doit etre dans ]0, 1]"),
        (alpha_floor >= 0, "alpha_floor ne peut pas etre negatif"),
        (eval_games >= 0, "eval_games ne peut pas etre negatif"),
        (0 <= epsilon <= 1, "epsilon doit etre dans [0, 1]"),
    ]
    for ok, message in checks:
        if not ok:
            print(f"Erreur : {message}.", file=sys.stderr)
            sys.exit(1)

def evaluate_weights(weights, bias, num_games=20, depth=2, layout_name="smallClassic"):
    """Teste des poids sans les modifier.

    Cette fonction joue plusieurs parties avec les poids fournis et renvoie le
    score moyen ainsi que le taux de victoire. Elle sert a controler la qualite
    des poids apres l'entrainement.
    """
    lay = _load_layout_or_exit(layout_name)
    scores = []
    wins = []

    for _ in range(num_games):
        agent = TrainingRLMinimaxAgent(weights[:], bias, depth=depth)
        ghosts = [ghostAgents.DirectionalGhost(i + 1) for i in range(lay.getNumGhosts())]
        game = run_single_game(lay, agent, ghosts, quiet=True)
        scores.append(game.state.getScore())
        wins.append(game.state.isWin())

    return sum(scores) / len(scores), sum(wins) / len(wins)

def _td_update(weights, bias, history, final_state, won, alpha, gamma):
    """Met a jour les poids avec une logique de difference temporelle.

    Pour chaque transition, on compare la valeur actuelle avec la recompense
    observee et la valeur estimee de l'etat suivant. L'erreur sert ensuite a
    ajuster les poids de la fonction d'evaluation affine.
    """
    final_snapshot = (extract_features(final_state), final_state.getScore())
    if not history or history[-1] != final_snapshot:
        history.append(final_snapshot)
    if len(history) < 2:
        return weights, bias

    weight_grad = [0.0] * NUM_FEATURES
    bias_grad = 0.0
    transition_count = 0

    for (features_t, score_t), (features_next, score_next) in zip(history, history[1:]):
        reward = score_next - score_t
        current = _weighted_value(weights, bias, features_t)
        future = _weighted_value(weights, bias, features_next)
        error = reward + gamma * future - current
        for i in range(NUM_FEATURES):
            weight_grad[i] += error * features_t[i]
        bias_grad += error
        transition_count += 1

    terminal_features, _ = history[-1]
    terminal_error = (500.0 if won else -500.0) - _weighted_value(weights, bias, terminal_features)
    for i in range(NUM_FEATURES):
        weight_grad[i] += terminal_error * terminal_features[i]
    bias_grad += terminal_error
    transition_count += 1

    clip = 5.0
    for i in range(NUM_FEATURES):
        grad = weight_grad[i] / transition_count
        weights[i] += alpha * max(-clip, min(clip, grad))
    bias += alpha * max(-clip, min(clip, bias_grad / transition_count))
    return weights, bias

def _is_better(avg, wr, best_avg, best_wr):
    """Dit si une fenetre d'entrainement est meilleure que la meilleure connue."""
    return best_wr is None or wr > best_wr or (wr == best_wr and avg > best_avg)

def train(num_games=200, depth=2, layout_name="smallClassic", alpha=0.001,
          gamma=0.9, alpha_decay=0.99995, alpha_floor=0.0001,
          early_stop_windows=10, eval_games=20, seed=None, resume=False,
          epsilon=0.05):
    """Entraine les poids de la fonction d'evaluation.

    L'agent joue des parties, observe les recompenses, ajuste les poids, puis
    sauvegarde les meilleurs poids dans `weights.json`. C'est la partie
    reinforcement learning du projet.
    """
    _validate_training_args(num_games, alpha, gamma, alpha_decay, alpha_floor, eval_games, epsilon)
    if seed is not None:
        random.seed(seed)

    lay = _load_layout_or_exit(layout_name)
    weights, bias = load_weights() if resume else (_build_initial_weights(), 0.0)
    best_avg = best_wr = best_weights = best_bias = None
    stale_windows = windows_without_best = 0
    scores, wins = [], []

    print(f"Entrainement layout={layout_name}, depth={depth}, features={FEATURE_NAMES}")
    print(f"Poids initiaux : {[round(w, 4) for w in weights]}, biais={bias:.4f}")
    print(f"alpha={alpha}, gamma={gamma}, epsilon={epsilon}, parties={num_games}")

    for game_idx in range(num_games):
        agent = TrainingRLMinimaxAgent(weights[:], bias, depth=depth, epsilon=epsilon)
        ghosts = [ghostAgents.DirectionalGhost(i + 1) for i in range(lay.getNumGhosts())]
        game = run_single_game(lay, agent, ghosts, quiet=True)

        final_score = game.state.getScore()
        won = game.state.isWin()
        scores.append(final_score)
        wins.append(won)

        weights, bias = _td_update(
            weights, bias, agent.state_history, game.state, won, alpha, gamma
        )
        alpha = max(alpha * alpha_decay, alpha_floor)

        if (game_idx + 1) % 10 != 0:
            continue

        avg = sum(scores[-10:]) / len(scores[-10:])
        wr = sum(wins[-10:]) / len(wins[-10:])
        if _is_better(avg, wr, best_avg, best_wr):
            best_avg, best_wr = avg, wr
            best_weights, best_bias = weights[:], bias
            stale_windows = windows_without_best = 0
            _save_weights_or_exit(best_weights, best_bias)
        else:
            stale_windows += 1
            windows_without_best += 1

        restored = ""
        if stale_windows >= 3 and best_weights is not None:
            weights, bias = best_weights[:], best_bias
            alpha = max(alpha * 0.5, alpha_floor)
            stale_windows = 0
            restored = " | retour meilleurs poids"

        print(
            f"Partie {game_idx + 1:4d}/{num_games} | "
            f"score(10)={avg:8.1f} | winrate(10)={wr:.0%} | alpha={alpha:.6f}"
            f"{restored}"
        )
        if early_stop_windows > 0 and windows_without_best >= early_stop_windows:
            print(f"Arret anticipe apres {early_stop_windows * 10} parties sans progres.")
            break

    final_weights = best_weights if best_weights is not None else weights
    final_bias = best_bias if best_weights is not None else bias
    _save_weights_or_exit(final_weights, final_bias)

    print("-" * 60)
    print(f"Poids finaux : {[round(w, 4) for w in final_weights]}")
    print(f"Biais final : {final_bias:.4f}")
    print(f"Score moyen global : {sum(scores) / len(scores):.1f}")
    print(f"Taux de victoire global : {sum(wins) / len(wins):.0%}")
    if eval_games > 0:
        eval_avg, eval_wr = evaluate_weights(final_weights, final_bias, eval_games, depth, layout_name)
        print(f"Evaluation finale : score={eval_avg:.1f}, winrate={eval_wr:.0%}")
    print("Poids sauvegardes dans weights.json")
    return final_weights, final_bias

def _parse_args():
    """Lit les options de la commande `python train.py`."""
    parser = argparse.ArgumentParser(description="Train RLMinimax weights")
    parser.add_argument("--num-games", type=int, default=200)
    parser.add_argument("--depth", type=int, default=2)
    parser.add_argument("--layout", type=str, default="smallClassic")
    parser.add_argument("--alpha", type=float, default=0.001)
    parser.add_argument("--gamma", type=float, default=0.9)
    parser.add_argument("--epsilon", type=float, default=0.05)
    parser.add_argument("--alpha-decay", type=float, default=0.99995)
    parser.add_argument("--alpha-floor", type=float, default=0.0001)
    parser.add_argument("--early-stop-windows", type=int, default=10)
    parser.add_argument("--eval-games", type=int, default=20)
    parser.add_argument("--seed", type=int, default=None)
    parser.add_argument("--resume", action="store_true")
    return parser.parse_args()

if __name__ == "__main__":
    args = _parse_args()
    train(
        num_games=args.num_games, depth=args.depth, layout_name=args.layout,
        alpha=args.alpha, gamma=args.gamma, epsilon=args.epsilon,
        alpha_decay=args.alpha_decay, alpha_floor=args.alpha_floor,
        early_stop_windows=args.early_stop_windows, eval_games=args.eval_games,
        seed=args.seed, resume=args.resume,
    )
