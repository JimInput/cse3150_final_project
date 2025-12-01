# cse3150_final_project (a bgp simulator)

A small BGP simulator written in C++17 and bound to python via pybind11. The simulator 
models AS relationships from publicly available CAIDA graph data. We simulate the peer-provider-customer 
relationship, seed announcements into our graph, propagate announcements through a DAG-ifyed graph, 
and support simple ROV policies.

## How to Build:

Requirements:
- Python 3.11 or 3.12.

Start by downloading dependencies from the project root by:

```bash
pip3 install .
```

Then you can use the script with the following command:

```bash
python3 -m cse3150_final_project [GRAPH_PATH] [ANNS_PATH] [ROV_ANNS_PATH] [OUTPUT_PATH]
```

## Usage

The simulator executable accepts three files:

- `[GRAPH_PATH]`: relationship file with lines: `asn1|asn2|relationship`
  where `relationship` is `-1` for asn1=provider and asn2=customer
  or `0` for peers.
- `[ANNS_PATH]`: CSV with header `seed_asn,prefix,rov_invalid`.
- `[ROV_ANNS_PATH]`: file with one ASN per line indicating ASes that have
  ROV deployed.
- `[OUTPUT_PATH]`: locate for the ribs output csv.

Example:

```bash
python3 -m cse3150_final_project build/many/CAIDAASGraphCollector_2025.10.15.txt build/many/anns.csv build/many/rov_anns.csv out.csv
```

After running, the program writes `out.csv` to whatever location you specify. The CSV format is the same as the desired as listed by the project.

In addition, running this program takes approximately 40 seconds on my machine for all 3 test cases together.

## Tests

There are system tests listed under `src/cse3150_final_project/tests` These tests are system tests which 
run the code output against the bench output as desired by the project. If you first run `pip install .[test]` you can install the dependencies.
The copy on github runs this code against the prefix and subprefix examples since the `ribs.csv` for the many example is too large. You can run
these pytests by running the following command from the root:

```bash
python3 -m pytest
```

There do exist other tests written in C++ in `src/cpp/tests/test_graph.cpp` however they are less cumulative and may be ran in more specific circumstances.

## Design Decisions

