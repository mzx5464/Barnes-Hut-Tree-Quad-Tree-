#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <utility>
#include <fstream>
#include <iomanip>
using namespace std;
const double G = 100;       //����ϵ��
int c;

/**
* ( @x, @y)���ڱ�ʾx��y�����ƫ����
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
        * ���Ҷ�ڵ�
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
        * @x @y ΪNode����
        * @leaf ��ʾ�Ƿ�ΪҶ�ڵ�
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
        *  �� @leaf = true ��ýڵ�ΪҶ�ڵ�
        *  @x @y ��ʾ����ֵ
        *  @width @height��Ч
        *
        *  �� @leaf = false��Ϊ�ڲ��ڵ�
        *  @x @y ��ʾ��������
        *  @width @height��ʾ�����Ⱥ͸߶�
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
        *  �ݹ�ɾ���ӽڵ�
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
        * �ݹ�print
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
        *  ������( @x, @y)�����������λ�� @p
        *  ������ڲ��ڵ㣬�� @theta < 0.5ʱ��������Ϊ�ôؿ��Ա�����һ����ڵ�
        *  �����Ҷ�ڵ㣬��ֱ�Ӽ�����
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
        *  ��Ҷ�ڵ���������÷�Ҷ���ԣ��ٲ����ӽڵ���� @ax @ay @weight
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
        * �ݹ����ڵ㲢���·�Ҷ�ڵ�
        * @ax @ay �������� @weight ������
        * @ax @ay �ֱ�Ϊ������Ҷ�ڵ�� @x @y�ۼ�
        * ��Ϊ @weight ʼ��Ϊ1���ʿ��Դ����ӽڵ���Ŀ
        * �ظ����bug��û����
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

            //��������
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
        * �ݹ�ɾ���ڵ�
        */
        ~BarnesHutTree(){root->Delete(); delete root;}

        BarnesHutTree& insert(Node &node)
        {
            root->insert(node);
            return *this;
        }

        /**
        * ��ĳһ���ĺ���
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
