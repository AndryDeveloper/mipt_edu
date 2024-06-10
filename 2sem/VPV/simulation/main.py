from numba import njit
import numpy as np
from random import randint, random


HEIGHT = 700
WIDTH = 700
GRAVITY = 0.2
M_RADIUS = 1
D_FORCE = 5000
N_MOLECULES = 100
START_VELOCITY = 0
dt = 0.5
dx = 50
alpha = 6
eps = 0.0


@njit(cache=True)
def norm(vector):
    return (vector[0] ** 2 + vector[1] ** 2) ** 0.5


@njit(cache=True)
def dot(vector1, vector2):
    return vector1[0] * vector2[0] + vector1[1] * vector2[1]


@njit(cache=True)
def calculate_force(molecule1, molecule2):
    radius_vector = molecule1 - molecule2
    distance = norm(molecule1 - molecule2)
    if distance >= 2*M_RADIUS:
        q = M_RADIUS / distance
        return -alpha * D_FORCE / M_RADIUS ** 2 * (q ** (alpha + 2)) * radius_vector
    else:
        return np.array([0., 0.])


@njit(cache=True)
def compute_extra_forces(molecules):
    forces = np.zeros((molecules.shape[0], 2))
    for i in range(molecules.shape[0]):
        for j in range(i + 1, molecules.shape[0]):
            force = calculate_force(molecules[i][:2], molecules[j][:2])
            forces[i], forces[j] = force + np.array([0, 1]) * GRAVITY, -force + np.array([0, 1]) * GRAVITY
    return forces


@njit(cache=True)
def calculate_collisions(molecules):
    for i in range(molecules.shape[0]):
        for j in range(i + 1, molecules.shape[0]):
            radius = molecules[i][:2] - molecules[j][:2]
            distance = norm(radius)
            if distance <= 2 * M_RADIUS:
                v_1 = dot(molecules[i][2:], radius) / distance ** 2 * radius
                v_2 = dot(molecules[j][2:], radius) / distance ** 2 * radius
                molecules[i][2:] = v_2 + molecules[i][2:] - v_1
                molecules[j][2:] = v_1 + molecules[j][2:] - v_2

                molecules[i][:2] -= radius / 2 - radius / distance * M_RADIUS
                molecules[j][:2] += radius / 2 - radius / distance * M_RADIUS


@njit(cache=True)
def collisions_with_walls(molecules):
    for i in range(molecules.shape[0]):
        if molecules[i][0] - M_RADIUS <= 0:
            molecules[i][0] = M_RADIUS
            molecules[i][2] *= -1
        if molecules[i][0] + M_RADIUS >= WIDTH:
            molecules[i][0] = WIDTH - M_RADIUS
            molecules[i][2] *= -1

        if molecules[i][1] - M_RADIUS <= 0:
            molecules[i][1] = M_RADIUS
            molecules[i][3] *= -1
        if molecules[i][1] + M_RADIUS >= HEIGHT:
            molecules[i][1] = HEIGHT - M_RADIUS
            molecules[i][3] *= -1


@njit(cache=True)
def apply_forces(molecules, forces):
    for i in range(molecules.shape[0]):
        molecules[i][:2] += molecules[i][2:] * dt + forces[i] * dt ** 2 / 2
        molecules[i][2:] += forces[i] * dt


@njit(cache=True)
def prepare():
    molecules = np.zeros((N_MOLECULES, 4))
    for molecule in molecules:
        molecule[0] = randint(M_RADIUS, WIDTH - M_RADIUS)
        molecule[1] = randint(HEIGHT*3 // 4 + M_RADIUS, HEIGHT - M_RADIUS)
        velocity = START_VELOCITY
        angle = random() * 2 * np.pi
        molecule[2] = velocity * np.cos(angle)
        molecule[3] = velocity * np.sin(angle)
    return molecules


@njit(cache=True)
def calculate_mean_force(x, dx, molecules, molecules_previous):
    force = np.array([0., 0.])
    N = 0
    for i in range(molecules.shape[0]):
        if x <= molecules[i][1] <= x + dx:
            force += (molecules_previous[i][2:] - molecules[i][2:]) / dt
            N += 1
    if N == 0:
        return np.array([0., -GRAVITY])
    else:
        force /= N
        return np.array([N, force[1]])


@njit(cache=True)
def simulation_step(molecules, forces_accumulate, K):
    molecules_copy = molecules.copy()
    calculate_collisions(molecules)
    forces = compute_extra_forces(molecules)
    apply_forces(molecules, forces)

    i = 0
    for x in range(dx, HEIGHT - dx, dx):
        forces_accumulate[i] = (forces_accumulate[i] * (K - 1) +
                                calculate_mean_force(x, dx, molecules, molecules_copy)) / K
        i += 1

    collisions_with_walls(molecules)


def simulate():
    molecules = prepare()

    forces_accumulate = np.array([np.array([0., 0.]) for x in range(dx, HEIGHT - dx, dx)])
    K = 1
    while True:
        forces_accumulate_copy = forces_accumulate.copy()
        simulation_step(molecules, forces_accumulate, K)
        K += 1

        acc = np.sqrt(np.sum((forces_accumulate[:, 1] - forces_accumulate_copy[:, 1])**2)) / np.mean(forces_accumulate[:, 1])

        if K % 50 == 0:
            with open('stock.txt', 'w') as f:
                for q in forces_accumulate:
                    f.write(f'{q[0]},{q[1]}\n')

            print(K, abs(acc), np.mean(molecules[:, 2]**2 + molecules[:, 3]**2))
        if abs(acc) < eps and K >= 100:
            break


if __name__ == '__main__':
    simulate()

