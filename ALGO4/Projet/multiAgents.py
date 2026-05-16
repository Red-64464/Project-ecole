# multiAgents.py
# Agents du labo Berkeley completes pour le projet.
#
# Attribution Berkeley :
# The Pacman AI projects were developed at UC Berkeley by John DeNero,
# Dan Klein, Brad Miller, Nick Hay, and Pieter Abbeel.

import random
import util

from game import Agent
from features import _bfs_distance


class ReflexAgent(Agent):
    """Agent simple : il regarde seulement le prochain etat."""

    def get_action(self, game_state):
        """Choisit l'action qui obtient la meilleure evaluation immediate."""
        actions = game_state.getLegalActions()
        scores = [self.evaluation_function(game_state, action) for action in actions]
        best_score = max(scores)
        best_indices = [i for i, score in enumerate(scores) if score == best_score]
        return actions[random.choice(best_indices)]

    def evaluation_function(self, current_game_state, action):
        """Evalue l'etat obtenu apres une action reflexe.

        Utilise BFS pour les distances reelles (respecte les murs).
        """
        successor = current_game_state.generatePacmanSuccessor(action)
        new_pos = successor.getPacmanPosition()
        new_food = successor.getFood().asList()
        ghost_states = successor.getGhostStates()
        walls = successor.getWalls()
        score = successor.getScore()

        if action == "Stop":
            score -= 10

        if new_food:
            # BFS : distance reelle vers la nourriture la plus proche
            nearest_food = _bfs_distance(new_pos, set(new_food), walls) or 0
            score += 10.0 / (1.0 + nearest_food)
            score -= 4.0 * len(new_food)

        for ghost_state in ghost_states:
            ghost_pos = (int(ghost_state.getPosition()[0]), int(ghost_state.getPosition()[1]))
            # BFS : distance reelle vers ce fantome (contourne les murs)
            distance = _bfs_distance(new_pos, {ghost_pos}, walls) or 999
            if ghost_state.scaredTimer > 0:
                score += 20.0 / (1.0 + distance)
            elif distance <= 1:
                return float("-inf")
            else:
                score -= 8.0 / distance

        return score


def score_evaluation_function(current_game_state):
    """Retourne simplement le score officiel de l'etat."""
    return current_game_state.getScore()


class MultiAgentSearchAgent(Agent):
    """Classe parent : Pac-Man est MAX, les fantomes sont MIN."""

    def __init__(self, evalFn="score_evaluation_function", depth="2"):
        """Prepare la fonction d'evaluation et la profondeur de recherche."""
        self.index = 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)


def _next_agent_and_depth(state, depth, agent_index):
    """Calcule quel agent joue ensuite et quand diminuer la profondeur."""
    num_agents = state.getNumAgents()
    next_agent = (agent_index + 1) % num_agents
    next_depth = depth - 1 if next_agent == 0 else depth
    return next_agent, next_depth


class MinimaxAgent(MultiAgentSearchAgent):
    """Minimax classique : MAX pour Pac-Man, MIN pour les fantomes."""

    def get_action(self, game_state):
        """Choisit une action avec Minimax sans elagage.

        Pac-Man maximise la valeur et chaque fantome la minimise. Cette classe
        sert de baseline principale pour comparer RLMinimax.
        """
        def minimax(state, depth, agent_index):
            if depth == 0 or state.isWin() or state.isLose():
                return self.evaluationFunction(state)

            actions = state.getLegalActions(agent_index)
            if not actions:
                return self.evaluationFunction(state)

            next_agent, next_depth = _next_agent_and_depth(state, depth, agent_index)
            values = [
                minimax(state.generateSuccessor(agent_index, action), next_depth, next_agent)
                for action in actions
            ]
            return max(values) if agent_index == 0 else min(values)

        best_score = float("-inf")
        best_action = None
        for action in game_state.getLegalActions(0):
            score = minimax(game_state.generateSuccessor(0, action), self.depth, 1)
            if best_action is None or score > best_score:
                best_score = score
                best_action = action
        return best_action


