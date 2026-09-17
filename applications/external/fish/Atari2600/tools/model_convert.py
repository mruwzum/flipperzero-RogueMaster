import numpy as np
import trimesh

points = [
    [3, 2],
    [4, 2],
    [5, 2],
    [6, 2],
    [7, 2],
    [8, 2],
    [2, 3],
    [3, 3],
    [4, 3],
    [5, 3],
    [6, 3],
    [7, 3],
    [8, 3],
    [9, 3],
    [2, 4],
    [3, 4],
    [4, 4],
    [5, 4],
    [6, 4],
    [7, 4],
    [8, 4],
    [9, 4],
    [3, 5],
    [4, 5],
    [5, 5],
    [6, 5],
    [7, 5],
    [8, 5],
    [3, 6],
    [4, 6],
    [6, 6],
    [8, 6],
    [9, 6],
    [4, 7],
    [6, 7],
    [9, 7],
    [2, 8],
    [4, 8],
    [6, 8],
    [7, 8],
    [2, 9],
    [3, 9],
    [4, 9],
]

# Every point represents a 1x1x1 cube
voxels = {(x, y, 1) for x, y in points}

vertices = []
faces = []


def add_face(v0, v1, v2, v3):
    i = len(vertices)

    vertices.extend([v0, v1, v2, v3])

    faces.append([i, i + 1, i + 2])
    faces.append([i, i + 2, i + 3])


# Directions to neighboring cubes
directions = [(1, 0, 0), (-1, 0, 0), (0, 1, 0), (0, -1, 0), (0, 0, 1), (0, 0, -1)]

for x, y, z in voxels:
    for dx, dy, dz in directions:
        # Is there another cube next to this face?
        neighbor = (x + dx, y + dy, z + dz)

        if neighbor in voxels:
            # Shared face -> don't create it
            continue

        # No neighboring cube -> this face is visible
        if (dx, dy, dz) == (1, 0, 0):
            add_face(
                (x + 1, y, z),
                (x + 1, y + 1, z),
                (x + 1, y + 1, z + 1),
                (x + 1, y, z + 1),
            )

        elif (dx, dy, dz) == (-1, 0, 0):
            add_face((x, y, z), (x, y, z + 1), (x, y + 1, z + 1), (x, y + 1, z))

        elif (dx, dy, dz) == (0, 1, 0):
            add_face(
                (x, y + 1, z),
                (x, y + 1, z + 1),
                (x + 1, y + 1, z + 1),
                (x + 1, y + 1, z),
            )

        elif (dx, dy, dz) == (0, -1, 0):
            add_face((x, y, z), (x + 1, y, z), (x + 1, y, z + 1), (x, y, z + 1))

        elif (dx, dy, dz) == (0, 0, 1):
            add_face(
                (x, y, z + 1),
                (x + 1, y, z + 1),
                (x + 1, y + 1, z + 1),
                (x, y + 1, z + 1),
            )

        elif (dx, dy, dz) == (0, 0, -1):
            add_face((x, y, z), (x, y + 1, z), (x + 1, y + 1, z), (x + 1, y, z))

mesh = trimesh.Trimesh(vertices=np.array(vertices), faces=np.array(faces), process=True)

mesh.export("jellyfish.stl")
