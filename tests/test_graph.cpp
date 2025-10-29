#include <gtest/gtest.h>

#include <cstdint>

#include "file_reader.h"
#include "graph.h"

class GraphTest : public ::testing::Test {
   protected:
    Graph g;
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(GraphTest, TestEmptyContruction) {
    EXPECT_EQ(g.get_num_edges(), 0);
    EXPECT_EQ(g.get_num_nodes(), 0);
    EXPECT_EQ(g.get_num_customer_provider(), 0);
    EXPECT_EQ(g.get_num_peers(), 0);
    EXPECT_EQ(g.get_nodes().size(), 0);
}

TEST_F(GraphTest, TestPeerCount) {
    g.add_peer(2, 99);
    g.add_peer(4, 67);
    g.add_peer(3, 82);

    EXPECT_EQ(g.get_num_edges(), 3);
    EXPECT_EQ(g.get_num_nodes(), 6);
    EXPECT_EQ(g.get_num_customer_provider(), 0);
    EXPECT_EQ(g.get_num_peers(), 3);
    EXPECT_EQ(g.get_nodes().size(), 6);
}

TEST_F(GraphTest, TestProviderCustomerCount) {
    g.add_customer_provider(40, 89);
    g.add_customer_provider(77, 40);

    EXPECT_EQ(g.get_num_edges(), 2);
    EXPECT_EQ(g.get_num_nodes(), 3);
    EXPECT_EQ(g.get_num_customer_provider(), 2);
    EXPECT_EQ(g.get_num_peers(), 0);
    EXPECT_EQ(g.get_nodes().size(), 3);
}

TEST_F(GraphTest, TestEdgeCountSum) {
    g.add_peer(2, 99);
    g.add_peer(4, 67);
    g.add_peer(3, 82);
    g.add_customer_provider(40, 89);
    g.add_customer_provider(77, 40);

    EXPECT_EQ(g.get_num_peers() + g.get_num_customer_provider(), g.get_num_edges());
    EXPECT_EQ(g.get_num_nodes(), 9);
    EXPECT_EQ(g.get_nodes().size(), 9);
}

TEST_F(GraphTest, TestReadFile) {
    g = FileReader::readGraphFromCSV("data/test.txt");

    EXPECT_EQ(g.get_num_peers() + g.get_num_customer_provider(), g.get_num_edges());
    EXPECT_EQ(g.get_num_nodes(), 9);
    EXPECT_EQ(g.get_nodes().size(), 9);
}
