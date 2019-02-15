#include<iostream>
#include<fstream>
#include<queue>
#include<cmath>
#include<vector>
#include<iterator>
#include<algorithm>
#include<iomanip>
#include<windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;
const int sodiemxet = 50; // so diem xet tu toa do dau den dot
int hang, cot,dotantoan = 0, dot=0, sodot = 0, dem = 0;
bool ghostdot, chanle; //chan true, le false
char map[200][200];
char trongso[200][200]; //bieu dien trong so cac vi tri tren map
int mapblock[200][200];
int mapdot[200][200];
ofstream fileoutput("PHistory.txt");
void gotoxy(int x, int y) {
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
} 
class toado{
	double fx; // danh gia f(x)
	int gx; //danh gia g(x)
public:
	int x;
	int y;
	int xback;
	int yback; //khai bao 2 bien back
	toado(){
		x = 0;
		y = 0;
		fx = 0;
		gx = 0;
		xback = 0;
		yback = 0;
	}
	toado(int x, int y){
		this->x = x;
		this->y = y;
	}
	void setfx(toado td); 
	void operator =(const toado& td1){ //dinh nghia toan tu gan
		this->x = td1.x;
		this->y = td1.y;
		this->fx = td1.fx;
		this->gx = td1.gx;
		this->xback = td1.xback;
		this->yback = td1.yback;
	}
	bool operator ==(const toado& td1){ // dinh nghia toan tu ==
		if (this->x == td1.x && this->y == td1.y) return true;
		else return false;
	}
	bool operator >(const toado& td1){ // dinh nghia toan tu >
		if (this->fx > td1.fx) return true;
		else return false;
	}
} toadodau, toadodich, toadoghost;
class toadodg { // kieu du lieu toa do don gian hon
public:
	int x, y;
	double fx;
	bool operator >(const toadodg &td) {
		return this->fx > td.fx;
	}
	bool operator ==(const toadodg &td){
		return this->x == td.x && this->y == td.y;
	}
	toadodg(int x, int y) {
		this->x = x;
		this->y = y;
		this->fx = pow(this->x - toadodau.x, 2) + pow(this->y - toadodau.y, 2);
	}
};
double khoangcach(toado t1, toado t2){
	return sqrt(pow(t1.x - t2.x, 2) + pow(t1.y - t2.y, 2));
};
double khoangcach(toadodg t1, toadodg t2) {
	return sqrt(pow(t1.x - t2.x, 2) + pow(t1.y - t2.y, 2));
};
void toado::setfx(toado td){
	this->gx = td.gx + 1;
	this->fx = this->gx + khoangcach(*this,toadodich); // dánh giá heuristic
};
class nhanvat{
public:
	toado* td;
	bool timduong(vector<toadodg> &chiduong);
	bool dichuyen(vector<toadodg> &chiduong);
	bool phantichmap(vector<toadodg> &chiduong, bool &skip);
	void len() {
		map[td->y][td->x] = ' ';
		td->y--;
		if(map[td->y][td->x]=='.') {sodot--; dot++;}
		map[td->y][td->x] = 'v';
	}
	void xuong() {
		map[td->y][td->x] = ' ';
		td->y++;
		if(map[td->y][td->x]=='.') {sodot--; dot++;}
		map[td->y][td->x] = '^';
	}
	void trai() {
		map[td->y][td->x] = ' ';
		td->x--;
		if(map[td->y][td->x]=='.') {sodot--; dot++;}
		map[td->y][td->x] = '>';
	}
	void phai() {
		map[td->y][td->x] = ' ';
		td->x++;
		if(map[td->y][td->x]=='.') {sodot--; dot++;}
		map[td->y][td->x] = '<';
	}
} pacman;
struct cmp1{
	bool operator() (toadodg t1, toadodg t2) { return (t1 > t2); }
};
struct cmp{
	bool operator() (toado t1, toado t2) { return (t1 > t2); }
};
void taokhoiblock(int x, int y, priority_queue<toadodg, vector<toadodg>, cmp1> &tap, vector<toadodg> &v){
	toadodg td(x,y);
	if(x>=0 && x<cot && y>=0 && y<hang && mapblock[y][x]==35 && find(v.begin(),v.end(),td)==v.end()){
		tap.push(td);
		v.push_back(td);
	}
}
void datlaimap(char copymap[200][200]){
	for(int i=0;i<hang;i++)
		for(int j=0;j<cot;j++)
			map[i][j]=copymap[i][j];
}
bool hamdatdot(int x, int y, vector<toadodg> &v){
	pacman.td->x=x;
	pacman.td->y=y;
	for(int t=0;t<v.size();t++){
		toadodich.x = v.at(t).x;
		toadodich.y = v.at(t).y;
		vector<toadodg> vt;
 		if(pacman.timduong(vt) || toadodich == toadodau) return true; 
	}
	return false;	
}
void docmap(ifstream& in){
	in >> hang >> cot;
	char trash[1];
	in.getline(trash, 1);
	int min = 100000;
	double mint = 100000;
	for (int i = 0; i<hang; i++)
		for (int j = 0; j<cot + 1; j++) {
			in.get(map[i][j]);
			if (map[i][j] != '#' && j!=cot && i*i + j*j < min) {
				min = i*i + j*j;
				pacman.td->x = j;
				pacman.td->y = i;
			}
			double t = pow(i - hang + 1,2) + pow(j - cot + 1,2);
			if (map[i][j] != '#' && j!=cot && t < mint) {
				mint = t;
				toadoghost.x = j;
				toadoghost.y = i;
			}
			if (map[i][j] =='.' && j!=cot) sodot++; 
			if (j!=cot) trongso[i][j] = map[i][j];
		}
	if(map[pacman.td->y][pacman.td->x]=='.') {sodot--; dot++;}
	map[pacman.td->y][pacman.td->x] = '<';
	if (map[toadoghost.y][toadoghost.x]=='.') ghostdot = true; else ghostdot = false;
	map[toadoghost.y][toadoghost.x] = 'G';
	bool stop;
	do{ //set trong so cho map
		stop = false;
		for(int i=0;i<hang;i++){
			for(int j=0;j<cot;j++)
				if(trongso[i][j]!='#' && trongso[i][j]!='-') trongso[i][j] = '0';
		}
		for (int i=0;i<hang;i++){
			for(int j=0;j<cot;j++){
				if(trongso[i][j]=='#' || trongso[i][j]=='-'){
					if(trongso[i+1][j]!='#' && trongso[i+1][j]!='-') trongso[i+1][j]++;
					if(trongso[i-1][j]!='#' && trongso[i-1][j]!='-') trongso[i-1][j]++;
					if(trongso[i][j+1]!='#' && trongso[i][j+1]!='-') trongso[i][j+1]++;
					if(trongso[i][j-1]!='#' && trongso[i][j-1]!='-') trongso[i][j-1]++;
				}
			}
		}
		for(int i=0;i<hang;i++){
			for (int j=0;j<cot;j++){
				if(trongso[i][j]=='3') {trongso[i][j] = '-'; stop = true;} // dau tru tuong duong voi tuong #
			}
		}
	} while(stop);
	for(int i=0;i<hang;i++){
		for(int j=0; j<cot;j++){
			if(trongso[i][j]=='#' || trongso[i][j]=='-') mapblock[i][j]=35;
			else mapblock[i][j]=0;
		}
	} //cap nhat cac o co so 3.
	int t=5; // tao cac block tuong
	for(int i=0;i<hang;i++){
		for(int j=0;j<cot;j++){
			if(mapblock[i][j]==35){
				priority_queue<toadodg, vector<toadodg>, cmp1> tap;
				vector<toadodg> v;
				toadodg td(j,i);
				tap.push(td);
				v.push_back(td);
				do{
					int x = tap.top().x;
					int y = tap.top().y;
					mapblock[y][x]=t;	
					tap.pop();
					taokhoiblock(x+1,y,tap,v);
					taokhoiblock(x-1,y,tap,v);
					taokhoiblock(x,y+1,tap,v);
					taokhoiblock(x,y-1,tap,v);
				    taokhoiblock(x+1,y+1,tap,v);
					taokhoiblock(x-1,y-1,tap,v);
					taokhoiblock(x-1,y+1,tap,v);
					taokhoiblock(x+1,y-1,tap,v);
					} while(!tap.empty());
				t++;
			}
		}
	}
	do{ 
		vector<int> tdx,tdy;
		int ymin,ymax,xmin,xmax;
		for(int i=0;i<hang;i++){
			for(int j=0;j<cot;j++){
				if(mapblock[i][j]==t-1) {
					tdx.push_back(j);
					tdy.push_back(i);
				}
			}
		}
		if(!tdx.empty() && !tdy.empty()){
		ymin = *min_element(tdy.begin(),tdy.end());
		ymax = *max_element(tdy.begin(),tdy.end());
		xmin = *min_element(tdx.begin(),tdx.end());
		xmax = *max_element(tdx.begin(),tdx.end());
		for(int i=ymin-1;i<=ymax+1;i++){
			if(mapblock[i][xmax+1]==0 && (mapblock[i][xmax]==t-1 || mapblock[i+1][xmax]==t-1 || mapblock[i-1][xmax]==t-1)) mapblock[i][xmax+1]=2;
			if(mapblock[i][xmin-1]==0 && (mapblock[i][xmin]==t-1 || mapblock[i+1][xmin]==t-1 || mapblock[i-1][xmin]==t-1)) mapblock[i][xmin-1]=2;
		}
		for(int j=xmin-1;j<=xmax+1;j++){
			if(mapblock[ymax+1][j]==0 && (mapblock[ymax][j]==t-1 || mapblock[ymax][j+1]==t-1 || mapblock[ymax][j-1]==t-1)) mapblock[ymax+1][j]=2;
			if(mapblock[ymin-1][j]==0 && (mapblock[ymin][j]==t-1 || mapblock[ymin][j+1]==t-1 || mapblock[ymin][j-1]==t-1)) mapblock[ymin-1][j]=2;
		}
		}
		t--;	
	} while(t>6);
	char copymap[200][200];
	for(int i=0;i<hang;i++)
		for(int j=0;j<cot;j++){
			if(map[i][j]=='#') mapdot[i][j]=35;
			else mapdot[i][j]=0;
			copymap[i][j]=map[i][j];
		}
	int xp = pacman.td->x;
	int yp = pacman.td->y;
	for(int i=1;i<hang-1;i++){
		datlaimap(copymap);
		vector<toadodg> v;
		for(int j=0;j<cot;j++)	map[i-1][j]='#';
		for(int h=i;h<hang;h++)
			for(int c=0;c<cot;c++)
				if(mapblock[h][c]==2){
					toadodg td(c,h);
					v.push_back(td);
				}
		for(int j=0;j<cot;j++){
			if(map[i][j]!='#' && !hamdatdot(j,i,v)) mapdot[i][j]++;
			}
	}
	for(int i=1;i<hang-1;i++){
		datlaimap(copymap);
		vector<toadodg> v;
		for(int j=0;j<cot;j++)	map[i+1][j]='#';
		for(int h=0;h<=i;h++)
			for(int c=0;c<cot;c++)
				if(mapblock[h][c]==2){
					toadodg td(c,h);
					v.push_back(td);
				}
		for(int j=0;j<cot;j++){
			if(map[i][j]!='#' && !hamdatdot(j,i,v)) mapdot[i][j]++;
			}
	}
	for(int j=1;j<cot-1;j++){
		datlaimap(copymap);
		vector<toadodg> v;
		for(int i=0;i<hang;i++)	map[i][j-1]='#';
		for(int h=0;h<hang;h++)
			for(int c=j;c<cot;c++)
				if(mapblock[h][c]==2){
					toadodg td(c,h);
					v.push_back(td);
				}
		for(int i=0;i<hang;i++){
			if(map[i][j]!='#' && !hamdatdot(j,i,v)) mapdot[i][j]++;
		}
	}
	for(int j=1;j<cot-1;j++){
		datlaimap(copymap);
		vector<toadodg> v;
		for(int i=0;i<hang;i++)	map[i][j+1]='#';
		for(int h=0;h<hang;h++)
			for(int c=0;c<=j;c++)
				if(mapblock[h][c]==2){
					toadodg td(c,h);
					v.push_back(td);
				}
		for(int i=0;i<hang;i++){
			if(map[i][j]!='#' && !hamdatdot(j,i,v)) mapdot[i][j]++;
		}
	}
	pacman.td->x = xp;
	pacman.td->y = yp;
	datlaimap(copymap);
	for(int i=0;i<hang;i++)
		for(int j=0;j<cot;j++)
			 if(mapdot[i][j]==0 && map[i][j]=='.') dotantoan++;
	for(int i=0;i<hang;i++){
		for(int j=0;j<cot;j++){
			 cout << setw(2) << mapblock[i][j];
		}
		cout << endl;
	}
	
	for(int i=0;i<hang;i++){
		for(int j=0;j<cot;j++){
			cout << setw(2) << mapdot[i][j];
		}
		cout << endl;
	}
	cout <<"Tong so dot an toan co the an duoc: " << dotantoan << endl;
	cout <<"Chiem ti le: " << static_cast<float>(dotantoan) / sodot << endl;
	system("pause");
	system("cls");
}
toado chuyendoi(int x, int y, toado &td){
	toado temp(x, y);
	temp.setfx(td);
	temp.xback = td.x;
	temp.yback = td.y;
	return temp;
}; // chuyen doi x,y thanh kieu toado
void bosung(int x, int y,toado &temp, priority_queue<toado, vector<toado>, cmp> &tapmo, vector<toado> &tapmo1) {
	toado td = chuyendoi(x, y, temp);
	if (map[y][x] != '#' && map[y][x]!='G' && find(tapmo1.begin(), tapmo1.end(), td) == tapmo1.end()) {
		tapmo1.push_back(td);
		tapmo.push(td);
	}
}
void fileluotchoi(){
	char turn = '\0';
		int xghost, yghost;
		do{
			ifstream lcin("luotchoi.txt");
			lcin >> turn >> yghost >> xghost;
			lcin.close();
			if (turn!='G') Sleep(50);
		} while(turn!='G');
		if(ghostdot) map[toadoghost.y][toadoghost.x]='.';  else map[toadoghost.y][toadoghost.x]=' ';
		toadoghost.x = xghost;
		toadoghost.y = yghost;
		if(map[toadoghost.y][toadoghost.x]=='.') ghostdot = true; else ghostdot = false;
		map[toadoghost.y][toadoghost.x]='G';
}
void loaibodotkhonganduoc(){
	for(int i=0;i<hang;i++){
			for(int j=0;j<cot;j++){
				if(map[i][j]=='.') {
					vector<toadodg> v;
					toadodich.x = j;
					toadodich.y = i;
					if(!pacman.timduong(v)) map[i][j] ='x'; // danh 'x' tat ca cac dot khong the an duoc ngay khi bat dau game.
				}
			}
		}
}
int timghost(vector<toadodg> &v){
	int xtemp =toadodich.x;
	int ytemp = toadodich.y;
	toadodich = toadoghost;
	map[toadoghost.y][toadoghost.x]='g';
	pacman.timduong(v);
	map[toadoghost.y][toadoghost.x]='G';
	toadodich.x = xtemp;
	toadodich.y = ytemp;
	return v.size();
}
bool kiemtracacphia(int x, int y){
	if(mapblock[y+1][x]==1 || mapblock[y+1][x]>5) return true;
	if(mapblock[y-1][x]==1 || mapblock[y-1][x]>5) return true;
	if(mapblock[y][x+1]==1 || mapblock[y][x+1]>5) return true;
	if(mapblock[y][x-1]==1 || mapblock[y][x-1]>5) return true;
	if(mapblock[y+1][x+1]==1 || mapblock[y+1][x+1]>5) return true;
	if(mapblock[y-1][x-1]==1 || mapblock[y-1][x-1]>5) return true;
	if(mapblock[y-1][x+1]==1 || mapblock[y-1][x+1]>5) return true;
	if(mapblock[y+1][x-1]==1 || mapblock[y+1][x-1]>5) return true;
	return false;
}
bool chedoA(){ //che do thu di truoc 1 nuoc.
	char copymap[200][200];
	for(int i=0;i<hang;i++) 
		for(int j=0;j<cot;j++)
			copymap[i][j]=map[i][j]; //tao copy map
	int xp=pacman.td->x; //luu lai gia tri cua pacman
	int yp=pacman.td->y;
	vector<toadodg> vtruoc,vsau;
	double t=pow(xp-toadoghost.x,2)+pow(yp-toadoghost.y,2); 
	if (pow(xp+1-toadoghost.x,2)+pow(yp-toadoghost.y,2) > t && map[yp][xp+1]!='#' && map[yp][xp+1]!='G'){ 
		toadodg td(xp+1,yp);
		vtruoc.push_back(td);
	}
	if (pow(xp-1-toadoghost.x,2)+pow(yp-toadoghost.y,2) > t && map[yp][xp-1]!='#' && map[yp][xp-1]!='G'){
		toadodg td(xp-1,yp);
		vtruoc.push_back(td);
	}
	if (pow(xp-toadoghost.x,2)+pow(yp+1-toadoghost.y,2) > t && map[yp+1][xp]!='#' && map[yp+1][xp]!='G'){
		toadodg td(xp,yp+1);
		vtruoc.push_back(td);
	}
	if (pow(xp-toadoghost.x,2)+pow(yp-1-toadoghost.y,2) > t && map[yp-1][xp]!='#' && map[yp-1][xp]!='G'){
		toadodg td(xp,yp-1);
		vtruoc.push_back(td);
	} // vtruoc chua 4 huong toa do ma pacman co the di trong nuoc ke tiep
	for(int i=0;i< vtruoc.size();i++){
		vector<toadodg> tapdot0,tapdot1,tapdot2;
		priority_queue<toadodg, vector<toadodg>, cmp1> tapdieman; // chua cac diem se an khi xong cong doan phan tich
		datlaimap(copymap); // tra lai map nhu cu vong lap quan trong
		pacman.td->x = vtruoc.at(i).x;
		pacman.td->y = vtruoc.at(i).y; // bat dau gan
		vector<toadodg> tap; // chua toa do cac diem la tuong hoac 
		if(pacman.td->x == toadoghost.x && pacman.td->y > toadoghost.y){//1
			for(int c=0; c< cot ;c++ )
				if(mapblock[pacman.td->y-1][c] < 5) map[pacman.td->y-1][c] ='#';
			for(int c=0; c<cot;c++)
				for(int h=pacman.td->y;h<hang;h++){
					toadodg td(c,h);
					if(mapblock[h][c] == 2 && kiemtracacphia(c,h))	tap.push_back(td);
				}
		}
   		if(pacman.td->x == toadoghost.x && pacman.td->y < toadoghost.y){//2
   			for(int c=0;c<cot;c++)
   				if(mapblock[pacman.td->y+1][c] < 5) map[pacman.td->y+1][c] ='#';
			for(int c=0; c<cot;c++)
				for(int h=pacman.td->y;h>=0;h--){
					toadodg td(c,h);
					if(mapblock[h][c] == 2 && kiemtracacphia(c,h))	tap.push_back(td);
				}	
   		}
   		if(pacman.td->x > toadoghost.x && pacman.td->y == toadoghost.y){//3
   			for(int h=0;h<hang;h++)
   				if(mapblock[h][pacman.td->x-1] < 5) map[h][pacman.td->x-1] ='#';
			for(int h=0; h<hang;h++)
				for(int c=pacman.td->x;c<cot;c++){
					toadodg td(c,h);
					if(mapblock[h][c] == 2 && kiemtracacphia(c,h))	tap.push_back(td);
				}	
   		}
		if(pacman.td->x < toadoghost.x && pacman.td->y == toadoghost.y){//4
			for(int h=0;h<hang;h++)
				if(mapblock[h][pacman.td->x+1] < 5) map[h][pacman.td->x+1] ='#';
			for(int h=0; h<hang;h++)
				for(int c=pacman.td->x;c>=0;c--){
					toadodg td(c,h);
					if(mapblock[h][c] == 2 && kiemtracacphia(c,h))	tap.push_back(td);
				}	
		}
		if(pacman.td->x < toadoghost.x && pacman.td->y < toadoghost.y){//5
			for(int h=0;h<=pacman.td->y+1;h++)
				if(mapblock[h][pacman.td->x+1] < 5) map[h][pacman.td->x+1] ='#';
			for(int c=0;c<=pacman.td->x+1;c++)
				if(mapblock[pacman.td->y+1][c] < 5) map[pacman.td->y+1][c] ='#';
			for(int h=pacman.td->y;h>=0;h--)
				for(int c=pacman.td->x;c>=0;c--){
					toadodg td(c,h);
					if(mapblock[h][c] == 2 && kiemtracacphia(c,h))	tap.push_back(td);
				}
		}
   		if(pacman.td->x > toadoghost.x && pacman.td->y > toadoghost.y){//6
   			for(int h=hang-1;h>=pacman.td->y-1;h--)
				if(mapblock[h][pacman.td->x-1] < 5) map[h][pacman.td->x-1] ='#';
			for(int c=cot-1;c>=pacman.td->x-1;c--)
				if(mapblock[pacman.td->y-1][c] < 5) map[pacman.td->y-1][c] ='#';
			for(int h=pacman.td->y;h<hang;h++)
				for(int c=pacman.td->x;c<cot;c++){
					toadodg td(c,h);
					if(mapblock[h][c] == 2 && kiemtracacphia(c,h))	tap.push_back(td);
				}
		}
   		if(pacman.td->x > toadoghost.x && pacman.td->y < toadoghost.y){//7
   			for(int h=0;h<=pacman.td->y+1;h++)
				if(mapblock[h][pacman.td->x-1] < 5) map[h][pacman.td->x-1] ='#';
			for(int c=cot-1;c>=pacman.td->x-1;c--)
				if(mapblock[pacman.td->y+1][c] < 5) map[pacman.td->y+1][c] ='#';
			for(int h=pacman.td->y;h>=0;h--)
				for(int c=pacman.td->x;c<cot;c++){
					toadodg td(c,h);
					if(mapblock[h][c] == 2 && kiemtracacphia(c,h))	tap.push_back(td);
				}
		}
   		if(pacman.td->x < toadoghost.x && pacman.td->y > toadoghost.y){//8
			for(int h=hang-1;h>=pacman.td->y-1;h--)
				if(mapblock[h][pacman.td->x+1] < 5) map[h][pacman.td->x+1] ='#';
			for(int c=0;c<=pacman.td->x+1;c++)
				if(mapblock[pacman.td->y-1][c] < 5) map[pacman.td->y-1][c] ='#';
			for(int h=pacman.td->y;h<hang;h++)
				for(int c=pacman.td->x;c>=0;c--){
					toadodg td(c,h);
					if(mapblock[h][c] == 2 && kiemtracacphia(c,h))	tap.push_back(td);
				}
		}//ktra xong
		int xtemp = toadodich.x; //luu gia tri cu 
		int ytemp = toadodich.y;
		for(int t=0;t<tap.size();t++){
			toadodich.x = tap.at(t).x;
			toadodich.y = tap.at(t).y;
			vector<toadodg> vt;
			if(pacman.timduong(vt) || toadodau==toadodich)	{
				tapdieman.push(tap.at(t));
				break;
			}
		}
		if(!tapdieman.empty())	vsau.push_back(vtruoc.at(i));
			toadodich.x = xtemp; //gan lai nhu cu
			toadodich.y = ytemp;
	}// end for
	pacman.td->x = xp; // tra lai gia tri cu cho pacman
	pacman.td->y = yp;
	datlaimap(copymap);
	if(vsau.empty()) {
		for(int i=0;i<vtruoc.size();i++){
			if(map[vtruoc.at(i).y][vtruoc.at(i).x]=='.' && trongso[vtruoc.at(i).y][vtruoc.at(i).x]!='-'){
				toadodich.x = vtruoc.at(i).x;
				toadodich.y = vtruoc.at(i).y;
				return false;
			}
		} // uu tien 1
		for(int i=0;i<vtruoc.size();i++){
			if(trongso[vtruoc.at(i).y][vtruoc.at(i).x]!='-'){
				toadodich.x = vtruoc.at(i).x;
				toadodich.y = vtruoc.at(i).y;
				return false;
			}
		} //uu tien 2
		if(vtruoc.size()!=0){
			int t= vtruoc.size();
			srand(time(NULL));
			int a = rand() % t + 0;
			toadodich.x = vtruoc.at(a).x;
			toadodich.y = vtruoc.at(a).y;
			return false;	//uu tien cuoi cung
		} else{
			if(map[yp+1][xp]!='#'){
				toadodich.x=xp;
				toadodich.y=yp+1;	
			}
			if(map[yp-1][xp]!='#'){
				toadodich.x=xp;
				toadodich.y=yp-1;	
			}
			if(map[yp][xp+1]!='#'){
				toadodich.x=xp+1;
				toadodich.y=yp;	
			}
			if(map[yp][xp-1]!='#'){
				toadodich.x=xp;
				toadodich.y=yp-1;	
			}
			return false;
		}
	}		
	else{ // tim duoc diem an toan.
		double min =100000;
		int x=0,y=0;
		for(int i=0;i<hang;i++)
			for(int j=0;j<cot;j++){
				if(map[i][j]=='.' && mapdot[i][j]==0){
					toadodich.x=j;
					toadodich.y=i;
					if(khoangcach(toadodau,toadodich) < min){
						min = khoangcach(toadodau,toadodich);
						x=j;
						y=i;
					}
				}
			}
		min = 100000;
		if(x!=0 || y!=0){
			for(int i=0;i<vsau.size();i++){
				if(map[vsau.at(i).y][vsau.at(i).x]=='.'){
					toadodich.x = vsau.at(i).x;
					toadodich.y = vsau.at(i).y;
					return true;
				}
			} // neu cac diem pacman dinh di chua dot thi an luon.	
			srand(time(NULL)); // random doi che do
			int b = rand() % 2 + 0;
			if(b){
				for(int i=0;i<vsau.size();i++){
					double d = pow(vsau.at(i).x - x,2) + pow(vsau.at(i).y-y,2);
					if(d<min){
						min = d;
						toadodich.x = vsau.at(i).x;
						toadodich.y = vsau.at(i).y; 
					}
				}
				return true;
			}
		}
		int t= vsau.size();
		srand(time(NULL));
		int a = rand() % t + 0;
		toadodich.x = vsau.at(a).x;
		toadodich.y = vsau.at(a).y;
		
	}
return true;
}
bool chedoB(){
	int xp=pacman.td->x; //luu lai gia tri cua pacman
	int yp=pacman.td->y;
	vector<toadodg> vtruoc,vsau;
	if ( map[yp][xp+1]!='#' && map[yp][xp+1]!='G'){ 
		toadodg td(xp+1,yp);
		vtruoc.push_back(td);
	}
	if ( map[yp][xp-1]!='#' && map[yp][xp-1]!='G'){
		toadodg td(xp-1,yp);
		vtruoc.push_back(td);
	}
	if ( map[yp+1][xp]!='#' && map[yp+1][xp]!='G'){
		toadodg td(xp,yp+1);
		vtruoc.push_back(td);
	}
	if ( map[yp-1][xp]!='#' && map[yp-1][xp]!='G'){
		toadodg td(xp,yp-1);
		vtruoc.push_back(td);
	} // vtruoc chua 4 huong toa do ma pacman co the di trong nuoc ke tiep
	for(int i=0;i< vtruoc.size();i++){
		pacman.td->x = vtruoc.at(i).x;
		pacman.td->y = vtruoc.at(i).y; 
		priority_queue<toadodg, vector<toadodg>, cmp1> tapdiemantoan;
		for(int h=0;h<hang;h++){
			for(int c=0;c<cot;c++){
				if(mapblock[h][c]==2){
					toadodg td(c,h);
					tapdiemantoan.push(td);
				}
			}
		}
		int t;
		if(tapdiemantoan.size() < sodiemxet) t = tapdiemantoan.size(); else t = sodiemxet; 
		int xmin=0,ymin=0,dPmin = 100000;
		for(int k=0;k<t;k++){
			toadodich.x=tapdiemantoan.top().x;
			toadodich.y=tapdiemantoan.top().y;
			vector<toadodg> vt;
			if(pacman.timduong(vt) || toadodau==toadodich){
				if (vt.size() < dPmin){
					dPmin = vt.size();
					xmin = tapdiemantoan.top().x;
					ymin = tapdiemantoan.top().y;
				}
			}
			tapdiemantoan.pop();
		}
		if(xmin!=0 || ymin!=0){
			pacman.td->x = xmin;
			pacman.td->y = ymin;
			vector<toadodg> vt;
			int dGmin = timghost(vt);
			if(dGmin - dPmin >= 2) vsau.push_back(vtruoc.at(i)); 
		}
	}// end for
	pacman.td->x = xp; // tra lai gia tri cu cho pacman
	pacman.td->y = yp;
	if(vsau.empty()) {
		int t= vtruoc.size();
		srand(time(NULL));
		int a = rand() % t + 0;
		toadodich.x = vtruoc.at(a).x;
		toadodich.y = vtruoc.at(a).y;
	}						
	else{
		priority_queue<toadodg, vector<toadodg>, cmp1> tapdiemkhongantoan;
		for(int i=0;i<hang;i++)
			for(int j=0;j<cot;j++){
				if(mapdot[i][j]>0 && mapdot[i][j]<35 && map[i][j]=='.'){
					toadodg td(j,i);
					tapdiemkhongantoan.push(td);
				}
			}
		int t;
		if(tapdiemkhongantoan.size() < sodiemxet) t = tapdiemkhongantoan.size(); else t = sodiemxet; 
		int xmin=0,ymin=0,dmin = 100000;
		for(int i=0;i<t;i++){
			toadodich.x=tapdiemkhongantoan.top().x;
			toadodich.y=tapdiemkhongantoan.top().y;
			vector<toadodg> vt;
			if(pacman.timduong(vt) || toadodau==toadodich){
				if (vt.size() < dmin){
					dmin = vt.size();
					xmin = tapdiemkhongantoan.top().x;
					ymin = tapdiemkhongantoan.top().y;
				}
			}
			tapdiemkhongantoan.pop();
		}
		int min=100000;
		if(xmin!=0 || ymin!=0){
			toadodich.x=xmin;
			toadodich.y=ymin;
			vector<toadodg> v;
			if(pacman.timduong(v)){
				vector<toadodg>::iterator it;
				it = find(vsau.begin(),vsau.end(),v.at(v.size()-1));
				if(it !=vsau.end()){
					toadodich.x=it->x;
					toadodich.y=it->y;
					return true;
				}
			}
			for(int i=0;i<vsau.size();i++){
				double d = pow(vsau.at(i).x - xmin,2) + pow(vsau.at(i).y-ymin,2);
				if(d<min){
					min = d;
					toadodich.x = vsau.at(i).x;
					toadodich.y = vsau.at(i).y; 
				}
			}
			return true;
		}
		for(int i=0;i<vsau.size();i++){
			if(map[vsau.at(i).y][vsau.at(i).x]=='.'){
				toadodich.x = vsau.at(i).x;
				toadodich.y = vsau.at(i).y;
				return true;
			} // neu cac diem pacman dinh di chua dot thi an luon.
		int t= vsau.size();
		srand(time(NULL));
		int a = rand() % t + 0;
		toadodich.x = vsau.at(a).x;
		toadodich.y = vsau.at(a).y;
		}		
	}
}
bool chedoC(int khoangcachPG){
	int sodotconthieu = (sodot + dot)*7/10 +1 - dot;
	if(khoangcachPG%2!=0){
		priority_queue<toadodg, vector<toadodg>, cmp1> tapdiemkhongantoan;
		for(int i=0;i<hang;i++)
			for(int j=0;j<cot;j++){
				if(mapdot[i][j]>0 && mapdot[i][j]<35 && map[i][j]=='.' && trongso[i][j]!='-'){
					toadodg td(j,i);
					tapdiemkhongantoan.push(td);
				}
			}
		int t;
		if(tapdiemkhongantoan.size() < sodiemxet) t = tapdiemkhongantoan.size(); else t = sodiemxet; 
		int xmin=0,ymin=0,dmin = 100000;
		for(int i=0;i<t;i++){
			toadodich.x=tapdiemkhongantoan.top().x;
			toadodich.y=tapdiemkhongantoan.top().y;
			vector<toadodg> vt;
			if(pacman.timduong(vt)){
				if (vt.size() < dmin){
					dmin = vt.size();
					xmin = tapdiemkhongantoan.top().x;
					ymin = tapdiemkhongantoan.top().y;
				}
			}
			tapdiemkhongantoan.pop();
		}
		if(xmin!=0 || ymin!=0){
			toadodich.x=xmin;
			toadodich.y=ymin;
			return true;
		}else chedoA();	
	} 
	
/*	int xp=pacman.td->x; //luu lai gia tri cua pacman
	int yp=pacman.td->y;
	vector<toadodg> vtruoc,vsau;
	double t=pow(xp-toadoghost.x,2)+pow(yp-toadoghost.y,2); 
	if (pow(xp+1-toadoghost.x,2)+pow(yp-toadoghost.y,2) > t && map[yp][xp+1]!='#' && map[yp][xp+1]!='G' && mapdot[yp][xp+1]!=0){ 
		toadodg td(xp+1,yp);
		vtruoc.push_back(td);
	}
	if (pow(xp-1-toadoghost.x,2)+pow(yp-toadoghost.y,2) > t && map[yp][xp-1]!='#' && map[yp][xp-1]!='G' && mapdot[yp][xp-1]!=0){
		toadodg td(xp-1,yp);
		vtruoc.push_back(td);
	}
	if (pow(xp-toadoghost.x,2)+pow(yp+1-toadoghost.y,2) > t && map[yp+1][xp]!='#' && map[yp+1][xp]!='G' && mapdot[yp+1][xp]!=0){
		toadodg td(xp,yp+1);
		vtruoc.push_back(td);
	}
	if (pow(xp-toadoghost.x,2)+pow(yp-1-toadoghost.y,2) > t && map[yp-1][xp]!='#' && map[yp-1][xp]!='G' && mapdot[yp-1][xp]!=0){
		toadodg td(xp,yp-1);
		vtruoc.push_back(td);
	}
	if(vtruoc.empty()) return false;
	if(pacman.td->x == toadoghost.x && pacman.td->y > toadoghost.y){//1
			for(int c=0; c< cot ;c++ )
				if(mapblock[pacman.td->y-1][c] < 5) map[pacman.td->y-1][c] ='#';
			for(int c=0; c<cot;c++)
				for(int h=pacman.td->y;h<hang;h++){
					toadodg td(c,h);
					if(mapblock[h][c] == 2 && kiemtracacphia(c,h))	tap.push_back(td);
				}
		}*/
}
bool nhanvat::phantichmap(vector<toadodg> &chiduong, bool &skip) {
	int khoangcachPG;
	if(dem==0) {
			loaibodotkhonganduoc();
			timghost(chiduong); // di den ghost trong nuoc dau tien
		} //loai bo ngay khi bat dau game
	else{
		vector<toadodg> temp;
		fileluotchoi();
		khoangcachPG=timghost(temp);
		dotantoan = 0;
		for(int i=0;i<hang; i++)
			for(int j=0;j<cot;j++){
				if(mapdot[i][j]==0 && map[i][j]=='.') dotantoan++;
			}
		if(dotantoan == 0){
			srand(time(NULL));
			int a = rand() % 2 + 0;
			if(a) chedoC(khoangcachPG);
			else if(khoangcachPG<3) chedoA();
		}
		else if(khoangcachPG<3) chedoA();
		else chedoB();
		
	}
	if (!chiduong.empty()) skip = true; else skip = false;
	if (static_cast<float>(dot)/(dot+sodot) < 0.8  && !(toadodau==toadoghost)) return true; else return false; // dieu kien thang cua pacman.
}
bool nhanvat::timduong(vector<toadodg> &chiduong){ // su dung thuat toan A*
	priority_queue<toado, vector<toado>, cmp> tapmo;
	vector<toado> tapmo1; // ban sao chep cua tap mo
	vector<toado> tapdong;
	if (*pacman.td == toadodich) return false; // không tìm chính nó
	tapmo.push(*pacman.td);
	tapmo1.push_back(*pacman.td);
	do {
		if (tapmo.empty()) return false; //neu khong tim thay duong di
		if (find(tapdong.begin(), tapdong.end(), tapmo.top()) != tapdong.end()) {
			tapmo.pop(); 
			continue;
		}// neu da co trong tap dong thi bo qua
		if (toadodich == tapmo.top()) {
			tapdong.push_back(tapmo.top()); break; // dieu kien thoat khoi vong lap
		}
		else {
			toado temp = tapmo.top();
			tapdong.push_back(temp);
			tapmo.pop(); //xoa diem da xet khoi hang doi
			// bo sung them cac diem xet ve 4 huong
			bosung(temp.x, temp.y + 1, temp, tapmo, tapmo1);
			bosung(temp.x, temp.y - 1, temp, tapmo, tapmo1);
			bosung(temp.x + 1, temp.y, temp, tapmo, tapmo1);
			bosung(temp.x - 1, temp.y, temp, tapmo, tapmo1);
		}
	} while (true);
	toado temp = tapdong.back();
	do {
		toadodg t(temp.x,temp.y);
		chiduong.push_back(t);
		for (int i = 0; i < tapdong.size(); i++) {
			if (tapdong.at(i).x == temp.xback && tapdong.at(i).y == temp.yback){  //truy ngược lại đường đi.
				temp = tapdong.at(i); 
				tapdong.erase(tapdong.begin() + i);
				break;
			}
		}
	} while (!(temp==toadodau));
	return true;
};
bool nhanvat::dichuyen(vector<toadodg> &chiduong) { 
		int i = chiduong.size() - 1; 
		int xtrc = pacman.td->x, ytrc = pacman.td->y;
		bool yes;
		Sleep(0);
		gotoxy(0, 0); 
		if (map[chiduong.at(i).y][chiduong.at(i).x] == '.') yes = true; else yes = false;
		if (this->td->x == chiduong.at(i).x && this->td->y < chiduong.at(i).y) this->xuong();
		else if (this->td->x == chiduong.at(i).x && this->td->y > chiduong.at(i).y) this->len();
		else if (this->td->y == chiduong.at(i).y && this->td->x < chiduong.at(i).x) this->phai();
		else this->trai();
		dem++;
		fileoutput <<"\n" << pacman.td->y << " " << pacman.td->x; 
		ofstream lcout("luotchoi.txt");
		lcout << "P " << pacman.td->y << " " << pacman.td->x << " ";
		lcout.close();
		cout << dem << " | " << "(" << xtrc << "," << ytrc << ") ---> (" << pacman.td->x << "," << pacman.td->y << ")" << " | ";
		if (yes) cout << "YES   " << endl;
		else cout << "NO   " << endl;
		cout << "So dot da an: " << dot << "     " << endl;
		cout << "So dot con lai: " << sodot << "    " << endl;
		cout << "Ti le: " << static_cast<float>(dot) / (sodot+dot) << "      " << endl;
		for (int y = 0; y < hang; y++) {
			for (int x = 0; x < cot; x++)
				cout << map[y][x];
			cout << endl;
		}
	return true;
}
int main(){
	system("cls");
	bool stop, skip;
	pacman.td = &toadodau;
	ifstream fileinput("pacman.txt");
	docmap(fileinput);
	fileinput.close(); 
	fileoutput << pacman.td->y << " " << pacman.td->x ;
	do {
		vector<toadodg> chiduong;
		stop = pacman.phantichmap(chiduong,skip); 
		if (stop && !skip ) {
			if(pacman.timduong(chiduong)) pacman.dichuyen(chiduong);
		} 
		if(stop && skip) pacman.dichuyen(chiduong);
	} while (stop); 
	if(static_cast<float>(dot)/(dot+sodot) >= 0.7){
		cout << "PACMAN WIN" << endl;
		ofstream lcout("luotchoi.txt");
		lcout <<"PACMAN WIN";
		lcout.close();
	}
	fileoutput.close();
	system("pause");
}
