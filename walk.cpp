#include <iostream>
using namespace std;

#define ROAD_NUM 1000
#define M 100
#define N 100

struct roadDataStructblock
{
    int len;                          //记录该结构体状态，0表示空，1表示搜索阶段，大于1表示完整路径的长度
    int index;                        //记录当前的索引
    int x[10001];
    int y[10001];
};

roadDataStructblock road[ROAD_NUM];
int s[M][N];
int ridx[ROAD_NUM];

//函数的声明
int dirdetect(int x,int y,int dir,int m,int n);
int findblankroad(void* phandle);
int creatroad(void* phandle, int x,int y,int dir,int oldroad);
void delroad(void* phandle,int delroads);
int renewroad(void* phandle, int x,int y,int dir,int road);
int checkroad(void* phandle, int x,int y,int dir,int nowroad);
void checkroadend(void* phandle,int road,int edx,int edy);
void showroad(void* phandle);

int main()
{
    int m, n, stx, sty, cx, cy, croad, cindex, edx, edy, first, rdnum, end,counte = 0;

    //输入整个网格
    cin >> m >> n;
    for (int i = 0; i < m; i++)               
    {
        for (int j = 0; j < n; j++)
        {
            cin >> s[i][j];
        }
    }

    //按照要求输入开始和结尾
    cin >> stx >> sty;
    cin >> edx >> edy;
    stx--;                      //对输入的内容做预处理
    sty--;
    edx--;
    edy--;

    //初始化
    for (int i = 0; i < ROAD_NUM; i++)       
    {
        road[i].len = 0;
        road[i].index = 1;
        road[i].x[0] = stx;
        road[i].y[0] = sty;
    }
    road[0].len = 1;

    //整个处理过程
    for (int step = 0; step < 10001; step++)              //一步一步走
    {
        rdnum = 0;

        for(int i = 0; i < ROAD_NUM; i++)                 //遍历每一条路
        {
            if( road[i].len == 1)
            {
                ridx[rdnum] = i;                          //数出有哪些路正在搜索
                rdnum++;
            }
        }

        if(rdnum > 0)
        {
            for(int i = 0; i < rdnum; i++)                 //遍历每一条路
            {
                croad = ridx[i];
                cindex = road[croad].index - 1;                //初始化
                cx = road[croad].x[cindex];
                cy = road[croad].y[cindex];
                first = 0;
                end = 0;
                for(int j = 0;j < 4;j++)
                {
                    if(dirdetect(cx,cy,j,m,n) == 1)                 //判断是否更新路径
                    {
                        if(checkroad(road,cx,cy,j,croad) == 0)
                        {
                             if(first == 0)                              //判断是更新还是创建新路径
                            {
                                renewroad(road,cx,cy,j,croad);
                                checkroadend(road,croad,edx,edy);
                                first++;
                            }
                            else
                            {
                                cindex = creatroad(road,cx,cy,j,croad);
                                if(cindex >= 0)
                                {
                                    checkroadend(road,cindex,edx,edy);                                 
                                }
                            }
                            end = 0;
                        }
                        else 
                        {
                            end++;
                        }
                    }
                    else
                    {
                        end++;
                    }
                }
                if(end == 4)            //如果所有路径都无法走，直接删除该路径
                {
                    delroad(road,croad);
                }
            }
        }
        else
        {
            break;
        }
    }

    //检查是否没有一个路径符合情况
    for(int i = 0; i < ROAD_NUM; i++)                 
    {
        if( road[i].len > 1)
        {
            counte++;
        }
    }
    if(counte == 0)
    {
        cout<<-1<<endl;
    }

    //输出整个队伍
    showroad(road);
    return 0;
}

//判断路径方向
int dirdetect(int x,int y,int dir,int m,int n)
{
    int dx[4] = {0, -1, 0, 1};
    int dy[4] = {-1, 0, 1, 0};
    int nx = x + dx[dir];
    int ny = y + dy[dir];
    if((nx < 0) || (nx > m)) return 0;
    if((ny < 0) || (ny > n)) return 0;
    return (s[nx][ny]);
}

