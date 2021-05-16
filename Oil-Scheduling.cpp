#include<iostream>
#include<fstream>
#include<queue>
#include<vector>
#include<string>
#include<limits.h>
using namespace std;

int read(int i);

void set_finish(bool* isset, int n);

void set_map(vector<int> Vset);

bool is_finish(vector<int> Vset, int n);

bool is_finish(bool* isset, int n);

bool is_end(int V, int n);

void search_in_Vset(vector<int> Vset, int n);

void search_in_Vset(vector<int> Vset, int n, int now, vector<int> tempset);

vector<int> set_Vset(int n)
{
	vector<int> Vset;
	for (int i = 1; i <= n; i++)
	{
		if (!is_end(i, n) && i != 1)//不是终点也不是源点
			Vset.push_back(i);//可能可以插入压力增加器的地方，升序排列了几个可以插入的元素
	}
	return Vset;
}
#define MAXN 1000
int max(int a, int b) { return (a > b) ? a : b; }
int min(int a, int b) { return (a > b) ? b : a; }
int PMAX = 0;
bool map[MAXN] = { 0 };
vector<int> setmin;
int ct_min = INT_MAX;

struct Graph
{
	int weight_max[MAXN][MAXN];   //按最长边建图
	int weight_min[MAXN][MAXN];   //按最短边建图
	int nweight[MAXN][MAXN];      
	bool isset[MAXN];     //设置了放大器的点
	int degree[MAXN];     
	Graph()
	{
		memset(weight_max, 0, sizeof(weight_max));        //最长边初始化为最小值
		memset(weight_min, 1000000, sizeof(weight_min));  //最短边初始化为最大值
		memset(isset, 0, sizeof(isset));
		memset(nweight, 0, sizeof(nweight));
		memset(degree, 0, sizeof(degree));
	}
	void clear()
	{
		memset(weight_max, 0, sizeof(weight_max));
		memset(weight_min, 1000000, sizeof(weight_min));
		memset(isset, 0, sizeof(isset));
		memset(nweight, 0, sizeof(nweight));
		memset(degree, 0, sizeof(degree));
	}
};
Graph g;
vector<int> topo;   //拓扑序列
void print(bool* set, int n)
{
	for (int i = 1; i <= n; i++)
	{
		if (set[i] != 0)
			printf("%d ", i);
	}
	printf("\n");
}
void print(vector<int>set)
{
	int n = set.size();
	for (int i = 0; i <= n - 1; i++)
		printf("%d ", set[i]);
	printf("\n");
}

int main()
{
	for (int i = 1; i <= 100; i++)
	{
	case1:
		{
			int n = read(i);    //按文件输入建图，存拓扑序列
			time_t start = clock();
			set_finish(g.isset, n);
			time_t end = clock();
		}
	case2:
		{
			int n = read(i);   //按文件输入建图，存拓扑序列
			time_t start = clock();
			vector<int> Vset = set_Vset(n);
			search_in_Vset(Vset, n);
			vector<int> ans = setmin;
			time_t end = clock();
			//print(ans);
			cout << ct_min << endl;
			string filename = "outputSTD/" + to_string(i) + ".out";
			int num;
			ifstream fin_(filename);
			fin_ >> num;
			if (ct_min == num)
				cout << "成功！" << endl;
			else
				cout << "不成功！" << endl;
			fin_.close();
		}
	}
}

void calculate_degree(int n)     //计算n个点的入度
{
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (g.nweight[i][j] != 0)     //从i到j存在边
				g.degree[j] += g.nweight[i][j];   //计算j的入度
		}
	}
}

int read(int i)  //读取一组数据，并且计算出拓扑排序
{
	string filename = "input/input" + to_string(i) + ".in";
	static int now = 0;
	int last = now;
	now = i;
	if (last != now)
		cout << endl << "当前文件： " << filename << endl;
	ifstream fin(filename);
	g.clear();        //重置图
	topo.clear();     //重置拓扑序列
	setmin.clear();   
	ct_min = INT_MAX;
	int n, m;
	fin >> n >> m >> PMAX;  //节点个数、边的条数、Pmax
	int s1, e1;             //起点，终点
	while (!fin.eof())
	{
		fin >> s1 >> e1;
		int temp;
		fin >> temp;
		g.weight_max[s1][e1] = max(g.weight_max[s1][e1], temp);  //按大边建图
		g.weight_min[s1][e1] = min(g.weight_min[s1][e1], temp);  //按小边建图
		g.nweight[s1][e1]++;  //从s1到e1是否有边
	}
	int s = 1;    //源点是1
	queue<int> q;
	q.push(1);
	topo.push_back(1);
	calculate_degree(n);  //计算每个点的入度
	while (!q.empty())
	{
		int now = q.front();
		q.pop();
		for (int j = 1; j <= n; j++)
		{
			g.degree[j] -= g.nweight[now][j];  //更新入度
			if (g.nweight[now][j] && g.degree[j] == 0)  //从now到j有通路，并且j入度为0
			{
				q.push(j);  //入度为0的加入队列中
				topo.push_back(j);
			}
		}
	}
	fin.close();
	return n;
}

