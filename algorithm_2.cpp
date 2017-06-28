#include <iostream>
#include <string>
#include <stack>
#include <algorithm>
#include <fstream>
#include <vector>
using namespace std;

bool arr[2010][2010];//ù��° dfs ����� ����ϴ� ���
bool t_arr[2010][2010];//�ι�° dfs ����� ����ϴ� ���
bool visited[2010];//�湮�ߴ��� ���θ� �����ϴ� �迭
int k, Min, cnt, m, n, s,chk;
char val;
stack <int> stk;//ù��° dfs �� ������ �ֱ� ���� stack
vector <vector <int> > vec;// strongly connected componets�� �����ϴ� ����

//ù��° dfs ����� ����ϴ� �Լ�(���� A,B�� A->B�� ���� ��� �ľ�)
void dfs(int pos) {

	visited[pos] = true;//�湮������ üũ
	for (int i = 0; i < k; i++) 
		//pos�� ������ �� �� ������ ���� �湮���� ���� �л� �湮
		if (arr[pos][i] && !visited[i]) {
			dfs(i);
		}
	//dfs�� ���� �� stack�� �л��� ��带 �ִ´�

	stk.push(pos);
}

//�ι�° dfs ����� ����ϴ� �Լ�(���� A,B�� B->A�� ���� ��� �ľ�)
void scc(int pos) {
	visited[pos] = true;//�湮������ üũ
	if (pos == (s-1000)) {//�Է¹��� �л��� ���� �׷��� ���� 
		chk = cnt;
	}
	vec[cnt].push_back(pos);//SCC �׷� ���� ����
	for (int i = 0; i < k; i++)
		//�� ����� ����Ͽ� �ݴ�� ������ �Ǿ��ִ��� ���θ� �ľ�
		if (t_arr[pos][i] && !visited[i])
			scc(i);
}

int main() {
	
	//���ϸ��� �Է¹޾� ������ ������ �Է¹���
	string str;
	cout << "�Է������� �̸��� �Է��ϼ��� ";
	cin >> str; cout <<endl;
	//�����Է� �κ�
	ifstream fp;
	fp.open(str);
	
	fp >> k;
	//����������� ���� �̸�, ���̸� �Է¹޴� �κ�
	for (int i = 0; i < k; i++) {
		string name;
		int age;
		fp >> name;
		fp >> age;
	}
	//���� ���θ� ������ ��� �ʱ�ȭ
	memset(arr, false, sizeof(arr));
	memset(t_arr, false, sizeof(arr));

	while (1) {
	
		cin >> val;
		//�� ��쿡 ���� �ش� ��� ����
		if (val == 'I') {
			m, n;
			cin >> m >> n;
			//�迭�� ũ�⸦ �ٿ��� ����ϱ� ���� 1000��ŭ �� ���� index�� ��
			//m���� n���� ������ �� ������ ����
			arr[m - 1000][n - 1000] = true;
			//�ι�° dfs�� ����ϱ� ���� ���������
			t_arr[n - 1000][m - 1000] = true;
			
		}
		else if (val == 'O') {
			cnt = 0;
			vec.resize(k);
			cin >> s;
			
			memset(visited, 0, sizeof(visited));
			//ù��° dfs����(��� ��忡 ���Ͽ�)
			for (int i = 0; i < k; i++) {
				if (visited[i])
					continue;
				dfs(i);
			
			}
		
			memset(visited, 0, sizeof(visited));

			Min = 5000;
			//stack�� ������� �� ���� pop�Ͽ� �ݴ� ���� ���� �ľ�
			while (stk.size()) {
				int pos = stk.top();
				stk.pop();
				if (visited[pos])
					continue;
				scc(pos);
				cnt++;
			}
		
			//�Է� ���� �л��� ���� �׷� �� �й��� ���� ���� �л��� ���ϴ� ����
			for (int i = 0; i < vec[chk].size(); i++)
				Min = min(Min, vec[chk][i]);
			//index���� 1000�� ������
			cout << Min + 1000 << endl;
			vec.clear();
		}
		//����
		else if (val == 'Q')
			break;
	}
}