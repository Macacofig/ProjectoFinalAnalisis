import pandas as pd


# CARGAR CSV

nodes_original = pd.read_csv("nodes.csv")
edges_original = pd.read_csv("edges.csv")


print("DATOS ORIGINALES")
print(f"Nodos: {len(nodes_original)}")
print(f"Aristas: {len(edges_original)}")

# CLASES VEHICULARES VALIDAS

vehicular_classes = [

    "motorway",
    "motorway_link",

    "trunk",
    "trunk_link",

    "primary",
    "primary_link",

    "secondary",
    "secondary_link",

    "tertiary",
    "tertiary_link",

    "residential",
    "living_street",

    "service",
    "unclassified",

    "busway"
]

# LIMPIEZA 1
# LIMPIEZA AGRESIVA

print("LIMPIEZA AGRESIVA")

nodes_clean = nodes_original.copy()
edges_clean = edges_original.copy()

# LIMPIAR NODOS

nodes_clean = nodes_clean.drop_duplicates(
    subset=["node_id"]
)

# NODOS VALIDOS

valid_nodes_clean = set(
    nodes_clean["node_id"]
)

# ELIMINAR DUPLICADOS

edges_clean = edges_clean.drop_duplicates(
    subset=["from_id", "to_id", "distance_m"]
)

# ELIMINAR SELF LOOPS

edges_clean = edges_clean[
    edges_clean["from_id"]
    != edges_clean["to_id"]
]

# ELIMINAR NODOS INVALIDOS

edges_clean = edges_clean[
    edges_clean["from_id"].isin(valid_nodes_clean)
]

edges_clean = edges_clean[
    edges_clean["to_id"].isin(valid_nodes_clean)
]

# ELIMINAR DISTANCIAS ABSURDAS

edges_clean = edges_clean[
    (edges_clean["distance_m"] > 0)
    &
    (edges_clean["distance_m"] < 10000)
]

# NORMALIZAR MAXSPEED

edges_clean["maxspeed"] = pd.to_numeric(
    edges_clean["maxspeed"]
)

# ELIMINAR VELOCIDADES ABSURDAS

edges_clean = edges_clean[
    (edges_clean["maxspeed"] >= 10)
    &
    (edges_clean["maxspeed"] <= 130)
]

# FILTRAR SOLO VIAS VEHICULARES

edges_clean = edges_clean[
    edges_clean["fclass"].isin(
        vehicular_classes
    )
]

# ELIMINAR NODOS HUERFANOS

used_nodes_clean = set(
    edges_clean["from_id"]
) | set(
    edges_clean["to_id"]
)

nodes_clean = nodes_clean[
    nodes_clean["node_id"].isin(
        used_nodes_clean
    )
]

# RESETEAR INDICES

nodes_clean = nodes_clean.reset_index(drop=True)
edges_clean = edges_clean.reset_index(drop=True)

# GUARDAR CSV

nodes_clean.to_csv(
    "nodes_clean.csv",
    index=False
)

edges_clean.to_csv(
    "edges_clean.csv",
    index=False
)

print(f"Nodos limpios: {len(nodes_clean)}")
print(f"Aristas limpias: {len(edges_clean)}")

# LIMPIEZA 2
# LIMPIEZA CONSERVADORA / INTELIGENTE

print("LIMPIEZA CONSERVADORA")

nodes_smart = nodes_original.copy()
edges_smart = edges_original.copy()

# NODOS VALIDOS

valid_nodes_smart = set(
    nodes_smart["node_id"]
)

# ELIMINAR DUPLICADOS

edges_smart = edges_smart.drop_duplicates(
    subset=["from_id", "to_id", "distance_m"]
)

# ELIMINAR SELF LOOPS

edges_smart = edges_smart[
    edges_smart["from_id"]
    != edges_smart["to_id"]
]

# ELIMINAR ARISTAS INVALIDAS

edges_smart = edges_smart[
    edges_smart["from_id"].isin(valid_nodes_smart)
]

edges_smart = edges_smart[
    edges_smart["to_id"].isin(valid_nodes_smart)
]

# CONVERTIR MAXSPEED A NUMERICO

edges_smart["maxspeed"] = pd.to_numeric(
    edges_smart["maxspeed"]
)

# PROMEDIO VELOCIDAD POR FCLASS

valid_speed_rows = edges_smart[
    (edges_smart["maxspeed"] >= 10)
    &
    (edges_smart["maxspeed"] <= 130)
]

speed_mean_by_class = valid_speed_rows.groupby(
    "fclass"
)["maxspeed"].mean()

# CORREGIR VELOCIDADES

def fix_speed(row):

    speed = row["maxspeed"]

    if (
        speed < 10
        or speed > 130
    ):

        road_class = row["fclass"]

        class_average = speed_mean_by_class.get(
            road_class,
            30.0
        )

        if pd.isna(class_average):
            return 30.0

        return round(class_average, 2)

    return speed

edges_smart["maxspeed"] = edges_smart.apply(
    fix_speed,
    axis=1
)


# PROMEDIO DISTANCIA POR FCLASS


valid_distance_rows = edges_smart[
    (edges_smart["distance_m"] > 0)
    &
    (edges_smart["distance_m"] < 10000)
]

distance_mean_by_class = valid_distance_rows.groupby(
    "fclass"
)["distance_m"].mean()


# CORREGIR DISTANCIAS


def fix_distance(row):

    distance = row["distance_m"]

    if (
        distance <= 0
        or distance > 10000
    ):

        road_class = row["fclass"]

        class_average = distance_mean_by_class.get(
            road_class,
            100.0
        )

        if pd.isna(class_average):
            return 100.0

        return round(class_average, 2)

    return distance

edges_smart["distance_m"] = edges_smart.apply(
    fix_distance,
    axis=1
)


# FILTRAR SOLO CLASES VEHICULARES


edges_smart = edges_smart[
    edges_smart["fclass"].isin(
        vehicular_classes
    )
]


# ELIMINAR NODOS HUERFANOS


used_nodes_smart = set(
    edges_smart["from_id"]
) | set(
    edges_smart["to_id"]
)

nodes_smart = nodes_smart[
    nodes_smart["node_id"].isin(
        used_nodes_smart
    )
]


# RESETEAR INDICES


nodes_smart = nodes_smart.reset_index(drop=True)
edges_smart = edges_smart.reset_index(drop=True)


# GUARDAR CSV


nodes_smart.to_csv(
    "nodes_smart.csv",
    index=False
)

edges_smart.to_csv(
    "edges_smart.csv",
    index=False
)


# ESTADISTICAS FINALES


print(f"Nodos conservados: {len(nodes_smart)}")
print(f"Aristas conservadas: {len(edges_smart)}")


print("ARCHIVOS GENERADOS")


print("nodes_clean.csv")
print("edges_clean.csv")

print("nodes_smart.csv")
print("edges_smart.csv")