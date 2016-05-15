#include "route.h"
#include "lib_record.h"
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <ctime>
#include<vector>
#include<queue>
#define LL long long
#include <ctime>

const int NODENUM_SIZE = 2000 + 3;
const int SIDENUM_SIZE = 40000 + 3;
const int MUSTNODE_SIZE = 120;
const int INF = 0x3f3f3f3f;
using namespace std;

	LL limit = 200000000;                 //大概9秒
	LL limit2 = 50000000 / 1500;                 //大概9/4秒
	LL clk = 0;
	LL time_limit = 3900000;
	LL time_change = 1500000;
	LL time_delta = time_limit - time_change;
	LL new_time = 1900000;
	clock_t time_start_dfs2, time_end_dfs2,pro_begin,pro_end;

	int num_Side = 0;

	bool	repeat_path[SIDENUM_SIZE][2];
	int 	repeat_path_val[SIDENUM_SIZE][2];
	bool	check_repeat[SIDENUM_SIZE];
	int 	conflict_val=100000;
	vector<int> conflict_path;

	int dist[NODENUM_SIZE][NODENUM_SIZE];               //记录各点之间距离
	bool vit_one[NODENUM_SIZE];                   //记录节点是否访问
	bool vit_two[NODENUM_SIZE];                   //记录节点是否访问
	vector<int>link_node[NODENUM_SIZE];      //记录各点邻接节点
	vector<int>link_in[NODENUM_SIZE];
	int Road_id[NODENUM_SIZE][NODENUM_SIZE];         //记录点与点之间的路径id
	struct Side
	{
		int LinkID, SourceID, DestinationID, Cost;

	}S[SIDENUM_SIZE];

	/////////// 必经路径1
	vector<int>must_pass_one;           //必须经过的点集
	bool must_one[NODENUM_SIZE];                //是否走过必须进过的点

	int ans_one = INF;                  //最终权值
	int Start_one, Target_one, must_num_one;         //起点和终点，包括必经点的数目
	int final_pat_one[SIDENUM_SIZE];         //记录路径
	int final_num_path_one;          //路径长度
	struct path_detail
	{
		int sum;
		int num;
		vector<int>path;
	};
	path_detail ANS_ONE[SIDENUM_SIZE];
	path_detail ANS_two[SIDENUM_SIZE];                //路径信息
	int ind_ONE = 0;                //候选路径个数
	int FLAG_ONE = 0;              //是否找到路径

								   ///////// 必经路径2
	vector<int>must_pass_two;           //必须经过的点集
	bool must_two[NODENUM_SIZE];                //是否走过必须进过的点

	int ans_two = INF;                  //最终权值
	int Start_two, Target_two, must_num_two;         //起点和终点，包括必经点的数目
	int final_pat_two[SIDENUM_SIZE];         //记录路径
	int final_num_path_two;          //路径长度
	int ind_two = 0;                //候选路径个数
	int FLAG_two = 0;              //是否找到路径

	int nodeNum = 0;                //节点总数

	bool is_reverse=1;
	bool is_data15;
	vector<int>final_result1;
	vector<int>final_result2;
    int deg[NODENUM_SIZE];

    bool changeCost=0;
    bool path_one_have_change = 0;
    bool running_two = 0;

	bool cmp(path_detail a, path_detail b)
	{
		return a.sum<b.sum;
	}

	int Atoi(char num[20])
	{
		int len = strlen(num);
		int ans = 0;
		int ten = 1;
		for (int i = len - 1;i >= 0;i--)
		{
			ans += ten*(num[i] - '0');
			ten *= 10;
		}
		return ans;
	}

	bool visit_one[NODENUM_SIZE];
	int ans_path_one[SIDENUM_SIZE], ans_path_num_one;
	int tmp_path_one[SIDENUM_SIZE], tmp_path_num_one;

	bool visit_two[NODENUM_SIZE];
	int ans_path_two[SIDENUM_SIZE], ans_path_num_two;
	int tmp_path_two[SIDENUM_SIZE], tmp_path_num_two;

	//新加的
	struct node_detail
	{
		int dist, id;
		int steps;
		node_detail() {};
		node_detail(int d, int i)
		{
			dist = d;
			id = i;
		}
		node_detail(int d, int i, int s)
		{
			dist = d;
			id = i;
			steps = s;
		}
		bool operator <(const node_detail& r) const {
			return dist > r.dist;
		}
	};
	bool cmp_node_detail(node_detail a, node_detail b)
	{
		///return a.dist<b.dist;
		// if(deg[a.id]!=deg[b.id])
  //       	return deg[a.id] < deg[b.id];           ///新增
  //   	else
  //   		return a.dist<b.dist;
		if(a.dist==b.dist)
			return deg[a.id] < deg[b.id];
		else
			return a.dist<b.dist;
	}


	void SPFA0(int start, node_detail Dist[], int pre[], node_detail must_node[], int &nmust_node, bool must[], bool visit[]) {
		///    priority_queue<node_detail> que;
		nmust_node = 0;
		int id[NODENUM_SIZE];   //记录顶点v在must_node中的下标
		for (int i = 0;i<nodeNum;i++) {
			Dist[i].dist = INF, Dist[i].id = i;
			if (must[i])  //如果是必须经过的点的话
				must_node[nmust_node].dist = INF, must_node[nmust_node].id = i, id[i] = nmust_node++;
		}
		int vis[NODENUM_SIZE] = { 0 };
		Dist[start].dist = 0;
		///node_detail tmp(0,start);
		queue<int> que;
		///que.push(tmp);
		que.push(start);
		vis[start] = 1;
		while (!que.empty()) {
			int u = que.front(); que.pop();
			vis[u] = 0;
			for (int i = 0; i<(int)link_node[u].size(); i++) {
				// time_end_dfs2=clock();
				// if(time_end_dfs2 - time_start_dfs2>time_limit)
				// {
				//   return ;
				// }
				int v = link_node[u][i], c = dist[u][v];
				if (visit[v])
					continue;
				if (Dist[v].dist > Dist[u].dist + c) {  //更新v
					pre[v] = u;
					Dist[v].dist = Dist[u].dist + c;
					node_detail tt(Dist[v].dist, v);
					if (must[v])          //如果v是必经结点的话
						must_node[id[v]] = tt;
					///que.push(tt);
					if (!vis[v]) {
						que.push(v);
						vis[v] = 1;
					}
				}
			}
		}
	}


	//////以节点少的做最短路
	void SPFA(int start, node_detail Dist[], int pre[], node_detail must_node[], int &nmust_node, bool must[], bool visit[]) {
		///    priority_queue<node_detail> que;
		nmust_node = 0;
		int id[NODENUM_SIZE];   //记录顶点v在must_node中的下标
		for (int i = 0;i<nodeNum;i++) {
			Dist[i].dist = INF, Dist[i].id = i;
			///新加的
			Dist[i].steps = INF;
			if (must[i])  //如果是必须经过的点的话
				must_node[nmust_node].dist = INF, must_node[nmust_node].id = i, id[i] = nmust_node++;
		}
		int vis[NODENUM_SIZE] = { 0 };
		Dist[start].dist = 0;
		///新加的
		Dist[start].steps = 0;
		///node_detail tmp(0,start);
		queue<int> que;
		///que.push(tmp);
		que.push(start);
		vis[start] = 1;
		while (!que.empty()) {
			int u = que.front(); que.pop();
			vis[u] = 0;
			for (int i = 0; i<(int)link_node[u].size(); i++) {
				// time_end_dfs2=clock();
				// if(time_end_dfs2 - time_start_dfs2>time_limit)
				// {
				//   return ;
				// }
				int v = link_node[u][i], c = dist[u][v];
				if (visit[v]) continue;

				if(changeCost)
				{
					// if(running_two)
					// {
					// 	//printf("************");
					// }
					if (Dist[v].steps>Dist[u].steps + 1)
					{
						pre[v] = u;
						Dist[v].dist = Dist[u].dist + c;
						Dist[v].steps = Dist[u].steps + 1;
						node_detail tt(Dist[v].dist, v, Dist[v].steps);
						if (must[v])
							must_node[id[v]] = tt;
						if (!vis[v]) {
							que.push(v);
							vis[v] = 1;
						}
					}
				}
				else{
					if (Dist[v].steps>Dist[u].steps + 1 ||
					(Dist[v].steps == Dist[u].steps + 1 && Dist[v].dist>Dist[u].dist + c))
					{
						pre[v] = u;
						Dist[v].dist = Dist[u].dist + c;
						Dist[v].steps = Dist[u].steps + 1;
						node_detail tt(Dist[v].dist, v, Dist[v].steps);
						if (must[v])
							must_node[id[v]] = tt;
						if (!vis[v]) {
							que.push(v);
							vis[v] = 1;
						}
					}
				}
				

				
			}
		}
	}

