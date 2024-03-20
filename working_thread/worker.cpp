#include "./worker.hpp"
#include "../library/global_namespace.hpp"

Worker::Worker(int workerNum) 
	: workerNum_(workerNum)
{
	this->addTask(TASK_MSG, [this]() -> void {
		while (true) {
			Global::WorkerTask* task_ = nullptr;
			{
				std::unique_lock<std::mutex> lock(Global::workerManager.mtx);
				std::cout << "worker thread waiting for work\n";
				Global::workerManager.cv.wait(lock, [] {
					return !Global::workerManager.workQueue.empty();
				});

				std::cout << "worker thread got work\n";
				if (Global::workerManager.workQueue.empty()) {
					// no more work to do so exit and close the program
					break;
				}
				task_ = Global::workerManager.workQueue.front();
				Global::workerManager.workQueue.pop();
				lock.unlock();
			}
			task_->task(task_->bytes_read);
			delete task_;
		}
	});
	this->start();
}

void Worker::start() {
	for (int i = 0; i < workerNum_; i++) {
		threads_.push_back(std::thread(tasks_[TASK_MSG]));
		threads_[i].detach();
	}
}

void Worker::addTask(std::string key, std::function<void()> task) {
	tasks_[key] = task;
}

Worker::~Worker() {}
