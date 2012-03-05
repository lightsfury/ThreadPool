#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <boost/utility/result_of.hpp>

#include <queue>

#if defined(WIN32) || defined(_WIN32)
	#ifdef THREADPOOL_EXPORTS
		#define THREADPOOL_EXPORT __declspec(dllexport)
	#else
		#define THREADPOOL_EXPORT /*__declspec(dllimport)*/
	#endif
#else
	#define THREADPOOL_EXPORT
#endif

namespace ThreadPool
{
	class THREADPOOL_EXPORT ThreadPool
	{
		typedef boost::function<void()> Job_t;
		typedef std::queue<Job_t> JobQueue_t;
		typedef boost::shared_ptr<boost::thread> ThreadPtr_t;
		typedef std::list<ThreadPtr_t> ThreadList_t;
	public:
		ThreadPool();
		~ThreadPool();

		template<class FunctorType>
		boost::shared_future<typename boost::result_of<FunctorType()>::type> AddJob(FunctorType f)
		{
			typedef typename boost::result_of<FunctorType()>::type ReturnType;
			typedef boost::packaged_task<ReturnType> task_t;
			typedef boost::shared_ptr<task_t > taskptr_t;
			typedef boost::shared_future<ReturnType> future_t;
			typedef boost::function<void()> function_t;

			taskptr_t task(new task_t(f));
			future_t future(task->get_future());
			function_t func(boost::bind(&task_t::operator(), task));

			PushJob(func);

			return future;
		}

		template<class FunctorType, class CallbackType>
		boost::shared_future<typename boost::result_of<FunctorType()>::type> AddJob(FunctorType f, CallbackType c)
		{
			typedef typename boost::result_of<FunctorType()>::type ReturnType;
			typedef boost::packaged_task<ReturnType> task_t;
			typedef boost::shared_ptr<task_t > taskptr_t;
			typedef boost::shared_future<ReturnType> future_t;
			typedef boost::function<void()> function_t;

			taskptr_t task(new task_t(f));
			future_t future(task->get_future());
			function_t func(boost::bind(&task_t::operator(), task));

			task->set_wait_callback(c);

			PushJob(func);

			return future;
		}

		/*
		template<class ReturnType, class FunctorType>
		boost::shared_future<ReturnType> AddJob(FunctorType& f)
		{
			typedef boost::packaged_task<ReturnType> task_t;
			typedef boost::shared_ptr<task_t > taskptr_t;
			typedef boost::shared_future<ReturnType> future_t;
			typedef boost::function<void()> function_t;

			taskptr_t task;
			future_t future;
			function_t func;

			task = taskptr_t(new task_t(f));
			future = task->get_future();
			func = boost::bind(&task_t::operator(), task);

			PushJob(func);

			return future;
		}

		template<class ReturnType>
		boost::shared_future<ReturnType> AddJob(ReturnType (*f)())
		{
			return AddJob<ReturnType, ReturnType (*)()>(f);
		}

		template<class ReturnType, class FunctorType, class CallbackType>
		boost::shared_future<ReturnType> AddJob(FunctorType& f, CallbackType& c)
		{
			typedef boost::packaged_task<ReturnType> task_t;
			typedef boost::shared_ptr<task_t > taskptr_t;
			typedef boost::shared_future<ReturnType> future_t;
			typedef boost::function<void()> function_t;

			taskptr_t task;
			future_t future;
			function_t func;

			task = taskptr_t(new task_t(f));
			future = task->get_future();
			func = boost::bind(&task_t::operator(), task);

			task->set_wait_callback(c);

			PushJob(func);

			return future;
		}

		template<class ReturnType, class CallbackType>
		boost::shared_future<ReturnType> AddJob(ReturnType (*f)(), CallbackType& c)
		{
			return AddJob<ReturnType, ReturnType (*)(), CallbackType>(f, c);
		} // */

		void Close();
		void Terminate();

	private:
		void PushJob(Job_t);

		void WorkerThread();

		struct ThreadPoolData_t
		{
			boost::condition_variable_any DataBlock;
			boost::mutex DataLock;
			JobQueue_t Jobs;
			ThreadList_t WorkerThreads;
			boost::uint32_t State;

			ThreadPoolData_t() : State(0) {}
		};

		typedef boost::shared_ptr<ThreadPoolData_t> ThreadPoolDataPtr_t;

		ThreadPoolDataPtr_t Data;
	};
}

#endif // _THREADPOOL_H_
