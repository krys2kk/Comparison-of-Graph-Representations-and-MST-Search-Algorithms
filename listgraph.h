#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include<vector>
#include<algorithm>
#include <queue>
#include <tuple>

struct Edgee												//struct to store edges
{
	int start;
	int val;
	int stop;
};
struct EdgeeNoVal											//struct to store results of BFS
{
	int start;
	int stop;
};

class GraphList
{
private:
	std::vector<std::vector<std::tuple<int, int>>> edges; //table of links between nodes (end point and value)
	int size;
public:
	GraphList(int s = 10) : size(s), edges(s) {}		//constructors

	GraphList(const GraphList& other)
	{
		edges = other.edges;
		size = other.size;
	}

	~GraphList() = default;								//destructor
		
	std::vector<Edgee> dijkstra(int first)
	{
		const int INF = std::numeric_limits<int>::max();			//def of inf
		std::vector<int> dist(size, INF);							//vectors to store results
		std::vector<int> prev(size, -1);
		std::vector<bool> visited(size, false);						//vector to check if node was already visited
		dist[first] = 0;
		for (int count = 0; count < size; ++count)
		{
			int u = -1;
			int minDist = INF;
			for (int i = 0; i < size; ++i)
			{
				if (!visited[i] && dist[i] < minDist)
				{
					minDist = dist[i];
					u = i;
				}
			}
			if (u == -1)
				break;
			visited[u] = true;
			for (const auto& neighbor : edges[u])
			{
				int v = std::get<0>(neighbor);
				int weight = std::get<1>(neighbor);

				if (!visited[v] && dist[u] + weight < dist[v])
				{
					dist[v] = dist[u] + weight;
					prev[v] = u;
				}
			}
		}
		std::vector<Edgee> results(size);							//results
		for (int i = 0; i < size; ++i)
		{
			results[i].start = prev[i];
			results[i].stop = i;
			results[i].val = dist[i];
		}
		return results;
	}

	std::vector<Edgee> bellmanford(int first)						//Bellman Ford algorithm 
	{
		const int INF = std::numeric_limits<int>::max();			//def of inf
		std::vector<int> dist(size, INF);							//vectors to store results
		std::vector<int> prev(size, -1);
		dist[first] = 0;
		std::vector<Edgee> allEdges;								//vector with all edges
		for (int u = 0; u < size; ++u)
		{
			for (const auto& neighbor : edges[u])
			{
				int v = std::get<0>(neighbor);
				int w = std::get<1>(neighbor);
				allEdges.push_back({ u, w, v });
			}
		}
		for (int i = 0; i < size - 1; ++i)
		{
			for (const auto& edge : allEdges)
			{
				if (dist[edge.start] != INF && dist[edge.start] + edge.val < dist[edge.stop])
				{
					dist[edge.stop] = dist[edge.start] + edge.val;
					prev[edge.stop] = edge.start;
				}
			}
		}
		std::vector<Edgee> results(size);							//results
		for (int i = 0; i < size; ++i)
		{
			results[i].start = prev[i];
			results[i].stop = i;
			results[i].val = dist[i];
		}
		return results;
	}

	std::vector<EdgeeNoVal> bfs(int first)							//BFS algorithm
	{
		std::vector<EdgeeNoVal> results(size);
		std::vector<int> prev(size, -1);							//vectors to store results
		std::vector<bool> vis(size, false);
		std::queue<int> q;
		q.push(first);
		vis[first] = true;
		while (!q.empty())
		{
			int u = q.front();
			q.pop();
			for (const auto& neighbor : edges[u])
			{
				int v = std::get<0>(neighbor);
				if (!vis[v])
				{
					vis[v] = true;
					prev[v] = u;
					q.push(v);
				}
			}
		}
		for (int i = 0; i < size; i++)								//results
		{
			results[i].start = i;
			results[i].stop = prev[i];
		}
		return results;
	}
	int getSize()													//getters and setters
	{
		return size;
	}
	void setEdge(int start, int stop, int w)
	{
		edges[start].push_back(std::make_tuple(stop, w));
	}
	std::vector<std::vector<std::tuple<int, int>>> getEdge()
	{
		return edges;
	}
};

#endif