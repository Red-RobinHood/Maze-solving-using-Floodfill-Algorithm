#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int n, x, y, o=0, write=0, read=0;

int** createMaze(){
    int** maze = (int**)malloc((n+2) * sizeof(int*));
    for(int i=0; i<=(n+1)/2; i++) {
        *(maze+i) = (int*)malloc((n+2) * sizeof(int));
        *(maze+n+1-i) = (int*)malloc((n+2) * sizeof(int));
        for(int j=0; j<=(n+1)/2; j++) {
            *(*(maze+i)+j) = (n+1)/2*2-i-j;
            *(*(maze+i)+n+1-j) = (n+1)/2*2-i-j;
            *(*(maze+n+1-i)+n+1-j) = (n+1)/2*2-i-j;
            *(*(maze+n+1-i)+j) = (n+1)/2*2-i-j;
        }
    }
    return maze;
}

int** createData(){
    int** data = (int**)malloc((n+2) * sizeof(int*));
    for(int i=0; i<n+2; i++) {
        *(data+i) = (int*)malloc((n+2) * sizeof(int));
        for(int j=0; j<n+2; j++) {
            *(*(data+i)+j) = 2;
        }
    }
    return data;
}

void resetMaze(int ** maze, int ** queue, int k){
    for(int i=0; i<n+2; i++) {
        for(int j=0; j<n+2; j++) {
            *(*(maze+i)+j) = n*n;
        }
    }
    read=0, write=0;
    if(k==0){
        for(int i=(n+1)/2; i<=n/2+1; i++) {
            for(int j=(n+1)/2; j<=n/2+1; j++) {
                *(*(maze+i)+j) = 0;
            }
        }
        for(int i=0; i<=(n-1)%2; i++) {
            for(int j=0; j<=(n-1)%2; j++) {
                *(*(queue+0)+write) = (n+1)/2+i;
                *(*(queue+1)+write) = (n+1)/2+j;
                write++;
            }
        }
    }
    else{
        *(*(maze+n)+1) = 0;
        *(*(queue+0)+write) = n;
        *(*(queue+1)+write) = 1;
        write++;
    }

}

int** createQueue(){
    int** queue = (int**)malloc(2 * sizeof(int*));
    *(queue+0) = (int*)malloc(n*n * sizeof(int));
    *(queue+1) = (int*)malloc(n*n * sizeof(int));
    return queue;
}

void getconfig(int** data){
    *(*(data+y)+x)
    =   (*(*(data+y)+x)/1000)    %2*pow(10,(o+3)%4)
    +   (*(*(data+y)+x)/100)     %2*pow(10,(o+2)%4)
    +   (*(*(data+y)+x)/10)      %2*pow(10,(o+1)%4)
    +   (*(*(data+y)+x))         %2*pow(10,o%4);
}

int dec(int** maze, int** data){
    int f, r, l;
        f=   maze[y+(o%2==0)*(o-1)][x-(o%2==1)*(o-2)]
        +       (data[y+(o%2==0)*(o-1)][x-(o%2==1)*(o-2)]!=-1)*512
        +       (data[y][x]/10)%2*1024;
        r=   maze[y-(o%2==1)*(o-2)][x-(o%2==0)*(o-1)]
        +       (data[y-(o%2==1)*(o-2)][x-(o%2==0)*(o-1)]!=-1)*512
        +       (data[y][x]/100)%2*1024;
        l=   maze[y+(o%2==1)*(o-2)][x+(o%2==0)*(o-1)]
        +       (data[y+(o%2==1)*(o-2)][x+(o%2==0)*(o-1)]!=-1)*512
        +       (data[y][x])%2*1024;
    int dec=-2*(1-f/1024)*(f<=r)*(f<=l)-(r<f)*(r<=l)*(1-r/1024)+(l<f)*(l<r)*(1-l/1024)+2;
    return dec;
}

void turn(int dec){
    x+=((o+dec)%2)*(2-(o+dec)%4);
    y+=((o+dec+1)%2)*((o+dec)%4-1);
    o=(o+dec)%4;
    printf("y = %d, x = %d, o = %d\n",y,x,o);
}

