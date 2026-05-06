# Tarea2 Estructura de Datos

A C console application that manages a movie database by reading data from a CSV file and storing it in custom-built Abstract Data Types (Maps and Lists).

## Run & Operate

- **Run**: `cd Tarea2_Estructura_de_Datos && gcc -o tarea2_app tarea2.c tdas/extra.c tdas/list.c tdas/map.c && mkdir -p data && cp Top1500.csv data/ && ./tarea2_app`
- **Compile only**: `cd Tarea2_Estructura_de_Datos && gcc -o tarea2_app tarea2.c tdas/extra.c tdas/list.c tdas/map.c`

## Stack

- **Language**: C (GCC 14.3.0)
- **Build**: Manual GCC compilation
- **Platform**: Replit (NixOS stable-25_05)

## Where things live

- `Tarea2_Estructura_de_Datos/tarea2.c` — Main entry point, menu-driven interface
- `Tarea2_Estructura_de_Datos/tdas/` — Custom data structure implementations (list, map, heap, extra utils)
- `Tarea2_Estructura_de_Datos/Top1500.csv` — Movie dataset (copied to `data/Top1500.csv` at runtime)
- `Tarea2_Estructura_de_Datos/data/Top1500.csv` — Runtime data location expected by the program

## Architecture decisions

- The program reads CSV at `data/Top1500.csv` (relative to working directory), so it must be run from within `Tarea2_Estructura_de_Datos/`
- Movies are stored in two maps: `pelis_byid` (keyed by ID) and `pelis_bygenres` (keyed by genre)
- Custom linked-list and hash-map TDA implementations are in `tdas/`

## Product

- Menu-driven CLI for loading and searching a 1500-movie database
- Option 1: Load movies from CSV into memory
- Option 2: Search a movie by ID
- Options 3–7: Placeholder for future features

## User preferences

_Populate as you build_

## Gotchas

- Must compile before running; the pre-built `tarea2` binary in the repo may be stale
- `data/Top1500.csv` must exist before running option 1 (the workflow copies it automatically)
- Run commands must be executed from within `Tarea2_Estructura_de_Datos/` directory

## Pointers

- Workflows skill: `.local/skills/workflows/SKILL.md`
