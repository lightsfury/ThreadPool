#include "UnitTestBP.h"

#include "ThreadPool/ThreadPool.h"

void UnitTest::SetupInformation()
{
	Info.FileName = __FILE__;
	Info.ShortDesc = "ThreadPool - Add job";
}

int Answer()
{
	return 42;
}

template<class Functor>
bool TestThreadPoolAdd(FILE* err, ThreadPool::ThreadPool& pool, Functor f)
{
	typedef typename boost::result_of<Functor()>::type result_type;
	fprintf(err, "*Adding the job\n");
	boost::shared_future<result_type> future = pool.AddJob(f);

	fprintf(err, "* Waiting on future\n");
	future.wait();

	if (!future.is_ready() || !future.has_value())
	{
		fprintf(err, "*  Something went wrong\n");
		return false;
	}
	else
	{
		fprintf(err, "*  Future is ready and has a value\n");
		return true;
	}
}

bool UnitTest::RunTest(FILE* err)
{
	bool pass = true;
	fprintf(err, "Creating ThreadPool object\n");
	ThreadPool::ThreadPool pool;

	fprintf(err, "Testing function pointer\n");
	if (!TestThreadPoolAdd(err, pool, &Answer))
	{
		pass = false;
	}

	fprintf(err, "Testing boost::bind\n");
	if (!TestThreadPoolAdd(err, pool, boost::bind(&Answer)))
	{
		pass = false;
	}

	return pass;

	/*
	fprintf(err, "Adding a job\n");
	boost::shared_future<int> future = pool.AddJob(&Answer);
	//boost::unique_future<int> future = pool.AddJob(&Answer);

	fprintf(err, "Waiting on future\n");
	future.wait();

	if (future.is_ready())
	{
		fprintf(err, "Future is ready\n");
		int val = future.get();
		if (val != 42)
		{
			fprintf(err, "Value incorrect. Expected %d. Found %d.\n", 42, val);
		}
		else
		{
			fprintf(err, "Value correct.\n");
			return true;
		}
	}
	else
	{
		fprintf(err, "Future is not ready\n");
	} // */

	return false;
}
