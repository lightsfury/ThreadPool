#include "UnitTestBP.h"

#include "ThreadPool/ThreadPool.h"

struct LockableFile
{
	boost::mutex Lock;
	FILE* File;
};

void UnitTest::SetupInformation()
{
	Info.FileName = __FILE__;
	Info.ShortDesc = "ThreadPool - Add job stress test";
}

int Answer(boost::shared_ptr<LockableFile> file, uint32_t num)
{
	{
		boost::mutex::scoped_lock lock(file->Lock);
		fprintf(file->File, "Running task %u\n", num);
	}

	//boost::this_thread::sleep(boost::posix_time::milliseconds(5));
	return 42;
}

void WaitAnswer(boost::shared_ptr<LockableFile> file, uint32_t num)
{
	boost::mutex::scoped_lock lock(file->Lock);
	fprintf(file->File, "Waiting on task %u\n", num);
}

bool UnitTest::RunTest(FILE* err)
{
	bool pass = true;
	fprintf(err, "Creating ThreadPool object\n");
	ThreadPool::ThreadPool pool;
	boost::shared_ptr<LockableFile> file(new LockableFile);
	file->File = err;
	
	typedef boost::shared_future<int> intAnswer;
	typedef std::vector<intAnswer> Answers_t;
	
	Answers_t answers;
	
	uint32_t numJobs = 128;
	
	fprintf(err, "Adding %d jobs\n", numJobs);
	for (uint32_t i = 0; i < numJobs; i++)
	{
		answers.push_back(pool.AddJob(boost::bind(&Answer, file, i), boost::bind(&WaitAnswer, file, i)));
	}
	
	file->Lock.lock();
	fprintf(err, "Waiting on futures\n");
	file->Lock.unlock();
	Answers_t::iterator it = answers.begin();
	uint32_t val;
	for (; it !=  answers.end(); it++)
	{
		val = it->get();
		if (val != 42)
		{
			file->Lock.lock();
			fprintf(err, "Value incorrect. Expected %d. Found %d.\n", 42, val);
			file->Lock.unlock();
			pass = false;
		}
	}

	return pass;
}