#include<stdio.h>
int counte(int n){
int i=1,c=0;
while(i<=n){
i=i<<1; c++;

}
return c;
}
int sendersum(int x,int cnt){
    int k=0,num=15,sum=0,j,t;
if(cnt<4) return ~x;
 else{
    k=cnt/4+1;
    for(j=0;j<k;j++){
    t=x&num;
    t=t>>j*4;
    sum+=t;
    num=num<<4;
    }

 }
int ff=15;
return ~sum&ff;
}
int receiversum(int x,int cnt,int sendsum){
int k=0,num=15,sum=0,j,t;
    k=cnt/4+1;
    for(j=0;j<k;j++){
    t=x&num;
    t=t>>j*4;
    sum+=t;
    num=num<<4;

    }
    sum+=sendsum;
    return ~sum&15;
}
int main(){
 int x,cnt=0,k=0,num=15,sum=0,j,t,n;
 printf("Enter the size:\n");
 scanf("%d",&n);
 printf("\nEnter all the data:");
 for(j=0;j<n;j++)
 {scanf("%d",&x); sum+=x; }
 cnt=counte(sum);
 int sendsum=sendersum(sum,cnt);
 printf("\nSender sum = %d",sendsum);
 printf("\nReceiver sum = %d",receiversum(sum,cnt,sendsum));
 return  0;
}
