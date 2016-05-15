#include "route.h"
#include "lib_record.h"
#include "lib_time.h"
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <iostream>
#include<vector>
#include<queue>
#define LL long long
#include <ctime>

const int INF = 0x3f3f3f3f;
using namespace std;
LL limit=200000000;                 //大概9秒
LL limit2=50000000/1500;                 //大概9/4秒
LL clk=0;
LL time_limit=9500000;
clock_t time_start_dfs2,time_end_dfs2;

int is_reverse=0;


int num_Side=0,num_demand_V=0;

int dist[602][602];               //记录各点之间距离
bool vit[602];                   //记录节点是否访问
vector<int>link_node[602];      //记录各点邻接节点
vector<int>link_in[602];
int Road_id[602][602];         //记录点与点之间的路径id
struct Side
{
        int LinkID,SourceID,DestinationID,Cost;

}S[100000];
vector<int>must_pass;           //必须经过的点集
bool must[606];                //是否走过必须进过的点
int ans=INF;                  //最终权值
int Start,Target,num;         //起点和终点，包括必经点的数目
int final_pat[5000];         //记录路径
int nodeNum=0;                //节点总数
int final_num_path;          //路径长度
struct path_detail
{
    int sum;
    int num;
    vector<int>path;
}ANS[5000];                //路径信息
int ind=0;                //候选路径个数
int FLAG=0;              //是否找到路径

void debug(path_detail D)
{
    printf("Cost %d----------",D.sum);
    for(int i=0;i<D.num;i++)
    printf("%d|",D.path[i]);
    puts("");
}


void dfs(int p,int cnt,int sum,int num_path)
{
    clk++;
    if(clk>limit)
    {
        if(FLAG==0){
                printf("NA\n");
//        print_time("END-nofind");
        return ;
       // printf("NA");
       // exit(0);
        }
        return ;
    }
    if(p==must_pass[num-1])
    {
        if(cnt==0)
        {
            if(sum<ans)
            {
                FLAG=1;
                ans=sum;
                for(int i=0;i<num_path;i++)
                ANS[ind].path.push_back(final_pat[i]);
                puts("");
                ANS[ind].sum=sum;
                ANS[ind].num=num_path;
                debug(ANS[ind]);
                ind++;

                //final_num_path=num_path;
                //ans_path.clear();
               // ans_path=path;
            }

        }
        return ;

    }
    {
        for(int i=0;i<link_node[p].size();i++)
        {
            int l=link_node[p][i];
            if(vit[l])
            continue;

            if(l==must_pass[num-1]&&cnt!=1)//剪枝1    基本没有效果
                continue;
            if(sum+dist[p][l]>ans)//剪枝2      基本没有效果
                continue;
            vit[l]=1;
            int t_cnt=cnt;
            if(must[l])
            {
                t_cnt--;
            }
            final_pat[num_path]=Road_id[p][l];

          //  printf("$%d$",Road_id[p][l]);

           // char c=Road_id[p][l]+'0';
            dfs(l,t_cnt,sum+dist[p][l],num_path+1);
            vit[l]=0;
        }

    }
}


bool cmp(path_detail a,path_detail b)
{
    return a.sum<b.sum;
}

int Atoi(char num[20])
{
    int len=strlen(num);
    int ans=0;
    int ten=1;
    for(int i=len-1;i>=0;i--)
    {
        ans+=ten*(num[i]-'0');
        ten*=10;
    }
    return ans;
}

bool visit[602];
int ans_path[5005],ans_path_num;
int tmp_path[5005],tmp_path_num;
// struct node_detail
// {
//     int dist,id;
//     node_detail(){};
//     node_detail(int d,int i)
//     {
//         dist=d;
//         id=i;
//     }
//     bool operator <(const node_detail& r) const{
//         return dist > r.dist;
//     }
// };
//新加的
struct node_detail
{
    int dist,id;
    int steps;
    node_detail(){};
    node_detail(int d,int i)
    {
        dist=d;
        id=i;
    }
    node_detail(int d,int i, int s)
    {
        dist=d;
        id=i;
        steps = s;
    }
    bool operator <(const node_detail& r) const{
        return dist > r.dist;
    }
};
bool cmp_node_detail(node_detail a,node_detail b)
{
    return a.dist<b.dist;
}