//查找空路径
int findblankroad(void* phandle)
{
    int i = -1;
    roadDataStructblock* pRd = (roadDataStructblock*)(phandle);

    for (i = 0; i < ROAD_NUM; i++)
    {
        if(pRd[i].len == 0) break;
    }
    return i;
}

//创建新路径
int creatroad(void* phandle, int x,int y,int dir,int oldroad)
{
    int newroad = -1;
    int i,lens;
    roadDataStructblock* pRd = (roadDataStructblock*)(phandle);
    int dx[4] = {0, -1, 0, 1};
    int dy[4] = {-1, 0, 1, 0};
    int nx = x + dx[dir];
    int ny = y + dy[dir];

    //搜索空路径
    newroad = findblankroad(phandle);
    if (newroad < 0) return newroad;

    //复制旧路径的数据
    lens = pRd[oldroad].index;
    for (i = 0; i < lens; i++)
    {
        pRd[newroad].x[i] = pRd[oldroad].x[i];
        pRd[newroad].y[i] = pRd[oldroad].y[i];
    }
    pRd[newroad].index = pRd[oldroad].index;

    //更新新路径的状态
    pRd[newroad].len = 1;                          //表示当前路径处于搜索阶段
    i = pRd[newroad].index - 1;
    pRd[newroad].x[i] = nx;
    pRd[newroad].y[i] = ny;

    return newroad;
}

//删除路径
void delroad(void* phandle,int delroads)
{
    roadDataStructblock* pRd = (roadDataStructblock*)(phandle);
    pRd[delroads].len = 0;
}

//更新路径
int renewroad(void* phandle, int x,int y,int dir,int road)
{
    roadDataStructblock* pRd = (roadDataStructblock*)(phandle);
    int dx[4] = {0, -1, 0, 1};
    int dy[4] = {-1, 0, 1, 0};
    int nx = x + dx[dir];
    int ny = y + dy[dir];

    //更新
    int i = pRd[road].index;
    pRd[road].x[i] = nx;
    pRd[road].y[i] = ny;
    pRd[road].index++;

    return road;
}

//判断有没有重复
int checkroad(void* phandle, int x,int y,int dir,int nowroad)
{
    roadDataStructblock* pRd = (roadDataStructblock*)(phandle);
    int dx[4] = {0, -1, 0, 1};
    int dy[4] = {-1, 0, 1, 0};
    int nx = x + dx[dir];
    int ny = y + dy[dir];
    int cindex = pRd[nowroad].index;
    int result = 0;

    //查找与之前的是否一样
    for(int i = 0; i < cindex; i++)
    {
        // if( (pRd[nowroad].x[cindex] == pRd[nowroad].x[i]) && 
        //     (pRd[nowroad].y[cindex] == pRd[nowroad].y[i]) ) 
        if( (nx == pRd[nowroad].x[i]) && 
            (ny == pRd[nowroad].y[i]) ) 
        {
            result = 1;
            break;
        }
    }  
        
    return result;
}

//判断是否结束
void checkroadend(void* phandle,int road,int edx,int edy)
{
    roadDataStructblock* pRd = (roadDataStructblock*)(phandle);
    int cindex = pRd[road].index - 1;
    if( (pRd[road].x[cindex] == edx) && 
        (pRd[road].y[cindex] == edy) )
    {
        pRd[road].len = pRd[road].index;
    } 
}

//输出函数
void showroad(void* phandle)
{
    roadDataStructblock* pRd = (roadDataStructblock*)(phandle);
    for(int i = 0; i < ROAD_NUM; i++)
    {
        if(pRd[i].len > 1)
        {
            cout<<"("<<pRd[i].x[0]+1<<","<<pRd[i].y[0]+1<<")";
            for(int j = 1;j < pRd[i].len;j++)
            {
                cout<<"->"<<"("<<pRd[i].x[j]+1<<","<<pRd[i].y[j]+1<<")";
            }
            cout<<endl;
        }
    }
}