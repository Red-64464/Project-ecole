"""Features de l'agent RLMinimax.

Chaque fonction retourne un nombre x_i(s). Ces nombres sont multiplies par les
poids appris dans la formule affine :

    f(s) = a1*x1(s) + ... + a7*x7(s) + C
"""

from util import Queue

DIRECTIONS = ((0, 1), (0, -1), (1, 0), (-1, 0))


def _neighbor_cells(position, walls):
    """Retourne les cases voisines accessibles depuis une position.

    Cette fonction regarde les quatre directions possibles. Elle ignore les
    murs, car Pac-Man ne peut pas les traverser. Elle sert ensuite aux BFS.
    """
    x, y = position
    for dx, dy in DIRECTIONS:
        nx, ny = int(x + dx), int(y + dy)
        if not walls[nx][ny]:
            yield nx, ny


def _true_cells(grid):
    """Transforme une grille Berkeley en ensemble de positions vraies.

    Berkeley stocke la nourriture dans une grille. Pour faire une recherche
    plus simple, on convertit cette grille en positions `(x, y)`.
    """
    return {
        (x, y)
        for x in range(grid.width)
        for y in range(grid.height)
        if grid[x][y]
    }


def _ghost_positions(game_state, scared_only=None):
    """Retourne les positions des fantomes selon leur etat.

    `scared_only=True` garde seulement les fantomes effrayes.
    `scared_only=False` garde seulement les fantomes dangereux.
    `scared_only=None` garde tous les fantomes.
    """
    positions = []
    for ghost_state in game_state.getGhostStates():
        is_scared = ghost_state.scaredTimer > 0
        if scared_only is True and not is_scared:
            continue
        if scared_only is False and is_scared:
            continue
        gx, gy = ghost_state.getPosition()
        positions.append((int(gx), int(gy)))
    return positions


def _bfs_distance(start, targets, walls):
    """Calcule la distance reelle la plus courte vers une cible.

    La distance reelle tient compte des murs du labyrinthe. C'est plus utile
    que la distance Manhattan quand un mur se trouve entre Pac-Man et la cible.
    La fonction renvoie `None` si aucune cible n'existe ou n'est atteignable.
    """
    if not targets:
        return None

    queue = Queue()
    queue.push((start, 0))
    visited = {start}

    while not queue.is_empty():
        position, distance = queue.pop()
        if position in targets:
            return distance

        for neighbor in _neighbor_cells(position, walls):
            if neighbor not in visited:
                visited.add(neighbor)
                queue.push((neighbor, distance + 1))

    return None


def _count_targets_within(start, targets, walls, max_distance):
    """Compte les cibles accessibles dans un rayon donne.

    Cette fonction sert surtout a savoir combien de fantomes sont proches.
    Elle utilise aussi BFS, donc le calcul respecte les murs du labyrinthe.
    """
    queue = Queue()
    queue.push((start, 0))
    visited = {start}
    target_set = set(targets)
    count = 0

    while not queue.is_empty():
        position, distance = queue.pop()
        if distance > max_distance:
            continue
        if position in target_set:
            count += 1
        if distance == max_distance:
            continue

        for neighbor in _neighbor_cells(position, walls):
            if neighbor not in visited:
                visited.add(neighbor)
                queue.push((neighbor, distance + 1))

    return count


def _closest_target_score(start, targets, walls, empty_value=0.0):
    """Convertit la distance a la cible la plus proche en score.

    Plus la cible est proche, plus le score `1/(1+distance)` est grand. Cela
    permet a la fonction affine de comparer facilement les situations.
    """
    if not targets:
        return empty_value
    distance = _bfs_distance(start, targets, walls)
    if distance is None:
        return 0.0
    return 1.0 / (1.0 + distance)


def nearest_food_bfs(game_state):
    """x1: indique si une nourriture est proche de Pac-Man.

    La methode cherche la nourriture la plus proche avec BFS. Le resultat aide
    l'agent a choisir des actions qui font avancer la partie.
    """
    position = game_state.getPacmanPosition()
    food_positions = _true_cells(game_state.getFood())
    return _closest_target_score(
        position, food_positions, game_state.getWalls(), empty_value=1.0
    )


def ghosts_within_3(game_state):
    """x2: compte les fantomes dangereux tres proches.

    Si cette valeur augmente, Pac-Man est dans une zone risquee. Avec un poids
    negatif, cette feature pousse l'agent a eviter le danger.
    """
    position = game_state.getPacmanPosition()
    ghosts = _ghost_positions(game_state, scared_only=False)
    if not ghosts:
        return 0.0
    return float(_count_targets_within(position, ghosts, game_state.getWalls(), 3))


def scared_ghosts_nearby(game_state):
    """x3: compte les fantomes effrayes assez proches.

    Un fantome effraye peut rapporter des points. Cette feature donne donc une
    information positive quand Pac-Man peut profiter de cette opportunite.
    """
    position = game_state.getPacmanPosition()
    ghosts = _ghost_positions(game_state, scared_only=True)
    if not ghosts:
        return 0.0
    return float(_count_targets_within(position, ghosts, game_state.getWalls(), 5))


def remaining_food(game_state):
    """x4: mesure la nourriture restante.

    La valeur est negative pour qu'il soit meilleur d'avoir moins de nourriture
    restante. Cela encourage Pac-Man a terminer la carte.
    """
    return -float(game_state.getNumFood())


def nearest_capsule_bfs(game_state):
    """x5: indique si une capsule est proche.

    Les capsules sont utiles quand les fantomes sont dangereux. La distance est
    calculee avec BFS pour respecter les murs du labyrinthe.
    """
    position = game_state.getPacmanPosition()
    capsules = set(game_state.getCapsules())
    return _closest_target_score(position, capsules, game_state.getWalls())


def current_score(game_state):
    """x6: reprend le score officiel du jeu sous forme reduite.

    Le score est divise par 1000 pour ne pas ecraser les autres features. Il
    garde quand meme le lien avec l'objectif officiel de Pac-Man.
    """
    return game_state.getScore() / 1000.0


def nearest_dangerous_ghost_bfs(game_state):
    """x7: mesure la proximite du fantome dangereux le plus proche.

    Plus le fantome dangereux est proche, plus cette valeur est grande. Son
    poids negatif penalise donc les positions dangereuses.
    """
    position = game_state.getPacmanPosition()
    ghosts = set(_ghost_positions(game_state, scared_only=False))
    return _closest_target_score(position, ghosts, game_state.getWalls())


FEATURE_FUNCTIONS = [
    nearest_food_bfs,
    ghosts_within_3,
    scared_ghosts_nearby,
    remaining_food,
    nearest_capsule_bfs,
    current_score,
    nearest_dangerous_ghost_bfs,
]

FEATURE_NAMES = [feature.__name__ for feature in FEATURE_FUNCTIONS]
NUM_FEATURES = len(FEATURE_FUNCTIONS)


def extract_features(game_state):
    """Retourne toutes les features dans l'ordre attendu par `weights.json`.

    Cet ordre est important : le premier poids correspond a x1, le deuxieme a
    x2, etc. C'est ce lien qui permet de calculer la fonction affine.
    """
    return [feature(game_state) for feature in FEATURE_FUNCTIONS]
