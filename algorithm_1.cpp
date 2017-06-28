#include <iostream>
#include <time.h>
#include <algorithm>
using namespace std;


int t, n, m1, m2, D, K;
int val[10000010];//heapsort 결과 저장
int a_val[10000010];//accelerated Heapsort 결과 저장
void createHeap(int pos);//Heap의 순서조건 구현
void fixHeap(int pos, int k_val, int size, int cnt);//Bottom-heap 방식
void HeapSort();//heapsort 구현
void fixHeapFast(int size, int k_val, int vacant, int h);//accelerated의 서브함수
int promote(int hStop, int vacant, int h);//accelerated에서 hStop까지 내려가는 함수
void bubbleUpHeap(int root, int k_val, int vacant);//parent와 비교하여 위치 찾는 함수
void AcceleratedHeapSort();//accelerated heapsort구현


int main() {
	cin >> t;
	while (t--) {
		//출력값 초기화		
		D = -1;
		K = 0;

		cin >> n;
		//입력받아 저장
		for (int i = 1; i <= n; i++)
			cin >> val[i];

		//heap구현
		createHeap(1);

		//accelerated heapsort 실행할 입력값 복사
		copy(val + 1, val + n + 1, a_val + 1);

		cin >> m1 >> m2;

		clock_t start_time, end_time;
		double elapsedTime;
		start_time = clock();
		//heapsort실행
		HeapSort();
		end_time = clock();
		elapsedTime = (double)(end_time - start_time) / CLOCKS_PER_SEC;		//heapsort시 걸리는 시간

		clock_t a_start_time, a_end_time;
		double a_elapsedTime;
		a_start_time = clock();
		AcceleratedHeapSort();
		a_end_time = clock();
		a_elapsedTime = (double)(a_end_time - a_start_time) / CLOCKS_PER_SEC;		//accelerated heapsort시 걸리는 시간

																					//출력
		cout << D << " " << K << endl;
		cout << elapsedTime << " " << a_elapsedTime << endl;

	}
}

//Bottom-heap방식을 사용하여 heap의 순서조건 구현
//pos = 현재 위치 , k_val = 내려와야하는 값, size = 정렬해야 할 node수, cnt = depth 계산을 위한 변수 
void fixHeap(int pos, int k_val, int size, int cnt) {
	//leaf node인경우 예외처리
	//현재 위치에 k_val 넣고 종료
	if (pos * 2 > size) {
		val[pos] = k_val;
		D = cnt - 1;
		return;
	}
	//child node가 하나인 경우 예외처리
	if (pos * 2 + 1 > size) {
		//child node의 값이 더 큰경우
		//child node의 값을 올리고 child node 위치에 k_val 넣음
		if (val[pos * 2] > k_val) {
			val[pos] = val[pos * 2];
			val[pos * 2] = k_val;
			D = cnt;
		}
		//k_val의 값이 더 큰경우
		//현재 위치에 k_val 넣음
		else {
			D = cnt;
			val[pos] = k_val;
		}
		return;
	}

	//child node 중 큰 수 찾기
	int next_pos, next_num;
	if (val[pos * 2] <= val[pos * 2 + 1]) {
		next_pos = pos * 2 + 1;
		next_num = val[pos * 2 + 1];
	}
	else {
		next_pos = pos * 2;
		next_num = val[pos * 2];
	}

	//child node 중 큰 값이 k_val보다 큰 경우
	//child node 중 큰 값을 현재 위치에 저장하고 fixHeap 재귀
	if (next_num > k_val) {
		val[pos] = next_num;

		fixHeap(next_pos, k_val, size, cnt + 1);

	}
	//k_val이 더 큰 경우
	//현재 위치에 k_val 저장하고 fixHeap 종료
	else {
		D = cnt - 1;
		val[pos] = k_val;
	}
}


//heap 초기화 함수
void createHeap(int pos) {
	//leaf node인 경우 예외처리
	if (pos * 2 > n)
		return;
	//left subHeap에 대해 재귀
	createHeap(pos * 2);
	//right subHeap이 있는 경우에만 right subHeap에 대해 재귀
	if (pos * 2 + 1 <= n)
		createHeap(pos * 2 + 1);

	//fixHeap으로 순서 조건 구현
	fixHeap(pos, val[pos], n, 0);
}

//heapsort 구현
//size = input node 수
void HeapSort() {

	//in-place 방식 구현을 위해 i=size부터 시작
	//m1번 반복
	for (int i = n; i > n - m1; i--) {
		//root값 저장
		int temp = val[1];

		//root에 위치한 값을 빼서 정렬하므로 heap으로 만들어야 하는 node의 수를 한개 줄여줌
		int node_size = i - 1;

		//fixHeap으로 node_size개의 node로 구성된 heap 구현
		fixHeap(1, val[i], node_size, 0);
		//뒤에서부터 저장
		val[i] = temp;
	}
}


