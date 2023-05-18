#include <iostream>
#include <vector>
#include <iterator>



template<typename Vertex, typename Distance = double>
struct Edge {
    Distance dist;
    Vertex from;
    Vertex to;
    Edge(Vertex& from, const Vertex& to, const Distance& d) : dist(d), from(from), to(to) {}
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
        //auto edge = new Edge<Vertex, Distance>(name, to, d);
        edge_link.push_back(edge);
    }

    IteratorEdge remove_edge_vert(IteratorEdge iter) {
        return edge_link.erase(iter);
    }
};


template<typename Vertex, typename Distance = double>
class Graph {
private:
    std::vector<VertexG<Vertex, Distance>> graph;
public:
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

    void add_vertex(const Vertex& v) {
        if (has_vertex(v))
            return;
        VertexG<Vertex, Distance> new_vert(v);
        graph.push_back(new_vert);
    }


    void add_edge(const Vertex& from, const Vertex& to, const Distance& d)
    {
        if (!has_vertex(from) && !has_vertex(to))
            throw std::logic_error("Has not vertex");
        auto vert = get_vertex(from);
        if (vert != graph.end())
            vert->add_edge_vert(to, d);
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


    bool remove_edges(const Vertex& from, const Vertex& to)
    {
        if (!has_vertex(from) && !has_vertex(to))
            throw std::logic_error("Has not vertex");
        auto vert = get_vertex(from);
        if (vert != graph.end())
        {
            auto begin_edge = vert->begin_edge();
            auto end_edge = vert->end_edge();

            //валится на последней итерации
            while (begin_edge != end_edge)
            {
                if (begin_edge->to == to)
                    begin_edge = vert->remove_edge_vert(begin_edge);
                else
                    ++begin_edge;

                /*if (begin_edge->to == to)
                {
                    begin_edge = vert->remove_edge_vert(begin_edge);
                    return true;
                }*/
            }
            return false;
        }
    }



    bool remove_edge(const Edge<Vertex, Distance>& e)
    {
        if (!has_vertex(e.from) && !has_vertex(e.to))
            throw std::logic_error("Has not vertex");
        auto vert = get_vertex(e.from);
        if (vert != graph.end())
        {

        }
    }

    /*
    bool has_edge(const Vertex& from, const Vertex& to) const;
    bool has_edge(const Edge& e); //c учетом расстояния в Edge
    */

};


int main() {
    Graph<std::string, double> graph;
    graph.add_vertex("Пирогово");
    graph.add_vertex("13123");
    graph.add_vertex("hhh");

    graph.add_edge("Пирогово", "hhh", 12.435);
    graph.add_edge("Пирогово", "hhh", 1567.435);
    graph.add_edge("Пирогово", "hhh", 12131.435);
    graph.add_edge("Пирогово", "13123", 23);
    graph.add_edge("13123", "Пирогово", 10);

    graph.remove_edges("Пирогово", "hhh");

    int abc = 0;
}