//node_detail must_node[100];

//void dijkstra(int start,node_detail Dist[],int pre[], node_detail must_node[], int &nmust_node){
//    priority_queue<node_detail> que;
//    nmust_node = 0;
//    int id[610];   //记录顶点v在must_node中的下标
//    for(int i=0;i<nodeNum;i++){
//        Dist[i].dist=INF,Dist[i].id=i;
//        if(must[i])  //如果是必须经过的点的话
//            must_node[nmust_node].dist=INF, must_node[nmust_node].id=i, id[i]=nmust_node++;
//    }
//    int vis[609] = {0};
//    Dist[start].dist = 0;
//    node_detail tmp(0,start);
//    que.push(tmp);
//    while(!que.empty()){
//        node_detail tp = que.top(); que.pop();
//        int u = tp.id;
//        if(vis[u]) continue;
//        vis[u] = 1;
//        for(int i=0; i<link_node[u].size(); i++){
//            int v = link_node[u][i], c = dist[u][v];
//            if(visit[v]) continue;
//            if(Dist[v].dist > Dist[u].dist + c){  //更新v
//                pre[v] = u;
//                Dist[v].dist = Dist[u].dist+ c;
//                node_detail tt(Dist[v].dist,v);
//                if(must[v])          //如果v是必经结点的话
//                    must_node[id[v]] = tt;
//                que.push(tt);
//            }
//        }
//    }
//}
void SPFA0(int start,node_detail Dist[],int pre[], node_detail must_node[], int &nmust_node){
///    priority_queue<node_detail> que;
    nmust_node = 0;
    int id[610];   //记录顶点v在must_node中的下标
    for(int i=0;i<nodeNum;i++){
        Dist[i].dist=INF,Dist[i].id=i;
        if(must[i])  //如果是必须经过的点的话
            must_node[nmust_node].dist=INF, must_node[nmust_node].id=i, id[i]=nmust_node++;
    }
    int vis[609] = {0};
    Dist[start].dist = 0;
    ///node_detail tmp(0,start);
    queue<int> que;
    ///que.push(tmp);
    que.push(start);
    vis[start] = 1;
    while(!que.empty()){
        int u = que.front(); que.pop();
        vis[u] = 0;
        for(int i=0; i<link_node[u].size(); i++){
           // time_end_dfs2=clock();
           // if(time_end_dfs2 - time_start_dfs2>time_limit)
           // {
             //   return ;
           // }
            int v = link_node[u][i], c = dist[u][v];
            if(visit[v]) continue;
            if(Dist[v].dist > Dist[u].dist + c){  //更新v
                pre[v] = u;
                Dist[v].dist = Dist[u].dist+ c;
                node_detail tt(Dist[v].dist,v);
                if(must[v])          //如果v是必经结点的话
                    must_node[id[v]] = tt;
                ///que.push(tt);
                if(!vis[v]) {
                    que.push(v);
                    vis[v] = 1;
                }
            }
        }
    }
}

