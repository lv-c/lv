// *********************************************************************
//  Source   version:  1.0   ¡¤  date: 10/24/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_SOURCE_HPP
#define LV_DATAFLOW_SOURCE_HPP

#include <lv/DataFlow/Fwd.hpp>

#include <lv/IBufferManager.hpp>
#include <lv/Serialization/OArchiveWrapper.hpp>
#include <lv/Stream/OStreamFactory.hpp>


namespace lv { namespace flow {

	/// thread-safe (as long as the BufferManager is thread-safe)
	template<class Key, class OArchive>
	class Source
	{
	public:

		typedef typename Key		key_type;
		typedef typename OArchive	oarchive_type;

		typedef std::function<void(BufferPtr)>	callback_type;

	private:

		BufferManagerPtr	buf_manager_;

		callback_type	callback_;

		OStreamFactory	ostream_factory_;

		// It's a simple flag. So we don't have a mutex for ( it.
		volatile bool	enabled_;

	public:

		Source(callback_type const & callback, BufferManagerPtr buf_manager)
			: callback_(callback)
			, buf_manager_(buf_manager)
			, enabled_(true)
		{
		}

		virtual ~Source()
		{
		}

		void	enable(bool enabled)
		{
			this->enabled_ = enabled;
		}

		bool	enabled() const
		{
			return enabled_;
		}

		/**
		 * call the target function.
		 * @note Do NOT call this after the corresponding DataFlow object is destroyed.
		 */ 
		template<class ...Types>
		void	call(key_type const & key, Types const &... args)
		{
			// it's not enabled
			if (!enabled_)
			{
				return;
			}

			BufferPtr buf = buf_manager_->get();
			OArchiveWrapper<oarchive_type> oa(ostream_factory_, *buf);

			oa.get() << key;
			int dummy[] = { 0, ((oa.get() << args), 0)... };

			oa.flush();

			push(std::move(buf));
		}

	protected:

		virtual	void	push(BufferPtr buf)
		{
			callback_(std::move(buf));
		}

	};

} }

#endif