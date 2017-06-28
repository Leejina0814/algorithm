#include <iostream>
#include <time.h>
#include <algorithm>
using namespace std;


int t, n, m1, m2, D, K;
int val[10000010];//heapsort ��� ����
int a_val[10000010];//accelerated Heapsort ��� ����
void createHeap(int pos);//Heap�� �������� ����
void fixHeap(int pos, int k_val, int size, int cnt);//Bottom-heap ���
void HeapSort();//heapsort ����
void fixHeapFast(int size, int k_val, int vacant, int h);//accelerated�� �����Լ�
int promote(int hStop, int vacant, int h);//accelerated���� hStop���� �������� �Լ�
void bubbleUpHeap(int root, int k_val, int vacant);//parent�� ���Ͽ� ��ġ ã�� �Լ�
void AcceleratedHeapSort();//accelerated heapsort����


int main() {
	cin >> t;
	while (t--) {
		//��°� �ʱ�ȭ		
		D = -1;
		K = 0;

		cin >> n;
		//�Է¹޾� ����
		for (int i = 1; i <= n; i++)
			cin >> val[i];

		//heap����
		createHeap(1);

		//accelerated heapsort ������ �Է°� ����
		copy(val + 1, val + n + 1, a_val + 1);

		cin >> m1 >> m2;

		clock_t start_time, end_time;
		double elapsedTime;
		start_time = clock();
		//heapsort����
		HeapSort();
		end_time = clock();
		elapsedTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;		//heapsort�� �ɸ��� �ð�

		clock_t a_start_time, a_end_time;
		double a_elapsedTime;
		a_start_time = clock();
		AcceleratedHeapSort();
		a_end_time = clock();
		a_elapsedTime = (double)(a_end_time - a_start_time) / CLOCKS_PER_SEC;		//accelerated heapsort�� �ɸ��� �ð�

																					//���
		cout << D << " " << K << endl;
		cout << elapsedTime << " " << a_elapsedTime << endl;

	}
}

//Bottom-heap����� ����Ͽ� heap�� �������� ����
//pos = ���� ��ġ , k_val = �����;��ϴ� ��, size = �����ؾ� �� node��, cnt = depth ����� ���� ���� 
void fixHeap(int pos, int k_val, int size, int cnt) {
	//leaf node�ΰ�� ����ó��
	//���� ��ġ�� k_val �ְ� ����
	if (pos * 2 > size) {
		val[pos] = k_val;
		D = cnt - 1;
		return;
	}
	//child node�� �ϳ��� ��� ����ó��
	if (pos * 2 + 1 > size) {
		//child node�� ���� �� ū���
		//child node�� ���� �ø��� child node ��ġ�� k_val ����
		if (val[pos * 2] > k_val) {
			val[pos] = val[pos * 2];
			val[pos * 2] = k_val;
			D = cnt;
		}
		//k_val�� ���� �� ū���
		//���� ��ġ�� k_val ����
		else {
			D = cnt;
			val[pos] = k_val;
		}
		return;
	}

	//child node �� ū �� ã��
	int next_pos, next_num;
	if (val[pos * 2] <= val[pos * 2 + 1]) {
		next_pos = pos * 2 + 1;
		next_num = val[pos * 2 + 1];
	}
	else {
		next_pos = pos * 2;
		next_num = val[pos * 2];
	}

	//child node �� ū ���� k_val���� ū ���
	//child node �� ū ���� ���� ��ġ�� �����ϰ� fixHeap ���
	if (next_num > k_val) {
		val[pos] = next_num;

		fixHeap(next_pos, k_val, size, cnt + 1);

	}
	//k_val�� �� ū ���
	//���� ��ġ�� k_val �����ϰ� fixHeap ����
	else {
		D = cnt - 1;
		val[pos] = k_val;
	}
}


//heap �ʱ�ȭ �Լ�
void createHeap(int pos) {
	//leaf node�� ��� ����ó��
	if (pos * 2 > n)
		return;
	//left subHeap�� ���� ���
	createHeap(pos * 2);
	//right subHeap�� �ִ� ��쿡�� right subHeap�� ���� ���
	if (pos * 2 + 1 <= n)
		createHeap(pos * 2 + 1);

	//fixHeap���� ���� ���� ����
	fixHeap(pos, val[pos], n, 0);
}

//heapsort ����
//size = input node ��
void HeapSort() {

	//in-place ��� ������ ���� i=size���� ����
	//m1�� �ݺ�
	for (int i = n; i > n - m1; i--) {
		//root�� ����
		int temp = val[1];

		//root�� ��ġ�� ���� ���� �����ϹǷ� heap���� ������ �ϴ� node�� ���� �Ѱ� �ٿ���
		int node_size = i - 1;

		//fixHeap���� node_size���� node�� ������ heap ����
		fixHeap(1, val[i], node_size, 0);
		//�ڿ������� ����
		val[i] = temp;
	}
}