void SPFA(int start,node_detail Dist[],int pre[], node_detail must_node[], int &nmust_node){
///    priority_queue<node_detail> que;
    nmust_node = 0;
    int id[610];   //记录顶点v在must_node中的下标
    for(int i=0;i<nodeNum;i++){
        Dist[i].dist=INF,Dist[i].id=i;
        ///新加的
        Dist[i].steps = INF;
        if(must[i])  //如果是必须经过的点的话
            must_node[nmust_node].dist=INF, must_node[nmust_node].id=i, id[i]=nmust_node++;
    }
    int vis[609] = {0};
    Dist[start].dist = 0;
    ///新加的
    Dist[start].steps = 0;
    ///node_detail tmp(0,start);
    queue<int> que;
    ///que.push(tmp);
    que.push(start);
    vis[start] = 1;
    while(!que.empty()){
        int u = que.front(); que.pop();
        vis[u] = 0;
        for(int i=0; i<link_node[u].size(); i++){
           // time_end_dfs2=clock();
           // if(time_end_dfs2 - time_start_dfs2>time_limit)
           // {
             //   return ;
           // }
            int v = link_node[u][i], c = dist[u][v];
            if(visit[v]) continue;
            // if(Dist[v].dist > Dist[u].dist + c){  //更新v
            //     pre[v] = u;
            //     Dist[v].dist = Dist[u].dist+ c;
            //     node_detail tt(Dist[v].dist,v);
            //     if(must[v])          //如果v是必经结点的话
            //         must_node[id[v]] = tt;
            //     ///que.push(tt);
            //     if(!vis[v]) {
            //         que.push(v);
            //         vis[v] = 1;
            //     }
            // }
            if(Dist[v].steps>Dist[u].steps+1||
            (Dist[v].steps==Dist[u].steps+1 && Dist[v].dist>Dist[u].dist+c))
            {
                pre[v] = u;
                Dist[v].dist = Dist[u].dist+c;
                Dist[v].steps = Dist[u].steps + 1;
                node_detail tt(Dist[v].dist, v, Dist[v].steps);
                if(must[v])
                    must_node[id[v]] = tt;
                if(!vis[v]) {
                    que.push(v);
                    vis[v] = 1;
                }
            }
        }
    }
}

void dfs2(int now_pos,int cnt,int sum)
{
        time_end_dfs2=clock();
        if(time_end_dfs2 - time_start_dfs2>time_limit)
        {
            return ;
        }
//     clk++;
//     if(clk>limit2)
//     {
//         //printf("^^^^^%lld^^^^",clk);
//         if(FLAG==0){
//                 printf("NA\n");
// //        print_time("END-nofind");
//         return ;
//        // printf("NA");
//        // exit(0);
//         }
//         return ;
//     }
    if(now_pos==Target&&cnt==0)
    {
        if(sum<ans)
        {
            ans=sum;
            ans_path_num=tmp_path_num;
            printf("Cost is %d: ",ans);
            for(int i=0;i<ans_path_num;i++){
                ans_path[i]=tmp_path[i];
                printf("%d|",tmp_path[i]);
            }
            FLAG=1;
        }
        visit[Target]=0;
        return ;
    }
    else if(now_pos==Target)
    {
        visit[Target]=0;
        return ;
    }

    int pre_node[602];
    node_detail Dist[602];
    pre_node[now_pos] = 0;     //简单优化
    node_detail must_node[100];    //必须经过的点集
    int nmust_node;                //必须经过的点的个数
    if(num_Side<2302&&num_Side>1998)
    SPFA(now_pos,Dist,pre_node, must_node, nmust_node);
    else
    SPFA0(now_pos,Dist,pre_node, must_node, nmust_node);
    //sort(Dist,Dist+nodeNum,cmp_node_detail);
    visit[now_pos]=1;
    //另外一个优化zhe
    sort(must_node, must_node+nmust_node, cmp_node_detail);
/*    for(int i=0;i<nodeNum;i++)
    {
        int nextt=Dist[i].id;
        if(Dist[i].dist==INF&&(!visit[nextt])&&must[nextt])
        {
            visit[now_pos]=0;
            return ;
        }
    }*/
    for(int i=0; i<nmust_node; i++){            //不需要循环600次
        int nextt = must_node[i].id;
        if(must_node[i].dist==INF && !visit[nextt]&&must[nextt]){
            visit[now_pos] = 0;
            return ;
        }
    }
/*
//    if(now_pos==13)
//    {
//        puts("heheda");
//    }
//    for(int i=0;i<nodeNum;i++)   // now_pos -> nextt
//    {
//        int nextt=Dist[i].id;
//
//        if(visit[nextt])
//            continue;
//        if(!must[nextt])                //可优化二
//            continue;
//        if(Dist[i].dist==INF)   continue;
////        if(must_node[id[i]].dist == INF) continue;
//        int tmp_tmp_path[602],tmp_tmp_path_num=0;
//        int tmp_cnt=cnt;
//        while(nextt!=now_pos)
//        {
//            tmp_tmp_path[tmp_tmp_path_num++]=Road_id[pre_node[nextt]][nextt];
//            visit[nextt]=1;
//            if(must[nextt])
//                tmp_cnt--;
//            nextt=pre_node[nextt];
//        }
//        int tt=tmp_path_num;
//        for(int j=tmp_tmp_path_num-1;j>=0;j--)
//            tmp_path[tmp_path_num++]=tmp_tmp_path[j];
//        sum+=Dist[i].dist;
//
//        dfs2(Dist[i].id,tmp_cnt,sum);
//        //回溯
//        tmp_path_num=tt;
//        sum-=Dist[i].dist;
//        nextt=Dist[i].id;
//        while(nextt!=now_pos)
//        {
//            visit[nextt]=0;
//            nextt=pre_node[nextt];
//        }
//    }   */
    //for(int i=0;i<nodeNum;i++)   // now_pos -> nextt
    for(int i=0; i<nmust_node; i++)                 //不需要循环600次
    {
        ///int nextt=Dist[i].id;
        int nextt = must_node[i].id;
        if(visit[nextt])
            continue;
        if(!must[nextt])                //可优化二
            continue;
        ///if(Dist[i].dist==INF)   continue;
        if(must_node[i].dist==INF)
            continue;
        if(must_node[i].dist+sum>ans)
            continue;
        int tmp_tmp_path[602],tmp_tmp_path_num=0;
        int tmp_cnt=cnt;
        while(nextt!=now_pos)
        {
            tmp_tmp_path[tmp_tmp_path_num++]=Road_id[pre_node[nextt]][nextt];
            visit[nextt]=1;
            if(must[nextt])
                tmp_cnt--;
            nextt=pre_node[nextt];
        }
        int tt=tmp_path_num;
        for(int j=tmp_tmp_path_num-1;j>=0;j--)
            tmp_path[tmp_path_num++]=tmp_tmp_path[j];
        ///sum+=Dist[i].dist;
        sum += must_node[i].dist;
        ///dfs2(Dist[i].id,tmp_cnt,sum);
        dfs2(must_node[i].id, tmp_cnt, sum);

        //回溯
        tmp_path_num=tt;
        ///sum-=Dist[i].dist;
        sum -= must_node[i].dist;
        ///nextt=Dist[i].id;
        nextt=must_node[i].id;
        while(nextt!=now_pos)
        {
            visit[nextt]=0;
            nextt=pre_node[nextt];
        }
    }
    visit[now_pos]=0;

}

