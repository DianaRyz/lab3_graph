#include <iostream>
#include <vector>
#include <iterator>
#include <stack>
#include <unordered_map>

template<typename Vertex, typename Distance = double>
struct Edge {
    Distance dist;
    Vertex from;
    Vertex to;
    Edge(const Vertex& from, const Vertex& to, const Distance& d) : dist(d), from(from), to(to) {}
    Edge() : dist(0), from(from), to(to) {}
};


template<typename Vertex, typename Distance = double>
class VertexG {
private:
    Vertex name;
    std::vector<Edge<Vertex, Distance>> edge_link;
public:
    Vertex get_name() const
    {
        return name;
    }
    using IteratorEdge = typename std::vector<Edge<Vertex, Distance>>::iterator;

    IteratorEdge begin_edge() {
        return edge_link.begin();
    }

    IteratorEdge end_edge() {
        return edge_link.end();
    }

    VertexG(const Vertex& name) : name(name) {}

    void add_edge_vert(const Vertex& to, const Distance& d)
    {
        Edge<Vertex, Distance> edge(name, to, d);
        edge_link.push_back(edge);
    }

    IteratorEdge remove_edge_vert(IteratorEdge iter) {
        return edge_link.erase(iter);
    }

    std::vector<Edge<Vertex, Distance>> get_edges() {
        return edge_link;
    }

};


template<typename Vertex, typename Distance = double>
class Graph {
private:
    std::vector<VertexG<Vertex, Distance>> graph;
public:
    void print_graph() {
        for (auto begin = graph.begin(); begin != graph.end(); begin++) {
            for (auto begin_edge = begin->begin_edge(); begin_edge != begin->end_edge(); begin_edge++) {
                std::cout << "From: " << begin_edge->from << " To: " << begin_edge->to << " Dist: " << begin_edge->dist << std::endl;
            }
        }
    }

    using Iterator = typename std::vector<VertexG<Vertex, Distance>>::iterator;

    bool has_vertex(const Vertex& v) const {
        auto begin = graph.begin();
        auto end = graph.end();
        for (; begin != end; begin++)
        {
            if (begin->get_name() == v)
                return true;
        }
        return false;
    }

    Iterator get_vertex(const Vertex& from) {
        auto begin = graph.begin();
        auto end = graph.end();
        for (; begin != end; begin++)
        {
            if (from == begin->get_name())
                return begin;
        }
        return end;
    }

    void add_vertex(const Vertex& v) {
        if (has_vertex(v))
            return;
        VertexG<Vertex, Distance> new_vert(v);
        graph.push_back(new_vert);
    }

    bool remove_vertex(const Vertex& vertex) {
        auto vert = get_vertex(vertex);
        if (vert == graph.end())
            return false;
        graph.erase(vert);
        return true;
    }

    bool has_edge(const Vertex& from, const Vertex& to)
    {
        if (!has_vertex(from) && !has_vertex(to))
            throw std::logic_error("Has not vertex");
        auto vert = get_vertex(from);
        if (vert != graph.end())
        {
            for (auto begin_edge = vert->begin_edge(); begin_edge != vert->end_edge(); begin_edge++)
            {
                if (begin_edge->to == to)
                    return true;
            }
        }
        return false;
    }

    bool has_edge(const Edge<Vertex, Distance>& e) {
        if (!has_vertex(e.from) && !has_vertex(e.to))
            throw std::logic_error("Has not vertex");
        auto vert = get_vertex(e.from);
        if (vert != graph.end())
        {
            for (auto begin_edge = vert->begin_edge(); begin_edge != vert->end_edge(); begin_edge++)
            {
                if (begin_edge->to == e.to && begin_edge->dist == e.dist)
                    return true;
            }
        }
        return false;
    }

    void add_edge(const Vertex& from, const Vertex& to, const Distance& d)
    {
        if (!has_vertex(from) && !has_vertex(to))
            throw std::logic_error("Has not vertex");
        auto vert = get_vertex(from);
        if (vert != graph.end())
            vert->add_edge_vert(to, d);
    }

    bool remove_edges(const Vertex& from, const Vertex& to)
    {
        if (!has_vertex(from) && !has_vertex(to))
            throw std::logic_error("Has not vertex");
        auto vert = get_vertex(from);
        bool return_flag = false;
        if (vert != graph.end())
        {
            bool flag = true;
            while (flag)
            {
                flag = false;
                auto begin_edge = vert->begin_edge();
                auto end_edge = vert->end_edge();
                while (begin_edge != end_edge)
                {
                    if (begin_edge->to == to) {
                        begin_edge = vert->remove_edge_vert(begin_edge);
                        flag = true;
                        return_flag = true;
                        break;
                    }
                    else
                        ++begin_edge;
                }
            }
        }
        return return_flag;
    }

    bool remove_edge(const Edge<Vertex, Distance>& e)
    {
        if (!has_vertex(e.from) && !has_vertex(e.to))
            throw std::logic_error("Has not vertex");
        auto vert = get_vertex(e.from);
        if (vert != graph.end())
        {
            for (auto begin_edge = vert->begin_edge(); begin_edge != vert->end_edge(); begin_edge++)
            {
                if (begin_edge->to == e.to && begin_edge->dist == e.dist)
                {
                    begin_edge = vert->remove_edge_vert(begin_edge);
                    return true;
                }
            }
        }
        return false;
    }

    std::vector<Edge<Vertex, Distance>> edges(const Vertex& from) {
        if (!has_vertex(from))
            throw std::invalid_argument("Has not vertex");
        return get_vertex(from)->get_edges();
    }

