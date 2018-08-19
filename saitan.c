#include <stdio.h>
#include <stdlib.h>
#define N 81      // 頂点の数
#define M 99.0   // M = 重み最大値(無限大の代わり)
#define H 21     //配送先+SP

int a[H] = {39,42,50,57,48,47,30,13,32,52,60,68,75,64,46,19,20,12,14,24,43};  //SP,A,B,...T
static char V[H+1] = "sABCDEFGHIJKLMNOPQRST";
double w[N][N];   //重み行列(iからjへの重み)、重み：ノード間経路長またはコスト
double s[H][H];   //最短コスト経路図を作るための配列
double cost[N]; // 開始頂点からの重み和
int visit[N]; // 未訪問=0, 訪問済み=1
int parent[N]; // 直前(parent)の頂点番号
void shortest_path(void);
void rinsetugyouretu();
void greedy();
int start; // 開始頂点


int main(void)
{
  int j,k,p,i,o;
  double min;

  rinsetugyouretu();  //隣接行列生成
  
  for(i=0; i<H; i++){
    for(j=0; j<H; j++){
      s[i][j] = 0.0;
    }
  }

  for(start=0; start<81; start++){  //すべてのダイクストラ表を作る

    for(k=0; k<N; k++)
      {                  // 開始時の全長点：条件設定
	cost[k] = M;   // 開始頂点からの重み和 D[.] = ∞
	visit[k] = 0;  // 未訪問(EV-X)
	parent[k] = -2;  // 直前頂点は無い=-2
      }
    
    cost[start] = 0;  // 開始頂点の重み和=0
    parent[start] = -1; // 直前の頂点番号=-1

    for(j=0; j<N; j++)  // while(X!=V)の代わりにノード個数分だけループを回す
      {
	min = M;
	for(k=0; k<N; k++)  // 未訪問＆重み和最小の頂点を探す
	  {
	    if(visit[k] == 0 && cost[k] < min)
	      {
		min = cost[k];
		p = k;  // p:未訪問＆重み和最小の頂点番号
	      }
	  }
	               // 条件を満たす重み和最小の頂点pが見つかった
	visit[p] = 1;  // 頂点pを訪問済
	for(k=0; k<N; k++)
	  {
	    if(visit[k] == 1)
	      {
		continue;
	      }
	    if((cost[p] + w[p][k]) < cost[k])
	      {
		cost[k] = cost[p] + w[p][k];
		parent[k] = p;
	      }
	  }
      }
    shortest_path();
  }
  greedy();
}


void shortest_path(void)
{
  int i,k,p,q,path[N],o;

  for(k=0; k<N; k++)  // startから頂点kへの最短経路の表示:親ノードをさかのぼる
    {
      if(cost[k] == M)
	{
	  continue;
	}
      p = k;
      q = N-1;
      path[q--] = k;
      while(parent[p] != -1)
	{
	  path[q--] = parent[p];
	  p = parent[p];
	}
      path[q] = parent[p]; // p = start 開始地点
      
      //配送先間の重みをとる
      for(i=0; i<H; i++){
	if(a[i] == start){
	  for(o=0; o<H; o++){
	    if(a[o] == k){
	      s[i][o] = cost[k];
	    }
	  }
	}
      }
    }
}


void greedy(){
  int i,j,g,r,h,tugi,f;  //gは現在地、tugiは次の地点
  int keiro[H+1],hkeiro[H+1];  //経路保存用
  double min;
  double time, htime = 999.0;
  int flag[H];

  for(r=0; r<100; r++)  // 同コストの分岐点でランダム選択して一番良い結果を得る
    {
      time = 0.0;
      keiro[0] = 0;  //スタートはSPのため
      g = 0;  //スタート位置
      flag[0] = 1;  //二回目でSPに戻ってこないように
      
      for(i=1; i<H; i++) //最初以外を未探索(flag = 0)にする。探索済みは(flag = 1)
	{
	  flag[i] = 0;
	}
      
      for(j=0; j<H; j++)
	{
	  min = M;
	  for(i=0; i<H; i++)
	    {
	      if(s[g][i] != 0 && s[g][i] < min && flag[i] == 0)
		// 同じ場所でなくコストが小さくなりまだ未探索のとき
		{
		  min = s[g][i];
		  tugi = i;
		}
	      else if (s[g][i] != 0 && s[g][i] == min && flag[i] == 0)
		{
		  if(rand()%2 == 0)
		    {
		      min = s[g][i];
		      tugi = i;
		    }
		}
	    }
	  flag[tugi] = 1;
	  g = tugi;
	  if(j < H-1)
	    {
	      time += min;
	      // printf("%2d:%2.0lf\n",j+1,time);
	    }
	  keiro[j+1] = tugi;
	}
      
      // 最後にSPに戻らなくてはならない
      time += s[g][0];
      printf("%2d回目:%2d:%2.0lf\n",r+1,H,time);
      keiro[H] = 0;
  
      // 経路の更新
      if(time < htime)
	{
	  printf("更新\n");
	  htime = time;
	  for(h=0; h<H; h++)
	    {
	      hkeiro[h] = keiro[h];
	    }
	}
    }
  
  // 結果表示
  printf("\n\n配送順序:");
  for(i=0; i<H+1; i++)
    {
      if(i != H)
	{
	  printf("%c-",V[hkeiro[i]]);
	}
      else
	{
	  printf("%c",V[hkeiro[i]]);
	}
    }
  printf("\ntime = %2.2lf\n",htime/6);
}


// 隣接行列作成
void rinsetugyouretu(){
  int i,j;

  for(i = 0;i < 81;i++){
    for(j = 0;j < 81;j++)
      w[i][j] = M;
  }
  for(i=0; i<81; i++){
    //左上の処理
    if(i  == 0){
      w[i][i + 1] = 1;
      w[i][i + 9] = 1;
    }
    //左下の処理
    else if(i == 72){
      w[i][i + 1] = 1;
      w[i][i -9] = 1;
    }
    //左端の処理
    else if(i%9 == 0 && (i != 0 || i != 72)){
      w[i][i + 1] = 1;
      w[i][i -9] = 1;
      w[i][i + 9] = 1;
    }
    //右上の処理
    else if(i == 8){
      w[i][i-1] = 1;
      w[i][i+9] = 1;
    }
    //右下の処理
    else if(i == 80){
      w[i][i-1] = 1;
      w[i][i-9] = 1;
    }
    //右端の処理
    else if(i%9 == 8 && i != 8 && i != 80){
      w[i][i-1] = 1;
      w[i][i-9] = 1;
      w[i][i+9] = 1;
    }
    //上側の処理
    else if(i>0 && i<8){
      w[i][i-1] = 1;
      w[i][i+1] = 1;
      w[i][i+9] = 1;
    }
    //下側の処理
    else if(i<80 && i>72 && j>72){
      w[i][i-1] = 1;
      w[i][i+1] = 1;
      w[i][i-9] = 1;
    }
    //それ以外の処理
    else if(i%9 != 0 && i%9 != 8 && i>8 && i<72){
      w[i][i-1] = 1;
      w[i][i+1] = 1;
      w[i][i+9] = 1;
      w[i][i-9] = 1;
    }
  }
}

