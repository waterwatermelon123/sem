#pragma once
#include<iostream>
#include<windows.h>
#include<thread>

using namespace std;
/*
WaitForSingleObject(g_hSemaphore, INFINITE);   //�ź���ֵ-1
ReleaseSemaphore(g_hSemaphore, 1, NULL);       //�ź���ֵ+1
*/

class RW_LOCK {
	//���ù�ƽ������ʽ :������������˳�������У�����˼·��w����sem_balanceһֱ����ֱ������
public:
	RW_LOCK():readCount(0) {
		sem_balance = CreateSemaphore(NULL,1,1,NULL);     //
		sem_rcount_mtx = CreateSemaphore(NULL, 1, 1, NULL);   //���ڶ���֮�䣬����readCount��������atomic����
		sem_w = CreateSemaphore(NULL, 1, 1, NULL);    //��д����������������
	}
	~RW_LOCK() {
		CloseHandle(sem_balance);
		CloseHandle(sem_rcount_mtx);
		CloseHandle(sem_w);
	}
	void get_read() {
		WaitForSingleObject(sem_balance, INFINITE);
		WaitForSingleObject(sem_rcount_mtx, INFINITE);
		if (readCount == 0) {
			WaitForSingleObject(sem_w, INFINITE);
		}
		readCount++;
		ReleaseSemaphore(sem_rcount_mtx,1,NULL);
		ReleaseSemaphore(sem_balance, 1, NULL);
	}
	void release_read() {
		WaitForSingleObject(sem_rcount_mtx, INFINITE);
		readCount--;
		if (readCount == 0) {
			ReleaseSemaphore(sem_w, 1, NULL);
		}
		ReleaseSemaphore(sem_rcount_mtx, 1, NULL);
	}
	void get_write() {
		WaitForSingleObject(sem_balance,INFINITE);
		WaitForSingleObject(sem_w, INFINITE);
	}
	void release_write() {
		ReleaseSemaphore(sem_w, 1, NULL);
		ReleaseSemaphore(sem_balance, 1, NULL);
	}

private:
	HANDLE sem_balance;
	HANDLE sem_rcount_mtx;
	HANDLE sem_w;
	int readCount;
};

void writeProcess(int i);
void readProcess(int i);
void writefun(RW_LOCK& rw_lock);
void readfun(RW_LOCK& rw_lock);
void test_rw_lock();


