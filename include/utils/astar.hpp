#ifndef RTS_ASTAR_HPP
#define RTS_ASTAR_HPP

#include <vector>

namespace grid {
    class HexSpace;

    struct Node {
        const HexSpace &space;
        Node *parent;
        int moveCost;
        int heuristic;
        [[nodiscard]] int totalCost() const { return moveCost + heuristic; }
    };

    class AStar {
    public:
        /// Calculates the shortest past from source to destination
        /// @param source The source HexSpace
        /// @param destination The destination HexSpace
        /// @param grid The grid containing the source and destination HexSpace
        /// @return The path from source to destination, will only contain source on error
        static std::vector<const HexSpace*> astar(
            const HexSpace &source,
            const HexSpace &destination,
            const std::vector<std::vector<HexSpace>> &grid);
    };
}

#endif //RTS_ASTAR_HPP
