#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <fstream>
#include <vector>
using namespace std;

bool arr[2010][2010];//첫번째 dfs 수행시 사용하는 행렬
bool t_arr[2010][2010];//두번째 dfs 수행시 사용하는 행렬
bool visited[2010];//방문했는지 여부를 저장하는 배열
int k, Min, cnt, m, n, s,chk;
char val;
stack <int> stk;//첫번째 dfs 시 값들을 넣기 위한 stack
vector <vector <int> > vec;// strongly connected componets를 저장하는 벡터

//첫번째 dfs 수행시 사용하는 함수(정점 A,B중 A->B로 가는 경로 파악)
void dfs(int pos) {

	visited[pos] = true;//방문했음을 체크
	for (int i = 0; i < k; i++) 
		//pos가 연락을 할 수 있으며 아직 방문하지 않은 학생 방문
		if (arr[pos][i] && !visited[i]) {
			dfs(i);
		}
	//dfs가 끝날 때 stack에 학생의 노드를 넣는다

	stk.push(pos);
}

//두번째 dfs 수행시 사용하는 함수(정점 A,B중 B->A로 가는 경로 파악)
void scc(int pos) {
	visited[pos] = true;//방문했음을 체크
	if (pos == (s-1000)) {//입력받은 학생이 속한 그룹을 저장 
		chk = cnt;
	}
	vec[cnt].push_back(pos);//SCC 그룹 별로 저장
	for (int i = 0; i < k; i++)
		//역 행렬을 사용하여 반대로 연결이 되어있는지 여부를 파악
		if (t_arr[pos][i] && !visited[i])
			scc(i);
}

int main() {
	
	//파일명을 입력받아 파일의 내용을 입력받음
	string str;
	cout << "입력파일의 이름을 입력하세요 ";
	cin >> str; cout <<endl;
	//파일입력 부분
	ifstream fp;
	fp.open(str);
	
	fp >> k;
	//파일입출력을 통해 이름, 나이를 입력받는 부분
	for (int i = 0; i < k; i++) {
		string name;
		int age;
		fp >> name;
		fp >> age;
	}
	//연결 여부를 저장할 행렬 초기화
	memset(arr, false, sizeof(arr));
	memset(t_arr, false, sizeof(arr));

	while (1) {
	
		cin >> val;
		//각 경우에 따라 해당 기능 수행
		if (val == 'I') {
			m, n;
			cin >> m >> n;
			//배열의 크기를 줄여서 사용하기 위해 1000만큼 뺀 값이 index가 됨
			//m에서 n으로 연락할 수 있음을 저장
			arr[m - 1000][n - 1000] = true;
			//두번째 dfs시 사용하기 위해 역행렬저장
			t_arr[n - 1000][m - 1000] = true;
			
		}
		else if (val == 'O') {
			cnt = 0;
			vec.resize(k);
			cin >> s;
			
			memset(visited, 0, sizeof(visited));
			//첫번째 dfs실행(모든 노드에 대하여)
			for (int i = 0; i < k; i++) {
				if (visited[i])
					continue;
				dfs(i);
			
			}
		
			memset(visited, 0, sizeof(visited));

			Min = 5000;
			//stack이 비어있을 때 까지 pop하여 반대 연결 여부 파악
			while (stk.size()) {
				int pos = stk.top();
				stk.pop();
				if (visited[pos])
					continue;
				scc(pos);
				cnt++;
			}
		
			//입력 받은 학생이 속한 그룹 중 학번이 가장 작은 학생을 구하는 과정
			for (int i = 0; i < vec[chk].size(); i++)
				Min = min(Min, vec[chk][i]);
			//index값에 1000을 더해줌
			cout << Min + 1000 << endl;
			vec.clear();
		}
		//종료
		else if (val == 'Q')
			break;
	}
}