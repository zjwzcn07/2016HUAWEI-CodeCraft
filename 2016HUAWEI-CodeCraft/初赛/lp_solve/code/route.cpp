#include "stdio.h"
#include "glpk.h"
#include "route.h"
#include "lib_record.h"
#include "lib_time.h"
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <set>

#define LL long long
#include <ctime>
using namespace std;
const int maxn = 10000000 + 100;
const int INF = 0x3f3f3f3f;

LL time_limit=9500000;
clock_t time_start_dfs2,time_end_dfs2;

int ia[maxn], ja[maxn];
int routid[605][605], z = 0;
double ar[maxn];
bool must[605];
int must_num = 0;
vector<int>must_pass;
int nodeNum ;
int Start;
int Target ;
int res[5005], nres;    //记录结果
struct edge { int u, v, cost, id; };
vector<edge> edges;
vector<int> G[605];   //点所连接的边
bool choose[5000+100];

//void get_res(int u){
//    if(u == Target) return;
//    int tp;
//    for(int i=0; i<G[u].size(); i++){
//        int ed = G[u][i];
//        if(!choose[ed]) continue;
//        tp = ed;
//        break;
//    }
//    int v = edges[tp].v;
//    get_res(v);
//    res[nres++] = tp;
//}

bool visit[700];    //找环的判重数组
bool find_ring;
int ring[700], nring;
void Find(int u){
    if(u == Target){
        visit[u] = true;
        return ;
    }else if(visit[u]){
        find_ring = true;
        return ;
    }
    visit[u] = true;
    int cned = -2;
    for(int i=0; i<G[u].size(); i++){
        int ed = G[u][i];
        if(!choose[ed]) continue;
        cned = ed;
        break;
    }
    int v = edges[cned].v;
    Find(v);
    ring[nring++] = cned;
}

void glpk_solve(){
    glp_prob *lp;
    lp = glp_create_prob();
    glp_set_obj_dir(lp, GLP_MIN);
    for(int i=1; i<=nodeNum; i++){  //点的出度-入度
        int tp = glp_add_rows(lp, 1);
        if(i == Start)
            glp_set_row_bnds(lp, tp, GLP_FX, 1, 1);
        else if(i == Target)
            glp_set_row_bnds(lp, tp, GLP_FX, -1, -1);
        else
            glp_set_row_bnds(lp, tp, GLP_FX, 0, 0);
    }
    for(int u=1; u<=nodeNum; u++){   //限制顶点出度
        int tp = glp_add_rows(lp, 1);
        if(must[u]||u==Start)
            glp_set_row_bnds(lp, tp, GLP_FX, 1, 1);
        else if(u == Target)
            glp_set_row_bnds(lp, tp, GLP_FX, 0, 0);
        else {
            glp_set_row_bnds(lp, tp, GLP_DB, 0, 1);
        }
    }
    for(int i=1; i<edges.size(); i++){   //设置变量类型以及权值
        int tp = glp_add_cols(lp, 1);
        glp_set_obj_coef(lp, tp, edges[i].cost);
        glp_set_col_kind(lp, tp, GLP_BV);
    }
    //出度 - 入度的系数矩阵
    int idx = 1;
    for(int i=1; i<=nodeNum; i++){
        for(int j=1; j<edges.size(); j++){
            if(edges[j].u == i)  //出度
            {
                ia[idx] = i; ja[idx] = j;
                ar[idx++] = 1;
            }else if(edges[j].v == i){   //入度
                ia[idx] = i; ja[idx] = j;
                ar[idx++] = -1;
            }
        }
    }
    //出度系数矩阵
    for(int i=nodeNum+1; i<=2*nodeNum; i++)
        for(int j=1; j<edges.size(); j++){
            if(edges[j].u==i-nodeNum)  //出度
            {
                ia[idx] = i; ja[idx] = j;
                ar[idx++] = 1;
            }
        }
    int choose_num;
    while(1){
        printf("idx-1 = %d\n", idx-1);
        glp_load_matrix(lp, idx-1, ia, ja, ar);
        glp_iocp parm;
        glp_init_iocp(&parm);
        parm.presolve = GLP_ON;
        glp_intopt(lp, &parm);
        z = glp_mip_obj_val(lp);
        printf("z = %d \n", z);
        memset(choose, 0, sizeof(choose));
        set<int> rep;
        rep.clear();
        int pt[700], npt=0;
        choose_num = 0;
        for(int i=1; i<edges.size(); i++){
            int x = glp_mip_col_val(lp, i);
            if(x){
                printf("edge id is %d from %d to %d *****\n", i,edges[i].u,edges[i].v);
                choose[i] = 1;                                  //必要路经过的点
                int u = edges[i].u, v = edges[i].v;
                if(rep.find(u) ==rep.end()) pt[npt++] = u, rep.insert(u);
                if(rep.find(v) ==rep.end()) pt[npt++] = v, rep.insert(v);
                choose_num++;                                   //必经路的个数
            }
        }
//        for(int i=1; i<edges.size(); i++)
//            printf("%d ", choose[i]);
//        printf("\n");
        memset(visit, 0, sizeof(visit));
        find_ring = false;
        nring = 0;
        Find(Start);
        bool has = false;                                   //开始找环
        for(int i=0; i<npt; i++){
            if(visit[pt[i]]) continue;
            find_ring = false;
            nring = 0;
            Find(pt[i]);
//            if(find_ring) {
//                for(int i=nring-1; i>=0; i--) printf("%d-", ring[i]);
//                printf("\n");
//            }
            has = has || find_ring;
            if(find_ring){   //如果找到了环增加约束条件
                int tp = glp_add_rows(lp, 1);
                for(int i=0; i<nring; i++){
                    ia[idx]=tp; ja[idx]=ring[i];
                    ar[idx++] = 1;
                }
                glp_set_row_bnds(lp, tp, GLP_UP, nring-1, nring-1);
            }
        }
        if(!has) {
            break;
        }
    }
    //打印路径
    nres = 0;
    int now = Start;
    while(nres<choose_num){
        int tped;
        for(int i=0; i<G[now].size(); i++){
            int ed = G[now][i];
            if(!choose[ed]) continue;
            tped = ed;
            break;
        }
        now = edges[tped].v;
        res[nres++] = tped-1;
    }
    for(int i=0; i<nres; i++) printf("%d-", res[i]);
    printf("\n");
}