//parent와 비교하며 잘 내려왔는지 확인하는 함수
//root = fixHeapFast를 시작한 위치, k_val = 비교해야 하는 값, vacant = 현재위치
void bubbleUpHeap(int root, int k_val, int vacant) {
	//내려온 만큼 올라가며 확인한 경우 현위치에 k_val 저장
	if (vacant == root)
		a_val[vacant] = k_val;
	//아닌경우
	else {
		int parent = vacant / 2;
		//parent가 k_val보다 큰 경우 잘 내려온 경우이므로 현재위치에 k_val저장
		if (k_val <= a_val[parent])
			a_val[vacant] = k_val;
		//아닌경우는 잘못 내려온 경우이므로 현재위치에 parent값 저장 후 bubbleUpHeap 재귀
		else {
			a_val[vacant] = a_val[parent];
			bubbleUpHeap(root, k_val, parent);
		}
	}
}

//hStop번 내려간 위치 return
//hStop = 내려가야 할 횟수, vacant = 위치, h = 높이
int promote(int hStop, int vacant, int h) {
	int vacStop;
	//hStop번 내려갔으면 vacStop에 현재위치 저장
	if (h <= hStop)
		vacStop = vacant;
	//child node중 큰 값을 현재위치에 올리고 내려감
	else if (a_val[2 * vacant] <= a_val[2 * vacant + 1]) {
		a_val[vacant] = a_val[2 * vacant + 1];
		vacStop = promote(hStop, 2 * vacant + 1, h - 1);
	}
	else {
		a_val[vacant] = a_val[2 * vacant];
		vacStop = promote(hStop, 2 * vacant, h - 1);
	}
	//hStop번 내려간 위치 return
	return vacStop;
}

//accelerated heapsort의 서브함수(heap의 순서조건 구현)
//n = 정렬해야 할 node 수, k_val = 내려가야 할 값, vacant = 현재위치, h = vacant의 높이
void fixHeapFast(int n, int k_val, int vacant, int h) {
	int hStop;
	int vacStop;
	//h 가 0인경우 예외처리
	//현재위치에 k_val 저장
	if (h == 0) {
		a_val[vacant] = k_val;
		return;
	}
	//leaf node인 경우 예외처리
	//현재위치에 k_val 저장
	if (vacant * 2 > n) {
		a_val[vacant] = k_val;
		return;
	}

	//child node가 1개인 경우 예외처리
	if (vacant * 2 + 1 > n) {
		//child node의 값이 크거나 같은경우
		//bubbleUpHeap이 발생되지 않기 때문에 child node의 값을 올려주고 k_val을 child node 위치에 저장
		if (a_val[vacant * 2] >= k_val) {
			a_val[vacant] = a_val[vacant * 2];
			a_val[vacant * 2] = k_val;
		}
		//child node의 값이 큰 경우
		//bubbleUpHeap이 발생되므로 K값에 child node의 값을 저장 하고 현재 위치에 k_val 저장
		else {
			K = a_val[vacant * 2];
			a_val[vacant] = k_val;
		}
		return;
	}

	//높이가 홀수인경우
	//hStop은 나눈 값에 올림을 해주고 promote 매개변수의 h를 하나 올려준다.
	if (h % 2) {
		hStop = h / 2 + 1;
		vacStop = promote(hStop, vacant, h + 1);
		//hStop을 올림해줬기 때문에 하나 내려줌
		hStop--;
	}
	//높이가 짝수인경우
	else {
		hStop = h / 2;
		vacStop = promote(hStop, vacant, h);
	}

	//vacStop의 parent의 위치
	int vacParent = vacStop / 2;

	//parent의 값보다 k_val이 큰경우(잘못 내려온 경우)
	//parent의 값을 내려주고 k_val의 위치를 찾기위해 bubbleUpHeap실행
	//bubbleUpHeap이 실행되었으므로 K값에 원래 위치 했던 값을 저장
	if (a_val[vacParent] <= k_val) {
		a_val[vacStop] = a_val[vacParent];
		K = a_val[vacStop];
		bubbleUpHeap(vacant, k_val, vacParent);
	}
	//parent의 값보다 k_val이 작은 경우(잘 내려온 경우)
	//남은 node들에 대해 fixHeapFast 진행
	else {
		fixHeapFast(n, k_val, vacStop, hStop);
	}


}

//accelerated heapsort 함수
//size = input node 수
void AcceleratedHeapSort() {
	//in-place 방식 구현을 위해 i=size부터 시작
	//m2번 반복
	for (int i = n; i > n - m2; i--) {
		//K초기화
		K = 0;
		//root에 위치한 값을 빼서 정렬하므로 heap으로 만들어야 하는 node의 수를 한개 줄여줌
		int node_size = i - 1;

		//node_size에 대한 heap 높이 구하기
		int height = 0;
		while (node_size > 1) {
			node_size /= 2;
			height++;
		}
		//root값 저장
		int temp = a_val[1];

		//fixHeapFast으로 node_size개의 node로 구성된 heap 구현
		fixHeapFast(i - 1, a_val[i], 1, height);
		a_val[i] = temp;
	}
}
