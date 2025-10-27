Just finished printing for graphs and nodes to get their information.

Next is ingesting the data and adding it to each Nodes vectors so that they can have their providers, peers, and customers in easily binary searchable std::vectors.

To do this we will parse the file using FileReader, and get all of the connections, then after ingesting and reading all of the data, we will iterate over each node and sort their std::vectors so that they will be easier to work with.
