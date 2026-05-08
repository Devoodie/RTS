// src/tests/astartest.cpp

#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "utils/grid.hpp"
#include "engine/entities.hpp"

namespace {

std::vector<std::vector<HexSpace>> MakeGrid(int rows, int cols) {
    std::vector<std::vector<HexSpace>> map(
        rows,
        std::vector<HexSpace>(cols)
    );

    grid::initGrid(rows, cols, map);
    return map;
}

bool AreNeighbors(const HexSpace* a, const HexSpace* b) {
    return std::any_of(
        std::begin(a->neighbors),
        std::end(a->neighbors),
        [b](const HexSpace* neighbor) {
            return neighbor == b;
        }
    );
}

void ExpectValidPath(
    const std::vector<HexSpace*>& path,
    HexSpace& source,
    const HexSpace& destination
) {
    ASSERT_FALSE(path.empty());

    EXPECT_EQ(path.front(), &source);
    EXPECT_EQ(*path.back(), destination);

    for (size_t i = 1; i < path.size(); ++i) {
        EXPECT_TRUE(AreNeighbors(path[i - 1], path[i]))
            << "Invalid step between path[" << (i - 1)
            << "] and path[" << i << "]";
    }
}

} // namespace

TEST(AStarTest, SourceEqualsDestinationReturnsSourceOnly) {
    auto map = MakeGrid(3, 3);

    HexSpace& source = map[1][1];
    const HexSpace& destination = map[1][1];

    std::vector<HexSpace*> path =
        grid::astar(source, destination, map);

    ASSERT_EQ(path.size(), 1u);
    EXPECT_EQ(path[0], &source);
}

TEST(AStarTest, FindsPathToAdjacentHex) {
    auto map = MakeGrid(3, 3);

    HexSpace& source = map[1][1];

    ASSERT_NE(source.neighbors[grid::EAST], nullptr);

    const HexSpace& destination =
        *source.neighbors[grid::EAST];

    std::vector<HexSpace*> path =
        grid::astar(source, destination, map);

    ExpectValidPath(path, source, destination);
    EXPECT_EQ(path.size(), 2u);
}

TEST(AStarTest, FindsPathAcrossGrid) {
    auto map = MakeGrid(6, 6);

    HexSpace& source = map[0][0];
    const HexSpace& destination = map[5][5];

    std::vector<HexSpace*> path =
        grid::astar(source, destination, map);

    ExpectValidPath(path, source, destination);
}

TEST(AStarTest, ReturnsSourceOnlyWhenSourceIsNotInGrid) {
    auto map = MakeGrid(3, 3);

    HexSpace sourceOutsideGrid;
    const HexSpace& destination = map[0][0];

    std::vector<HexSpace*> path =
        grid::astar(sourceOutsideGrid, destination, map);

    ASSERT_EQ(path.size(), 1u);
    EXPECT_EQ(path[0], &sourceOutsideGrid);
}

TEST(AStarTest, ReturnsSourceOnlyWhenNoPathExists) {
    auto map = MakeGrid(3, 3);

    HexSpace& source = map[1][1];
    const HexSpace& destination = map[0][0];

    for (HexSpace*& neighbor : source.neighbors) {
        neighbor = nullptr;
    }

    std::vector<HexSpace*> path =
        grid::astar(source, destination, map);

    ASSERT_EQ(path.size(), 1u);
    EXPECT_EQ(path[0], &source);
}