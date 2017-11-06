#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <utility>
#include <fstream>
#include <iomanip>
using namespace std;
const double G = 100;       //斥力系数
int c;

/**
* ( @x, @y)用于表示x和y方向的偏移量
*/
struct P
{
    double x = 0;
    double y = 0;
};

class Node
{

    public:
        /**
        * 随机叶节点
        */
        Node()
        {
            this->x = 500 + (rand()%201 - 100);
            this->y = 500 + (rand()%201 - 100);
            setLeaf(true);
            for(int i = 0;i < 4; ++i)
                childern[i] = NULL;
        }

        /**
        * @x @y 为Node坐标
        * @leaf 表示是否为叶节点
        */
        Node(double x, double y, bool leaf)
        {
            this->x = x;
            this->y = y;
            setLeaf(leaf);

            for(int i = 0;i < 4; ++i)
                childern[i] = NULL;
        }

        /**
        *  若 @leaf = true 则该节点为叶节点
        *  @x @y 表示坐标值
        *  @width @height无效
        *
        *  若 @leaf = false则为内部节点
        *  @x @y 表示区域中心
        *  @width @height表示区域宽度和高度
        */
        Node(double x, double y, double width ,double height, bool leaf)
        {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            setLeaf(leaf);

            for(int i = 0;i < 4; ++i)
                childern[i] = NULL;

        }

        /**
        *  递归删除子节点
        */
        void Delete()
        {
            for(int i = 0;i < 4; ++i)
                if(this->childern[i] != NULL)
                    if(!this->childern[i]->leaf)
                    {
                        this->childern[i]->Delete();
                        delete this->childern[i];
                        this->childern[i] = NULL;
                    }
                    else
                    {
                        delete this->childern[i];
                        this->childern[i] = NULL;
                    }
        }

        /**
        * 递归print
        */
        void print()
        {
            if(!leaf)
                printf("Not a leaf: x = %f y = %f weight = %d ax = %f ay = %f\n", x, y, weight, ax, ay);
            else
                printf("leaf node: x = %f y = %f weight = %d\n", x, y, weight);

            for(int i = 0;i < 4; ++i)
                if(this->childern[i] != NULL)
                    this->childern[i]->print();
        }

        /**
        *  迭代求( @x, @y)点受力引起的位移 @p
        *  如果是内部节点，当 @theta < 0.5时，我们认为该簇可以被看作一个大节点
        *  如果是叶节点，则直接计算力
        */
        P force(double x, double y)
        {
            P p;
            queue<Node> q;
            Node node;
            double pdx, pdy;
            double dx, dy;
            double d, force;
            double theta;

            q.push(*this);
            while(!q.empty())
            {
                node = q.front();
                q.pop();

                pdx = node.ax / node.weight - x;
                pdy = node.ay / node.weight - y;
                d = sqrt(pdx*pdx + pdy*pdy);


                if(!node.leaf)
                {
                    if(d == 0)
                    {
                        for(int i = 0;i < 4; ++i)
                            if(node.childern[i] != NULL)
                                q.push(*node.childern[i]);
                    }
                    theta = node.width / d;
                    if(theta < 0.1)
                    {
                        force = tension(node.weight, d);
                        dx = force * pdx / d;
                        dy = force * pdy / d;

                        p.x += dx;
                        p.y += dy;
                    }
                    else
                    {
                        for(int i = 0;i < 4; ++i)
                            if(node.childern[i] != NULL)
                                q.push(*node.childern[i]);
                    }
                }
                else
                {
                    if(d == 0)
                        d = 0.5;
                    force = tension(node.weight, d);
                    dx = force * pdx / d;
                    dy = force * pdy / d;

                    p.x += dx;
                    p.y += dy;
                }
            }
            return p;
        }

        /**
        *  非叶节点必须先设置非叶属性，再插入子节点更新 @ax @ay @weight
        */
        void setLeaf(bool leaf)
        {
            this->leaf = leaf;
            if(leaf)
            {
                weight = 1;
                ax = x;
                ay = y;
            }
            else
            {
                weight = 0;
                ax = ay = 0;
            }
        }

        /**
        * 递归插入节点并更新非叶节点
        * @ax @ay 总质心与 @weight 总重量
        * @ax @ay 分别为所有子叶节点的 @x @y累加
        * 因为 @weight 始终为1，故可以代表子节点数目
        * 重复点的bug还没调好
        */
        void insert(Node &child)
        {
            if(!child.leaf)
            {
                printf("%d error: inserted node must be a child\n", child.leaf);
                return;
            }
            double x0 = child.x;
            double y0 = child.y;
            int index = 0;

            if(x0 >= x)
                if(y0 >= y)
                    index = 0;
                else
                    index = 1;
            else
                if(y0 < y)
                    index = 2;
                else
                    index = 3;

            //更新属性
            this -> ax += child.x;
            this -> ay += child.y;
            this -> weight ++;
            if(childern[index] == NULL)
            {
                childern[index] = &child;
            }
            else
            {
                if(childern[index]->leaf)
                {
                    if(childern[index]->x == child.x && childern[index]->y == child.y)
                    {
                        child.x += (rand() % 5) * 0.1 + 0.1;
                        child.y += (rand() % 5) * 0.1 + 0.1;
                        this->insert(child);
                        return;
                    }

                    Node *tmp = childern[index];
                    double w = this -> width / 4;
                    double h = this -> height / 4;
                    if(index == 0)
                        this->childern[index] = new Node(x + w, y + h, 2*w, 2*h, false);
                    else if(index == 1)
                        this->childern[index] = new Node(x + w, y - h, 2*w, 2*h, false);
                    else if(index == 2)
                        this->childern[index] = new Node(x - w, y - h, 2*w, 2*h, false);
                    else if(index == 3)
                        this->childern[index] = new Node(x - w, y + h, 2*w, 2*h, false);

                    childern[index]->insert(*tmp);
                    childern[index]->insert(child);
                }
                else
                    childern[index]->insert(child);
            }
        }

        double x = 0, y = 0;
        double ax, ay;
        double width = 0, height = 0;
        bool leaf = true;
        Node *childern[4];
        int weight = 0;

    private:
        double tension(int m, double r)
        {
            if(r == 0)
                return (-G * m / 0.5 / 0.5 );

            double F = -G * m / r / r;

            //printf("T: %f\n",F);
            if(abs(F) > 1)
                return F;
            return 0;
        }
};

class BarnesHutTree
{
    public:
        BarnesHutTree(double left, double right, double bottom, double top)
        {
            this->width = right - left;
            this->height = top - bottom;
            if(width <= 0 || height <= 0)
            {
                printf("error:parameter error\n");
                return;
            }
            this->x = left + width/2;
            this->y = bottom + height/2;
            this->root = new Node(x, y, width, height, false);
        }

        /**
        * 递归删除节点
        */
        ~BarnesHutTree(){root->Delete(); delete root;}

        BarnesHutTree& insert(Node &node)
        {
            root->insert(node);
            return *this;
        }

        /**
        * 求某一结点的合力
        */
        P force(double x, double y)
        {
            root->force(x,y);
        }

        void print()
        {
            root->print();
        }

        Node *root;
        double x, y;
        double width, height;

};

/*int main()
{
    BarnesHutTree *t = new BarnesHutTree(0,10,0,10);
    Node a(3,3,true),b(3,6,true),c(3,9,true);

    //t->print();
    t->root->Delete();
    t->print();
    t->root = new Node(5,5,10,10,false);
    t->insert(a).insert(b).insert(c);
    t->print();

}*/
