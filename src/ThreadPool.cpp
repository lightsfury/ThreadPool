#include "ThreadPool/ThreadPool.h"

namespace ThreadPool
{
	enum ThreadPoolState
	{
		State_Shutdown = 1,
	};

	ThreadPool::ThreadPool() : Data(new ThreadPoolData_t)
	{
		boost::mutex::scoped_lock Lock(Data->DataLock);
		uint32_t limit = boost::thread::hardware_concurrency();
		boost::thread* Thread;

		for (uint32_t i = 0; i < limit; i++)
		{
			Thread = new boost::thread(boost::bind(&ThreadPool::WorkerThread, this));
			Data->WorkerThreads.push_back(ThreadPtr_t(Thread));
		}
	}
	
	ThreadPool::~ThreadPool()
	{
		Terminate();
	}

	void ThreadPool::Close()
	{
		boost::unique_lock<boost::mutex> lock(Data->DataLock);

		Data->State |= State_Shutdown;

		Data->DataBlock.notify_all();
	}

	void ThreadPool::Terminate()
	{
		boost::unique_lock<boost::mutex> lock(Data->DataLock);

		Data->State |= State_Shutdown;

		ThreadList_t::iterator it = Data->WorkerThreads.begin();
		for (; it != Data->WorkerThreads.end(); ++it)
		{
			lock.unlock();
			(*it)->interrupt();
			(*it)->join();
			lock.lock();
		}
	}

	void ThreadPool::PushJob(Job_t job)
	{
		boost::unique_lock<boost::mutex> lock(Data->DataLock);

		Data->Jobs.push(job);

		Data->DataBlock.notify_one();
	}

	void ThreadPool::WorkerThread()
	{
		boost::function<void()> CurrentJob;

		boost::mutex::scoped_lock Lock(Data->DataLock);

		while ((Data->State & State_Shutdown) == 0)
		{
			if (!Data->Jobs.empty())
			{
				CurrentJob = Data->Jobs.front();
				Data->Jobs.pop();

				Lock.unlock();

				CurrentJob();

				Lock.lock();
			}
			else
			{
				// Wait in 100 ms intervals
				Data->DataBlock.timed_wait(Lock, boost::posix_time::milliseconds(100));
			}
		}

		Lock.unlock();
	}

#if 0
	enum RunStates
	{
		Stopped = 0
		Started = 1,
		Idle = 2,
		Shutdown = 4,
		Terminate = 8,
		Shutdown_Terminate = 12, // Shutdown | Terminate
	};
	
	ThreadPool::ThreadPool()
	{
		_Data._RunState = RunStates::Stopped;
		_Data._Options.CleanupPeriod = 0;
		_Data._Options.IdleThreshold = 0;
		_Data._Options.MinimumThreads = 0;
		_Data._Options.MaximumThreads = 0;
		_Data._LogChainID = 0;
	}
	
	ThreadPool::ThreadPool(UInt32 _Num)
	{
		_Data._RunState = RunStates::Stopped;
		_Data._Options.CleanupPeriod = (UInt16)-1;
		_Data._Options.IdleThreshold = 0;
		_Data._Options.MinimumThreads = _Num;
		_Data._Options.MaximumThreads = _Num;
		_Data._LogChainID = 0;
		
		StartScheduler();
	}
	
	ThreadPool::ThreadPool(const ThreadPoolOptions_t* _Options)
	{
		_Data._RunState = RunStates::Stopped;
		_Data._LogChainID = 0;
		
		SetThreadPoolOptions(_Options);
		
		StartScheduler();
	}
	
	UInt32 ThreadPool::AddToLogChain(function<void, int, const char*> _Function)
	{
		mutex::scoped_lock _Lock(_Data._Mutex);
		UInt32 id = _Data._LogChainID++;
		_Data._LogChain[id] = _Function;
		return id;
	}
	
	bool ThreadPool::RemoveFromLogChain(UInt32 _ID)
	{
		mutex::scoped_lock _Lock(_Data._Mutex);
		_LogChain_t::iterator iter = _Data._LogChain.find(_ID);
		if (iter != _Data._LogChain.end()) //! @todo Evaluate "std::map::erase(key_type) >= 1" method.
		{
			_Data._LogChain.erase(iter);
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool ThreadPool::HasLogChain() const
	{
		mutex::scoped_lock _Lock(_Data._Mutex);
		return !_Data._LogChain.empty();
	}
	
	bool ThreadPool::AddJob(function<void (void)> _Function)
	{
		try
		{
			mutex::scoped_lock _Lock(_Data._Mutex);
			
			if (_Data._RunState & RunStates::Shutdown)
			{
			//! @todo Add log output?
				return false;
			}
			
			system_time _AbsoluteTime = get_system_time() + time_duration_ms(1);
			ThreadPoolJobData_t _Job;
			
			_Job._AbsoluteTime = _AbsoluteTime;
			_Job._Function = _Function;
			
			_Data._JobQueue.push(_Job);
			
			return true;
		}
		catch(...)
		{
			//! @todo Add log output
			return false;
		}
	}
	
	bool ThreadPool::AddJob(function<void (void)> _Function, time_duration _Delay)
	{
		try
		{
			mutex::scoped_lock _Lock(_Data._Mutex);
			
			if (_Data._RunState & RunStates::Shutdown)
			{
			//! @todo Add log output?
				return false;
			}
			
			system_time _AbsoluteTime = get_system_time() + _Delay;
			ThreadPoolJobData_t _Job;
			
			_Job._AbsoluteTime = _AbsoluteTime;
			_Job._Function = _Function;
			
			_Data._JobQueue.push(_Job);
			
			return true;
		}
		catch(...)
		{
			//! @todo Add log output
			return false;
		}
	}
	
	bool ThreadPool::HasJobs() const
	{
		try
		{
			mutex::scoped_lock _Lock(_Data._Mutex);
			
			return !(_Data._JobQueue.empty());
		}
		catch(...)
		{
			//! @todo Add log output
			return false;
		}
	}
	
	ThreadPoolOptions_t ThreadPool::GetThreadPoolOptions() const
	{
		mutex::scoped_lock _Lock(_Data._Mutex);
		ThreadPoolOptions_t _Opts = _Data._Options;
		return _Opts;
	}
	
	bool ThreadPool::SetThreadPoolOptions(const ThreadPoolOptions_t* _Opts)
	{
		try
		{
			mutex::scoped_lock _Lock(_Data._Mutex);
			_Data._Options = _Opts;
			
			_Data._Event.notify_all();
			
			return true;
		}
		catch(...)
		{
			//! @todo Add log output
			return false;
		}
	}
	
	void ThreadPool::Shutdown(ShutdownModes _Mode)
	{
		_Data._Mutex.lock();
		
		_Data.RunState |= RunStates::Shutdown;
		
		if (_Mode & ShutdownModes::Terminate)
		{
			_Data.RunState |= RunStates::Terminate;
		}
		
		_Data._Event.notify_all();
		
		_Data._Mutex.unlock();
		
		if (_Mode & ShutdownModes::Block)
		{
			_Data._Thread.join();
		}
	}
#endif
}
