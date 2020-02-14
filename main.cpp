#include <iostream>
#include <pthread.h>
#include <bits/stdc++.h>
#include <vector>

using namespace std;

void Merge_Arrays(int left, int right, int midpoint);
vector<int> mergearray; /// input & result array.
vector<int> sortarray; /// temporary array to do the sorting process inside it.
int leftindex;
int rightindex;
int tempindex;
int SIZE;

struct limits
{
    int left;
    int right;
};

void* Merge_sort(void *args)
{
    struct limits* lim = (struct limits*) args;
    struct limits newLeft; /// new struct for the left limits.
    struct limits newRight; /// new struct for the right limits.

    if(lim->left < lim->right) /// stop condition for the division of the array
    {
        pthread_t id[2]; /// create 2 threads, thread for each half.
        int Midpoint = (lim->right + lim->left)/2; /// Midpoint of the current array.
        newLeft.right = Midpoint; /// limits of the left half.
        newLeft.left = lim->left;

        newRight.left = Midpoint+1; /// limits of the right half.
        newRight.right = lim->right;

        pthread_create(&id[0],NULL,Merge_sort,&newLeft); /// create a thread and pass the left half to it.
        pthread_create(&id[1],NULL,Merge_sort,&newRight); /// create a thread and pass the right half to it.

        pthread_join(id[0],NULL);
        pthread_join(id[1],NULL);

        Merge_Arrays(lim->left,lim->right,Midpoint); /// Merge the current halves.
    }
    else
    {
        pthread_exit(0);
    }
}

void Merge_Arrays(int left, int right, int midpoint)
{
    leftindex = left; /// the starting index of the left half.
    rightindex = midpoint+1; /// the starting index of the right half.
    tempindex = 0;
    sortarray.assign((right-left)+1,0); /// ** assign a new vector to store the sorted piece in it ** ///


    while(leftindex <= midpoint || rightindex <= right)   /// while either one of the 2 halves has elements.
    {

        if(leftindex <= midpoint && rightindex <= right) /// if both have elements
        {
            if (mergearray[leftindex] <= mergearray[rightindex])
            {
                sortarray[tempindex] = mergearray[leftindex];
                tempindex++;
                leftindex++;
            }
            else
            {
                sortarray[tempindex] = mergearray[rightindex];
                tempindex++;
                rightindex++;
            }
        }
        else if(leftindex <= midpoint) /// if only the left half has elements
        {
            sortarray[tempindex] =mergearray[leftindex]; /// sort
            tempindex++;
            leftindex++;

        }
        else if(rightindex <= right) /// if only the right half has elements
        {
            sortarray[tempindex] = mergearray[rightindex]; /// sort
            tempindex++;
            rightindex++;

        }
    }

    tempindex =0;
    for (leftindex=left; leftindex<=right; leftindex++)
    {
        /** return the sorted elements to their original place in the main array**/
        mergearray[leftindex] = sortarray[tempindex];
        tempindex++;
    }

}
void read_input() /// function to read the list of elements from the file
{
    fstream file;
    string fname;
    string word;
    fname = "input.txt";
    file.open(fname.c_str());
    int n = 0;
    while (file >> word)
    {
        n++; /// scan all the letters to know the size of the list
    }

    file.clear(); /// return the pointer of the file to the first letter
    file.seekg(0);

    mergearray.assign(n-1,0); /// assign size to the main array
    int i = 0;
    int c = 0;
    while (file >> word)
    {
        if(i == 0) /// if first letter
        {
            SIZE = stoi(word); /// store it in SIZE variable
        }
        else
        {
            mergearray[c++] = stoi(word); /// convert to int and store in the array.
        }
        i++;
    }

}

int main()
{
    read_input(); /// populate the main array.

    if(SIZE == 0 || SIZE ==1)
    {
        cout << "List already sorted" << endl;
        return 0;
    }

    cout << "unsorted list: " << endl; /// print the unsorted list
    for(int i=0; i<SIZE; i++)
        cout << mergearray[i] <<"  " ;
    cout << endl << endl;

    struct limits l; /// define a struct to hold the left and right limits of the main array
    l.left = 0;
    l.right = SIZE-1;

    pthread_t id;
    pthread_create(&id,NULL,&Merge_sort,&l); /// pass the struct to a thread
    pthread_join(id, NULL);

    cout << "sorted list: " << endl; /// print the sorted list
    for(int i=0; i<SIZE; i++)
        cout <<mergearray[i] <<"  ";
    cout << endl;

    return 0;
}
