#!/usr/bin/env python3
"""Generate comparison tables and plots from timing CSV output."""

import argparse
import os
from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Compare primality algorithm timings.")
    parser.add_argument(
        "csv",
        type=Path,
        help="Path to the CSV file produced by main1.c (File,N,Algorithm,Result,Time(s)).",
    )
    parser.add_argument(
        "--output-dir",
        type=Path,
        default=Path("analysis_output"),
        help="Directory to store generated tables and plots (default: analysis_output).",
    )
    parser.add_argument(
        "--max-rows",
        type=int,
        default=50,
        help=(
            "Number of rows to print to console for the comparison table preview "
            "(full table is always written to disk)."
        ),
    )
    parser.add_argument(
        "--dpi",
        type=int,
        default=120,
        help="DPI for saved matplotlib figures (default: 120).",
    )
    return parser


def load_timings(csv_path: Path) -> pd.DataFrame:
    df = pd.read_csv(csv_path)
    expected_cols = {"File", "N", "Algorithm", "Result", "Time(s)"}
    missing = expected_cols.difference(df.columns)
    if missing:
        raise ValueError(f"CSV file is missing columns: {', '.join(sorted(missing))}")
    return df


def build_comparison_table(df: pd.DataFrame) -> pd.DataFrame:
    """
    Return a wide table with one row per (File, N) and one column per algorithm containing the time in seconds.
    """
    pivot = (
        df.pivot_table(
            index=["File", "N"],
            columns="Algorithm",
            values="Time(s)",
            aggfunc="first",
        )
        .reset_index()
        .sort_values(["File", "N"])
    )
    pivot.columns.name = None
    return pivot


def print_preview(table: pd.DataFrame, max_rows: int) -> None:
    with pd.option_context("display.max_rows", max_rows, "display.width", 120):
        print("\n=== Comparison table preview ===")
        print(table.head(max_rows).to_string(index=False))


def save_table(table: pd.DataFrame, output_dir: Path) -> Path:
    csv_path = output_dir / "comparison_table.csv"
    table.to_csv(csv_path, index=False)
    return csv_path


def plot_by_file(df: pd.DataFrame, output_dir: Path, dpi: int) -> list[Path]:
    paths: list[Path] = []
    algorithms = sorted(df["Algorithm"].unique())
    for file_name, group in df.groupby("File"):
        fig, ax = plt.subplots(figsize=(12, 6))
        for algo in algorithms:
            subset = group[group["Algorithm"] == algo].sort_values("N")
            ax.plot(subset["N"], subset["Time(s)"], label=algo)
        ax.set_title(f"Timing comparison for {file_name}")
        ax.set_xlabel("N")
        ax.set_ylabel("Time (s)")
        ax.set_yscale("log")
        ax.grid(True, which="both", ls="--", alpha=0.5)
        ax.legend(title="Algorithm")
        fig.tight_layout()
        out_path = output_dir / f"plot_{Path(file_name).stem}.png"
        fig.savefig(out_path, dpi=dpi)
        paths.append(out_path)
        plt.close(fig)
    return paths


def main() -> None:
    parser = build_parser()
    args = parser.parse_args()

    output_dir: Path = args.output_dir
    output_dir.mkdir(parents=True, exist_ok=True)

    df = load_timings(args.csv)
    table = build_comparison_table(df)

    table_path = save_table(table, output_dir)
    print_preview(table, args.max_rows)
    print(f"\nFull comparison table saved to: {table_path}")

    plot_paths = plot_by_file(df, output_dir, args.dpi)
    if plot_paths:
        print("Generated plots:")
        for path in plot_paths:
            print(f"  - {path}")
    else:
        print("No plots generated (no data).")


if __name__ == "__main__":
    main()
