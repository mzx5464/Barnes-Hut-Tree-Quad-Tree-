#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<fstream>
using namespace std;
const int vertexNum = 686;
//const int edgeNum = 80;
const double G = 100;
const double k = 1;
const double PI = 3.1415926535;

int graph[vertexNum][vertexNum];

struct node
{
    //(400~600)随机初始化节点坐标
    double x = 500 + (rand()%201 - 100);
    double y = 500 + (rand()%201 - 100);
};

node v[vertexNum];

double tension(double r)
{
    if(r == 0)
        return (-G / 0.5 / 0.5 );

    double F = -G / r / r;
    if(abs(F) > 1)
        return F;

    //printf("F:%f\n",F);
    return 0;
}

double repulsion(int len, double r)
{
    double F = k * (r - len);
    if(abs(F) > 1)
        return F;
    return 0;
}

int main()
{
    double x = 0, y = 0;
    double dx = 0, dy = 0;
    double pdx = 0, pdy = 0;
    double theta, force, d;
    int randi, randj, weight;

    /*读取bus685*/
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
    int c= 0;
    for(int t = 0;t < 3000; ++t)
    {
        LOSS = 0;
        //计算偏移距离
        for(int i = 0;i < vertexNum; ++i)
        {
            //定点的坐标
            x = v[i].x;
            y = v[i].y;
            for(int j = 0;j < vertexNum; ++j)
            {
                if(i == j)
                    continue;

                //计算力的大小和方向
                pdx = (v[j].x - x);
                pdy = (v[j].y - y);

                if(pdx == 0)
                {
                    theta = PI / 2;
                    if(pdy < 0)
                        theta *= -1;
                    d = abs(pdy);
                }
                else
                {
                    div = pdy / pdx;
                    theta = atan(div);
                    sign = pdy / abs(pdy);


                    if(div > 0)
                    {
                        if(sign < 0)
                            theta += PI;
                    }else
                    {
                        if(sign > 0)
                            theta += PI;
                    }
                    d = sqrt(pdx*pdx + pdy*pdy);
                }

                if(graph[i][j] == 0)
                    force = tension(d);
                else
                    force = repulsion(graph[i][j] , d);

                pdx = force * cos(theta);
                pdy = force * sin(theta);

                dx += pdx;
                dy += pdy;
            }
            v[i].x += dx / 10;
            v[i].y += dy / 10;

            LOSS += (v[i].x - x) * (v[i].x - x) + (v[i].y - y) * (v[i].y - y);
            dx = 0;
            dy = 0;
        }
        outLoss << LOSS <<endl;
        printf("t = %d LOSS = %f\n", t, LOSS);
    }
    for(int i = 0;i < vertexNum; ++i)
        outVertex << v[i].x << " " << v[i].y <<endl;

    outLoss.close();
    outVertex.close();
}















