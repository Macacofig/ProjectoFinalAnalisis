import polars as pl
import os
import sys
sys.stdout.reconfigure(encoding='utf-8')
# ========================================================
# CREAR CARPETA DE REPORTES
# ========================================================

output_folder = "unique_reports"

os.makedirs(output_folder, exist_ok=True)

# ========================================================
# CARGAR CSV
# ========================================================

nodes = pl.read_csv("nodes.csv")
edges = pl.read_csv("edges.csv")

# ========================================================
# FUNCION PARA EXPORTAR VALORES UNICOS
# ========================================================

def export_unique_values(dataframe, dataframe_name):

    print("\n========================================")
    print(f"ANALISIS {dataframe_name}")
    print("========================================")

    for column_name in dataframe.columns:

        print(f"\nColumna: {column_name}")

        # obtener valores únicos
        unique_values = (
            dataframe
            .select(pl.col(column_name).unique())
            .sort(column_name)
        )

        total_unique = unique_values.height

        print(
            f"Cantidad de valores únicos: "
            f"{total_unique}"
        )

        # mostrar algunos ejemplos
        print("\nPrimeros 20 valores:")

        print(
            unique_values.head(20)
        )

        # =================================================
        # EXPORTAR CSV
        # =================================================

        output_path = (
            f"{output_folder}/"
            f"{dataframe_name}_{column_name}_unique.csv"
        )

        unique_values.write_csv(output_path)

        print(
            f"\nArchivo generado:\n{output_path}"
        )

# ========================================================
# EXPORTAR NODES
# ========================================================

export_unique_values(
    nodes,
    "nodes"
)

# ========================================================
# EXPORTAR EDGES
# ========================================================

export_unique_values(
    edges,
    "edges"
)

# ========================================================
# ANALISIS EXTRA IMPORTANTE
# ========================================================

print("\n========================================")
print("ANALISIS ESPECIAL")
print("========================================")

# --------------------------------------------------------
# FCLASS CON CONTEOS
# --------------------------------------------------------

print("\nFCLASS Y FRECUENCIA")

fclass_count = (
    edges
    .group_by("fclass")
    .len()
    .sort("len", descending=True)
)

print(fclass_count)

fclass_count.write_csv(
    f"{output_folder}/fclass_frequency.csv"
)

# --------------------------------------------------------
# MAXSPEED CON CONTEOS
# --------------------------------------------------------

print("\nMAXSPEED Y FRECUENCIA")

speed_count = (
    edges
    .group_by("maxspeed")
    .len()
    .sort("maxspeed")
)

print(speed_count)

speed_count.write_csv(
    f"{output_folder}/maxspeed_frequency.csv"
)

# --------------------------------------------------------
# SELF LOOPS
# --------------------------------------------------------

print("\nSELF LOOPS")

self_loops = edges.filter(
    pl.col("from_id") == pl.col("to_id")
)

print(f"Cantidad: {self_loops.height}")

self_loops.write_csv(
    f"{output_folder}/self_loops.csv"
)

# --------------------------------------------------------
# DUPLICADOS
# --------------------------------------------------------

print("\nDUPLICADOS")

duplicates = (
    edges
    .group_by(
        [
            "from_id",
            "to_id",
            "distance_m"
        ]
    )
    .len()
    .filter(pl.col("len") > 1)
)

print(f"Cantidad: {duplicates.height}")

duplicates.write_csv(
    f"{output_folder}/duplicates.csv"
)

print("\n========================================")
print("REPORTES EXPORTADOS")
print("========================================")