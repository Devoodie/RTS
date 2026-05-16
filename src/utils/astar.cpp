#include "utils/astar.hpp"
#include "utils/grid.hpp"

#include <algorithm>
#include <cmath>
#include <memory>
#include <unordered_map>

namespace grid {
	// Returns a heuristic using Euclidean approximation
	int getHeuristicDistance(const HexSpace &a, const HexSpace &b) {
		return static_cast<int>(std::sqrt(std::pow(a.x_position - b.x_position, 2) +
			std::pow(a.y_position - b.y_position, 2)));
	}

	// Returns a path reconstruction where path[0] is start
	std::vector<const HexSpace*> reconstructPath(const Node *currentNode) {
		std::vector<const HexSpace*> wPath;
		std::vector<const HexSpace*> path;

		while (currentNode != nullptr) {
			wPath.push_back(&currentNode->space);
			currentNode = currentNode->parent;
		}

		while (!wPath.empty()) {
			path.push_back(wPath.back());
			wPath.pop_back();
		}

		return path;
	}

	std::vector<const HexSpace *> AStar::astar(
		const HexSpace &source,
		const HexSpace &destination,
		const std::vector<std::vector<HexSpace>> &grid)
	{
		auto closedList = std::vector<Node*>();
		auto openList   = std::vector<Node*>();
		auto nodeList   = std::unordered_map<const HexSpace*, std::unique_ptr<Node>>();
		Node* startNode = nullptr;
		static int maxCost = 9999;

		// Create nodes for each HexSpace in grid, set startNode
		for (auto &x : grid) {
			for (auto &y : x) {
				nodeList.emplace(&y, std::make_unique<Node>(
					Node{
					y,
					nullptr,
					y.move_cost,
						maxCost
				}
				));
			}
		}

		if (nodeList.contains(&source)) {
			startNode = nodeList.at(&source).get();
			startNode->moveCost = 0;
			startNode->heuristic = 0;
		} else {
			// Add error logging here if engine-applicable
			return std::vector(1, &source); // source not found on provided grid
		}

		openList.push_back(startNode);

		while (!openList.empty()) {
			// Set currentNode as node with the lowest total cost
			std::ranges::sort(openList, [](const Node* a, const Node* b)
			{
				return a->totalCost() > b->totalCost();
			});

			Node* currentNode = openList.back();

			// Return if destination has been reached
			if (&currentNode->space == &destination) {
				return reconstructPath(currentNode);
			}

			// Move currentNode from open to closed
			closedList.push_back(currentNode);
			openList.pop_back();

			// Establish list of neighbors
			std::vector<Node*> neighbors;
			for (const HexSpace* neighbor : currentNode->space.neighbors) {
				if (neighbor != nullptr &&
					nodeList.contains(neighbor)) {
					neighbors.push_back(nodeList.at(neighbor).get());
					}
			}

			// Check all neighboring nodes
			for (auto &neighbor : neighbors) {
				if (std::ranges::contains(closedList, neighbor)) continue;

				if (neighbor->space.occupier != nullptr &&
					&neighbor->space != &destination) continue;

				const int tentativeCost =
					currentNode->moveCost
					+ getHeuristicDistance(currentNode->space, neighbor->space)
					+ neighbor->space.move_cost;

				if (!std::ranges::contains(openList, neighbor)) {
					openList.push_back(neighbor);
				} else if (tentativeCost > neighbor->moveCost) {
					continue;
				}

				neighbor->parent = currentNode;
				neighbor->moveCost = tentativeCost;
				neighbor->heuristic = getHeuristicDistance(neighbor->space, destination);
			}
		}

		// Return just source on failure
		return std::vector(1, &source);
	}
}