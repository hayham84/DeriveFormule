#define mini_prog_exo4
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
using namespace std;


class Noeud 
{
    friend class Pile;
    friend class Arbre;
    
    char type;//‘o’ pour opérateur et ‘f’ pour valeur et v pour variable
    char ope;
    char var;
    double val;
    Noeud *fg, *fd;
    public:
        Noeud()
        {
            //cout<<"Ajout d'un noeud"<<endl;
            type = ' ';
            ope = ' ';
            val = 0;
            fg = fd = NULL;
        }
        Noeud(char t, char o = ' ', char variable = ' ', double v = 0, Noeud * f = NULL, Noeud * d = NULL)
        {
            //cout<<"Ajout d'un noeud"<<endl;
            type = t;
            ope = o;
            var = variable;
            val = v;
            fg = f;
            fd = d;
        }
        ~Noeud() 
        {
            if (fg != NULL) 
            {
                if ( fg->type == 'o' )
                {
                    cout << "Suppression du fils gauche  : " << fg->ope << endl;
                }
                else if ( fg->type == 'f' )
                {
                    cout << "Suppression du fils gauche  : " << fg->val << endl;
                }
                delete fg;
            }
            if (fd != NULL) 
            {
                if ( fd->type == 'o' )
                {
                    cout << "Suppression du fils droit  : " << fd->ope << endl;
                }
                else if ( fd->type == 'f' )
                {
                    cout << "Suppression du fils droit  : " << fd->val << endl;
                }
                delete fd;
            }
        }
        void affiche();
};

class Maillon
{
    friend class Arbre;
    friend class Pile;
    Noeud * info;
    Maillon * suivant;
    public :
        Maillon()
        {
            info = NULL;
            suivant = NULL;
        }
        Maillon(Noeud * i, Maillon * s)
        {
            info = i;
            suivant = s;
        }
};

class Pile
{
    friend class Maillon;
    friend class Arbre;
    Maillon *tete;
    public:
        Pile()
        {
            tete = NULL;        
        }
        ~Pile()
        {
            if (tete != NULL)
            {
                if ( tete->info->type == 'o' )
                {
                    cout << "Suppression de la tete  : " << tete->info->ope << endl;
                }
                else if (tete->info->type == 'f' )
                {
                    cout << "Suppression de la tete  : " << tete->info->val << endl;
                }
                delete tete;
            }
        }
        
        string infix_to_sufix(string);
        int prioriter(char);
        void empiler(Noeud *);
        Maillon* depiler();
        bool vide();
};

class Arbre 
{
    friend class Noeud;
    friend class Pile;   
    Noeud * racine;
    public:
        Arbre()
        {
            racine = NULL ;
        }
        Arbre(Noeud *r) 
        {
            racine = r;
        }
        Arbre(string a) 
        {
        // Utilisation de la fonction infix_to_sufix de la classe Pile pour convertir l'expression
        Pile k;
        string s = k.infix_to_sufix(a);

        // Utilisation de la pile pour construire l'arbre
        Pile P;
        cout << "Transformation de l'expression infixé en suffixé : " << s << endl;
        for (int i = 0; i < s.size(); i++) 
        {
            if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/') 
            {
                Noeud *n = new Noeud('o', s[i]);
                n->fd = P.depiler()->info;
                n->fg = P.depiler()->info;
                P.empiler(n);
            }
            if (s[i] >= 'a' && s[i] <= 'z' || s[i] >= 'A' && s[i] <= 'Z') 
            {
                Noeud *l = new Noeud('v',' ', s[i]);
                P.empiler(l);
            }
            else if (isdigit(s[i])) 
            {
                string str(1,s[i]);
                double val = stod(str);
                //cout<<"val = "<<val<<endl;
                Noeud *l = new Noeud('f', ' ', ' ', val);
                P.empiler(l);
                while (isdigit(s[i+1]))
                {
                    string str(1,s[i+1]);
                    double val = stod(str);
                    //cout<<"val 2 = "<<val<<endl;
                    l->val = (l->val*10)+val;
                    //cout<<l->val<<endl;
                    i++;
                }
            }
        }
        // À la fin, la pile devrait contenir un seul nœud, qui est la racine de l'arbre
        racine = P.depiler()->info;
        }     
        ~Arbre()
        {
            if (racine != NULL)
            {
                if ( racine->type == 'o' )
                {
                    cout << "Suppression de la racine  : " << racine->ope << endl;
                }
                else if ( racine->type == 'f' )
                {
                    cout << "Suppression de la racine  : " << racine->val << endl;
                }
                delete racine;
            }
        }
        void afficherInfixe(Noeud*);
        void afficher_inf();
        Noeud* deriver();
        Noeud* D(Noeud *);
        Noeud* copieArbre(Noeud *);
        

    
};


