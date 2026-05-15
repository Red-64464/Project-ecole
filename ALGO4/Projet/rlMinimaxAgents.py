"""Agent final du projet : RLMinimaxAgent.

Il combine alpha-beta avec une fonction d'evaluation affine :

    f(s) = a1*x1(s) + ... + a7*x7(s) + C

Les features viennent de features.py et les poids appris de weights.json.
"""

import json
import math
import os
import sys
import tempfile

from game import Agent
from features import NUM_FEATURES, extract_features

WEIGHTS_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "weights.json")
DEFAULT_WEIGHTS = [0.0] * NUM_FEATURES
DEFAULT_BIAS = 0.0
MAX_ABS_PARAMETER = 1_000_000.0


def _coerce_finite_float(value, field_name):
    """Verifie qu'une valeur de poids est un nombre utilisable.

    Cette securite evite qu'un fichier `weights.json` casse l'agent avec une
    valeur non numerique, infinie ou beaucoup trop grande.
    """
    if isinstance(value, bool):
        raise ValueError(f"{field_name} doit etre un nombre reel")
    try:
        number = float(value)
    except (TypeError, ValueError) as exc:
        raise ValueError(f"{field_name} doit etre un nombre reel") from exc

    if not math.isfinite(number):
        raise ValueError(f"{field_name} doit etre fini")
    if abs(number) > MAX_ABS_PARAMETER:
        raise ValueError(f"{field_name} est trop grand")
    return number


def _normalize_weights(weights, bias):
    """Controle la taille des poids et convertit tout en nombres reels.

    Le projet utilise exactement une valeur par feature. Cette fonction garantit
    donc que les 7 poids correspondent bien aux 7 facteurs de `features.py`.
    """
    weights = list(weights)
    if len(weights) != NUM_FEATURES:
        raise ValueError(
            f"Nombre de poids invalide : attendu {NUM_FEATURES}, obtenu {len(weights)}"
        )
    normalized = [
        _coerce_finite_float(value, f"weights[{index}]")
        for index, value in enumerate(weights)
    ]
    return normalized, _coerce_finite_float(bias, "bias")


def load_weights():
    """Charge les poids appris depuis `weights.json`.

    Si le fichier manque ou contient une erreur, l'agent reste lancable avec des
    poids par defaut. C'est utile pour eviter un crash pendant une demonstration.
    """
    if os.path.exists(WEIGHTS_FILE):
        try:
            with open(WEIGHTS_FILE, "r", encoding="utf-8") as file:
                data = json.load(file)
            if not isinstance(data, dict):
                raise ValueError("Le fichier doit contenir un objet JSON")
            return _normalize_weights(data["weights"], data["bias"])
        except (OSError, ValueError, TypeError, KeyError, json.JSONDecodeError) as exc:
            print(
                f"Avertissement : poids invalides ({exc}). Valeurs par defaut.",
                file=sys.stderr,
            )
    return DEFAULT_WEIGHTS[:], DEFAULT_BIAS


def save_weights(weights, bias):
    """Sauvegarde les poids appris dans `weights.json`.

    La sauvegarde est atomique : on ecrit d'abord dans un fichier temporaire,
    puis on remplace le vrai fichier. Cela evite un `weights.json` coupe en deux.
    """
    directory = os.path.dirname(WEIGHTS_FILE)
    os.makedirs(directory, exist_ok=True)
    temp_path = None

    try:
        normalized_weights, normalized_bias = _normalize_weights(weights, bias)
        payload = {"weights": normalized_weights, "bias": normalized_bias}

        with tempfile.NamedTemporaryFile(
            "w", encoding="utf-8", dir=directory, delete=False
        ) as temp_file:
            temp_path = temp_file.name
            json.dump(payload, temp_file, indent=2)

        os.replace(temp_path, WEIGHTS_FILE)
    except (OSError, ValueError, TypeError) as exc:
        raise RuntimeError(f"Impossible de sauvegarder les poids : {exc}") from exc
    finally:
        if temp_path and os.path.exists(temp_path):
            try:
                os.remove(temp_path)
            except OSError:
                pass


def rl_evaluation_function(game_state, weights, bias):
    """Calcule la note d'un etat avec la fonction affine du sujet.

    La methode prend les features `x_i(s)`, les multiplie par les poids appris
    `a_i`, puis ajoute le biais `C`. Les etats de victoire et de defaite ont un
    bonus/malus tres fort pour que l'arbre Alpha-Beta les reconnaisse clairement.
    """
    if game_state.isWin():
        return 100000.0 + game_state.getScore()
    if game_state.isLose():
        return -100000.0 + game_state.getScore()

    value = bias
    for weight, feature in zip(weights, extract_features(game_state)):
        value += weight * feature
    return value


class RLMinimaxAgent(Agent):
    """Agent final du projet.

    Pac-Man joue le role MAX, les fantomes jouent le role MIN. L'agent explore
    les actions avec Alpha-Beta, puis utilise la fonction affine apprise pour
    evaluer les positions quand la profondeur maximale est atteinte.
    """

    def __init__(self, depth="3"):
        """Initialise la profondeur de recherche et charge les poids appris."""
        self.index = 0
        self.depth = int(depth)
        self.weights, self.bias = load_weights()

    def get_action(self, game_state):
        """Choisit l'action de Pac-Man avec Alpha-Beta.

        La methode simule les coups possibles de Pac-Man et des fantomes. Elle
        garde la meilleure action pour Pac-Man, tout en supposant que les
        fantomes choisissent les reponses les plus defavorables.
        """
        num_agents = game_state.getNumAgents()

        def eval_fn(state):
            return rl_evaluation_function(state, self.weights, self.bias)

        def ordered_actions(state, agent_index):
            actions = state.getLegalActions(agent_index)
            if agent_index != 0 or "Stop" not in actions:
                return actions

            moving = [action for action in actions if action != "Stop"]
            # Stop n'est garde que si Pac-Man n'a aucun vrai mouvement possible.
            # Les autres actions legales restent toutes comparees par alpha-beta.
            return moving or actions

        def alpha_beta(state, depth, agent_index, alpha, beta):
            if state.isWin() or state.isLose() or depth == 0:
                return eval_fn(state), None

            actions = ordered_actions(state, agent_index)
            if not actions:
                return eval_fn(state), None

            next_agent = (agent_index + 1) % num_agents
            next_depth = depth - 1 if next_agent == 0 else depth

            if agent_index == 0:
                best_value = float("-inf")
                best_action = actions[0]
                for action in actions:
                    successor = state.generateSuccessor(agent_index, action)
                    value, _ = alpha_beta(successor, next_depth, next_agent, alpha, beta)
                    if value > best_value:
                        best_value = value
                        best_action = action
                    alpha = max(alpha, best_value)
                    if alpha >= beta:
                        break
                return best_value, best_action

            best_value = float("inf")
            best_action = actions[0]
            for action in actions:
                successor = state.generateSuccessor(agent_index, action)
                value, _ = alpha_beta(successor, next_depth, next_agent, alpha, beta)
                if value < best_value:
                    best_value = value
                    best_action = action
                beta = min(beta, best_value)
                if alpha >= beta:
                    break
            return best_value, best_action

        _, action = alpha_beta(game_state, self.depth, 0, float("-inf"), float("inf"))
        return action
