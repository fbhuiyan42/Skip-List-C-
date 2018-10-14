#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>
#define MAX_LEVEL 20
using namespace std;


struct Node
{
    int value;
    Node **After;
    int *span;
    Node(int h, int &value)
    {
        After = new Node * [h + 1];
        memset(After, 0, sizeof(Node*) * (h + 1));
        span = new int[h + 1];
        for(int i=0;i<h+1;i++) span[i]=0;
        this->value = value;
    }
    ~Node()
    {
        delete [] After;
    }
};


struct skiplist
{
    Node *head;
    int value;
    int h;
    int listsize;
    skiplist()
    {
        head = new Node(MAX_LEVEL, value);
        h = 0;
        listsize=0;
    }

    ~skiplist()
    {
        delete head;
    }

    Node* Search(int &searchValue,Node* update[MAX_LEVEL+1],int Rank[MAX_LEVEL+1])
    {
        Node *current = head;
        for (int i = h;i >= 0;i--)
        {
            if (i == h) Rank[i] = 0;
            else
            {
                Rank[i] = Rank[i+1];
                //cout<<"Inside search  i : "<<i<<"  rank : "<<Rank[i]<<endl;
            }
            while (current->After[i] != NULL && current->After[i]->value < searchValue)
            {
                Rank[i] =Rank[i] + current->span[i];
                //cout<<"Inside search while  i : "<<i<<"  rank : "<<Rank[i]<<"value : "<<current->value<<"span : "<<current->span[i]<<endl;
                current = current->After[i];
            }
            update[i] = current;
        }
        current = current->After[0];
        if(current==NULL || current->value != searchValue) return NULL;
        return current;
    }

    void Insert(int &value)
    {
        Node *update[MAX_LEVEL + 1];
        int Rank[MAX_LEVEL + 1];
        memset(update, 0, sizeof(Node*) * (MAX_LEVEL + 1));
        Node *x=Search(value,update,Rank);
        if (x == NULL || x->value != value)
        {
            int lvl = RandomLevel();
            if (lvl > h)
            {
                for (int i = h + 1;i <= lvl;i++)
                {
                    update[i] = head;

                    Rank[i] = 0;
                    update[i]->span[i] = listsize;
                }
                h = lvl;
            }
            x = new Node(lvl, value);
            for (int i = 0;i <= lvl;i++)
            {
                x->After[i] = update[i]->After[i];
                update[i]->After[i] = x;

                x->span[i] = update[i]->span[i] - (Rank[0] - Rank[i]);
                //cout<<"update "<<update[i]->span[i]<<endl;
                update[i]->span[i] = (Rank[0] - Rank[i]) + 1;
                //cout<<"i : "<<i<<"  rank[0] : "<<Rank[0]<<"rank[i] : "<<Rank[i]<<"span : "<<x->span[i]<<endl;

            }
            listsize++;
            //increment span for untouched levels
            for (int i = lvl+1; i < h+1; i++) update[i]->span[i]++;
        }
    }

    int IndexOf(int x)
    {
        int index=0;
        Node *current = head;
	    for(int i = h+1;i >= 0;i--)
        {
            while (current->After[i] != NULL && current->After[i]->value < x)
            {
                index= index + current->span[i];
                current =current->After[i];
            }
        }
        return index;
    }

    int ElementAt(int index)
    {
        int traversed = 0;
        Node *current = head;
	    for(int i = h+1;i >= 0;i--)
        {
            while(current->After[i] != NULL && (traversed+ current->span[i] <= index+1))
            {
                traversed += current->span[i];
                current =current->After[i];
            }
            if (traversed == index+1) return current->value;
        }
        return 0;
    }

    void Delete(int &value)
    {
        Node *update[MAX_LEVEL + 1];
        int Rank[MAX_LEVEL + 1];
        memset (update, 0, sizeof(Node*) * (MAX_LEVEL + 1));
        Node *x=Search(value,update,Rank);
        if (x->value == value)
        {
            for (int i = 0;i <= h;i++)
            {
                if (update[i]->After[i] != x)
                {
                    for(int j=i;j<=h;j++)
                        update[j]->span[j] -= 1;
                    break;
                }
                update[i]->After[i] = x->After[i];
                update[i]->span[i] += x->span[i] - 1;
            }
            delete x;
            listsize--;
            while (h > 0 && head->After[h] == NULL)
            {
                h--;
            }
        }
    }

