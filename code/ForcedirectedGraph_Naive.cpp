#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <utility>
#include <fstream>
#include <iomanip>
using namespace std;
const int vertexNum = 686;
const int topK = 680;
const double G = 100;
const double k = 1;
const double PI = 3.1415926535;

int graph[vertexNum][vertexNum];
double delta = 0.01;
struct node
{
    //(400~600)随机初始化节点坐标
    double x = 500 + (rand()%201 - 100);
    double y = 500 + (rand()%201 - 100);
};

/*K近邻*/
node topNodes[topK];
node v[vertexNum];

double tension(double r)
{
    if(r == 0)
        return (-G / 0.5 / 0.5 );

    double F = -G / r / r;
    if(abs(F) > 1)
        return F;

    return 0;
}

double repulsion(int len, double r)
{
    double F = k * (r - len);
    if(abs(F) > 1)
        return F;
    return 0;
}

int sign(double num)
{
    return (num > 0)? 1 : -1;
}

int main()
{
    double x = 0, y = 0;
    double dx = 0, dy = 0;
    double pdx = 0, pdy = 0;
    double theta, force, d;
    int randi, randj, weight;

    priority_queue< pair<int, int> > q;
    pair<int, int> tmp;

    /*load bus685*/
    int va, vb;
    char buffer[128];
    ifstream in("bus685.txt",ios::in);
    ofstream outLoss("LOSS.txt",ios::trunc);
    ofstream outVertex("Vertex.txt",ios::trunc);

    if(! in.is_open())
    {
        printf("Open file failed\n");
        return 1;
    }

    int dist = 0;
    in.getline(buffer, 100);
    while(!in.eof())
    {
        in >> va >> vb ;
        graph[va][vb] = graph[vb][va] = 15;
    }
    in.close();




    double LOSS = 0;
    double div;
    double sign;
    for(int t = 0;t < 800; ++t)
    {
        LOSS = 0;
        //计算偏移距离
        for(int i = 0;i < vertexNum; ++i)
        {
            //定点的坐标
            x = v[i].x;
            y = v[i].y;
            dx = dy = 0;

            /*
                更新节点坐标
            */
            for(int j = 0;j < vertexNum; ++j)
            {
                if(i == j)
                    continue;

                //计算力的大小和方向
                pdx = (v[j].x - x);
                pdy = (v[j].y - y);
                d = sqrt(pdx*pdx + pdy*pdy);

                if(graph[i][j] == 0)
                    force = tension(d);
                else
                    force = repulsion(graph[i][j] , d) + tension(d);

                pdx = pdx * force;
                pdy = pdy * force;
                printf("pdx = %f pdy = %f d = %f force= %f \n", pdx, pdy, d, force);


                dx += pdx;
                dy += pdy;
            }

            v[i].x += dx * delta;
            v[i].y += dy * delta;
            LOSS += dx*dx + dy*dy;
        }
        outLoss << LOSS <<endl;

        /*
        calculate rate
        */
        /*
        if(LOSS < 10)
            delta = 0.05;
        if(LOSS < 1)
            delta = 0.01;
        */
        printf("t:%d LOSS:%f delta:%f\n", t, LOSS, delta);
    }

    for(int i = 0;i < vertexNum; ++i)
        outVertex << setprecision(6) << v[i].x << " " << setprecision(6) << v[i].y <<endl;

    outLoss.close();
    outVertex.close();
}
















