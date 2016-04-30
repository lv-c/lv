#include "MemoryAnalyser.h"
#include <cassert>
#include <algorithm>

#include <Windows.h>
#include "detours.h"
#pragma comment(lib, "detours.lib")
#pragma comment(lib, "detoured.lib")

void * (* true_malloc) (size_t) = malloc;
void (* true_free) (void*) = free;


MemoryAnalyser::MemoryAnalyser()
	: attached_(false)
	, begun_(false)
	, cur_memory_(0)
	, max_memory_(0)
	, counter_(0)
{
}

MemoryAnalyser::~MemoryAnalyser()
{
	detach();
}

void MemoryAnalyser::attach()
{
	if (! attached_)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)true_malloc, &MemoryAnalyser::my_malloc);
		DetourAttach(&(PVOID&)true_free, &MemoryAnalyser::my_free);
		DetourTransactionCommit();

		attached_ = true;
	}
	else
		assert(false);
}

void MemoryAnalyser::detach()
{
	if (attached_)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)true_malloc, &MemoryAnalyser::my_malloc);
		DetourDetach(&(PVOID&)true_free, &MemoryAnalyser::my_free);
		DetourTransactionCommit();

		attached_ = false;
	}
}

void * MemoryAnalyser::my_malloc(size_t size)
{
	void * addr = true_malloc(size);

	// avoid recursion
	static bool local_alloc = false;
	if (local_alloc)
		return addr;

	local_alloc = true;
	MemoryAnalyser::instance().add(addr, size);
	local_alloc = false;

	return addr;
}

void MemoryAnalyser::my_free(void * addr)
{
	true_free(addr);

	static bool local_free = false;
	if (local_free)
		return;

	local_free = true;
	MemoryAnalyser::instance().remove(addr);
	local_free = false;

}

void MemoryAnalyser::begin_analyse()
{
	assert(attached_);

	if (begun_)
		throw std::logic_error("You have already begun the analysis");

	cur_memory_ = max_memory_ = 0;
	counter_ = 0;

	allocated_.clear();

	begun_ = true;
}

uint64 MemoryAnalyser::end_analyse(int & counter)
{
	if (! begun_)
		throw std::logic_error("You haven't begun the analysis");

	begun_ = false;

	counter = counter_;

	return max_memory_;
}


void MemoryAnalyser::add(void const * addr, size_t size)
{
	if (addr != NULL)
	{
		counter_ ++;

		allocated_.insert(std::make_pair(addr, size));
		cur_memory_ += size;

		max_memory_ = std::max(cur_memory_, max_memory_);
	}
}

void MemoryAnalyser::remove(void const * addr)
{
	allocated_map::iterator it = allocated_.find(addr);

	if (it != allocated_.end())
	{
		assert(cur_memory_ >= it->second);
		cur_memory_ = (cur_memory_ > it->second ? cur_memory_ - it->second : 0);

		allocated_.erase(it);
	}
}