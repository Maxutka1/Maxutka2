#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define scanf scanf_s
#define ERROR -1

typedef struct
{
	unsigned n1;
	unsigned n2;
} Edge;// ребра

typedef struct
{
	Edge* edges;
	unsigned length;// количество рёбер
} Graph;// граф


void ReadGraphData(Graph* _graph);
void PrintGraph(const Graph* _graph);
unsigned GetNodesCount(const Graph* _graph);//объявление функций

void PrintGraphToFile(const Graph* _graph, const char* _file_name);

bool IsBasic(const Graph* _graph);
bool IsConnected(const Graph* _graph);
bool HaveLoop(const Graph* _graph);
bool IsTree(const Graph* _graph);

bool IsEqual(const Edge* _edge1, const Edge* _edge2);

int main(void)
{
	Graph graph;
	scanf("%u", &graph.length);
	graph.edges = (Edge*)malloc(
		sizeof(Edge) * graph.length);
	if (graph.edges == NULL)
	{
		return(ERROR);
	}

	ReadGraphData(&graph);

	if (IsTree(&graph))
	{
		printf("\nGraph is a tree.\n");
	}
	else
	{
		printf("\nGraph is not a tree.");
	}

	PrintGraphToFile(&graph, "graph.txt");

	return 0;
}

void ReadGraphData(Graph* _graph)
{
	for (unsigned s = 0; s < _graph->length; s++)
	{
		scanf("%u", &_graph->edges[s].n1);
		scanf("%u", &_graph->edges[s].n2);
	}
}

void PrintGraph(const Graph* _graph)
{
	for (unsigned s = 0; s < _graph->length; s++)
	{
		printf("%u ", _graph->edges[s].n1);
		printf("%u\n", _graph->edges[s].n2);
	}
}

