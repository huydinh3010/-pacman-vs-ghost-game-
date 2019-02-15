#include<iostream>
#include<fstream>
#include<queue>
#include<cmath>
#include<vector>
#include<algorithm>
#include<windows.h>
using namespace std;
int hang, cot, dem = 0;
bool ghostdot;
char map[200][200];
ofstream fileoutput("GHistory.txt");
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
} toadodau, toadodich;
class toadodg { // kieu du lieu toa do don gian hon
public:
	int x, y;
	double fx;
	bool operator >(const toadodg &td) {
		return this->fx > td.fx;
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
	bool phantichmap();
	void len() {
		td->y--;
		if (map[td->y][td->x]=='.') ghostdot = true; else ghostdot = false;
		map[td->y][td->x] = 'G';
	}
	void xuong() {
		td->y++;
		if (map[td->y][td->x]=='.') ghostdot = true; else ghostdot = false;
		map[td->y][td->x] = 'G';
	}
	void trai() {
		td->x--;
		if (map[td->y][td->x]=='.') ghostdot = true; else ghostdot = false;
		map[td->y][td->x] = 'G';
	}
	void phai() {
		td->x++;
		if (map[td->y][td->x]=='.') ghostdot = true; else ghostdot = false;
		map[td->y][td->x] = 'G';
	}
} ghost;
void docmap(ifstream& in){
	in >> hang >> cot;
	char trash[1];
	in.getline(trash, 1);
	double min = 100000;
	for (int i = 0; i<hang; i++)
		for (int j = 0; j<cot + 1; j++) {
			in.get(map[i][j]);
			double t = pow(i - hang + 1,2) + pow(j - cot + 1,2);
			if (map[i][j] != '#' && j!=cot && t < min) {
				min = t;
				ghost.td->x = j;
				ghost.td->y = i;
			}
		}
	if (map[ghost.td->y][ghost.td->x]=='.') ghostdot = true; else ghostdot = false;
	map[ghost.td->y][ghost.td->x] = 'G';
};
struct cmp1{
	bool operator() (toadodg t1, toadodg t2) { return (t1 > t2); }
};
struct cmp{
	bool operator() (toado t1, toado t2) { return (t1 > t2); }
};
toado chuyendoi(int x, int y, toado &td){
	toado temp(x, y);
	temp.setfx(td);
	temp.xback = td.x;
	temp.yback = td.y;
	return temp;
}; // chuyen doi x,y thanh kieu toado
void bosung(int x, int y,toado &temp, priority_queue<toado, vector<toado>, cmp> &tapmo, vector<toado> &tapmo1) {
	toado td = chuyendoi(x, y, temp);
	if (map[y][x] != '#' && find(tapmo1.begin(), tapmo1.end(), td) == tapmo1.end()) {
		tapmo1.push_back(td);
		tapmo.push(td);
	}
}
bool nhanvat::phantichmap() { // ham gan toadodich
	//DOC TU FILE LAY TOA DO CUA PACMAN.
	char turn = '\0';
	int xpac, ypac;
	do{
		ifstream lcin("luotchoi.txt");
		lcin >> turn >> ypac >> xpac;
		lcin.close();
		if (turn!='P') Sleep(50);
	} while(turn!='P');

	if(dem!=0) map[toadodich.y][toadodich.x]=' ';
	toadodich.x = xpac;
	toadodich.y = ypac;
	map[toadodich.y][toadodich.x]='P';
	if(toadodau==toadodich) return false; else return true;
}
bool nhanvat::timduong(vector<toadodg> &chiduong){ // su dung thuat toan A*
	priority_queue<toado, vector<toado>, cmp> tapmo;
	vector<toado> tapmo1; // ban sao chep cua tap mo
	vector<toado> tapdong;
	if (*ghost.td == toadodich) return false; // không tìm chính nó
	tapmo.push(*ghost.td);
	tapmo1.push_back(*ghost.td);
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
	//for (int i = chiduong.size() - 1; i >= 0; i--) {
		int i = chiduong.size() - 1; // điểm cuối cùng của vector chiduong là tọa độ kế tiếp của pacman mà nó phải đi
		int xtrc = ghost.td->x, ytrc = ghost.td->y;
		bool yes;
		Sleep(50);
		gotoxy(0, 0); // tích hợp in ra màn hình
		if (map[chiduong.at(i).y][chiduong.at(i).x] == '.') yes = true; else yes = false;
		if (ghostdot) map[this->td->y][this->td->x]='.'; else map[this->td->y][this->td->x]=' '; // dat lai dot neu ghost di qua
		// di chuyen
		if (this->td->x == chiduong.at(i).x && this->td->y < chiduong.at(i).y) this->xuong();
		else if (this->td->x == chiduong.at(i).x && this->td->y > chiduong.at(i).y) this->len();
		else if (this->td->y == chiduong.at(i).y && this->td->x < chiduong.at(i).x) this->phai();
		else this->trai();	// ket thuc di chuyen
		dem++;
		fileoutput <<"\n" << ghost.td->y << " " << ghost.td->x; // đưa vào file
		ofstream lcout("luotchoi.txt"); 
		lcout << "G " << ghost.td->y << " " << ghost.td->x << " "; // in vao file luotchoi
		lcout.close();
		cout  <<  dem << " | " << "(" << xtrc << "," << ytrc << ") ---> (" << ghost.td->x << "," << ghost.td->y << ")           " << endl;
		for (int y = 0; y < hang; y++) {
			for (int x = 0; x < cot; x++)
				cout << map[y][x];
			cout << endl;
		}
	//}
	return true;
}
int main(){
	system("cls");
	bool stop;
	ghost.td = &toadodau; // toa do dau == toadoghost
	ifstream fileinput("pacman.txt");
	docmap(fileinput);
	fileinput.close(); 
	fileoutput << ghost.td->y << " " << ghost.td->x ; //in toado dau
	do {
		vector<toadodg> chiduong;
		stop = ghost.phantichmap(); 
		if (stop) {
			if(ghost.timduong(chiduong)) ghost.dichuyen(chiduong);
		}
	} while (stop);
	if(toadodau==toadodich){
		cout <<"GHOST WIN" << endl;
		ofstream lcout("luotchoi.txt"); 
		lcout << "GHOST WIN"; // in vao file luotchoi
		lcout.close();	
	}
	fileoutput.close();
	system("pause");
}