//parent�� ���ϸ� �� �����Դ��� Ȯ���ϴ� �Լ�
//root = fixHeapFast�� ������ ��ġ, k_val = ���ؾ� �ϴ� ��, vacant = ������ġ
void bubbleUpHeap(int root, int k_val, int vacant) {
	//������ ��ŭ �ö󰡸� Ȯ���� ��� ����ġ�� k_val ����
	if (vacant == root)
		a_val[vacant] = k_val;
	//�ƴѰ��
	else {
		int parent = vacant / 2;
		//parent�� k_val���� ū ��� �� ������ ����̹Ƿ� ������ġ�� k_val����
		if (k_val <= a_val[parent])
			a_val[vacant] = k_val;
		//�ƴѰ��� �߸� ������ ����̹Ƿ� ������ġ�� parent�� ���� �� bubbleUpHeap ���
		else {
			a_val[vacant] = a_val[parent];
			bubbleUpHeap(root, k_val, parent);
		}
	}
}

//hStop�� ������ ��ġ return
//hStop = �������� �� Ƚ��, vacant = ��ġ, h = ����
int promote(int hStop, int vacant, int h) {
	int vacStop;
	//hStop�� ���������� vacStop�� ������ġ ����
	if (h <= hStop)
		vacStop = vacant;
	//child node�� ū ���� ������ġ�� �ø��� ������
	else if (a_val[2 * vacant] <= a_val[2 * vacant + 1]) {
		a_val[vacant] = a_val[2 * vacant + 1];
		vacStop = promote(hStop, 2 * vacant + 1, h - 1);
	}
	else {
		a_val[vacant] = a_val[2 * vacant];
		vacStop = promote(hStop, 2 * vacant, h - 1);
	}
	//hStop�� ������ ��ġ return
	return vacStop;
}

//accelerated heapsort�� �����Լ�(heap�� �������� ����)
//n = �����ؾ� �� node ��, k_val = �������� �� ��, vacant = ������ġ, h = vacant�� ����
void fixHeapFast(int n, int k_val, int vacant, int h) {
	int hStop;
	int vacStop;
	//h �� 0�ΰ�� ����ó��
	//������ġ�� k_val ����
	if (h == 0) {
		a_val[vacant] = k_val;
		return;
	}
	//leaf node�� ��� ����ó��
	//������ġ�� k_val ����
	if (vacant * 2 > n) {
		a_val[vacant] = k_val;
		return;
	}

	//child node�� 1���� ��� ����ó��
	if (vacant * 2 + 1 > n) {
		//child node�� ���� ũ�ų� �������
		//bubbleUpHeap�� �߻����� �ʱ� ������ child node�� ���� �÷��ְ� k_val�� child node ��ġ�� ����
		if (a_val[vacant * 2] >= k_val) {
			a_val[vacant] = a_val[vacant * 2];
			a_val[vacant * 2] = k_val;
		}
		//child node�� ���� ū ���
		//bubbleUpHeap�� �߻��ǹǷ� K���� child node�� ���� ���� �ϰ� ���� ��ġ�� k_val ����
		else {
			K = a_val[vacant * 2];
			a_val[vacant] = k_val;
		}
		return;
	}

	//���̰� Ȧ���ΰ��
	//hStop�� ���� ���� �ø��� ���ְ� promote �Ű������� h�� �ϳ� �÷��ش�.
	if (h % 2) {
		hStop = h / 2 + 1;
		vacStop = promote(hStop, vacant, h + 1);
		//hStop�� �ø������ ������ �ϳ� ������
		hStop--;
	}
	//���̰� ¦���ΰ��
	else {
		hStop = h / 2;
		vacStop = promote(hStop, vacant, h);
	}

	//vacStop�� parent�� ��ġ
	int vacParent = vacStop / 2;

	//parent�� ������ k_val�� ū���(�߸� ������ ���)
	//parent�� ���� �����ְ� k_val�� ��ġ�� ã������ bubbleUpHeap����
	//bubbleUpHeap�� ����Ǿ����Ƿ� K���� ���� ��ġ �ߴ� ���� ����
	if (a_val[vacParent] <= k_val) {
		a_val[vacStop] = a_val[vacParent];
		K = a_val[vacStop];
		bubbleUpHeap(vacant, k_val, vacParent);
	}
	//parent�� ������ k_val�� ���� ���(�� ������ ���)
	//���� node�鿡 ���� fixHeapFast ����
	else {
		fixHeapFast(n, k_val, vacStop, hStop);
	}


}

//accelerated heapsort �Լ�
//size = input node ��
void AcceleratedHeapSort() {
	//in-place ��� ������ ���� i=size���� ����
	//m2�� �ݺ�
	for (int i = n; i > n - m2; i--) {
		//K�ʱ�ȭ
		K = 0;
		//root�� ��ġ�� ���� ���� �����ϹǷ� heap���� ������ �ϴ� node�� ���� �Ѱ� �ٿ���
		int node_size = i - 1;

		//node_size�� ���� heap ���� ���ϱ�
		int height = 0;
		while (node_size > 1) {
			node_size /= 2;
			height++;
		}
		//root�� ����
		int temp = a_val[1];

		//fixHeapFast���� node_size���� node�� ������ heap ����
		fixHeapFast(i - 1, a_val[i], 1, height);
		a_val[i] = temp;
	}
}
