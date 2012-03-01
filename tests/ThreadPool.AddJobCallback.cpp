#include "UnitTestBP.h"

#include "ThreadPool/ThreadPool.h"

#pragma comment(lib, "ThreadPool_static")

void UnitTest::SetupInformation()
{
	Info.FileName = __FILE__;
	Info.ShortDesc = "ThreadPool - Add job with callback";
}

struct LockableFile
{
	boost::mutex Lock;
	FILE* File;
};

int Answer()
{
	return 42;
}

void AnswerCallback(boost::shared_ptr<LockableFile> ptr)
{
	fprintf(ptr->File, "In callback\n");
}

bool UnitTest::RunTest(FILE* err)
{
	fprintf(err, "Creating ThreadPool object\n");
	ThreadPool::ThreadPool pool;
	boost::shared_ptr<LockableFile> ptr(new LockableFile);
	ptr->File = err;

	//*
	fprintf(err, "Adding a job\n");
	boost::shared_future<int> future = pool.AddJob(&Answer, boost::bind(&AnswerCallback, ptr));
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