void set_finish(bool* isset, int n)  //topo排序为topo，设置加压点
{
	//cout << "拓扑序列：";
	//for (auto x : topo)
	//	cout << x << " ";
	//cout << endl;
	int p = PMAX;
	int ep[MAXN] = { 0 };           //每个节点的“当前”压力
	vector<int> topo_now = topo;    //拓扑序列
	while (!topo_now.empty())   //假如topo_now排序为1,3,2,4 就从1开始，1完了3后面肯定没有入度了，这样3后面的气压就是确定的最小气压
	{
		int cost_p = INT_MAX;
		int now = topo_now.front();

		if (isset[now] || now == 1) //加了加压器
			p = PMAX;
		else
			p = ep[now];

		topo_now.erase(topo_now.begin(), topo_now.begin() + 1);
		for (int j = 1; j <= n; j++)
		{
			if (g.nweight[now][j] == 0)
				continue;
			if (p - g.weight_max[now][j] < 0 && isset[now] == 0) //权数最大的边不能成功到达对面，证明这个方法是不科学的
			{
				isset[now] = 1;   //应该加上加压器
				ep[now] = PMAX;
				p = PMAX;         //把当前压力更新为最大压力
				j = 1;            //重新搜索
			}
			else                  //now->j 权数最大的边能到达对面，那么进行更新
				ep[j] = max(ep[j], p - g.weight_min[now][j]);   //此时如果走到j的花费更少了，就更新，否则保持原来的下一个节点的最小压力
		}
	}
	    //能循环完毕证明肯定是可行的
}

void set_map(vector<int> Vset)
{
	memset(map, 0, sizeof(map));
	int n = Vset.size();
	for (int i = 0; i <= n - 1; i++)
		map[Vset[i]] = 1;
}

bool is_finish(vector<int> Vset, int n)//重载函数，初始化map
{
	set_map(Vset);
	return is_finish(map, n);
}

bool is_finish(bool* isset, int n)//判断在isset的加压条件下，topo排序为topo,从s出发能否到达每一个点
{
	int p = PMAX;
	int ep[MAXN] = { 0 };//每个节点的“当前”压力
	vector<int> topo_now = topo;
	while (!topo_now.empty())//假如topo_now排序为1,3,2,4 就从1开始，1完了3后面肯定没有入度了，这样3后面的气压就是确定的最小气压
	{
		int cost_p = INT_MAX;
		int now = topo_now.front();
		if (isset[now] || now == 1)//加了加压器
			p = PMAX;
		else
			p = ep[now];
		topo_now.erase(topo_now.begin(), topo_now.begin() + 1);
		for (int j = 1; j <= n; j++)
		{
			if (g.nweight[now][j] == 0)
				continue;
			if (p - g.weight_max[now][j] < 0)//权数最大的边不能成功到达对面，证明这个方法是不科学的
				return 0;
			else//能到达对面，那么更新
				ep[j] = max(ep[j], p - g.weight_min[now][j]);//此时如果走到j的花费更少了，就更新，否则保持原来的下一个节点的最小压力
		}
	}
	//能循环完毕证明肯定是可行的
	return 1;  
}

bool is_end(int V, int n)   //
{
	for (int j = 1; j <= n; j++)
	{
		if (g.nweight[V][j] != 0)
			return 0;
	}
	return 1;
}

void search_in_Vset(vector<int> Vset, int n)//函数重载，初始化Vset
{
	vector<int> tempset;
	search_in_Vset(Vset, n, 0, tempset);
}

void search_in_Vset(vector<int> Vset, int n, int now, vector<int> tempset)
{
	if (now == Vset.size())//到边界了
		return;
	else
	{
		search_in_Vset(Vset, n, now + 1, tempset);//搜索到Vset[now],选择不把Vset[now]加入进去
		if (is_finish(tempset, n))//如果能到达终点，证明这是一个能考虑的分支
		{
			if (tempset.size() < ct_min)  //如果得到的答案比之前的更小，则进行更新，否则不更新
			{
				ct_min = tempset.size();
				setmin = tempset;
			}
		}
		tempset.push_back(Vset[now]);   //把Vset[now]加进去
		search_in_Vset(Vset, n, now + 1, tempset);//搜索到Vset[now],选择把Vset[now]加入进去
		if (is_finish(tempset, n))      //如果能到达终点，证明这是一个能考虑的分支
		{
			if (tempset.size() < ct_min)
			{
				ct_min = tempset.size();
				setmin = tempset;
			}
		}
	}
}
