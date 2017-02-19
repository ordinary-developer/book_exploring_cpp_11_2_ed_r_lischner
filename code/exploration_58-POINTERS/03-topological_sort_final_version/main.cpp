#include "artifact.hpp"
#include "topological_sort.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>


class dependency_graph {
    public:
        typedef 
          std::unordered_map<artifact*, 
                             std::unordered_set<artifact*>> graph_type;

        void store_dependency(artifact* target, 
                              artifact* dependency)
        {
            graph_[dependency].insert(target);
            graph_[target];
        }

        graph_type const& graph() const { return graph_; }

        template <class OutIter>
        void sort(OutIter sorted) const {
            topological_sort(graph_, sorted);
        }

    private:
        graph_type graph_;
};

std::map<std::string, artifact> artifacts{};
artifact* lookup_artifact(std::string const& name) {
    auto iterator = artifacts.find(name);
    if (artifacts.end() == iterator)
        artifacts.emplace(name, artifact{name});
    return &artifacts[name];
}


int main() {
    dependency_graph graph{};

    std::string line{};
    while (std::getline(std::cin, line)) {
        std::string target_name{}, dependency_name{};
        std::istringstream stream{line};
        if (stream >> target_name >> dependency_name)
        {
            artifact* target{ lookup_artifact(target_name) };
            artifact* dependency{ lookup_artifact(dependency_name) };
            graph.store_dependency(target, dependency);
        }
        else if (not target_name.empty())
            std::cerr << "malformed input: target, " << target_name
                      << ", must be followed by a dependency name\n";
    }

    try {
        std::vector<artifact*> sorted{};
        graph.sort(std::back_inserter(sorted));
        for (auto it(sorted.rbegin()), end(sorted.rend()); it != end; ++it)
            std::cout << (*it)->name() << '\n';
    }
    catch (std::runtime_error const& ex) {
        std::cerr << ex.what() << '\n';
        return EXIT_FAILURE;
    }

    return 0;
}
