#include<iostream>
#include<fstream>
#include<cmath>
#include<string>
#include<stack>
#include<queue>
#include<windows.h>

using namespace std;

#define maxvalue 100
int n=9;
int dist[100];
int path[100];
struct link2
{
    string i;//路由名称
    int j;//权值
    link2 * link=NULL;
};
struct link1
{
    string i;//路由名称
    string s;//路由ip
    int flag;//删除位
    link2*link=NULL;
};
struct binary
{
    int pre;
    int next[100];
    int nwei=0;
};


binary create[100];
link1 main1[100];
int count1;

void primary1()//初始化路由结点
{
    string s;
    int i=0;
    fstream out("f://路由1//1.txt");
    for(i=0;i<9;i++)
    {
        out>>main1[i].i;
        out>>main1[i].s;
        main1[i].flag=0;
    }
}

void primary2()//初始化结点的关联结点
{
    link2*temp,*current;
    int sum;
    int i;
    fstream out1("f://路由1//2.txt");
    char s;
    string gh;
    for( i=0;i<9;i++)
    {
        temp=new link2;
        out1>>gh;
        temp->i=gh;
        out1>>s;
        sum=s-'0';
        temp->j=sum;
        main1[i].link=temp;
        current=temp;
        while(1)
        {
            out1>>gh;
            if(gh[0]=='#')
            {
                break;
            }
            temp=new link2;
            temp->i=gh;
            out1>>s;
            sum=s-'0';
            temp->j=sum;
            current->link=temp;
            current=current->link;
        }
    }
}
int getweight(int i,int j)//取权值
{
  if(i==j)
  {
      return 0;
  }
  if(main1[i].flag==1)
  {
      return maxvalue;
  }
  if(main1[j].flag==1)
  {
      return maxvalue;
  }
  link2 *current=main1[i].link;
  while(current!=NULL)
  {
      if(current->i==main1[j].i)
      {
          break;
      }
      current=current->link;
  }
  if(current==NULL)
  {
      return maxvalue;
  }
  return current->j;
}
void dex(int i)//递归生成树
{
    int count2=0;
    int fg=0;
    int k=create[i].nwei;
    if(k==0)
    {
        cout<<endl;
        cout<<endl;
        return;
    }
    while(k--)
    {
        fg=create[i].next[k];
        while(path[fg]!=count1)
        {
            ++count2;
            fg=path[fg];
        }
        if(path[create[i].next[k]]!=count1)
        {
        for(int i=0;i<count2-1;i++)
        {
            cout<<"     ";
        }
        }
        cout<<"----->";
        cout<<main1[create[i].next[k]].i;
        dex(create[i].next[k]);
    }
}
void prinluyou()//生成路由表
{
   string g;
   cout<<"输入要生成的路由表的名称"<<endl;
   cin>>g;
   for(count1=0;count1<n;count1++)
   {
       if(main1[count1].i==g)
       {
           break;
       }
   }
   bool *s=new bool[n];
   int i,j,k;
   int min1,w;
   for(i=0;i<n;i++)
   {
       dist[i]=getweight(count1,i);
       s[i]=false;
       if(i!=count1&&dist[i]<min1)
       {
           path[i]=count1;
       }
       else
       {
           path[i]=-1;
       }
   }
   s[count1]=true;
   dist[count1]=0;
   for(i=0;i<n-1;i++)
   {
       min1=maxvalue;
       int u=count1;
       for(j=0;j<n;j++)
       {
           if(s[j]==false&&dist[j]<maxvalue)
           {
               u=j;
               min1=dist[j];
           }
           s[u]=true;
           for(k=0;k<n;k++)
           {
               w=getweight(u,k);
               if(s[k]==false&&w<maxvalue&&dist[u]+w<dist[k])
               {
                   dist[k]=dist[u]+w;
                   path[k]=u;
               }
           }
       }
   }
   for(i=0;i<n;i++)//存储下一跳和上一跳
   {
       if(i==count1)
       {
           create[i].pre=-1;
       }
       else
       {
           create[i].pre=path[i];
           k=path[i];
           create[k].next[create[k].nwei]=i;
           create[k].nwei++;
       }
   }
   dex(count1);
   cout<<endl;
   cout<<"***************************************"<<endl;
   cout<<"      目的地址               下一跳   "<<endl;
   for(k=0;k<n;k++)
   {
       if(k==count1)
       {
           continue;
       }
       if(path[k]==-1)
       {
           continue;
       }
       if(main1[k].flag==1)
       {
           continue;
       }
       j=k;
       while(path[j]!=count1)
       {
           j=path[j];
       }
    cout<<"     "<<main1[k].s<<"                  "<<main1[j].i<<endl;
   }
    cout<<"*****************************************"<<endl;
}
void memory()//保存修改
{
   DeleteFile("f://路由1//1.txt");
   DeleteFile("f://路由1//2.txt");
   int i,j,k;
   link2 *current;
   fstream out("f://路由1//1.txt",ios::out);
   for(i=0;i<n;i++)
   {
       if(main1[i].flag==1)
       {
           continue;
       }
       out<<main1[i].i<<" "<<main1[i].s<<endl;
   }
   fstream out1("f://路由1//2.txt",ios::out);
   for(j=0;j<n;j++)
   {
       if(main1[j].flag==1)
       {
           continue;
       }
       current=main1[j].link;
       while(current!=NULL)
       {
           for(k=0;k<n;k++)
           {
               if(main1[k].i==current->i)
               {
                   break;
               }
           }
           if(main1[k].flag==1)
           {
               current=current->link;
           }
           if(current==NULL)
           {
               break;
           }
           out1<<current->i<<" "<<current->j<<" ";
           current=current->link;
       }
       out1<<'#'<<endl;
   }
}
int main()
{
    int weight;
    int chose;
    primary1();
    primary2();
    cout<<"                         *****************************"<<endl;
    cout<<"                         *                           *"<<endl;
    cout<<"                         *                           *"<<endl;
    cout<<"                         *     路由表生成            *  "<<endl;
    cout<<"                         *                           *"<<endl;
    cout<<"                         *                           *"<<endl;
    cout<<"                         *****************************"<<endl;
    cout<<"                                 1 输出指定路由表          "<<endl;
    cout<<"                                 2 结点之间边管理             "<<endl;
    cout<<"                                 3 路由表结点管理      "<<endl;
    cout<<"                         *****************************"<<endl;
    cout<<"                      注意：路由表不能连续输出，第二次输出前请重新启动程序"<<endl;
    cin>>weight;
    while(1)
    {
    switch(weight)
    {
    case 1:
         prinluyou();
         break;
    case 2:
           cout<<"                   1 增加边 2删除边"<<endl;
           cin>>chose;
           if(chose==1)
           {
               operaddpa();
           }
           else
           {
               operdepa();
           }
          cout<<"              0:保存  其他：不保存"<<endl;
          cout<<"              输入一个数字："<<endl;
          cin>>chose;
          if(chose==0)
          {
              memory();
          }
          break;
    case 3:
          cout<<"              操作选项"<<endl;
          cout<<"              1 增加结点 2删除结点"<<endl;
          cin>>chose;
          if(chose==1)
          {
             operaddpo();
          }
          else
          {
              operdepo();
          }
          cout<<" 输入一个数字： 0:保存  其他：不保存"<<endl;
          cin>>chose;
          if(chose==0)
          {
              memory();
          }
          break;
    }
    cout<<"输入一个数字：结束：0    继续：其他数字"<<endl;
    cin>>chose;
    if(chose==0)
    {
        break;
    }
    cout<<"                         *****************************"<<endl;
    cout<<"                                 1 输出指定路由表        "<<endl;
    cout<<"                                 2 结点之间边管理             "<<endl;
    cout<<"                                 3 路由表结点管理        "<<endl;
    cout<<"                         *****************************"<<endl;
    cin>>weight;
  }
}