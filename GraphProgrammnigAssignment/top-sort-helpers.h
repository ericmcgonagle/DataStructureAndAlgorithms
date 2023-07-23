#pragma once

#include <unordered_map>

#include "weighted-graph.hpp"
#include "graph-types.h"

template <typename T>
void computeIndegrees(const WeightedGraph<T> &graph, std::unordered_map<value_type<T>, int> &indegrees)
{
    // TODO store the indegree for each vertex in the graph in the indegrees map
    for (auto [vertex, adj_list] : graph)
    {
        indegrees[vertex] = 0;
    }

    for (auto [adjList, adj_list] : graph)
    {
        for (auto [vertex, ver_weight] : adj_list)
        {
            indegrees[vertex] += 1;
        }
    }

}
