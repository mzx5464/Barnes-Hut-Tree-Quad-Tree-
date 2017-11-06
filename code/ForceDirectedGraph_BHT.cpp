#include "Parameter.h"
#include "BarnesHutTree.cpp"

using namespace std;
const double k = 1;       //拉力系数
const int L = 10;           //边长
const int vertexNum = 686;
double delta = 0.1;   //learning rate
vector<int> graph[vertexNum];
BarnesHutTree tree(400,600,400,600);
Node v[vertexNum];
double MAXx = -1e8;
double MAXy = -1e8;
double MINx = 1e8;
double MINy = 1e8;

double repulsion(double d)
{
    double F = k * (d - L);
    //printf("F: %f\n",F);
    if(abs(F) > 1)
        return F;
    return 0;
}
void update()
{
    for(int i = 0;i < vertexNum; ++i)
        tree.insert(v[i]);
}
void init()
{
    /*读取bus685*/
    int va, vb;
    char buffer[128];
    ifstream in("bus685.txt",ios::in);

    if(! in.is_open())
    {
        printf("Open file failed\n");
        return ;
    }

    int dist = 0;
    in.getline(buffer, 100);
    while(!in.eof())
    {
        in >> va >> vb ;
        graph[va].push_back(vb);
        graph[vb].push_back(va);
    }

    in.close();
}


int main()
{
    double tmpx, tmpy;
    double pdx, pdy;
    double x, y;
    double dx, dy;
    double height,width;
    Node node;
    double d, force;
    int index = 0;
    ofstream outLoss("LOSS.txt",ios::trunc);
    ofstream outVertex("Vertex.txt",ios::trunc);
    P p;
    double Loss = 0;
    int c;

    init();
    for(int t = 0;t < 2000; ++t)
    {
        update();
        Loss = 0;

        for(int i = 0;i < vertexNum; ++i)
        {
            x = v[i].x;
            y = v[i].y;
            p = tree.force(x, y);

            for(int j = 0;j < graph[i].size(); ++j)
            {
                index = graph[i][j];
                tmpx = v[index].x;
                tmpy = v[index].y;
                pdx = tmpx - x;
                pdy = tmpy - y;

                d = sqrt(pdx*pdx + pdy*pdy);
                force = repulsion(d);

                dx = force * pdx / d;
                dy = force * pdy / d;
            }

            dx += p.x;
            dy += p.y;

            v[i].x += dx * delta;
            v[i].y += dy * delta;

            Loss += (dx*dx + dy*dy);
            if(Loss<1)delta =0.01;
        }
        printf("%f\n",Loss);
        outLoss << Loss <<endl;
        tree.root->Delete();

        for(int i = 0;i < vertexNum; ++i)
        {
            if(v[i].x > MAXx) MAXx = v[i].x;
            if(v[i].y > MAXy) MAXy = v[i].y;
            if(v[i].x < MINx) MINx = v[i].x;
            if(v[i].y < MINy) MINy = v[i].y;
        }
        width = MAXx - MINx;
        height = MAXy - MINy;
        tree.root = new Node(MINx + width / 2, MINy + height / 2, width, height, false);
    }
    delete tree.root;

    for(int i = 0;i < vertexNum; ++i)
        outVertex << v[i].x << " " << v[i].y <<endl;

    outLoss.close();
    outVertex.close();

}
