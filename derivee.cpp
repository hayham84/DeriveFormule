#include "mini_prog_exo4.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
using namespace std;


void Noeud::affiche()
{
    if ( type == 'o' )
    {
        cout << "Noeud  : " << ope << endl;
    }
    else if ( type == 'f' )
    {
        cout << "Noeud  : " << val << endl;
    }
     else if ( type == 'v' )
    {
        cout << "Noeud  : " << var << endl;
    }
}

Maillon* Pile::depiler()
{
    Maillon *k = tete;
    tete = k->suivant;
    if ( k->info->type == 'o' )
    {
        cout << "Noeud depiler : " << k->info->ope << endl;
    }
    else if ( k->info->type == 'f' )
    {
        cout << "Noeud depiler : " << k->info->val << endl;
    }
    else if ( k->info->type == 'v' )
    {
        cout << "Noeud depiler : " << k->info->var << endl;
    }
    return k;
}


void Pile::empiler(Noeud *P)
{
    tete = new Maillon(P,tete);
    if ( P->type == 'o' )
    {
        cout << "Noeud empiler : " << P->ope << endl;
    }
    else if ( P->type == 'f' )
    {
        cout << "Noeud empiler : " << P->val << endl;
    }
    else if ( P->type == 'v' )
    {
        cout << "Noeud empiler : " << P->var << endl;
    }
}

bool Pile::vide()
{
    if (tete == NULL) 
        return true;
    else  
        return false;
}


int Pile::prioriter(char op) 
{
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
}

string Pile::infix_to_sufix(string a)
{
    string s;
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] == '+' || a[i] == '-' || a[i] == '*' || a[i] == '/') 
        {
            while (!vide() && prioriter(a[i]) <= prioriter(tete->info->ope))
            {
               Noeud * j;
               j = depiler()->info;
               s += j->ope;
               s += " ";
            }
            Noeud *n = new Noeud('o',a[i]);
            empiler(n);
        }
        else if (a[i] == '(') 
        {
            Noeud *n = new Noeud('o',a[i]);
            empiler(n);
        }
        else if (a[i] == ')') 
        {
            Noeud *e = depiler()->info;
            while (e->ope != '(')
            {
                s += e->ope;
                s += " ";
                e = depiler()->info;
            }
        }
        else if (a[i] >= 'a' && a[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z') 
        {
             s += a[i];
             s += " ";
        }
        else
        {
            s += a[i];
            while (isdigit(a[i+1]))
            {
                s += a[i+1];
                i++;
            }
            s += " ";
        }
    }
    while (!vide()) 
    {
        Noeud * l;
        l = depiler()->info;
        s += l->ope;
        s += " ";
    }
    return s;
}


void Arbre::afficherInfixe(Noeud* a) 
{
    if (a == NULL) 
    {
        return;
    }
    // Cas de l'opérateur
    if (a->type == 'o') 
    {
        cout << "(";
        afficherInfixe(a->fg);
        cout << a->ope;
        afficherInfixe(a->fd);
        cout << ")";
    }
    else if (a->type == 'v') 
    {
        cout << a->var;
    }
    // Cas de la valeur
    else 
    {
        cout << a->val;
    }
}

void Arbre::afficher_inf() 
{
    return afficherInfixe(racine);
}

Noeud* Arbre::D(Noeud *n) 
{
    // si valeur alors 0;
    if (n->type == 'f') 
    {
        return new Noeud('f',' ',' ', 0);
    }
    // si variable
    if (n->type == 'v') 
    {
        // si x alors 1;
        if (n->var == 'X' || n->var == 'x' ) 
        {
            return new Noeud('f',' ',' ', 1); 
        }
        // si Y ou Z alors 0;
        return new Noeud('f',' ',' ', 0); 
    }
    if (n->type == 'o') 
    {
        switch (n->ope) 
        {
            case '+':
                return new Noeud('o', '+', ' ', 0, D(n->fg), D(n->fd));
            case '-':
                return new Noeud('o', '-', ' ', 0, D(n->fg), D(n->fd));
            case '*':
                return new Noeud('o', '+', ' ', 0, new Noeud('o', '*', ' ', 0, D(n->fg), copieArbre(n->fd)), 
                    new Noeud('o', '*', ' ', 0, copieArbre(n->fg), D(n->fd)));
            
            case '/':
                return new Noeud('o', '/', ' ', 0,
                    new Noeud('o', '-', ' ', 0,
                        new Noeud('o', '*', ' ', 0, D(n->fg), copieArbre(n->fd)),
                        new Noeud('o', '*', ' ', 0, copieArbre(n->fg), D(n->fd))),
                    new Noeud('o', '*', ' ', 0, copieArbre(n->fd), copieArbre(n->fd)));
            case '^':
                Noeud *base = copieArbre(n->fg);
                Noeud *exp = copieArbre(n->fd);
                return new Noeud('o', '*', ' ', 0,
                    new Noeud('o', '*', ' ', 0, exp,
                        new Noeud('o', '^', ' ', 0, base,
                            new Noeud('o', '-', ' ', 0, exp, new Noeud(1)))),
                    D(base));
            
        }
    }
    return NULL;
}

Noeud* Arbre::deriver() 
{
    return D(racine);
}
    
Noeud* Arbre::copieArbre(Noeud *n) 
{
        if ( n == NULL ) return nullptr;
        return new Noeud(n->type ,n->ope ,n->var ,n->val ,copieArbre(n->fg) ,copieArbre(n->fd));
}


int main()
{
    /* Pile P;
    string s = P.infix_to_sufix("2*(100+a)");
    cout<<s; */
    Arbre A("3*x+5");
    cout<<"Arbre parcourue et reconstituer : "; 
    A.afficher_inf();
    cout<<endl;
    cout<<"Résultat de la dérivé de l'arbre : ";
    A.afficherInfixe(A.deriver());
    cout<<endl;
   
  
}