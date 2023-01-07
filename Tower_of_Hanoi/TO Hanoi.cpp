#include <bits/stdc++.h>


using namespace std;

void TOH(int n, char t,char e,char f){
    if(n>0){
     TOH(n-1,t,f,e);
     printf("Move a disc from %c to %c.\n",t,f);
     TOH(n-1,e,t,f);
    }
}
const char a= 'A';
const char b = 'B',c ='C';
int main(){
    cout<<"               |     |     |\n               |     |     |\n               |     |     |\n               |     |     |\n               |     |     |\n               |     |     |\n             __|_____|_____|__\n            |_/A/___/B/___/C/_| \n\n\n";

    cout<<"Enter the number of discs in the tower of Hanoi. : ";
    int n;
    cin>>n;
    cout<<"The steps are as follows:-\n";
    TOH(n,a,b,c);
}
