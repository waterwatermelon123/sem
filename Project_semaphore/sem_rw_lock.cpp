#include"sem_rw_lock.h"


void readProcess(int i) {
	printf("reading ...%d--%d\n", i, std::this_thread::get_id());
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void writeProcess(int i) {
	printf("mywrite ...%d--%d\n", i, std::this_thread::get_id());
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


void readfun(RW_LOCK& rw_lock) {
	for (int i = 0; i < 100; i++) {
		rw_lock.get_read();
		readProcess(i);
		rw_lock.release_read();
	}
}

void writefun(RW_LOCK& rw_lock) {
	for (int i = 0; i < 10; ++i) {
		rw_lock.get_write();
		writeProcess(i);
		rw_lock.release_write();
	}
}

void test_rw_lock() {
	RW_LOCK rw_lock;
	thread threadwrite(writefun, rw_lock);
	thread threadread(readfun, rw_lock);

	if (threadwrite.joinable()) {
		threadwrite.join();
	}
	if (threadread.joinable()) {
		threadread.join();
	}
}


