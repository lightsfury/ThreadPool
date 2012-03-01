#include "UnitTestBP.h"

#include "ThreadPool/ThreadPool.h"

#pragma comment(lib, "ThreadPool_static")

void UnitTest::SetupInformation()
{
	Info.FileName = __FILE__;
	Info.ShortDesc = "ThreadPool - Add job";
}

int Answer()
{
	return 42;
}

bool UnitTest::RunTest(FILE* err)
{
	fprintf(err, "Creating ThreadPool object\n");
	ThreadPool::ThreadPool pool;

	//*
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