/////////////////
int dist[602][602];               //记录各点之间距离
vector<int>link_node[602];      //记录各点邻接节点
int Road_id[602][602];         //记录点与点之间的路径id
int num;
bool vit[602];                   //记录节点是否访问
int res_path[1000], nres_path;
int res_cost;
int vised[700];
int dp_state[800][100];

bool cut(int u, int cost){
    if(cost >= res_cost) return true;     //减掉
    int num = 0;
    for(int i=0; i<must_pass.size(); i++){
        if(must_pass[i]==Start || must_pass[i]==Target) continue;
        if(vised[must_pass[i]]) num++;
    }
    if(dp_state[u][num] == 0){
        dp_state[u][num] = cost;
        return false;
    }else{
        if(dp_state[u][num]>cost){
            dp_state[u][num] = cost;
            return false;  //刷新
        }else return true;
    }
}

int tp_path[1000];
void dfs15(int u, int cost, int deep, int visednum)   //当前在u点 路径话费为cost 搜索深度为deep 经过visednum个必经点
{
    time_end_dfs2=clock();
    if(time_end_dfs2 - time_start_dfs2>time_limit)
    {
        return ;
    }
    if(deep > 150) return ;
    vised[u] = true;
    tp_path[deep] = u;
    int tp_visednum = visednum;
    if(must[u] && u!=Start && u!=Target) tp_visednum++;   //如果是必经点的话tp_visednum+1

    //预留剪枝
    if(cut(u, cost))
    {
        vised[u] = false;
        return ;
    }
    if(u==Target){   //找到了一条路径
        if(tp_visednum==num-2&&cost<res_cost){
            for(int i=0; i<=deep; i++) 
        {
               printf("%d|",tp_path[i]);
               res_path[i] = tp_path[i];
            }
            puts("");
            nres_path = deep+1;
            res_cost = cost;
        }
        vised[u] = 0;
        return ;
    }
    for(int i=0; i<link_node[u].size(); i++){
        int v = link_node[u][i];
        if(v==Target && tp_visednum!=num-2) continue;
        if(vised[v]) continue;
        dfs15(v, cost+dist[u][v], deep+1, tp_visednum);
    }
    vised[u] = false;
}

