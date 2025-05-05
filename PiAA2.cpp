#include <iostream>
#include <chrono>
#include <random>
#include <fstream>

#include "arraygraph.h"
#include "listgraph.h"

std::random_device(rd);
std::mt19937 gen(rd());
const int INF = std::numeric_limits<int>::max();                    //def inf
const int RUNS = 100;                                               //number of runs
const std::vector<float> densities = { 0.25, 0.5, 0.75, 1 };        //tested densities
const std::vector<int> sizes = { 10, 50, 100, 200, 500 };           //tested sizes
                        
GraphArray generateRandomArray(int size, float dens, bool neg)      //array generating function
{
    GraphArray graph = GraphArray(size);
    int ssize = size * size;
    int cap = ssize * dens;
    std::vector<bool> cells(ssize, false);                          //bool vector setting values on 'true' marks
    for (int i = 0; i < cap; i++)
        cells[i] = true;
    std::shuffle(cells.begin(), cells.end(), gen);
    std::vector<std::vector<bool>> boolGraph(size, std::vector<bool>(size, false));     //bool table on the basis of vector
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            boolGraph[i][j] = cells[i * size + j];
    for (int i = 0; i < size; i++)                                     //setting 0s in actual neighbors matrix
        for (int j = 0; j < size; j++)
            graph.setEdge(i, j, 0);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (boolGraph[i][j] == true)
            {   
                std::uniform_int_distribution<> g(1, 10);               //setting random values 1-10 on edges where boolGraph = true
                int v = g(gen);
                if (neg && g(gen) % 2 == 1)
                    v *= -1;
                if (v < 0 && i != j && boolGraph[j][i])                 //negative cycles prevention
                {
                    int opp = graph.getEdge()[j][i];
                    if (opp < 0)
                        v *= -1;
                }
                graph.setEdge(i, j, v);
            }
    return graph;
}
GraphList generateRandomList(int size, float dens, bool neg)
{
    GraphList graph = GraphList(size);
    int ssize = size * size;
    int cap = dens * ssize;
    
    for (int i = 0; i < size; i++)
        for (int j = 0; j < cap / size; j++)
        {
            std::uniform_int_distribution<> g(0, size - 1);             //generating random end to an edge
            int num = g(gen);
            std::uniform_int_distribution<> h(1, 10);                   //setting random values 1-10 on said edges
            int v = h(gen);
            if (neg && g(gen) % 2 == 0)
                v *= -1;
            graph.setEdge(i, num, v);
        }
    return graph;
}
void testingArray(std::ofstream& resFile, int runss = RUNS)
{
    bool cont = true;                                                   //bool to check if runss = 1
    for (int j = 0; j < sizes.size(); j++)      
        for (int k = 0; k < densities.size(); k++)
        {
            double avgDijkstraDur = 0;                                  //ints to store results
            double avgBFordDur = 0;
            double avgBFSDur = 0;
            for (int l = 0; l < runss; l++)
            {   
                GraphArray graphDij = generateRandomArray(sizes[j], densities[k], false);           //generating random arrays to be tested
                GraphArray graphBFord = generateRandomArray(sizes[j], densities[k], true);
                GraphArray graphBFS = generateRandomArray(sizes[j], densities[k], false);
                std::uniform_int_distribution<> s(0, sizes[j] - 1);
                int fst = s(gen);

                auto start = std::chrono::high_resolution_clock::now();                             //testing algorithms on generated arrays
                std::vector<Result> res1 = graphDij.dijkstra(fst);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duration = end - start;
                avgDijkstraDur += duration.count();

                start = std::chrono::high_resolution_clock::now();
                std::vector<Result> res2 = graphBFord.bellmanford(fst);
                end = std::chrono::high_resolution_clock::now();
                duration = end - start;
                avgBFordDur += duration.count();

                start = std::chrono::high_resolution_clock::now();
                std::vector<EdgeNoVal> res3 = graphBFS.bfs(fst);
                end = std::chrono::high_resolution_clock::now();
                duration = end - start;
                avgBFSDur += duration.count();
                if (runss == 1 && k == densities.size() - 1 && j == 0)                         //writing results in Graphviz form to check if they are correct if runss = 1
                {
                    for (int i = 0; i < res1.size(); i++)
                        std::cout << "g.edge('" << res1[i].point << "', '" << res1[i].prev << "', label = '" << res1[i].val << "')" << std::endl;
                    cont = false;
                }
            }
            avgDijkstraDur /= runss;
            avgBFordDur /= runss;
            avgBFSDur /= runss;

            std::cout << "Size: " << sizes[j] << " Density: " << densities[k] << std::endl;     //writing results on CLI
            std::cout << "  Dijkstra: " << avgDijkstraDur << " ms" << std::endl;
            std::cout << "  Bellman - Ford: " << avgBFordDur << " ms" << std::endl;
            std::cout << "  BFS: " << avgBFSDur << " ms" << std::endl;
            if (cont)
            {
                resFile << sizes[j] << ";" << densities[k] << ";"                               //saving results to a file
                    << avgDijkstraDur << ";"
                    << avgBFordDur << ";"
                    << avgBFSDur << std::endl << std::endl << std::endl << std::endl;
            }

        }
}