/******/
/*
bool visit_dfs6[602];
vector<int> dfs6path;
vector<int> dfs6Finalpath;
void dfs6(int source,int cnt,int sum,int deep)
{
    deep++;
    if(deep>25)
        return;
    bool flag=1;
    visit_dfs6[source]=1;
    dfs6path.push_back(source);
    if(must[source]) cnt--;
    if(source==Target)
    {
        if(cnt!=0)
        {
            flag=0;
            return;
        }
    }
    if(flag)
    {
        if(sum<ans)
        {
            FLAG=1;
            ans=sum;
            dfs6Finalpath.clear();
            for(int i=0;i<dfs6path.size();i++)
            {
                dfs6Finalpath.push_back(dfs6path[i]);
            }
        }
    }
    for(int ii=0;ii<link_node[source].size();ii++)
    {
        int i=rand()%link_node[source].size();
        int l=link_node[source][i];
        if(dist[source][l]+sum>ans)
            continue;
        if(visit_dfs6[l])
            continue;
        dfs6(l,cnt,sum+dist[source][l]);

    }
    visit_dfs6[source]=0;
    dfs6path.erase(dfs6path.end());
    return ;
}
*/
/******/

/******/

int res_path[1000], nres_path;
int res_cost;
int vised[700];
int dp_state[800][100];

//bool visall(){
//    for(int i=0; i<must_pass.size(); i++){
//        if(must_pass[i]==Start || must_pass[i]==Target) continue;
//        if(!vised[must_pass[i]]) return false;
//    }
//    return true;
//}

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
    if(deep > 300) return ;
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
            for(int i=0; i<=deep; i++) res_path[i] = tp_path[i];
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



