#include <bits/stdc++.h>
using namespace std;
ifstream f("mergeheap.in");
ofstream g("mergeheap.out");
const int inf = 2000000001;  //binomial heap ( reprezinta un heap alcatuit din mai multi arbori binomiali ) (fiecare fiu trebuie sa fie mai mare decat nodul parinte)
const int Max = 101;
int t,h1,h2,h,x,n,m;
struct nod
{
    int cheie, grad;
    nod * copil, * frate, * tata;

};
nod* nod_nou(int val)
{
    nod* temp = new nod;
    temp-> cheie = val;          //cream un nod nou
    temp->grad = 0;
    temp -> copil = temp -> frate = temp -> tata = nullptr;
    return temp;

}
struct heap_binomial
{
    list<nod *> hp;

    list<nod *>::iterator get_root() //functie pentru a afla cheia maxima a unui nod din lista
    {
        list<nod *>::iterator it_max;
        nod *vmax = nod_nou(-inf);
        for (list<nod *>::iterator it = hp.begin(); it != hp.end(); ++it)
        {
            if ((*it)->cheie > vmax->cheie)
            {
                vmax = *it;
                it_max = it;
            }
        }
        return it_max;
    }

    void stergere_radacina(nod *tree, heap_binomial &heap) //stergerea radacinii se face prin a impinge nodul in jos si prin schimbarea pozitiilor nodurilor
//stergerea minimului(radacinii) face ca arborele binomial din care a fost sters sa se imparta in mai multi subarbori care trebuie adaugati intr-un nou heap, si apoi unit
//cu heap ul original, complex O(log n)
    {
        if (tree->grad == 0)
        {
            delete tree;
            return;
        }
            nod *tree2 = tree;
            tree->copil->tata = nullptr;
            heap.hp.push_front(tree->copil);
            tree = tree->copil;
            while (tree->frate)
            {
                tree->frate->tata = nullptr;
                heap.hp.push_front(tree->frate);
                tree = tree->frate;
            }
            delete tree2;
    }

    void merge(nod *tree1, nod *tree2)  //se uneste arborele cu grad mic la cel cu grad mare
    //  O(log n)
    {
        if (tree1->cheie < tree2->cheie)
        {
            swap(*tree1, *tree2);
        }
        tree2->frate = tree1->copil;
        tree2->tata = tree1;
        tree1->copil = tree2;
        tree1->grad++;
    }

    void ajustare()
    {
        if (hp.size() <= 1)
            return;
        list<nod *>::iterator anterior, curent, urmator, temp;
        anterior = curent = hp.begin();
        curent++;
        urmator = curent;
        urmator++;
        while(curent != hp.end())
        {
            while((urmator == hp.end() || (*urmator) -> grad > (*curent) -> grad) && curent != hp.end() && (*anterior) -> grad == (*curent)->grad)
            {
                merge(*curent,*anterior);
                temp = anterior;
                if(anterior == hp.begin())
                {
                    anterior++;
                    curent++;
                    if(urmator != hp.end())
                        urmator++;
                }
                else
                    anterior--;                //trebuie sa ajustam heap ul pentru fiecare adaugare de nod / stergere
                hp.erase(temp);
            }
            anterior++;
            if(curent != hp.end())
                curent++;
            if(urmator != hp.end())
                urmator++;

        }

    }

    void push(int val)
    {
        nod *tree = nod_nou(val);      //adaugare nod  //insertul complex : O(k+log n) sau O(1) amortizat
        hp.push_front(tree);
        ajustare();
    }
    int varf()
    {
        return (*get_root()) -> cheie;        //cheia radacinii gasite         //gasire minim O(log n) / O(1) amortizat
    }
    void heap_union(heap_binomial& heap2)
    {
        list<nod*> :: iterator it1 = hp.begin();
        list<nod*> :: iterator it2 = heap2.hp.begin();
        list<nod*> heap_nou;
        while( it1 != hp.end() && it2 != heap2.hp.end() )   //unirea heap urilor , se leaga arborii de grad egal in heapul nou
            //interclasare in ordinea crescatoare a gradelor
        {
            if( (*it1) -> grad <= (*it2) -> grad )
            {
                heap_nou.push_back( *it1 );
                it1++;
            }
            else
            {
                heap_nou.push_back( *it2 );
                it2++;
            }
        }

        while( it1 != hp.end() )
        {
            heap_nou.push_back( *it1 );
            it1++;
        }

        while( it2 != heap2.hp.end() )
        {
            heap_nou.push_back( *it2 );
            it2++;
        }

        heap2.hp.clear();
        hp = heap_nou;
        ajustare();
    }
    void pop()  //pentru a sterge un nod trebuie sa-i scadem cheia si apoi sa stergem minimul din heap complex totala : O(log n)
    {
        list<nod*> :: iterator radacina = get_root();      //stergem radacina si refacem heap ul
        heap_binomial new_heap;
        stergere_radacina((*radacina),new_heap);
        hp.erase(radacina);
        heap_union(new_heap);
    }
};
heap_binomial Heap[Max];
int main()
{
    f>>n>>m;
    for(int i=1; i<=m; i++)
    {
        f>>t;
        if(t == 1)
        {
            f>>h>>x;
            Heap[h].push(x);
        }
        if(t == 2)
        {
            f>>h;
            g<<Heap[h].varf()<<'\n';
            Heap[h].pop();
        }
        if(t == 3)
        {
            f>>h1>>h2;
            Heap[h1].heap_union(Heap[h2]);
        }
    }
    return 0;
}
