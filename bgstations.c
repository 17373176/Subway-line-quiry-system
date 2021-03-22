#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define MAX 9999
#define N 20    /*最长站名*/
#define NUM 599 /*最大站数*/

/*---------------------------------------------------*/
/*--------------北京地铁乘坐线路查询-----------------*/
/*---------------------------------------------------*/


typedef struct{      /*车站信息*/
    char station[N]; /*站名称*/
    int change;      /*换乘状态,1为是,0为否*/
}sta;

typedef struct{      /*相邻两站信息*/
    int weight;      /*两站之间的距离为1*/
    int L;           /*两站所在的线号*/
}G;

sta vertex[NUM];     /*地铁图顶点数组*/
G graph[NUM][NUM];   /*图权重数组*/

int Vsum;            /*实际地铁站数*/
int path[NUM];       /*换乘路径*/
int s[NUM];          /*起始站到其它站最短路是否找到*/
int dis[NUM];        /*起始站到其他站的最短距离*/

int cout[NUM];       /*存储路径的栈*/
int top=-1;


/*---------------------------------------------------*/
void push(int v);
int empty();
int pop();
int search(char *s);
void create(int LINE);
void dijkstra(int V1,int V2);
void floyd(int V1,int V2);
void print(int V1,int V2);
void make();
void program();
/*---------------------------------------------------*/


void push(int v){/*入栈*/
    if(top==NUM) return;
    cout[++top]=v;
}

int empty(){/*栈是否为空*/
    if(top<0) return 1;
    return 0;
}

int pop(){/*出栈*/
    if(empty())return -1;
    return cout[top--];
}

int search(char *s){/*查找站是否存在*/
    int i;
    for(i=0;i<Vsum;i++)
        if(!strcmp(vertex[i].station,s))
            return i;
    return -1;
}

void create(int LINE){/*建图*/
    while(LINE--){
        int no,sum;            /*每条线路的编号和站数*/
        scanf("%d %d ",&no,&sum);
        printf("%d %d\n",no,sum);
        int last=-1;                         /*上一站*/
        while(sum--){            /*存储时注意站会重复*/
            char name[N];
            int is;
            scanf("%s %d ",name,&is);
            printf("%s %d\n",name,is);
            /*查找该站是否已存在*/
            int index=search(name);
            if(index==-1){                   /*不存在*/
                strcpy(vertex[Vsum].station, name);
                vertex[Vsum].change=is;
                if(last!=-1){
                    graph[last][Vsum].L=graph[Vsum][last].L=no;
                    graph[last][Vsum].weight=graph[Vsum][last].weight=1;
                }
                last=Vsum;               /*更新上一站*/
                Vsum++;                    /*更新顶点*/
            }else{                             /*存在*/
                if(last!=-1){
                    graph[last][index].L=graph[index][last].L=no;
                    graph[last][index].weight=graph[index][last].weight=1;
                }
                last=index;              /*更新上一站*/
            }
        }
    }
}

void dijkstra(int V1,int V2){
    int i;
    /*初始化*/
    for(i=0;i<Vsum;i++){
        dis[i]=graph[V1][i].weight;
        s[i]=0;
        path[i]=V1;
    }
    dis[V1]=0;
    s[V1]=1;
    int minweight,v=-1,j;
    for(i=0;i<Vsum-1;i++){
        minweight=MAX;
        /*找到未标记的最小权重值顶点 */
        for(j=0;j<Vsum;j++){
            if(!s[j]&&dis[j]<minweight){
                v=j;
                minweight=dis[v];
            }
        }
        s[v]=1;                /*标记顶点已找到最短路*/
        if(v==V2) return;          /*终点已标记则返回*/
        for(j=0;j<Vsum;j++){
            if(!s[j]&&graph[v][j].L&&minweight+graph[v][j].weight<dis[j]){
                dis[j]=minweight+graph[v][j].weight;
                path[j]=v;       /*记录路径的前驱顶点*/
            }
        }
    }
}

void floyd(int V1,int V2){
    int i,j,k;
    int p_ath[NUM][NUM];
    /*初始化路径*/
    for(i=0;i<Vsum;i++)
        for(j=0;j<Vsum;j++)
            if(i!=j&&graph[i][j].weight<MAX)
                p_ath[i][j]=i;
    for(k=0;k<Vsum;k++)
        for(i=0;i<Vsum;i++)
            for(j=0;j<Vsum;j++)
                if(graph[i][j].weight>graph[i][k].weight+graph[k][j].weight){
                    graph[i][j].weight=graph[i][k].weight+graph[k][j].weight;
                    p_ath[i][j]=p_ath[k][j];
                }
    int t=V2;
    for(;t!=V1;t=p_ath[V1][t])              /*路径追溯*/
        push(t);
}

void print(int V1,int V2){
    int last=V1;                            /*保存上一站*/
    int k=1;                               /*乘坐站数*/
    int u=pop();                             /*下一站*/
    int L=graph[last][u].L;                /*乘坐线路*/
    printf("%s-%d(",vertex[V1].station,L);
    last=u;                              /*更新上一站*/
    while(!empty()){
        u=pop();
        if(L!=graph[last][u].L){           /*需要换乘*/
            L=graph[last][u].L;
            /*打印乘坐站数和此换乘站*/
            printf("%d)-%s-%d(",k,vertex[last].station,L);
            k=0;
        }
        k++;
        last=u;                      /*更新上一站*/
    }
    printf("%d)-%s",k,vertex[V2].station);
}

void dp(int V1,int V2){
    int f[NUM]={MAX},p_ath[NUM]={V1};
    f[Vsum-1]=0;
    int i,j;
    for(i=Vsum-2;i>=0;i--){
        for(j=i+1;j<Vsum;j++){
            if(f[j]<MAX&&graph[i][j].weight+f[j]<f[i]){
                f[i]=graph[i][j].weight+f[j];
                p_ath[i]=j;
            }
        }
    }
    int t=V2;
    for(;t!=V1;t=p_ath[t])  push(t);
}

/*---------------------------------------------------*/

void make(){
    printf("\n1.开始查询\n");
    printf("2.exit\n");
    int op;
    scanf("%d",&op);
    if(op==1) program();
    else return;
}

void program(){
    int V1,V2;
    printf("请输入起始站名和终点站名，两站之间用回车分开\n");
    /*输入起始站和终点站*/
    char name_start[N],name_end[N];
    scanf("%s %s",name_start,name_end);
    V1=search(name_start);
    V2=search(name_end);
/*---------------------------------------------------*/
    /*查询换乘*/
    dijkstra(V1,V2);
    int t=V2;
    for(;t!=V1;t=path[t])  push(t);       /*路径追溯*/
    //floyd(V1,V2);
    //dp(V1,V2);
    /*打印结果*/
    print(V1,V2);
    make();
}

int main(){

    printf("北京地铁乘坐线路查询系统\n");
/*---------------------------------------------------*/

/*---------------------------------------------------*/
    freopen("bgstations.txt","r",stdin);
    int LINE;                            /*地铁总线数*/
    scanf("%d ",&LINE);
    int i,j;
    /*初始化*/
    for(i=0;i<NUM;i++)
        for(j=0;j<NUM;j++){
            graph[i][j].weight=graph[j][i].weight=MAX;
            graph[i][j].L=graph[j][i].L=0;
        }
/*---------------------------------------------------*/
    /*建图*/
    create(LINE);
    freopen("CON", "r", stdin);
/*--------------------------------------------------*/
    /*查找起始站与终点站对应的顶点编号*/
    make();
    system("pause");
    return 0;
}
