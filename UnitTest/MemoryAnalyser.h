#pragma once


#include <map>

#include <lv/Singleton.hpp>
#include <lv/IntType.hpp>

using namespace lv;

/**
 * Singleton patten.
 */
class MemoryAnalyser : public Singleton<MemoryAnalyser, true>
{
	bool	attached_;
	bool	begun_;

	uint64	cur_memory_;
	uint64	max_memory_;

	int	counter_;

	typedef std::map<void const *, size_t>	allocated_map;
	allocated_map		allocated_;		// allocated memory

public:

	MemoryAnalyser();
	~MemoryAnalyser();


	void	attach();
	void	detach();


	bool	attached() const
	{
		return attached_;
	}

	/**
	 * @exception std::logic_error if the analysis has already begun.
	 */
	void	begin_analyse();
	
	/**
	 * @exception std::logic_error if the analysis has not yet begun.
	 */
	uint64	end_analyse(int & counter);


private:

	static void * my_malloc(size_t size);
	static void	my_free(void * addr);


	void	add(void const * addr, size_t size);
	void	remove(void const * addr);

};