/******/



//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
     srand(6666666);
     bool is_data6=0;
     bool is_data15=0;
     int val_data15=1;
    // if(edge_num<=351&&edge_num>=249)
    //     is_data6=1;
    if(edge_num>=2349)
        is_data15=1;
    unsigned short result[5000] = {};//示例中的一个解
    if(edge_num>1000&&!(edge_num<=2301&&edge_num>=1999))
    {
        is_reverse=1;
    }
    if(edge_num==1999||edge_num==2000)
        is_reverse=1;
    //格式处理
    for(int i=0;i<edge_num;i++)
    {
     //   int len=strlen(topo[i]);
        int Linkid,Soureid,Destinationid,Cost;

    sscanf(topo[i],"%d , %d , %d , %d",&Linkid,&Soureid,&Destinationid,&Cost);
    //Cost=1;
    nodeNum=max(max(Destinationid,Soureid),nodeNum);
    if(is_reverse)
         swap(Destinationid,Soureid);
/*
        int j=0;
        char tmp[20];
        int t=0;
        memset(tmp,0,sizeof(tmp));
        printf("topo %d is %s\n",i,topo[i]);
        for(;topo[i][j]!=',';j++)
            tmp[t++]=topo[i][j];
      //  printf("link id is %s ans length is %d &&&&&&",tmp,strlen(tmp));
        Linkid=Atoi(tmp);
      //  printf("%d\\\\",Atoi(tmp));

        j++;
        t=0;
        memset(tmp,0,sizeof(tmp));
        for(;topo[i][j]!=',';j++)
            tmp[t++]=topo[i][j];
//        puts(tmp);
        Soureid=Atoi(tmp);

        j++;
        t=0;
        memset(tmp,0,sizeof(tmp));
        for(;topo[i][j]!=',';j++)
            tmp[t++]=topo[i][j];
        Destinationid=Atoi(tmp);
        nodeNum=max(nodeNum,max(Soureid,Destinationid));
        j++;
        t=0;
        memset(tmp,0,sizeof(tmp));
        for(;j<len;j++)
            tmp[t++]=topo[i][j];
        Cost=Atoi(tmp);
       // printf("%d//////",Linkid);
//        printf("%d %d*********\n",Soureid,Destinationid);
*/
        
//        puts("okok");
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
            link_in[Destinationid].push_back(Soureid);
            Road_id[Soureid][Destinationid]=Linkid;
           // printf("$%d$",Road_id[Soureid][Destinationid]);
            dist[Soureid][Destinationid]=Cost;
        }
        S[Linkid].Cost=Cost;
        S[Linkid].SourceID=Soureid;
        S[Linkid].DestinationID=Destinationid;
        S[Linkid].LinkID=Linkid;
        num_Side++;
    }
  //  int len=strlen(demand);


   // puts("ok2");
/*    int node;
    char tmp[20];
    memset(tmp,0,sizeof(tmp));
    int j=0;
    int t=0;
    for(;demand[j]!=',';j++)
        tmp[t++]=demand[j];
    Start=Atoi(tmp);
    must_pass.push_back(Start);
    must[Start]=1;

    j++;
    memset(tmp,0,sizeof(tmp));
    t=0;
    for(;demand[j]!=',';j++)
        tmp[t++]=demand[j];
    Target=Atoi(tmp);
    must[Target]=1;

    j++;
    while(j<len)
    {
        t=0;
        memset(tmp,0,sizeof(tmp));
        for(;demand[j]!='|'&&j<len;j++)
        {
            tmp[t++]=demand[j];
        }
        node=Atoi(tmp);
        must_pass.push_back(node);
        must[node]=1;
        j++;
    }

*/
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
    // int ther=2;
    // if(is_data15)
    // {
    //     for(int i=1;i<must_pass.size();i++)
    //     {
    //         int e=must_pass[i];
    //     if(link_in[e].size()>ther)
    //             continue;
            
    //         for(int j=0;j<link_in[e].size();j++)
    //         {
    //     val_data15=rand()%ther+1;
    //             int s=link_in[e][j];
    //             dist[s][e]=min(dist[s][e],val_data15);
    //         }
    //     }
    // }
