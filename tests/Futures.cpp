#include "UnitTestBP.h"

#define BOOST_NO_RVALUE_REFERENCES

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/cstdint.hpp>

using boost::bind;
using boost::uint32_t;

void UnitTest::SetupInformation()
{
	Info.FileName = __FILE__;
	Info.ShortDesc = "ThreadPool - Futures experiment";
}

int SampleTask()
{
	return 123;
}

bool UnitTest::RunTest(FILE* err)
{
	fprintf(err, "Creating packaged_task\n");
	boost::packaged_task<uint32_t> pt(bind(&SampleTask));
	
	fprintf(err, "Grabbing future\n");
	boost::unique_future<uint32_t> future = pt.get_future();

	fprintf(err, "Pushing task to a thread\n");
	boost::thread task(boost::move(pt));

	future.wait();

	if (!future.is_ready())
	{
		fprintf(err, "Future is not ready\n");
		return false;
	}

	fprintf(err, "Future is ready\n");
	uint32_t val = future.get();
	if (val != 123)
	{
			fprintf(err, "Future.get is incorrect. Expected %d. Found %d.\n", 123, val);
			return false;
	}

	return true;
}
