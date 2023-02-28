#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <unordered_set>
#include <unordered_map>
#include <map>

using namespace std;
pthread_barrier_t barrier;
pthread_mutex_t mutex;
typedef std::unordered_map<int, std::unordered_map<int, vector<int>>> mapExponents;

typedef struct Files
{
    vector<string> files;
    int number;
    int files_left;
} TFiles;

typedef struct MapRed
{
    int id;
    mapExponents *map;
    TFiles *files;
    int nrM;
    int nrR;
} TMapRed;

void sortFiles(vector<string> *files)
{
    map<int, string> mapTmp;

    for (auto &k : *files)
    {
        ifstream in_file(k, ios::binary);
        in_file.seekg(0, ios::end);
        int file_size = in_file.tellg();
        mapTmp[file_size] = k;
    }

    int i = 0;
    for (std::map<int, string>::iterator it = mapTmp.begin(); it != mapTmp.end(); ++it, ++i)
    {
        (*files).at(i) = it->second;
    }
}

long long fast_pow(int base, int power)
{
    long long result = 1;
    while (power > 0)
    {

        if (power & 1)
        {
            result = (result * base);
        }
        base = (base * base);
        power >>= 1;
    }
    return result;
}

int binarySearch(int power, int number)
{
    if (number == 0)
    {
        return 0;
    }
    if (number == 1)
    {
        return 1;
    }

    int r = sqrt(number);
    int l = 2;

    while (r >= l)
    {
        int mid = l + (r - l) / 2; //mid -> numar din intervalul 2 .. sqrt(number)
        unsigned long long midNr = (unsigned long long)fast_pow(mid, power); //midNr -> valoarea puterii lui mid

        if (midNr == (unsigned long long)number)
        {
            return 1;
        }
        else if (midNr < (unsigned long long)number)
        {
            l = mid + 1;
        }
        else
        {
            r = mid - 1;
        }
    }
    return 0;
}

void *f(void *arg)
{
    TMapRed *data = (TMapRed *)arg;
    if (data->id < data->nrM)
    {
        vector<int> tmp;

        while ((*data->files).files_left)
        {
            pthread_mutex_lock(&mutex);

            int n = (*data->files).files_left;
            (*data->files).files_left--;

            pthread_mutex_unlock(&mutex);

            ifstream file;
            string line;

            file.open((*data->files).files.at((*data->files).number - n));
            getline(file, line);
            
            while (getline(file, line))
            {
                tmp.push_back(stoi(line));
            }
        }

        for (auto i = tmp.begin(); i != tmp.end(); ++i)
        {
            for (int pow = 2; pow < data->nrR + 2; ++pow)
            {
                if (binarySearch(pow, *i))
                {
                    ((*data->map)[data->id][pow - 2]).push_back(*i);
                }
            }
        }

        pthread_barrier_wait(&barrier);
    }

    else
    {
        pthread_barrier_wait(&barrier);
        unordered_set<int> tmp1;

        for (int i = 0; i < data->nrM; i++)
        {
            for (auto const &k : (*data->map)[i][data->id - data->nrM])
            {
                tmp1.insert(k);
            }
        }

        string out_name = "out";
        string nmr = to_string(data->id - data->nrM + 2);
        string fin_nmr = to_string(tmp1.size());
        out_name.append(nmr);
        out_name.append(".txt");

        ofstream myfile;
        myfile.open(out_name);
        myfile << fin_nmr;
        myfile.close();
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        exit(1);
    }

    int r;
    void *status;
    int numar_Mapperi, numar_Reduceri;
    string fisier_initial;

    numar_Mapperi = atoi(argv[1]);
    numar_Reduceri = atoi(argv[2]);
    fisier_initial = argv[3];

    int numar_fisiere;
    vector<string> nume_fisiere;

    ifstream file;
    string line;
    file.open(fisier_initial);

    getline(file, line);

    numar_fisiere = stoi(line);

    while (getline(file, line))
    {
        nume_fisiere.push_back(line);
    }

    mapExponents mapExp;

    sortFiles(&nume_fisiere);

    TFiles files;
    files.files = nume_fisiere;
    files.number = numar_fisiere;
    files.files_left = numar_fisiere;

    TMapRed mapper[numar_Mapperi];
    TMapRed reducer[numar_Reduceri];

    pthread_t threads[numar_Mapperi + numar_Reduceri];
    pthread_barrier_init(&barrier, NULL, numar_Mapperi + numar_Reduceri);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < numar_Mapperi + numar_Reduceri; i++)
    {
        if (i < numar_Mapperi)
        {
            mapper[i].id = i;
            mapper[i].nrM = numar_Mapperi;
            mapper[i].nrR = numar_Reduceri;
            mapper[i].map = &mapExp;
            mapper[i].files = &files;
            r = pthread_create(&threads[i], NULL, f, &mapper[i]);
            if (r)
            {
                printf("Eroare la crearea thread-ului %d\n", i);
                exit(-1);
            }
        }
        else
        {
            reducer[i - numar_Mapperi].id = i;
            reducer[i - numar_Mapperi].nrM = numar_Mapperi;
            reducer[i - numar_Mapperi].nrR = numar_Reduceri;
            reducer[i - numar_Mapperi].map = &mapExp;
            reducer[i - numar_Mapperi].files = &files;
            r = pthread_create(&threads[i], NULL, f, &reducer[i - numar_Mapperi]);
            if (r)
            {
                printf("Eroare la crearea thread-ului %d\n", i);
                exit(-1);
            }
        }
    }

    for (int i = 0; i < numar_Mapperi + numar_Reduceri; i++)
    {
        r = pthread_join(threads[i], &status);

        if (r)
        {
            printf("Eroare la asteptarea thread-ului %d\n", i);
            exit(-1);
        }
    }

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);
    return 0;
}