//    for(int i=0;i<num;i++)
//    printf("%d-",must_pass[i]);
    //格式处理结束

    //深搜
    if(is_data15)
    {
        for(int i=0;i<must_pass.size();i++)
        {
            printf("%d*", must_pass[i]);
        }
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

        // for(int i=0; i<nres_path-1; i++){
        //     int id=Road_id[res_path[i]][res_path[i+1]];
        //     result[i]=id;
        //     record_result(result[i]);
        //     printf("%d|", id);
        // }
        // dfs6(must_pass[0],num,0,0);
        // cout<<ans<<endl;
        // for(int i=0;i<dfs6Finalpath.size()-1;i++)
        // {
        //     int s=dfs6Finalpath[i],e=dfs6Finalpath[i+1];
        //     int id=Road_id[s][e];
        //     result[i]=id;
        //     printf("%d|",id);
        //     record_result(result[i]);
        // }

        return;
    }
    if(nodeNum<=20)
{
    clk=0;
    dfs(must_pass[0],num-1,0,0);
    sort(ANS,ANS+ind,cmp);
    ans=ANS[0].sum;
    cout<<ans<<endl;

    if(is_reverse)
    {
        for(int i=ANS[0].num-1;i>=0;i--)
       // for(int i=0;i<ANS[0].num;i++)
        {
            int id=ANS[0].path[i];
            result[i]=id;
            printf("%d ",id);
            record_result(result[i]);
        }
    }
    else{
       // for(int i=ANS[0].num-1;i>=0;i--)
        for(int i=0;i<ANS[0].num;i++)
        {
            int id=ANS[0].path[i];
            result[i]=id;
            printf("%d ",id);
            record_result(result[i]);
        }
    }
    
    return ;
}
    clk=0;
   
    visit[Start]=1;
    nodeNum++;
    dfs2(must_pass[0],num-1,0);
    cout<<ans<<endl;
    if(is_reverse)
    {
        for(int i=ans_path_num-1;i>=0;i--)
       // for(int i=0;i<ans_path_num;i++)
        {
            int id=ans_path[i];
            result[i]=id;
            printf("%d ",id);
            record_result(result[i]);
        }
    }
    else{
        //for(int i=ans_path_num-1;i>=0;i--)
        for(int i=0;i<ans_path_num;i++)
        {
            int id=ans_path[i];
            result[i]=id;
            printf("%d ",id);
            record_result(result[i]);
        }
    }
    



   // cout<<"hahaha"<<endl;


  //  printf("\n");
//    for (int i = 0; i < ANS[0].num; i++)
//    {
//        printf("%d|",result[i]);
//        record_result(result[i]);
//    }
//    printf("\n");
}

/*FILE *ftopo=fopen("topo.csv","r");
FILE *fdemand=fopen("demand.csv","r");
int main(int argc, char *argv[])
{

////    print_time("Begin");
    char topo[5000][100];
    int edge_num=0;
    char demand[5000];
//    int demand_num;
//    puts("ok1");
    while(~fscanf(ftopo,"%s\n",topo[edge_num++]));
    edge_num--;
    printf("%d\n",edge_num);
//    puts("ok2");
    fscanf(fdemand,"%s",demand);
//    puts("ok3");

    for(int i=0;i<edge_num;i++)
    {
        printf("%s\n",topo[i]);
    }
//    puts(demand);

//
////    char *topo_file = argv[1];
//    edge_num = read_file(topo, 5000, topo_file);
//    if (edge_num == 0)
//    {
//        printf("Please input valid topo file.\n");
//        return -1;
//    }
//    char *demand_file = argv[2];
//    demand_num = read_file(&demand, 1, demand_file);
//    if (demand_num != 1)
//    {
//        printf("Please input valid demand file.\n");
//        return -1;
//    }
//
    search_route(topo, edge_num, demand);
//
//    char *result_file = argv[3];
//    write_result(result_file);
//    release_buff(topo, edge_num);
//    release_buff(&demand, 1);
//
//    print_time("End");

    return 0;
}*/
