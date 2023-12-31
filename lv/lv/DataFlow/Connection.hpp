// *********************************************************************
//  Connection   version:  1.0   ��  date: 10/24/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <boost/noncopyable.hpp>

#include <any>
#include <memory>


namespace lv::flow
{
	using disconnect_fn_type = void (*) (void *, std::any const &);


	namespace detail
	{
		class ConnectionImpl
		{
			void *	dataflow_;

			disconnect_fn_type	disconnect_fn_;

			std::any	iter_;

		public:

			ConnectionImpl(void * dataflow, disconnect_fn_type fn, std::any iter)
				: dataflow_(dataflow)
				, disconnect_fn_(fn)
				, iter_(std::move(iter))
			{
			}

			void	disconnect()
			{
				disconnect_fn_(dataflow_, iter_);
			}

		};
	}


	class Connection	
	{
		std::weak_ptr<detail::ConnectionImpl>	impl_;

	public:

		Connection()
		{
		}

		/**
		 * @note Do NOT call this when the corresponding DataFlow object is being destroyed. 
		 *	It's OK but not recommended to call this function after that.
		 */
		void	disconnect()
		{
			if (auto p = impl_.lock())
			{
				p->disconnect();
			}
		}

		bool	connected()
		{
			return bool(impl_.lock());
		}

	private:

		template<class, class> friend class DataFlow;

		Connection(std::shared_ptr<detail::ConnectionImpl> const & impl)
			: impl_(impl)
		{
		}
	};

	class ScopedConnection : public Connection, boost::noncopyable
	{
	public:

		ScopedConnection() = default;

		ScopedConnection(Connection const & other)
			: Connection(other)
		{
		}

		ScopedConnection & operator = (Connection const & other)
		{
			disconnect();
			Connection::operator = (other);

			return *this;
		}

		~ScopedConnection()
		{
			disconnect();
		}
	};

}