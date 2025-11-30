# tests/test_compare_outputs.py
from pathlib import Path
import subprocess
import pytest

from cse3150_final_project._core import FileReader, FileWriter

@pytest.mark.parametrize("case_name", [
    # "many",
    "prefix",
    "subprefix",
])
def test_compare_outputs(case_name):
    tests_dir = Path(__file__).resolve().parent       # tests/
    data_dir = tests_dir / "data"                     # tests/data
    bench_dir = data_dir / "bench" / case_name

    graph_path = bench_dir / "CAIDAASGraphCollector_2025.10.15.txt"
    rov_path   = bench_dir / "rov_asns.csv"
    anns_path  = bench_dir / "anns.csv"
    out_path   = bench_dir / "my_output.csv"
    ribs_path  = bench_dir / "ribs.csv"
    script_path = data_dir / "bench" / "compare_output.sh"   # single comparator script

    # ---- Run code that produces the output file ----
    g = FileReader.readGraphFromCSV(str(graph_path))
    FileReader.updateROV(g, str(rov_path))
    FileReader.seedAnnouncements(g, str(anns_path))
    g.propagate_announcements()
    FileWriter.writeRibs(g, str(out_path))

    # ---- Sanity checks before running the comparator ----
    assert graph_path.exists(), f"graph file missing: {graph_path}"
    assert rov_path.exists(),   f"rov file missing: {rov_path}"
    assert anns_path.exists(),  f"anns file missing: {anns_path}"
    assert out_path.exists(),   f"output file was not created: {out_path}"
    assert script_path.exists(), f"compare script not found: {script_path}"

    # Run the script with absolute paths. Call bash explicitly so exec bit is not required.
    result = subprocess.run(
        ["bash", str(script_path), str(ribs_path), str(out_path)],
        capture_output=True,
        text=True,
        # cwd=bench_dir
    )


    # Helpful failure message including stdout/stderr
    assert result.returncode == 0

    # Exact-check the expected success message
    assert "✓ Files match perfectly!" in result.stdout