    void Print()
    {
      cout <<endl;
      for (int i = h+1;i >= 0;i--)
      {
        cout<<"-inf";
        if(h==0 && head->span[0]==0 )
        {
            cout<<" ---- +inf";
            break;
        }
        if(head->span[i]==0) for(int j=0;j<listsize;j++) cout << "   ---- ";
        for(int j=0;j<head->span[i];j++) cout << "   ---- ";
        const Node *x = head->After[i];
        while (x != NULL)
        {
            cout << x->value;
            for(int j=0;j<x->span[i];j++) cout << " ---- ";
            x = x->After[i];
        }
        if(head->span[i]==0) cout<<"    ---- +inf";
        else cout<<"  ---- +inf";
        cout <<endl;
      }
    }



    int RandomLevel()
    {
        int lvl=0;
        float r=rand() / RAND_MAX;
        while(r < .5 && lvl<MAX_LEVEL)
        {
            lvl++;
            r=(float)rand() / RAND_MAX;
        }
        if(lvl>0) lvl--;
        return lvl;
    }

};


int main()
{
    skiplist S;
    int choice, n,index;
    Node* check;
    Node* update[MAX_LEVEL + 1];
    memset(update, 0, sizeof(Node*) * (MAX_LEVEL + 1));
    int Rank[MAX_LEVEL + 1];
    while (1)
    {
        cout<<endl<<"-----------------------"<<endl;
        cout<<endl<<"Operations on Skip list"<<endl;
        cout<<endl<<"-----------------------"<<endl;
        cout<<"1.Insert Element"<<endl;
        cout<<"2.Find Index of Element"<<endl;
        cout<<"3.Delete Element"<<endl;
        cout<<"4.Delete Element at Index"<<endl;
        cout<<"5.Search Element"<<endl;
        cout<<"6.Find Element at Index "<<endl;
        cout<<"7.Display List "<<endl;
        cout<<"8.Exit "<<endl;
        cout<<"Enter your choice : ";
        cin>>choice;
        switch(choice)
        {
        case 1:
             cout<<"Enter the element to be inserted: ";
             cin>>n;
             S.Insert(n);
             check=S.Search(n,update,Rank);
             if(check)
                 cout<<"Element Inserted"<<endl;
             break;
        case 2:
             cout<<"Enter the element to find index: ";
             cin>>n;
             check=S.Search(n,update,Rank);
             if(check)
             {
                index=S.IndexOf(n);
                cout<<"index of "<<n<<" : "<<index;
             }
             else cout<<"Element does not exist "<<endl;
             break;
        case 3:
             cout<<"Enter the element to be deleted: ";
             cin>>n;
             check=S.Search(n,update,Rank);
             if(!check)
             {
                 cout<<"Element not found"<<endl;
                 break;
             }
             S.Delete(n);
             check=S.Search(n,update,Rank);
             if(!check)
                cout<<"Element Deleted"<<endl;
             break;
         case 4:
             cout<<"Enter the index to Delete element: ";
             cin>>n;
             if(n>=S.listsize)
             {
                cout<<"index does not exist "<<endl;
                break;
             }
             index=S.ElementAt(n);
             S.Delete(index);
             check=S.Search(index,update,Rank);
             if(!check)
                cout<<"Element at index "<<n<<" Deleted"<<endl;
             break;
        case 5:
             cout<<"Enter the element to be searched: ";
             cin>>n;
             check=S.Search(n,update,Rank);
             if(check)
                 cout<<"Element "<<n<<" is in the list"<<endl;
             else
                 cout<<"Element not found"<<endl;
             break;
        case 6:
             cout<<"Enter the index to find element: ";
             cin>>n;
             if(n<S.listsize)
             {
                index=S.ElementAt(n);
                cout<<"Element at index "<<n<<" : "<<index;
             }
             else cout<<"index does not exist "<<endl;
             break;
        case 7:
             cout<<"The List is: ";
             S.Print();
             break;
        case 8:
             exit(1);
             break;
        default:
             cout<<"Wrong Choice"<<endl;
        }
    }
    return 0;
}