class AlphaBetaAgent(MultiAgentSearchAgent):
    """Meme resultat que Minimax, mais avec coupures alpha-beta."""

    def get_action(self, game_state):
        """Choisit une action avec Minimax et elagage Alpha-Beta.

        L'algorithme garde la meme decision que Minimax pour une meme profondeur,
        mais il evite d'explorer des branches qui ne peuvent plus aider.
        """
        def alphabeta(state, depth, agent_index, alpha, beta):
            if depth == 0 or state.isWin() or state.isLose():
                return self.evaluationFunction(state)

            actions = state.getLegalActions(agent_index)
            if not actions:
                return self.evaluationFunction(state)

            next_agent, next_depth = _next_agent_and_depth(state, depth, agent_index)

            if agent_index == 0:
                value = float("-inf")
                for action in actions:
                    successor = state.generateSuccessor(agent_index, action)
                    value = max(value, alphabeta(successor, next_depth, next_agent, alpha, beta))
                    alpha = max(alpha, value)
                    if alpha >= beta:
                        break
                return value

            value = float("inf")
            for action in actions:
                successor = state.generateSuccessor(agent_index, action)
                value = min(value, alphabeta(successor, next_depth, next_agent, alpha, beta))
                beta = min(beta, value)
                if alpha >= beta:
                    break
            return value

        alpha = float("-inf")
        beta = float("inf")
        best_score = float("-inf")
        best_action = None

        for action in game_state.getLegalActions(0):
            successor = game_state.generateSuccessor(0, action)
            score = alphabeta(successor, self.depth, 1, alpha, beta)
            if best_action is None or score > best_score:
                best_score = score
                best_action = action
            alpha = max(alpha, best_score)

        return best_action


class ExpectimaxAgent(MultiAgentSearchAgent):
    """Fantomes modelises comme aleatoires : on prend une moyenne."""

    def get_action(self, game_state):
        """Choisit une action en supposant des fantomes aleatoires."""
        def expectimax(state, depth, agent_index):
            if depth == 0 or state.isWin() or state.isLose():
                return self.evaluationFunction(state)

            actions = state.getLegalActions(agent_index)
            if not actions:
                return self.evaluationFunction(state)

            next_agent, next_depth = _next_agent_and_depth(state, depth, agent_index)
            values = [
                expectimax(state.generateSuccessor(agent_index, action), next_depth, next_agent)
                for action in actions
            ]
            if agent_index == 0:
                return max(values)
            return sum(values) / len(values)

        best_score = float("-inf")
        best_action = None
        for action in game_state.getLegalActions(0):
            score = expectimax(game_state.generateSuccessor(0, action), self.depth, 1)
            if best_action is None or score > best_score:
                best_score = score
                best_action = action
        return best_action


def betterEvaluationFunction(currentGameState):
    """Evaluation manuelle du labo, gardee comme reference.

    Utilise BFS pour des distances reelles dans le labyrinthe.
    """
    if currentGameState.isWin():
        return float("inf")
    if currentGameState.isLose():
        return float("-inf")

    pacman_pos = currentGameState.getPacmanPosition()
    food = currentGameState.getFood().asList()
    walls = currentGameState.getWalls()
    score = currentGameState.getScore() - 4.0 * len(food)

    if food:
        nearest_food = _bfs_distance(pacman_pos, set(food), walls) or 0
        score += 10.0 / (1.0 + nearest_food)

    for ghost_state in currentGameState.getGhostStates():
        ghost_pos = (int(ghost_state.getPosition()[0]), int(ghost_state.getPosition()[1]))
        distance = _bfs_distance(pacman_pos, {ghost_pos}, walls) or 999
        if ghost_state.scaredTimer > 0:
            score += 20.0 / (1.0 + distance)
        elif distance <= 1:
            return float("-inf")
        else:
            score -= 8.0 / distance

    capsules = currentGameState.getCapsules()
    score -= 20.0 * len(capsules)
    if capsules:
        nearest_capsule = _bfs_distance(pacman_pos, set(capsules), walls) or 0
        score += 8.0 / (1.0 + nearest_capsule)

    return score


better = betterEvaluationFunction
