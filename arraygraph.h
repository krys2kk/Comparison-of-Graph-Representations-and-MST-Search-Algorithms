#ifndef ARRAYGRAPH_H
#define ARRAYGRAPH_H

#include<vector>
#include<algorithm>
#include <queue>
#include<limits>

struct Result												//struct to store results of Dijkstra and Bellman Ford
{
	int point;
	int val;
	int prev;
};
struct Edge													//struct to store edges
{
	int start;
	int val;
	int stop;
};
struct EdgeNoVal											//struct to store results of BFS
{
	int start;
	int stop;
};
class GraphArray
{
private:
	std::vector<std::vector<int>> edges;					//table of edge values
	int size;
public:
	GraphArray(int s): size(s), edges(s, std::vector<int>(s, 0)) {}

	GraphArray& operator=(const GraphArray& other)			//constructor
	{
		if (this != &other)
		{
			size = other.size;
			edges = other.edges;
		}
		return* this;
	}

	~GraphArray() = default;								//destructor

	std::vector<Result> dijkstra(int first)					//Dijkstra algorithm
	{
		const int INF = std::numeric_limits<int>::max();
		std::vector<Result> results(size);					//vector with results
		for (int i = 0; i < size; i++)
		{
			results[i].point = i;
			results[i].prev = -1;
			results[i].val = INF;
		}
		results[first].val = 0;
		using Pair = std::pair<int, int>;
		std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;		//priority queue for optimalization
		pq.push({ 0, first });
		std::vector<bool> vis(size, false);					//vector to check if node was already visited
		while (!pq.empty())
		{
			int current = pq.top().second;
			pq.pop();
			if (vis[current])
				continue;
			vis[current] = true;
			for (int i = 0; i < size; i++)
			{
				int w = edges[current][i];
				if (w != 0 && !vis[i])
				{
					int dist = results[current].val + w;
					if (dist < results[i].val)
					{
						results[i].val = dist;
						results[i].prev = current;
						pq.push({ dist, i });
					}
				}
			}
		}
		return results;
	}

	std::vector<Result> bellmanford(int first)				//Bellman Ford algorithm
	{
		std::vector<Result> resultss(size);					//vector with results
		const int INF = std::numeric_limits<int>::max();
		for (int i = 0; i < size; i++)
		{
			resultss[i].point = i;
			resultss[i].prev = -1;
			resultss[i].val = INF;
		}
		resultss[first].val = 0;
		std::vector<Edge> pathss;							//vector with all examined paths
		pathss.reserve(size * size);
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (edges[i][j] != 0)
				{
					Edge link; //aktualnie badana krawêdŸ
					link.start = i;
					link.stop = j;
					link.val = edges[i][j];
					pathss.push_back(link);
				}
			}
		}
		bool updated = true;							//bool to check if anything changed for optimalization
		for (int i = 0; i < size - 1; i++)
		{
			updated = false;
			for (const Edge& edge : pathss)
			{
				int u = edge.start;
				int v = edge.stop;
				int w = edge.val;
				if (resultss[u].val != INF && resultss[u].val + w < resultss[v].val)
				{
					resultss[v].val = resultss[u].val + w;
					resultss[v].prev = u;
					updated = true;
				}
			}
			if (!updated)
				break;
		}	
		return resultss;
	}

	std::vector<EdgeNoVal> bfs(int first)						//BFS algorithm
	{
		std::vector<bool> visited(size, false);
		std::queue<int> q;
		std::vector<EdgeNoVal> results;					//vector with results
		visited[first] = true;
		q.push(first);
		while (!q.empty()) {
			int current = q.front();
			q.pop();
			for (int i = 0; i < size; i++) {
				if (edges[current][i] != 0 && !visited[i]) {
					visited[i] = true;
					q.push(i);
					results.push_back({ current, i });
				}
			}
		}
		return results;
	}

	int getSize()												//getters and setters
	{
		return size;
	}
	void setEdge(int u, int v, int val)
	{
		edges[u][v] = val;
	}
	std::vector<std::vector<int>> getEdge()
	{
		return edges;
	}
};
#endif