bool jumpOut = 0;
	void dfs2(int now_pos, int cnt, int sum, int Target, int &ans, int &ans_path_num, int &tmp_path_num, int ans_path[], int tmp_path[],
		int &FLAG, bool visit[], bool must[])
	{
		time_end_dfs2 = clock();
		if(changeCost==0 && time_end_dfs2 - time_start_dfs2 > time_change)
		{
			////printf("\n first tiaojian\n");
			if(FLAG==0){
				////printf("\n second tiaojian time_change is %lld\n",time_change);
				changeCost = 1;
				jumpOut = 1;
				return ;
			}
		}
		if(jumpOut)
			return ;
		if (time_end_dfs2 - time_start_dfs2>time_limit)
		{
			return;
		}
		//     clk++;
		//     if(clk>limit2)
		//     {
		//         ////printf("^^^^^%lld^^^^",clk);
		//         if(FLAG==0){
		//                 //printf("NA\n");
		// //        print_time("END-nofind");
		//         return ;
		//        // //printf("NA");
		//        // exit(0);
		//         }
		//         return ;
		//     }
		if (now_pos == Target&&cnt == 0)
		{
			if (sum<ans)
			{
				ans = sum;
				ans_path_num = tmp_path_num;
				////printf("Cost is %d: ", ans);
				for (int i = 0;i<ans_path_num;i++) {
					ans_path[i] = tmp_path[i];
					////printf("%d|", tmp_path[i]);
				}
				FLAG = 1;
			}
			visit[Target] = 0;
			return;
		}
		else if (now_pos == Target)
		{
			visit[Target] = 0;
			return;
		}

		int pre_node[NODENUM_SIZE];
		node_detail Dist[NODENUM_SIZE];
		pre_node[now_pos] = 0;     //简单优化
		node_detail must_node[MUSTNODE_SIZE];    //必须经过的点集
		int nmust_node;                //必须经过的点的个数
									   // if(num_Side<2302&&num_Side>1998)
		SPFA(now_pos, Dist, pre_node, must_node, nmust_node, must, visit);
									   // else
		//SPFA0(now_pos, Dist, pre_node, must_node, nmust_node, must, visit);
		//sort(Dist,Dist+nodeNum,cmp_node_detail);
		visit[now_pos] = 1;
		//另外一个优化zhe
		sort(must_node, must_node + nmust_node, cmp_node_detail);

		for (int i = 0; i<nmust_node; i++) {            //不需要循环600次
			int nextt = must_node[i].id;
			if (must_node[i].dist == INF && !visit[nextt] && must[nextt]) {
				visit[now_pos] = 0;
				return;
			}
		}

		//for(int i=0;i<nodeNum;i++)   // now_pos -> nextt
		for (int i = 0; i<nmust_node; i++)                 //不需要循环600次
		{
			///int nextt=Dist[i].id;
			int nextt = must_node[i].id;
			if (visit[nextt])
				continue;
			if (!must[nextt])                //可优化二
				continue;
			///if(Dist[i].dist==INF)   continue;
			if (must_node[i].dist == INF)
				continue;
			if (must_node[i].dist + sum>ans)
				continue;
			int tmp_tmp_path[NODENUM_SIZE], tmp_tmp_path_num = 0;
			int tmp_cnt = cnt;
			while (nextt != now_pos)
			{
				tmp_tmp_path[tmp_tmp_path_num++] = Road_id[pre_node[nextt]][nextt];
				visit[nextt] = 1;
				if (must[nextt])
					tmp_cnt--;
				nextt = pre_node[nextt];
			}
			int tt = tmp_path_num;
			for (int j = tmp_tmp_path_num - 1;j >= 0;j--)
				tmp_path[tmp_path_num++] = tmp_tmp_path[j];
			///sum+=Dist[i].dist;
			sum += must_node[i].dist;
			///dfs2(Dist[i].id,tmp_cnt,sum);
			//dfs2(must_node[i].id, tmp_cnt, sum);
			dfs2(must_node[i].id, tmp_cnt, sum, Target, ans, ans_path_num, tmp_path_num, ans_path, tmp_path, FLAG, visit, must);

			//回溯
			tmp_path_num = tt;
			///sum-=Dist[i].dist;
			sum -= must_node[i].dist;
			///nextt=Dist[i].id;
			nextt = must_node[i].id;
			while (nextt != now_pos)
			{
				visit[nextt] = 0;
				nextt = pre_node[nextt];
			}
		}
		visit[now_pos] = 0;
	}


	//////dp methon
	int res_path_one[SIDENUM_SIZE], nres_path_one;
	int res_cost_one;
	bool vised_one[NODENUM_SIZE];
	int dp_state_one[NODENUM_SIZE][MUSTNODE_SIZE];

	int res_path_two[SIDENUM_SIZE], nres_path_two;
	int res_cost_two;
	bool vised_two[NODENUM_SIZE];
	int dp_state_two[NODENUM_SIZE][MUSTNODE_SIZE];

	bool cut(int u, int cost, int res_cost, vector<int> must_pass, int Start,
		int Target, bool vised[], int dp_state[][MUSTNODE_SIZE]) {
		if (cost >= res_cost) return true;     //减掉
		int num = 0;
		for (int i = 0; i<(int)must_pass.size(); i++) {
			if (must_pass[i] == Start || must_pass[i] == Target) continue;
			if (vised[must_pass[i]]) num++;
		}
		if (dp_state[u][num] == 0) {
			dp_state[u][num] = cost;
			return false;
		}
		else {
			if (dp_state[u][num]>cost) {
				dp_state[u][num] = cost;
				return false;  //刷新
			}
			else return true;
		}
	}


	int tp_path_one[SIDENUM_SIZE];
	int tp_path_two[SIDENUM_SIZE];
	int deep_threshold = 300;

	void dfs15(int u, int cost, int deep, int visednum, bool vised[],
		int tp_path[], bool must[], int Start, int Target, int num, int &res_cost,
		int res_path[], int &nres_path, vector<int>must_pass, int dp_state[][MUSTNODE_SIZE])   //当前在u点 路径话费为cost 搜索深度为deep 经过visednum个必经点
	{
		time_end_dfs2 = clock();
		if (time_end_dfs2 - time_start_dfs2>time_limit)
		{
			return;
		}
		if (deep > deep_threshold)
			return;
		vised[u] = true;
		tp_path[deep] = u;
		int tp_visednum = visednum;
		if (must[u] && u != Start && u != Target)
			tp_visednum++;   //如果是必经点的话tp_visednum+1
							 //预留剪枝
		if (cut(u, cost, res_cost, must_pass, Start, Target, vised, dp_state))
		{
			vised[u] = false;
			return;
		}
		if (u == Target) {   //找到了一条路径
			if (tp_visednum == num - 2 && cost<res_cost) {
				for (int i = 0; i <= deep; i++) res_path[i] = tp_path[i];
				nres_path = deep + 1;
				res_cost = cost;
			}
			vised[u] = 0;
			return;
		}
		for (int i = 0; i<(int)link_node[u].size(); i++) {
			int v = link_node[u][i];
			if (v == Target && tp_visednum != num - 2) continue;
			if (vised[v]) continue;
			dfs15(v, cost + dist[u][v], deep + 1, tp_visednum, vised, tp_path, must, Start, Target, num, res_cost, res_path, nres_path, must_pass, dp_state);
		}
		vised[u] = false;
	}

	void updata_confict(int id)
	{
		int from=S[id].SourceID,to=S[id].DestinationID;
		// if(repeat_path_val[from][to]!=-1)
		// {
		// 	Road_id[from][to] = repeat_path[from][to];
		// 	dist[from][to] = repeat_path_val[from][to];
		// }
		// else
			dist[from][to]+=conflict_val;
	}

	void eliminate_conflict(int id)
	{
			int from=S[id].SourceID,to=S[id].DestinationID;
		// if(repeat_path_val[from][to]!=-1)
		// {
		// 	Road_id[from][to] = repeat_path[from][to];
		// 	dist[from][to] = repeat_path_val[from][to];
		// }
		// else
			dist[from][to]-=conflict_val;
	}

	void eliminate_path1()
	{
		for(int i=0;i<(int)final_result1.size();i++)
		{
			int id=final_result1[i];
			int from=S[id].SourceID,to=S[id].DestinationID;
			int val=dist[from][to];
			////printf("%d side ori cost is %d",id,val );
			eliminate_conflict(id);
			val=dist[from][to];
			////printf("  now cost is %d\n",val );
			check_repeat[id]=0;
		}
	}

	void updata_confict_path2()
	{
		conflict_val=100000;
		for(int i=0;i<(int)final_result2.size();i++)
		{
			int id=final_result2[i];
			int from=S[id].SourceID,to=S[id].DestinationID;
			int val=dist[from][to];
			////printf("%d side ori cost is %d",id,val );
			updata_confict(id);
			val=dist[from][to];
			////printf("  now cost is %d\n",val );
			check_repeat[id]=1;
		}
	}

	void init_path_one()
	{
		memset(vit_one,0,sizeof(vit_one));
		ans_one=INF;
		memset(visit_one,0,sizeof(visit_one));
		memset(ans_path_one,0,sizeof(ans_path_one));
		memset(tmp_path_one,0,sizeof(tmp_path_one));
		ans_path_num_one=tmp_path_num_one=0;
		memset(res_path_one,0,sizeof(res_path_one));
		nres_path_one=0;
		memset(dp_state_one,0,sizeof(dp_state_one));
		memset(tp_path_one,0,sizeof(tp_path_one));
		res_cost_one=0;
	}
	void init_path_two()
	{
		memset(vit_two,0,sizeof(vit_two));
		ans_two=INF;
		memset(visit_two,0,sizeof(visit_two));
		memset(ans_path_two,0,sizeof(ans_path_two));
		memset(tmp_path_two,0,sizeof(tmp_path_two));
		ans_path_num_two=tmp_path_num_two=0;
		memset(res_path_two,0,sizeof(res_path_two));
		nres_path_two=0;
		memset(dp_state_two,0,sizeof(dp_state_two));
		memset(tp_path_two,0,sizeof(tp_path_two));
		res_cost_two=0;
	}

	void debug_path_val()
	{
		int tpn=0;
		for(int i=0;i<(int)final_result1.size();i++)
		{
			int id=final_result1[i];
			int from=S[i].SourceID,to=S[i].DestinationID;
			int val=dist[from][to];
			if(val>conflict_val)
			{
				tpn++;

			}
			//printf("%d cost is %d^^^", id,val);
		}
		//printf("\nyour program have bug %d\n",tpn );
	}
	void updata_reSide()
	{
		for(int i=0;i<(int)final_result1.size();i++)
		{
			int id=final_result1[i];
			int from = S[id].SourceID,to=S[id].DestinationID;
			if(repeat_path[from][to]!=0)
			{
				////printf("re_side is %d\n",repeat_path[from][to]-1 );
				Road_id[from][to] = repeat_path[from][to]-1;
				dist[from][to] = repeat_path_val[from][to];
			}
		}
	}