bool IsBasic(const Graph* _graph)
{
	for (unsigned _edge1 = 0; _edge1 < _graph->length; _edge1++)
	{
		if (_graph->edges[_edge1].n1 != 0 && _graph->edges[_edge1].n1 == _graph->edges[_edge1].n2)
		{
			return false;
		}
		else
		{
			for (unsigned _edge2 = _edge1 + 1; _edge2 < _graph->length; _edge2++)
			{
				if (IsEqual(&_graph->edges[_edge1], &_graph->edges[_edge2]))
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool IsConnected(const Graph* _graph)
{
	bool* _flags = (bool*)malloc(sizeof(bool) * _graph->length);
	if (_flags == NULL)
	{
		exit(ERROR);
	}
	bool go_next = false;
	for (unsigned _flag = 1; _flag < _graph->length; _flag++)
	{
		_flags[_flag] = false;
	}
	for (unsigned _flag = 0; _flag < _graph->length; _flag++)
	{
		if (_graph->edges[_flag].n1 != 0 && _graph->edges[_flag].n2 != 0)
		{
			_flags[_flag] = true;
			go_next = true;
			break;
		}
	}
	if (!go_next)
	{
		free(_flags);
		return false;
	}
	for (unsigned _edge1 = 0; _edge1 < _graph->length; _edge1++)
	{
		if (_flags[_edge1])
		{
			for (unsigned _edge2 = _edge1 + 1; _edge2 < _graph->length; _edge2++)
			{
				if (!_flags[_edge2] &&
					(_graph->edges[_edge1].n1 == _graph->edges[_edge2].n1 ||
					_graph->edges[_edge1].n1 == _graph->edges[_edge2].n2 ||
					_graph->edges[_edge1].n2 == _graph->edges[_edge2].n1 || 
					_graph->edges[_edge1].n2 == _graph->edges[_edge2].n2))
				{
					_flags[_edge2] = true;
					_edge1 = 0;
				}
			}
		}
	}

	for (unsigned _flag = 0; _flag < _graph->length; _flag++)
	{
		if (!_flags[_flag])
		{
			free(_flags);
			return false;
		}
	}
	free(_flags);
	return true;
}

bool HaveLoop(const Graph* _graph)
{
	return _graph->length >= GetNodesCount(_graph);
}

bool IsTree(const Graph* _graph)
{
	return
		IsBasic(_graph) &&
		IsConnected(_graph) &&
		!HaveLoop(_graph);
}


bool IsEqual(const Edge* _edge1, const Edge* _edge2)
{
	return
		_edge1->n1 == _edge2->n1 && _edge1->n2 == _edge2->n2 ||
		_edge1->n1 == _edge2->n2 && _edge1->n2 == _edge2->n1;
}

unsigned GetNodesCount(const Graph* _graph)
{
	if (_graph->length == 0)
	{
		return 0;
	}
	unsigned* _nodes = (unsigned*)malloc(sizeof(unsigned*));
	if (_nodes == NULL)
	{
		exit(ERROR);
	}
	_nodes[0] = _graph->edges[0].n1;
	unsigned _nodes_count = 1;
	bool _add_node = false;
	for (unsigned _edge1 = 0; _edge1 < _graph->length; _edge1++)
	{
		_add_node = true;
		_nodes = (unsigned*)realloc(_nodes, sizeof(unsigned) * _nodes_count);
		if (_nodes == NULL)
		{
			exit(ERROR);
		}
		for (unsigned _node = 0; _node < _nodes_count; _node++)
		{
			if (_nodes[_node] == _graph->edges[_edge1].n1)
			{
				_add_node = false;
				break;
			}
		}
		if (_add_node)
		{
			_nodes_count++;
			
			_nodes[_nodes_count - 1] = _graph->edges[_edge1].n1;
		}

		_add_node = true;
		for (unsigned _node = 0; _node < _nodes_count; _node++)
		{
			if (_nodes[_node] == _graph->edges[_edge1].n2)
			{
				_add_node = false;
				break;
			}
		}
		if (_add_node)
		{
			_nodes_count++;
			_nodes = (unsigned*)realloc(_nodes, sizeof(unsigned) * _nodes_count);
			if (_nodes == NULL)
			{
				exit(ERROR);
			}
			_nodes[_nodes_count - 1] = _graph->edges[_edge1].n2;
		}
	}

	for (unsigned _node = 0; _node < _nodes_count; _node++)
	{
		if (_nodes[_node] == 0)
		{
			_nodes_count--;
			break;
		}
	}
	free(_nodes);
	return _nodes_count;
}

void PrintGraphToFile(const Graph* _graph, const char* _file_name)
{
	FILE* fp;
	fopen_s(&fp, _file_name, "w+");
	fprintf(fp, "graph graphName{\n");
	for (unsigned _edge1 = 0; _edge1 < _graph->length; _edge1++)
	{
		if (_graph->edges[_edge1].n1 == 0)
		{
			if (_graph->edges[_edge1].n2 != 0)
			{
				fprintf(fp, "%u;\n", _graph->edges[_edge1].n2);
			}
		}
		else if (_graph->edges[_edge1].n2 == 0)
		{
			fprintf(fp, "%u;\n", _graph->edges[_edge1].n1);
		}
		else
		{
			fprintf(fp, "%u -- %u;\n", _graph->edges[_edge1].n1, _graph->edges[_edge1].n2);
		}
	}
	fprintf(fp, "}\n");
	fclose(fp);
	system("dot C:\\Users\\Admin\\source\\repos\\DZ2\\DZ2\\graph.txt -Tbmp -o Dot_file.bmp");
	system("rundll32 \"%ProgramFiles%\\Windows Photo Viewer\\PhotoViewer.dll\", ImageView_Fullscreen C:\\Users\\Admin\\source\\repos\\DZ2\\DZ2\\Dot_file.bmp");
	/*return (0);*/
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define scanf scanf_s
#define ERROR -1

typedef struct
{
	unsigned n1;
	unsigned n2;
} Edge;// ребра

typedef struct
{
	Edge* edges;
	unsigned length;// количество рёбер
} Graph;// граф


void ReadGraphData(Graph* _graph);
void PrintGraph(const Graph* _graph);
unsigned GetNodesCount(const Graph* _graph);//объявление функций

void PrintGraphToFile(const Graph* _graph, const char* _file_name);

bool IsBasic(const Graph* _graph);
bool IsConnected(const Graph* _graph);
bool HaveLoop(const Graph* _graph);
bool IsTree(const Graph* _graph);

bool IsEqual(const Edge* _edge1, const Edge* _edge2);
bool EdgesIsConnected(const Edge* e1, const Edge* e2);

int main(void)
{
	Graph graph;
	scanf("%u", &graph.length);
	graph.edges = (Edge*)malloc(
		sizeof(Edge) * graph.length);
	//if (graph.edges == NULL)
	//{
	//	return(ERROR);
	//}

	ReadGraphData(&graph);

	if (IsTree(&graph))
	{
		printf("\nGraph is a tree.\n");
	}
	else
	{
		printf("\nGraph is not a tree.");
	}

	PrintGraphToFile(&graph, "graph.txt");

	return 0;
}

void ReadGraphData(Graph* _graph)
{
	for (unsigned s = 0; s < _graph->length; s++)
	{
		scanf("%u", &_graph->edges[s].n1);
		scanf("%u", &_graph->edges[s].n2);
	}
}

void PrintGraph(const Graph* _graph)
{
	for (unsigned s = 0; s < _graph->length; s++)
	{
		printf("%u ", _graph->edges[s].n1);
		printf("%u\n", _graph->edges[s].n2);
	}
}

bool IsBasic(const Graph* _graph)
{
	for (unsigned _edge1 = 0; _edge1 < _graph->length; _edge1++)
	{
		if (_graph->edges[_edge1].n1 != 0 && _graph->edges[_edge1].n1 == _graph->edges[_edge1].n2)
		{
			return false;
		}
		else
		{
			for (unsigned _edge2 = _edge1 + 1; _edge2 < _graph->length; _edge2++)
			{
				if (IsEqual(&_graph->edges[_edge1], &_graph->edges[_edge2]))
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool IsConnected(const Graph* _graph)
{
	bool* _flags = (bool*)malloc(sizeof(bool) * _graph->length);
	if (_flags == NULL)
	{
		exit(ERROR);
	}
	bool go_next = false;
	for (unsigned _flag = 0; _flag < _graph->length; _flag++)
	{
		_flags[_flag] = false;
	}
	for (unsigned _flag = 0; _flag < _graph->length; _flag++)
	{
		if (_graph->edges[_flag].n1 != 0 &&
			_graph->edges[_flag].n2 != 0)
		{
			_flags[_flag] = true;
			go_next = true;
			break;
		}
	}
	if (!go_next)
	{
		free(_flags);
		return false;
	}
	for (unsigned _edge1 = 0; _edge1 < _graph->length; _edge1++)
	{
		if (_flags[_edge1])
		{
			for (unsigned _edge2 = _edge1 + 1; _edge2 < _graph->length; _edge2++)
			{
				if (!_flags[_edge2] && IsConnected(&_graph->edges[_edge1], &_graph->edges[_edge2]))
				{
					_flags[_edge2] = true;
					_edge1 = 0;
				}
			}
		}
	}

	for (unsigned _flag = 0; _flag < _graph->length; _flag++)
	{
		if (!_flags[_flag])
		{
			free(_flags);
			return false;
		}
	}
	free(_flags);
	return true;
}

bool HaveLoop(const Graph* _graph)
{
	return _graph->length >= GetNodesCount(_graph);
}

bool IsTree(const Graph* _graph)
{
	IsBasic(_graph);
	printf("\n\n%u\n\n", _graph->length);
	IsConnected(_graph);
	printf("\n\n%u\n\n", _graph->length);
	HaveLoop(_graph);
	printf("\n\n%u\n\n", _graph->length);
	return
		IsBasic(_graph) &&
		IsConnected(_graph) &&
		!HaveLoop(_graph);
}


bool IsEqual(const Edge* _edge1, const Edge* _edge2)
{
	return
		_edge1->n1 == _edge2->n1 && _edge1->n2 == _edge2->n2 ||
		_edge1->n1 == _edge2->n2 && _edge1->n2 == _edge2->n1;
}

unsigned GetNodesCount(const Graph* _graph)
{
	if (_graph->length == 0)
	{
		return 0;
	}
	unsigned* _nodes = (unsigned*)malloc(sizeof(unsigned));
	if (_nodes == NULL)
	{
		exit(ERROR);
	}
	_nodes[0] = _graph->edges[0].n1;
	unsigned _nodes_count = 1;
	bool _add_node = false;
	for (unsigned _edge1 = 0; _edge1 < _graph->length; _edge1++)
	{
		_add_node = true;
		for (unsigned _node = 0; _node < _nodes_count; _node++)
		{
			if (_nodes[_node] == _graph->edges[_edge1].n1)
			{
				_add_node = false;
				break;
			}
		}
		if (_add_node)
		{
			_nodes_count++;
			_nodes = (unsigned*)realloc(_nodes, sizeof(unsigned) * _nodes_count);
			if (_nodes == NULL)
			{
				exit(ERROR);
			}
			_nodes[_nodes_count - 1] = _graph->edges[_edge1].n1;
		}

		_add_node = true;
		for (unsigned _node = 0; _node < _nodes_count; _node++)
		{
			if (_nodes[_node] == _graph->edges[_edge1].n2)
			{
				_add_node = false;
				break;
			}
		}
		if (_add_node)
		{
			_nodes_count++;
			_nodes = (unsigned*)realloc(_nodes, sizeof(unsigned) * _nodes_count);
			if (_nodes == NULL)
			{
				exit(ERROR);
			}
			_nodes[_nodes_count - 1] = _graph->edges[_edge1].n2;
		}
	}

	for (unsigned _node = 0; _node < _nodes_count; _node++)
	{
		if (_nodes[_node] == 0)
		{
			_nodes_count--;
			break;
		}
	}
	free(_nodes);
	return _nodes_count;
}

void PrintGraphToFile(const Graph* _graph, const char* _file_name)
{
	FILE* fp;
	fopen_s(&fp, _file_name, "w+");
	fprintf(fp, "graph graphName{\n");
	for (unsigned _edge1 = 0; _edge1 < _graph->length; _edge1++)
	{
		if (_graph->edges[_edge1].n1 == 0)
		{
			if (_graph->edges[_edge1].n2 != 0)
			{
				fprintf(fp, "%u;\n", _graph->edges[_edge1].n2);
			}
		}
		else if (_graph->edges[_edge1].n2 == 0)
		{
			fprintf(fp, "%u;\n", _graph->edges[_edge1].n1);
		}
		else
		{
			fprintf(fp, "%u -- %u;\n", _graph->edges[_edge1].n1, _graph->edges[_edge1].n2);
		}
	}
	fprintf(fp, "}\n");
	fclose(fp);
	system("dot C:\\Users\\Admin\\source\\repos\\DZ2\\DZ2\\graph.txt -Tbmp -o Dot_file.bmp");
	system("rundll32 \"%ProgramFiles%\\Windows Photo Viewer\\PhotoViewer.dll\", ImageView_Fullscreen C:\\Users\\Admin\\source\\repos\\DZ2\\DZ2\\Dot_file.bmp");
	return (0);
}

bool EdgesIsConnected(const Edge* e1, const Edge* e2)
{
	return
		e1->n1 == e2->n1 ||
		e1->n1 == e2->n2 ||
		e1->n2 == e2->n1 ||
		e1->n2 == e2->n2;
}
*/