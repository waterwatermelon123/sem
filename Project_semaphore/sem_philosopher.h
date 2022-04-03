#pragma once
#include<iostream>
#include<windows.h>
#include<thread>
#include<vector>
/*
WaitForSingleObject(g_hSemaphore, INFINITE);   //�ź���ֵ-1
ReleaseSemaphore(g_hSemaphore, 1, NULL);       //�ź���ֵ+1

linux �¶�Ӧ�ĺ���:
#include <semaphore.h>
sem_t mysem;
sem_init(&mysem��1��1)
sem_wait(&mysem);
sem_post(&mysem);
*/

#define N 5
#define LEFT  (i+N-1)%N
#define RIGHT (i+1)%N

using namespace std;

enum StateEat {
	EATING = 1,
	THINKING = 2,
	HUNGRY = 3,
	DEFAULT = 0
};

vector<StateEat> v_state;
vector<HANDLE> v_sems;
HANDLE mutex;

void think_fun(int i);
void eat_fun(int i);
void get_resource(int i);
void release_resource(int i);
void try_to_get(int i);
void test_philosopher();


void preloop(int i) {
	for (int t = 0; t < 100; ++t) {
		think_fun(i);
		get_resource(i);
		eat_fun(i);
		release_resource(i);
	}
}

void think_fun(int i) {
	printf("%d is thinking ...\n", i);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
void eat_fun(int i) {
	printf("%d is eating ...\n", i);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void get_resource(int i) {
	WaitForSingleObject(mutex, INFINITE);     //����  ������������
	v_state[i] = StateEat::HUNGRY;     //��״̬
	try_to_get(i);
	ReleaseSemaphore(mutex, 1, NULL);
	WaitForSingleObject(v_sems[i], INFINITE);  //����õ� ����������ס
}
void release_resource(int i) {
	WaitForSingleObject(mutex, INFINITE);
	v_state[i] = StateEat::THINKING;
	try_to_get(LEFT);       //���Ի������
	try_to_get(RIGHT);       //���Ի����ұ�
	ReleaseSemaphore(mutex, 1, NULL);

}

// similer to notify with condition
void try_to_get(int i) {
	if (v_state[i] == HUNGRY&&v_state[LEFT] != StateEat::EATING&&v_state[RIGHT] != StateEat::EATING) {
		ReleaseSemaphore(v_sems[i], 1, NULL);
		v_state[i] = StateEat::EATING;
	}
}

void test_philosopher() {
	v_state.resize(N);
	v_sems.resize(N);
	mutex = CreateSemaphore(NULL, 1, 1, NULL);
	for (int i = 0; i < N; ++i) {
		v_sems[i] = CreateSemaphore(NULL, 0, 1, NULL);
	}
	vector<thread> threads(N);
	for (int i = 0; i < N; ++i) {
		threads[i] = thread(preloop, i);
	}

	for (int i = 0; i < N; ++i) {
		if (threads[i].joinable()) {
			threads[i].join();
		}
	}
}