//你要完成的功能总入口
void search_route(char *topo[MAX_EDGE_NUM], int edge_num, char *demand[MAX_DEMAND_NUM], int demand_num)
{
	pro_begin=clock();
		unsigned short result1[] = { 0, 1, 2 };//P'路径
		unsigned short result2[] = { 5, 6, 2 };//P''路径
		memset(repeat_path,0,sizeof(repeat_path));	
		memset(repeat_path_val,0,sizeof(repeat_path_val));	
		////printf("****%d\n",repeat_path[0][0] );	

		for (int i = 0;i<edge_num;i++)
		{
			//   int len=strlen(topo[i]);
			int Linkid, Soureid, Destinationid, Cost;

			sscanf(topo[i], "%d , %d , %d , %d", &Linkid, &Soureid, &Destinationid, &Cost);
			//Cost=1;
			nodeNum = max(max(Destinationid, Soureid), nodeNum);
			if (is_reverse)
				swap(Destinationid, Soureid);
			if (dist[Soureid][Destinationid] != 0)
			{
			    //deg[Soureid] = 2;        ///新增
				if (dist[Soureid][Destinationid]>Cost)////////////////////////若有比当前路径小的路径，则将原路径给备用路径，更新该路径
				{
					////printf("re_side is %d\n",Road_id[Soureid][Destinationid] );
					repeat_path_val[Soureid][Destinationid] = dist[Soureid][Destinationid];////////////备用路径
					repeat_path[Soureid][Destinationid] = Road_id[Soureid][Destinationid]+1;
					Road_id[Soureid][Destinationid] = Linkid;
					dist[Soureid][Destinationid] = Cost;

				}
				else
				{                                  /////////////////若是其他cost路径，也可以由于备份需要自取
					if (repeat_path_val[Soureid][Destinationid] == 0)
					{
						////printf("re_side is %d\n",Linkid );
						repeat_path_val[Soureid][Destinationid] = Cost;
						repeat_path[Soureid][Destinationid] = Linkid+1;
					}
					else
					{
						if (repeat_path_val[Soureid][Destinationid]>Cost)
						{
							////printf("re_side is %d\n",Linkid );
							repeat_path_val[Soureid][Destinationid] = Cost;
							repeat_path[Soureid][Destinationid] = Linkid+1;
						}
					}
				}
			}
			else
			{
				link_node[Soureid].push_back(Destinationid);
				link_in[Destinationid].push_back(Soureid);
				Road_id[Soureid][Destinationid] = Linkid;
				// //printf("$%d$",Road_id[Soureid][Destinationid]);
				dist[Soureid][Destinationid] = Cost;
				deg[Soureid] ++;    ///新增
			}
			S[Linkid].Cost = Cost;
			S[Linkid].SourceID = Soureid;
			S[Linkid].DestinationID = Destinationid;
			S[Linkid].LinkID = Linkid;
			num_Side++;
		}

		int x = 0, p = 2;
		bool has = 1;
		while (1)
		{
			if (demand[0][p]<'0' || demand[0][p]>'9')
			{
				if (has)
				{
					must_pass_one.push_back(x);
					must_one[x] = 1;
				}
				x = 0;
				has = 0;
			}
			else {
				x = x * 10 + demand[0][p] - '0';
				has = 1;
			}
			if (demand[0][p] == 0)
			{
				break;
			}
			++p;
		}
		x = 0, p = 2, has = 1;
		while (1)
		{
			if (demand[1][p]<'0' || demand[1][p]>'9')
			{
				if (has)
				{
					must_pass_two.push_back(x);
					must_two[x] = 1;
				}
				x = 0;
				has = 0;
			}
			else {
				x = x * 10 + demand[1][p] - '0';
				has = 1;
			}
			if (demand[1][p] == 0)
			{
				break;
			}
			++p;
		}

		if (is_reverse) {
			swap(must_pass_one[0], must_pass_one[1]);
			swap(must_pass_two[0], must_pass_two[1]);
		}
		Start_one = must_pass_one[0], Start_two = must_pass_two[0];
		Target_one = must_pass_one[1], Target_two = must_pass_two[1];
		swap(must_pass_one[1], must_pass_one[must_pass_one.size() - 1]);
		swap(must_pass_two[1], must_pass_two[must_pass_two.size() - 1]);
		// must_pass.push_back(Target);
		vit_one[Start_one] = 1, vit_two[Start_two] = 1;
		must_num_one = must_pass_one.size(), must_num_two = must_pass_two.size();
		time_start_dfs2 = clock();

		if (is_data15)
		{
			for (int i = 0;i<(int)must_pass_one.size();i++)
			{
				//printf("%d*", must_pass_one[i]);
			}
			puts("");

			memset(vised_one, 0, sizeof(vised_one));
			res_cost_one = 0x3f3f3f3f;
			dfs15(Start_one, 0, 0, 0, vised_one, tp_path_one, must_one, Start_one, Target_one, must_num_one, res_cost_one, res_path_one, nres_path_one, must_pass_one, dp_state_one);
			//printf("first path is *****\n");
			//printf("weight = %d\n", res_cost_one);


			final_result1.resize(nres_path_one);
			for (int i = nres_path_one - 1;i>0;i--)
			{
				int id = Road_id[res_path_one[i - 1]][res_path_one[i]];
				// result1[i] = id;
				check_repeat[id]=1;///////////////////////////////重复经过的点
				updata_confict(id);/////重新赋值
				final_result1[i]=id;
				// record_result(WORK_PATH, result1[i]);
				//printf("%d|", id);
			}
			puts("");

			for (int i = 0;i<(int)must_pass_two.size();i++)
			{
				//printf("%d*", must_pass_two[i]);
			}
			puts("");

			memset(vised_two, 0, sizeof(vised_two));
			res_cost_two = 0x3f3f3f3f;
			dfs15(Start_two, 0, 0, 0, vised_two, tp_path_two, must_two, Start_two, Target_two, must_num_two, res_cost_two, res_path_two, nres_path_two, must_pass_two, dp_state_two);
			//printf("second path is *****\n");
			//printf("weight = %d\n", res_cost_two);
			int repeat_num=0;
			final_result2.resize(nres_path_two);
			for (int i = nres_path_two - 1;i>0;i--)
			{
				int id = Road_id[res_path_two[i - 1]][res_path_two[i]];
				// result2[i] = id;
				if(check_repeat[id])//////////////////////////////////如果重复经过
				{
					repeat_num++;
					conflict_path.push_back(id);
				}
				final_result2[i]=id;
				// record_result(BACK_PATH, result2[i]);
				//printf("%d|", id);
			}
			puts("");
			//printf("repeat_num is %d\n", repeat_num);

			init_path_one();
			debug_path_val();
			eliminate_path1();
			debug_path_val();
			updata_confict_path2();
			time_start_dfs2 = clock();


			res_cost_one = 0x3f3f3f3f;
			dfs15(Start_one, 0, 0, 0, vised_one, tp_path_one, must_one, Start_one, Target_one, must_num_one, res_cost_one, res_path_one, nres_path_one, must_pass_one, dp_state_one);
			//printf("first path is *****\n");
			//printf("weight = %d\n", res_cost_one);

			repeat_num=0;
			final_result1.clear();
			conflict_path.clear();
			final_result1.resize(nres_path_one);
			for (int i = nres_path_one - 1;i>0;i--)
			{
				int id = Road_id[res_path_one[i - 1]][res_path_one[i]];
				// result1[i] = id;
				if(check_repeat[id]){
					conflict_path.push_back(id);
					repeat_num++;
				}
				final_result1[i]=id;
				// record_result(WORK_PATH, result1[i]);
				//printf("%d|", id);
			}
			puts("");
			//printf("repeat_num is %d \n", repeat_num);
			//printf("conflict_path num is %d\n",(int)conflict_path.size() );
			for(int i=0;i<(int)conflict_path.size();i++)
			{
				//printf("%d**",conflict_path[i] );
			}


			// pro_end=clock();
			// cout<<"time is "<<pro_end - pro_begin<<endl;
		}
		else
		{

			// for(int i=0;i<(int)must_pass_one.size();i++)
	  //       {
	  //           //printf("%d*", must_pass_one[i]);
	  //       }
	  //       puts("");

	  //       for(int i=0;i<(int)must_pass_two.size();i++)
	  //       {
	  //           //printf("%d*", must_pass_two[i]);
	  //       }
	  //       puts("");

			visit_one[Start_one] = 1;
			nodeNum++;
			jumpOut = 0;
			//changeCost  = 1;

			dfs2(must_pass_one[0], must_num_one - 1, 0, Target_one, ans_one, ans_path_num_one, tmp_path_num_one, ans_path_one, tmp_path_one, FLAG_ONE, visit_one, must_one);
			//changeCost = 1;
			if(FLAG_ONE == 0)
			{
				
				
				init_path_one();
				time_start_dfs2 = clock();
				LL tmp = time_limit;
				time_limit = time_delta*2;
				changeCost = 1;
				jumpOut = 0;
				dfs2(must_pass_one[0], must_num_one - 1, 0, Target_one, ans_one, ans_path_num_one, tmp_path_num_one, ans_path_one, tmp_path_one, FLAG_ONE, visit_one, must_one);
				time_limit = tmp;
				changeCost = 0;
				path_one_have_change = 1;
			}
			FLAG_ONE = 0;
			changeCost = 0;

			//printf("first path is *****\n");
			//printf("%d\n",ans_one );
			//cout << ans_one << endl;

			final_result1.resize(ans_path_num_one);
			if (is_reverse)
			{
				for (int i = ans_path_num_one - 1;i >= 0;i--)
					// for(int i=0;i<ans_path_num;i++)
				{
					int id = ans_path_one[i];
					// result1[i] = id;
					check_repeat[id]=1;							//重复经过的点
					//printf("%d ", id);
					updata_confict(id);/////重新赋值
					final_result1[i]=id;
					// record_result(WORK_PATH, result1[i]);
				}
				puts("");
			}
			else {
				//for(int i=ans_path_num-1;i>=0;i--)
				for (int i = 0;i<ans_path_num_one;i++)
				{
					int id = ans_path_one[i];
					// result1[i] = id;
					check_repeat[id]=1;//////////////////////////////重复经过的点
					//printf("%d ", id);
					updata_confict(id);/////重新赋值
					final_result1[i]=id;
					// record_result(WORK_PATH, result1[i]);
				}
				puts("");
			}

			//debug_path_val();

			visit_two[Start_two] = 1;
			time_start_dfs2 = clock();
			
			//updata_reSide();   //对于重边的提取，看来不大敢加了
			
			//time_end_dfs2 = clock();
			//LL delta = time_end_dfs2 - time_start_dfs2;
			////printf("\n changeCost is %d ,FLAG_two is %d,time_change is %d ,delta is %d\n",changeCost,FLAG_two ,time_change,delta);
			// changeCost = 1;
			// //printf("\nFLAG_two is %d\n",FLAG_two );
			jumpOut = 0;
			changeCost = 0;
			dfs2(must_pass_two[0], must_num_two - 1, 0, Target_two, ans_two, ans_path_num_two, tmp_path_num_two, ans_path_two, tmp_path_two, FLAG_two, visit_two, must_two);
			//LL test_bug=time_end_dfs2 - time_start_dfs2;
			////printf("\n waste time %lld\n",test_bug );
			////printf("\nchangeCost is %d\n",changeCost );
			if(FLAG_two == 0)
			{
				eliminate_path1();
				////printf("\n *********try changeCost %d ,time is %lld \n", changeCost ,time_end_dfs2 - time_start_dfs2);
				//running_two = 1;
				init_path_two();
				time_start_dfs2 = clock();
				LL tmp = time_limit;
				time_limit = time_delta;
				changeCost = 1;
				jumpOut = 0;
				dfs2(must_pass_two[0], must_num_two - 1, 0, Target_two, ans_two, ans_path_num_two, tmp_path_num_two, ans_path_two, tmp_path_two, FLAG_two, visit_two, must_two);
				time_limit = tmp;
				changeCost = 0;
			}
			FLAG_two = 0;

			//printf("second path is *****\n");
			//printf("%d\n",ans_two );
			// cout << ans_two << endl;
			int repeat_num=0;
			final_result2.resize(ans_path_num_two);
			if (is_reverse)
			{
				for (int i = ans_path_num_two - 1;i >= 0;i--)
					// for(int i=0;i<ans_path_num;i++)
				{
					int id = ans_path_two[i];
					// result2[i] = id;
					if(check_repeat[id]){
						conflict_path.push_back(id);
						repeat_num++;
					}
					//printf("%d ", id);
					final_result2[i]=id;
					// record_result(BACK_PATH, result2[i]);
				}
				puts("");
			}
			else {
				//for(int i=ans_path_num-1;i>=0;i--)
				for (int i = 0;i<ans_path_num_two;i++)
				{
					int id = ans_path_two[i];
					// result2[i] = id;
					if(check_repeat[id]){
						conflict_path.push_back(id);
						repeat_num++;
					}
					//printf("%d ", id);
					final_result2[i]=id;
					// record_result(BACK_PATH, result2[i]);
				}
				puts("");
			}
			//printf("repeat_num is %d \n", repeat_num);
			//printf("conflict_path num is %d\n",(int)conflict_path.size() );
			for(int i=0;i<(int)conflict_path.size();i++)
			{
				//printf("%d**",conflict_path[i] );
			}
/*
跑第三次


*/
			pro_end=clock();
			//printf("\n time is %ld\n",pro_end - pro_begin);
				// cout<<"\n time is "<<pro_end - pro_begin<<endl;
			if(repeat_num==0)
				goto ans_end;
			init_path_one();
			//debug_path_val();
			eliminate_path1();
			//debug_path_val();
			updata_confict_path2();
			//debug_path_val();
			time_start_dfs2 = clock();
			time_limit=new_time;//////////////////时间参数有待商榷

			visit_one[Start_one] = 1;
			jumpOut = 0;
			if(path_one_have_change)
			{
				time_change = time_limit;
				changeCost = 1;
			}
			dfs2(must_pass_one[0], must_num_one - 1, 0, Target_one, ans_one, ans_path_num_one, tmp_path_num_one, ans_path_one, tmp_path_one, FLAG_ONE, visit_one, must_one);
			
			
			//printf("first path is *****\n");
			//printf("%d\n",ans_one );
			// cout << ans_one << endl;

			conflict_path.clear();
			final_result1.clear();
			final_result1.resize(ans_path_num_one);
			repeat_num=0;
			if (is_reverse)
			{
				for (int i = ans_path_num_one - 1;i >= 0;i--)
					// for(int i=0;i<ans_path_num;i++)
				{
					int id = ans_path_one[i];
					// result1[i] = id;
					if(check_repeat[id])
					{
						repeat_num++;
						conflict_path.push_back(id);
					}
					// check_repeat[id]=1;							//重复经过的点
					//printf("%d ", id);
					// updata_confict(id);/////重新赋值
					final_result1[i]=id;
					// record_result(WORK_PATH, result1[i]);
				}
				puts("");
			}
			else {
				//for(int i=ans_path_num-1;i>=0;i--)
				for (int i = 0;i<ans_path_num_one;i++)
				{
					int id = ans_path_one[i];
					if(check_repeat[id])
					{
						repeat_num++;
						conflict_path.push_back(id);
					}
					// result1[i] = id;
					// check_repeat[id]=1;//////////////////////////////重复经过的点
					//printf("%d ", id);
					// updata_confict(id);/////重新赋值
					final_result1[i]=id;
					// record_result(WORK_PATH, result1[i]);
				}
				puts("");
			}




		}

	ans_end:	//printf("\n*************our ans following*********\n");
		if(is_reverse)
		{
			reverse(final_result1.begin(),final_result1.end());
			reverse(final_result2.begin(),final_result2.end());
		}
		//printf("first path cost is %d\n",ans_one );
		for(int i=0;i<(int)final_result1.size();i++)
		{
			int id=final_result1[i];
			result1[i]=id;
			//printf("%d||",id );
			record_result(WORK_PATH,result1[i]);
		}
		//printf("\nsecond path cost is %d\n",ans_two );
		for(int i=0;i<(int)final_result2.size();i++)
		{
			int id=final_result2[i];
			result2[i]=id;
			//printf("%d||",id );
			record_result(BACK_PATH,result2[i]);
		}
		//printf("conflict_path num is %d\n", (int)conflict_path.size());
		for(int i=0;i<(int)conflict_path.size();i++)
		{
			int id=conflict_path[i];
			//printf("%d&&", id);
		}
		//printf("\ntotal cost is %d\n",ans_one+ans_two);



		pro_end=clock();
		//printf("\n time is %ld\n",pro_end - pro_begin);
		// cout<<"time is "<<pro_end - pro_begin<<endl;
		// for (int i = 0; i < 3; i++)
		// {
		// 	record_result(WORK_PATH, result1[i]);
		// 	record_result(BACK_PATH, result2[i]);
		// }
}