bool is_reverse;

/////////////////





void search_route(char *topo[5000], int edge_num, char *demand)
{
	unsigned short result[5000] = {};//示例中的一个解

    if(edge_num>=2349&&edge_num<=2371)
    {
        is_reverse=1;
        for(int i=0;i<edge_num;i++)
        {
         //   int len=strlen(topo[i]);
            int Linkid,Soureid,Destinationid,Cost;

            sscanf(topo[i],"%d , %d , %d , %d",&Linkid,&Soureid,&Destinationid,&Cost);
            //Cost=1;
            nodeNum=max(max(Destinationid,Soureid),nodeNum);
            if(is_reverse)
            swap(Destinationid,Soureid);
            if(dist[Soureid][Destinationid]!=0)
            {
                if(dist[Soureid][Destinationid]>Cost)
                {
                    Road_id[Soureid][Destinationid]=Linkid;


                    dist[Soureid][Destinationid]=Cost;
                }
            }
            else
            {
                link_node[Soureid].push_back(Destinationid);
                Road_id[Soureid][Destinationid]=Linkid;
               // printf("$%d$",Road_id[Soureid][Destinationid]);
                dist[Soureid][Destinationid]=Cost;
            }
        }

        int x=0,p=0;
        bool has=1;
        while(1)
        {
           if(demand[p]<'0'||demand[p]>'9')
           {
               if(has)
               {
                   must_pass.push_back(x);
                   must[x]=1;
               }
           x=0;
               has=0;
           }
           else{
               x=x*10+demand[p]-'0';
               has=1;
           }
           if(demand[p]==0)
           {
               break;
           }
           ++p;
        }



        if(is_reverse)
            swap(must_pass[0],must_pass[1]);
        Start=must_pass[0];
        Target=must_pass[1];
        swap(must_pass[1],must_pass[must_pass.size()-1]);
       // must_pass.push_back(Target);
        vit[Start]=1;
        num=must_pass.size();
        time_start_dfs2=clock();  
      //  for(int i=0;i<must_pass.size();i++)
      //  {
      //      printf("%d*", must_pass[i]);
      //  }
        puts("");

        memset(vised, 0, sizeof(vised));
        res_cost = 0x3f3f3f3f;
        dfs15(Start, 0, 0, 0);
        printf("weight = %d\n", res_cost);
        for(int i=nres_path-1;i>0;i--)
        {
            int id=Road_id[res_path[i-1]][res_path[i]];
            result[i]=id;
            record_result(result[i]);
            printf("%d|", id);
        }

        return;
    }

									 //格式处理
    edges.push_back((edge){0, 0, 0});
	for (int i = 0;i<edge_num;i++)
	{
		//   int len=strlen(topo[i]);
		int Linkid, Soureid, Destinationid, Cost;

		sscanf(topo[i], "%d,%d,%d,%d", &Linkid, &Soureid, &Destinationid, &Cost);
		Linkid++;
		Soureid++;
		Destinationid++;
		nodeNum = max(max(Destinationid, Soureid), nodeNum);
        edges.push_back((edge){Soureid, Destinationid, Cost, Linkid});
        G[Soureid].push_back(edges.size()-1);
	}
	//  int len=strlen(demand);
	int x = 0, p = 0;
	bool has = 1;
	while (1)
	{
		if (demand[p]<'0' || demand[p]>'9')
		{
			if (has)
			{
				x++;
				must_pass.push_back(x);
				must[x] = 1;
			}
			x = 0;
			has = 0;
		}
		else {
			x = x * 10 + demand[p] - '0';
			has = 1;
		}
		if (demand[p] == 0)
		{
			break;
		}
		++p;
	}
	Start = must_pass[0];
	Target = must_pass[1];
	swap(must_pass[1], must_pass[must_pass.size() - 1]);
	must[Start] = must[Target] = 0;
	// must_pass.push_back(Target);
	must_num = must_pass.size()-2;
	//    for(int i=0;i<num;i++)
	//    printf("%d-",must_pass[i]);
	//格式处理结束
	printf("nodeNum is %d, SideNum is %d\n", nodeNum,edge_num);
	glpk_solve();
	for (int i = 0;i<nres;i++)
	{
		int id = res[i];
		result[i] = id;
		printf("%d ", id);
		record_result(result[i]);
	}
}