void floodfill(int** maze, int** data, int** queue){
    int a,b;
    while(1){
        a=queue[0][read], b=queue[1][read];
        read++;
        int istrue;
        if(a<n){
            istrue=(1-(data[a+1][b]/10)%2)*(1-data[a][b]/1000)*(maze[a][b]+1<maze[a+1][b]);
            maze[a+1][b]+=(maze[a][b]+1-maze[a+1][b])*istrue;
            queue[0][write*istrue]=a+1;
            queue[1][write*istrue]=b;
            write+=istrue;
            }
        if(a>1){
            istrue=(1-(data[a][b]/10)%2)*(1-data[a-1][b]/1000)*(maze[a][b]+1<maze[a-1][b]);
            maze[a-1][b]+=(maze[a][b]+1-maze[a-1][b])*istrue;
            queue[0][write*istrue]=a-1;
            queue[1][write*istrue]=b;
            write+=istrue;
            }
        if(b<n){
            istrue=(1-(data[a][b]/100)%2)*(1-data[a][b+1]%2)*(maze[a][b]+1<maze[a][b+1]);
            maze[a][b+1]+=(maze[a][b]+1-maze[a][b+1])*istrue;
            queue[0][write*istrue]=a;
            queue[1][write*istrue]=b+1;
            write+=istrue;
            }
        if(b>1){
            istrue=(1-(data[a][b-1]/100)%2)*(1-data[a][b]%2)*(maze[a][b]+1<maze[a][b-1]);
            maze[a][b-1]+=(maze[a][b]+1-maze[a][b-1])*istrue;
            queue[0][write*istrue]=a;
            queue[1][write*istrue]=b-1;
            write+=istrue;
            }
        if(read==write) 
            break;

    }
}

int main(){
    printf("Welcome to the maze solver!\n");
    printf("The maze is a square maze with a size of n*n.\n");
    printf("I made it so dumb people who can not solve a maze can use this to come out of the maze.\n");
    printf("The fact that you are using this means you are dumb.\n");
    printf("The starting point of the maze is the bottom left corner and the ending point is one of the centre squares\n");
    printf("The maze can be of any size but the size should be a natural number.\n");
    printf("When prompted to tell the maze configuration, enter the configuration of the maze in the following format:\n");
    printf("The first digit is the back wall, the second digit is the right wall, the third digit is the front wall and the fourth digit is the left wall.\n");
    printf("If there is a wall, enter 1, else enter 0.\n");
    printf("For example, if there is a wall in the back and front, enter 1010.\n");
    printf("If there is no wall, enter 0000.\n");
    printf("The coordinates of you are printed on the screen along with your orientation.\n");
    printf("The orientation is printed as 0 for North, 1 for East, 2 for South and 3 for West.\n");
    printf("Always enter the maze configuration in the same orientation as you are facing.\n");
    printf("If your dumb brain still cannot process how to use this please contact my great creator Robin Hood");
    printf("\nEnter the size of the maze: ");
    scanf("%d", &n);
    int** maze = createMaze();
    int** data = createData();
    int** queue = createQueue();
    x=1, y=n;
    int d;
    printf("%d %d %d\n", y, x, o);
        while(1){
            printf("Enter maze cofiguration = ");
            scanf("%d", &data[y][x]);
            d=dec(maze, data);
            getconfig(data);
            //printf("dec = %d\n", d);
            turn(d);
            resetMaze(maze, queue,0);
            floodfill(maze, data, queue);
            if(maze[y][x]==0)
                break;
        }
        printf("Destination reached!\n");

    printf("%d %d %d\n", y, x, o);
        while(1){
            printf("\nEnter maze cofiguration = ");
            scanf("%d", &data[y][x]);
            d=dec(maze, data);
            getconfig(data);
            //printf("dec = %d\n", d);
            turn(d);
            resetMaze(maze, queue,1);
            floodfill(maze, data, queue);
            if(y==n && x==1)
                break;
        }
        printf("Starting Point reached!\n");
    return 0;
}