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
The copy on github runs this code against the prefix and subprefix examples since the `ribs.csv` for the many example is too large. With the local copy attached to the assignment submission, you will be able to run all 3 test just via the pytest. You can run
these pytests by running the following command from the root:

```bash
python3 -m pytest
```

There do exist other tests written in C++ in `src/cpp/tests/test_graph.cpp` however they are less cumulative and may be ran in more specific circumstances.

## Design Decisions
- Node Data Structure: My ASNodes are stored with 5 attributes, a list of their providers, customers, peers, and their policy. I reasoned that this was better than an adjacency list, since I was really only looking for fast recall of providers, customers, and peers during propagation. Since I wanted to minimize the size of these vectors, I store their uint32_t ids.

- Graph Data Structure: For the Graph there are 2 main structures in it, specifically, the nodes_ hashmap and the DAG_ vector of vectors. The nodes_ hashmap is the main lookup hashing uint32_t's to actual Node objects. This allows fast Node object lookups, so that we can keep all other structures using uint32_t.

- Announcement Data Structure: I store prefixes as std::strings, and the AS_path as a vector of uint32_t's, that I store in reverse order. This is so that I can push_back onto the AS_path with O(1) time, and I can just reverse the order of this vector when I do my final print to the csv.

- BGP Data Structure: Very similarly to the assignment guide, I just store two hashmaps, one for the local_rib and the other for the recieved queue. For the ROV part of the assignment, I thought that it would waste space doing a polymorphism scheme for this simple project, so I just added a boolean attribute to by BGP class.

- Memory Space: To minimize pointer overhead, I chose to just keep all memory allocation on the stack. As a result for many of my getters I had to overload them with a const return and a non-const return, for when I had to modify, for instance the rib_ (getting as just a reference) rather than just get the rib_ values as a const reference.

- My program takes about 50 seconds to run all 3 test cases (which can be seen if you just run the pytest (:

