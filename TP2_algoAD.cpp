// TP2_algoAD.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "Sommet.h"
#include <string>
#include <fstream>

using namespace std;
const int nb_sommet = 5;
static int nb;
static int dis_fin;
static int paire[nb_sommet + 1];
static int or_paire[nb_sommet + 1];
const int INTMAX = 2147483647;
int Merge[nb_sommet*2 + 1];
int Attend[nb_sommet * 2 + 1] = { 0 };

void lecteur(string nom, Sommet s[]) {
    ifstream infile;
    string vide;

    infile.open(nom);
    infile >> nb;
    for (int i = 1; i < nb_sommet*2+1; i++)
    {
        infile >> s[i].numero_sommet;
    }
    for (int i = 1; i < nb_sommet * 2+1; i++)
    {
        infile >> s[i].t1;
    }
    for (int i = 1; i < nb_sommet * 2+1; i++)
    {
        infile >> s[i].t2;
    }
    for (int i = 1; i < nb_sommet * 2 + 1; i++)
    {
        infile >> s[i].dist;
    }
    infile >> dis_fin;
    //cout << dis_fin << endl;
}



int* calculer_earlist_time(Sommet s[], int original) {
    //int * list_ear_time = new int[nb_sommet * 2 + 2];
    //list_ear_time[0] = 0;
    static int list_ear_time[nb_sommet * 2 + 2] = { 0 };
    list_ear_time[0] = original;
    for (int i = 1; i < nb*2+1; i++)
    {
        int t_e = list_ear_time[i-1] + s[i].dist;
        if (t_e>s[i].t1)
        {
            list_ear_time[i] = t_e;
        }
        else
        {
            list_ear_time[i] = s[i].t1;
        }
    }
    list_ear_time[nb_sommet * 2 + 1] = list_ear_time[nb_sommet * 2] + dis_fin;
    cout << endl;
    return list_ear_time;
}


int* calculer_latest_time(int *l, Sommet s[]) {
    static int list_lat_time[nb_sommet * 2 + 2]{0};
    int total_time = l[nb_sommet * 2 + 1];
    list_lat_time[nb_sommet * 2 + 1] = total_time;
    list_lat_time[nb_sommet * 2] = total_time - dis_fin;
    for (int i = nb * 2 - 1; i > 0; i--)
    {
        int t_l = list_lat_time[i+1] - s[i+1].dist;
        if (t_l > s[i].t2)
        {
            list_lat_time[i] = s[i].t2;
        }
        else
        {
            list_lat_time[i] = t_l;
        }
    }
    list_lat_time[0] = list_lat_time[1] - s[1].dist;
    //list_lat_time[nb_sommet * 2 + 1] = list_lat_time[nb_sommet * 2] + dis_fin;
    return list_lat_time;
}

void make_paire(Sommet s[]) {
    for (int i = 1; i < nb_sommet+1; i++){

        for (int j = 1; j < nb_sommet*2 + 1; j++)
        {
            if (s[j].numero_sommet == -i)
            {
                paire[i] = j;
            }
        }
    }

    for (int i = 1; i < nb_sommet + 1; i++)
    {
        for (int j = 1; j < nb_sommet * 2 + 1; j++)
        {
            if (s[j].numero_sommet == i)
            {
                or_paire[i] = j;
            }
        }
    }
}
int calculer_riding_time(int *ear_t_list, Sommet s[]) {
    int riding_time = 0;
    for (int i = 1; i < nb + 1; i++)
    {
        int livraision = paire[i];
        int de = or_paire[i];
        riding_time += ear_t_list[livraision] - ear_t_list[de];

    }
    return riding_time;
}

void generer_merge_attente(int* list_lst_time, int* new_ear_time,Sommet s[]) {
    for (int i = 1; i < nb_sommet*2+1; i++)
    {
        Merge[i] = list_lst_time[i] - new_ear_time[i];
    }
    for (int i = 1; i < nb_sommet + 1; i++)
    {
        for (int k = 1; k < nb_sommet*2+1; k++)
        {
            if (s[k].numero_sommet == i)
            {
                Attend[k] = new_ear_time[paire[i]] - new_ear_time[k];

                for (int j = k + 1; j <= paire[i]; j++)
                {
                    Attend[k] -= s[j].dist;
                }
            }
            
        }

    }

    cout << "(Marge,Attente):" << endl;

    for (int i = 1; i < nb_sommet*2 + 1; i++)
    {
        cout << s[i].numero_sommet << ":(" << Merge[i] << "," << Attend[i] << ")" << endl;

    }
    cout << endl;
}

void afficher_time(int* time, int riding_time, string type) {
    cout << type << ":   ";
    for (int i = 0; i < nb_sommet * 2 + 2; i++)
    {
        cout << time[i] << "   ";
    }
    cout << endl;
    cout << "Total duration:" << time[nb_sommet * 2 + 1] - time[0] << endl;
    cout << "Total riding time:" << riding_time << endl;
}

void iteratet_resultat(int* new_list, int* lst_list, Sommet s[]) {
    for (int i = 1; i < nb_sommet*2 + 1; i++)
    {
        int merge;
        if (Merge[i] != 0 && Attend[i] != 0)
        {
            if (Merge[i] > Attend[i])
            {
                Merge[i] -= Attend[i];
                new_list[i] += Attend[i];
                merge = Attend[i];
                Attend[i] = 0;
            }
            else
            {
                Attend[i] = Merge[i];
                new_list[i] += Merge[i];
                merge = Attend[i];

                Merge[i] = 0;
            }
            int b = i;

            while(new_list[b] + s[b+1].dist > new_list[b + 1])
            {
                cout << new_list[b + 1] << endl;
                new_list[b + 1] = new_list[b] + s[b + 1].dist;
                b++;
            }
        }
    }
    generer_merge_attente(lst_list, new_list, s);
    cout << "Nouvelles (Marge,Attente):" << endl;

    for (int i = 1; i < nb_sommet * 2 + 1; i++)
    {
        cout << s[i].numero_sommet << ":(" << Merge[i] << "," << Attend[i]<<")" << endl;
    }
    afficher_time(new_list, calculer_riding_time(new_list, s), "EST_4a");

}

void afficher(string nom, Sommet s[]) {
    cout << "Instance:" << nom << endl;
    cout << "Route:" << 0 << "   ";
    int zero = 0;
    for (int i = 1; i < nb * 2 + 1; i++)
    {
        cout << s[i].numero_sommet << "   ";
    }
    cout << 0 << "   " << endl;

    cout << "Time windows:";
    cout << "[" << zero << "," << INTMAX << "]" << "   ";
    for (int i = 0; i < nb * 2 + 1; i++)
    {
        cout << "[" << s[i].t1 << "," << s[i].t2 << "]" << "   ";
    }
    cout << "[" << zero << "," << INTMAX << "]" << "   " << endl;
    cout << "Transp.time:" << endl;

    for (int i = 1; i < nb * 2 + 1; i++)
    {
        cout << i - 1 << " to " << i << ": " << s[i].dist << endl;
    }
    cout << nb * 2 << " to " << nb * 2 + 1 << ": " << dis_fin << endl;
}



int main()
{
    Sommet mon_sommet[nb_sommet * 2 + 1];
    string nom_ficher = "DARP_1.dat";
    lecteur(nom_ficher, mon_sommet);
    make_paire(mon_sommet);
    //afficher(nom_ficher, mon_sommet);

    
    int* ear_t_list;
    int riding_time;
    string type1 = "EST";

    int point_de_depart = 0;
    ear_t_list = calculer_earlist_time(mon_sommet,point_de_depart);
    riding_time = calculer_riding_time(ear_t_list, mon_sommet);
    //afficher_time(ear_t_list,riding_time, type1);

    

    int* lst_t_list;
    int l_riding_time;
    string type2 = "LST";

    lst_t_list = calculer_latest_time(ear_t_list,mon_sommet);
    l_riding_time = calculer_riding_time(lst_t_list, mon_sommet);
    //afficher_time(lst_t_list, l_riding_time,type2);

    int* new_ear_t_list;
    int new_riding_time;
    string type3 = "EST_3a";
    int new_point_de_depart = lst_t_list[0];
    new_ear_t_list = calculer_earlist_time(mon_sommet, new_point_de_depart);
    new_riding_time = calculer_riding_time(new_ear_t_list, mon_sommet);
    //afficher_time(new_ear_t_list, new_riding_time,type3);


    generer_merge_attente(lst_t_list, new_ear_t_list, mon_sommet);
    iteratet_resultat(new_ear_t_list,lst_t_list, mon_sommet);
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