void testingList(std::ofstream& resFile, int runss = RUNS)
{
    bool cont = true;                                                   //bool to check if runss = 1
    for (int j = 0; j < sizes.size(); j++)
        for (int k = 0; k < densities.size(); k++)
        {
            double avgDijkstraDur = 0;                                  //ints to store results
            double avgBFordDur = 0;
            double avgBFSDur = 0;
            for (int l = 0; l < runss; l++)
            {
                GraphList graphDij = generateRandomList(sizes[j], densities[k], false);           //generating random lists to be tested
                GraphList graphBFord = generateRandomList(sizes[j], densities[k], true);
                GraphList graphBFS = generateRandomList(sizes[j], densities[k], false);
                std::uniform_int_distribution<> s(0, sizes[j] - 1);
                int fst = s(gen);

                auto start = std::chrono::high_resolution_clock::now();                           //testing algorithms on generated arrays
                std::vector<Edgee> res1 = graphDij.dijkstra(fst);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duration = end - start;
                avgDijkstraDur += duration.count();

                start = std::chrono::high_resolution_clock::now();
                std::vector<Edgee> res2 = graphBFord.bellmanford(fst);
                end = std::chrono::high_resolution_clock::now();
                duration = end - start;
                avgBFordDur += duration.count();

                start = std::chrono::high_resolution_clock::now();
                std::vector<EdgeeNoVal> res3 = graphBFS.bfs(fst);
                end = std::chrono::high_resolution_clock::now();
                duration = end - start;
                avgBFSDur += duration.count();
                if (runss == 1 && k == densities.size() - 1 && j == 0)                          //writing results in Graphviz form to check if they are correct if runss = 1
                {
                    for (int i = 0; i < res1.size(); i++)
                        std::cout << "g.edge('" << res1[i].start << "', '" << res1[i].stop << "', label = '" << res1[i].val << "')" << std::endl;
                    cont = false;
                }
            }
            avgDijkstraDur /= runss;
            avgBFordDur /= runss;
            avgBFSDur /= runss;

            std::cout << "Size: " << sizes[j] << " Density: " << densities[k] << std::endl;     //writing results on CLI
            std::cout << "  Dijkstra: " << avgDijkstraDur << " ms" << std::endl;
            std::cout << "  Bellman - Ford: " << avgBFordDur << " ms" << std::endl;
            std::cout << "  BFS: " << avgBFSDur << " ms" << std::endl;
            if (cont)
            {
                resFile << sizes[j] << ";" << densities[k] << ";"                               //saving results to a file
                    << avgDijkstraDur << ";"
                    << avgBFordDur << ";"
                    << avgBFSDur << std::endl;
            }
        }
}

int main()
{
    std::cout << "PERFORMANCE TESTING OF GRAPH SEARCH ALGORITHMS" << std::endl;
    std::cout << "=====================================" << std::endl;

    std::ofstream resultsFile("grpahs_results22.csv");                                          //creating a file to store results
    if (!resultsFile.is_open()) {
        std::cerr << "Error: Could not open results file." << std::endl;
        return 1;
    }

    resultsFile << "PERFORMANCE TESTING OF GRAPH ALGORITHMS" << std::endl;

    testingArray(resultsFile);                                                                  //testing all
    testingList(resultsFile);
    testingArray(resultsFile, 1);                                                               //testing for Graphviz results
    testingList(resultsFile, 1);

    resultsFile.close();

    std::cout << "\nResults have been saved to graphs_results.csv" << std::endl;

    return 0;
}
