#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <future>

class JobSystem {
public:
	explicit JobSystem(size_t threads = std::thread::hardware_concurrency()) : m_running(true) {
		for (size_t i = 0; i < threads; ++i) {
			m_threads.emplace_back(&JobSystem::ThreadLoop, this);
		}
	}

	~JobSystem() {
		m_running = false;
		m_cv.notify_all();
		for (auto& t : m_threads) if (t.joinable()) t.join();
	}

	template<typename F, typename... Args>
	auto Submit(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
		using RetType = std::invoke_result_t<F, Args...>;
		auto task = std::make_shared<std::packaged_task<RetType()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);
		{
			std::lock_guard lk(m_mutex);
			m_queue.emplace([task]() { (*task)(); });
		}
		m_cv.notify_one();
		return task->get_future();
	}

private:
	void ThreadLoop() {
		while (m_running) {
			std::function<void()> job;
			{
				std::unique_lock lk(m_mutex);
				m_cv.wait(lk, [&] { return !m_running || !m_queue.empty(); });
				if (!m_running && m_queue.empty()) return;
				job = std::move(m_queue.front());
				m_queue.pop();
			}
			job();
		}
	}

	std::vector<std::thread>       m_threads;
	std::queue<std::function<void()>> m_queue;
	std::mutex                     m_mutex;
	std::condition_variable        m_cv;
	std::atomic<bool>              m_running{ false };
};