    size_t order() const { 
        return graph.size();
    }

    size_t degree(const Vertex& v) { 
        if (!has_vertex(v))
            throw std::invalid_argument("Has not vertex");
        auto vect = get_vertex(v)->get_edges();
        return vect.size();
    }
   
    std::vector<Vertex> walk_depth(const Vertex& start_vertex)
    {
        if (!has_vertex(start_vertex))
            throw std::invalid_argument("Has not vertex");
        std::vector<Vertex> way;
        std::stack<Vertex> stack;

        stack.push(start_vertex);

        while (!stack.empty()) {

            auto elem = stack.top();
            stack.pop();
            if (std::find(way.begin(), way.end(), elem) == way.end()) {
                way.push_back(elem);
            }

            auto vert = get_vertex(elem); 
            for (auto begin = vert->begin_edge(); begin != vert->end_edge(); begin++) { 
                if (std::find(way.begin(), way.end(), begin->to) == way.end()) {
                    stack.push(begin->to);
                }
            }
        }
        return way;
    }

    std::vector<Edge<Vertex, Distance>> shortest_path(const Vertex& from, const Vertex& to) {
        if (!has_vertex(from) || !has_vertex(to))
            throw std::invalid_argument("Has not vertex");

        std::unordered_map<Vertex, Distance> distance;
        std::unordered_map<Vertex, Edge<Vertex, Distance>> way;
        std::vector<Edge<Vertex, Distance>> result;

        auto begin = graph.begin();
        auto end = graph.end();
        for (auto begin = graph.begin(); begin != graph.end(); begin++)
        {
            distance[begin->get_name()] = LLONG_MAX;
        }
        distance[from] = 0;

        for (size_t i = 0; i < order() - 1; ++i) {
            for (auto begin = graph.begin(); begin != graph.end(); begin++) {
                for (auto begin_edge = begin->begin_edge(); begin_edge != begin->end_edge(); begin_edge++) {

                    if (distance[begin_edge->from] + begin_edge->dist < distance[begin_edge->to]) {
                        distance[begin_edge->to] = distance[begin_edge->from] + begin_edge->dist;
                        way[begin_edge->to] = *begin_edge;
                    }

                }
            }
        }
        Vertex current = to;
        while (current != from) {
            result.push_back(way[current]);
            current = way[current].from;
        }

        std::reverse(result.begin(), result.end());

        return result;
    }

    std::vector<Edge<Vertex, Distance>> task(const Vertex& from, const Vertex& to) {
        if (!has_vertex(from) || !has_vertex(to))
            throw std::invalid_argument("Has not vertex");

        std::unordered_map<Vertex, Distance> distance;
        std::unordered_map<Vertex, Edge<Vertex, Distance>> way;
        std::vector<Edge<Vertex, Distance>> result;

        auto begin = graph.begin();
        auto end = graph.end();
        for (auto begin = graph.begin(); begin != graph.end(); begin++)
        {
            distance[begin->get_name()] = LLONG_MIN;
        }
        distance[from] = 0;

        for (size_t i = 0; i < order() - 1; ++i) {
            for (auto begin = graph.begin(); begin != graph.end(); begin++) {
                for (auto begin_edge = begin->begin_edge(); begin_edge != begin->end_edge(); begin_edge++) {

                    if (distance[begin_edge->from] + begin_edge->dist > distance[begin_edge->to]) {
                        distance[begin_edge->to] = distance[begin_edge->from] + begin_edge->dist;
                        way[begin_edge->to] = *begin_edge;
                    }

                }
            }
        }
        Vertex current = to;
        while (current != from) {
            result.push_back(way[current]);
            current = way[current].from;
        }

        std::reverse(result.begin(), result.end());
        return result;

    }
};

int main() {
    Graph<std::string, double> graph;
    graph.add_vertex("0");
    graph.add_vertex("1");
    graph.add_vertex("2");
    graph.add_vertex("3");
    graph.add_vertex("4");
    graph.add_vertex("5");

    graph.add_edge("0", "3", 1);
    graph.add_edge("0", "1", 3);
    graph.add_edge("1", "4", 6);
    graph.add_edge("1", "2", 5);
    graph.add_edge("3", "4", 2);
    graph.add_edge("2", "5", 1);
    graph.add_edge("5", "4", 5);

    std::cout << "Output Graph" << std::endl;
    graph.print_graph();

    graph.remove_vertex("5");
    graph.remove_edge(Edge<std::string, double>("5", "4", 5));
    graph.remove_edges("2", "5");
    
    std::cout << "Has vertex 2: " << graph.has_vertex("2") << std::endl;
    std::cout << "Has edge 2 -> 5: " << graph.has_edge("2", "5") << std::endl;
    std::cout << "Has edge 1 -> 2 (dist = 5): " << graph.has_edge(Edge<std::string, double>("1", "2", 5)) << std::endl;

    std::cout << "Output Graph" << std::endl;
    graph.print_graph();

    std::cout << "Degree vertex 1: " << graph.degree("1") << std::endl;

    std::cout << "Shortest path: " << std::endl;
    for (auto& i : graph.shortest_path("0", "4")) {
        std::cout << "From: " << i.from << " To:" << i.to << std::endl;
    }

    std::cout << "Long path: " << std::endl;
    for (auto& i : graph.task("0", "4")) {
        std::cout << "From: " << i.from << " To: " << i.to << std::endl;
    }
    
    std::cout << "Depth walk: " << std::endl;
    for (auto& i : graph.walk_depth("0")) {
        std::cout << i << " ";
    }
}
