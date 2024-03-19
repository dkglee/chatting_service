#ifndef WORKER_HPP
# define WORKER_HPP

# include <thread>
# include <mutex>
# include <condition_variable>
# include <vector>
# include <map>
# include <memory>
# include <functional>
# include <string>

# define TASK_MSG "msg"

class Worker {
private:
	int workerNum_;
	std::vector<std::thread> threads_;
	std::map<std::string, std::function<void()>> tasks_;
public:
	Worker(int workerNum);
	~Worker();
	void addTask(std::string key, std::function<void()> task);
	void start();
};

// 입력받은 workerNum만큼의 스레드를 생성하고, 스레드는 무한루프를 돌며 작업을 처리한다.
// 작업은 클라이언트로부터 받은 메시지를 처리하는 것으로, 클라이언트로부터 메시지를 받으면
// 해당 메시지를 처리하는 스레드에게 전달한다.

#endif
