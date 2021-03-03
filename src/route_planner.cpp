#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);
}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();
    for(auto neighbour :current_node->neighbors){
        //Set Parent Node
        neighbour->parent= current_node;
        //calc H value
        neighbour->h_value=CalculateHValue(neighbour);
        //Set G Value
        neighbour->g_value = current_node->g_value + current_node->distance(*neighbour);
        //Visited Node
        neighbour->visited = true;
        open_list.push_back(neighbour);
    }
    
}






bool Compare(RouteModel::Node *a,RouteModel::Node *b){
    auto f1 = a->g_value + a->h_value;
    auto f2 = b->g_value + b->h_value;
    return f1>f2;
}

RouteModel::Node *RoutePlanner::NextNode() {
    std::sort(open_list.begin(),open_list.end(),Compare); // Sort the open_list according to the sum of the h value and g value.
    auto *ptr = open_list.back(); //Create a pointer to the node in the list with the lowest sum.
    open_list.pop_back();// Remove that node from the open_list.
    return ptr; // - Return the pointer.
}


// ConstructFinalPath method that return the final path found from your A* search.
std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    
     while(current_node->parent != nullptr){
        path_found.push_back(*current_node);
        distance = distance + current_node->distance(*(current_node->parent));// - For each node in the chain, add the distance from the node to its parent to the distance variable.
        current_node = current_node->parent;
    }

    path_found.push_back(*current_node);
    std::reverse(path_found.begin(),path_found.end());
    distance *= m_Model.MetricScale();
    
    return path_found;
}


// A* Search algorithm
void RoutePlanner::AStarSearch() {
    start_node->visited = true;
    open_list.push_back(start_node);

    // TODO: Implement your solution here.
    while(open_list.size() != 0){
        auto checkNode = NextNode(); // NextNode() method to sort the open_list and return the next node.
        if(checkNode == RoutePlanner::end_node){
            m_Model.path = ConstructFinalPath(checkNode); //ConstructFinalPath method to return the final path that was found.
        }
        AddNeighbors(checkNode); // AddNeighbors method to add all of the neighbors of the current node to the open_list.
    }

}