import argparse

from cse3150_final_project._core import FileReader, FileWriter


def main():
    parser = argparse.ArgumentParser(description="Process four input files.")

    parser.add_argument("graph_file", help="Path to the graph file")
    parser.add_argument("announcement_file", help="Path to the anns file")
    parser.add_argument("rov_asn_file", help="Path to the rov_asns file")
    parser.add_argument("output_file", help="Path to the output file")

    args = parser.parse_args()
    try:
        g = FileReader.readGraphFromCSV(args.graph_file)
        FileReader.updateROV(g, args.rov_asn_file)
        FileReader.seedAnnouncements(g, args.announcement_file)
        g.propagate_announcements()
        FileWriter.writeRibs(g, args.output_file)
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

if __name__ == '__main__':
    main()
