// *********************************************************************
//  Connection   version:  1.0   ¡¤  date: 10/24/2009
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2009 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_DATAFLOW_CONNECTION_HPP
#define LV_DATAFLOW_CONNECTION_HPP

#include <boost/any.hpp>

#include <memory>


namespace lv { namespace flow {

	typedef void (* disconnect_fun_type) (void *, boost::any const &);


	namespace detail
	{
		class ConnectionImpl
		{
			void *	dataflow_;

			disconnect_fun_type	disconnect_fun_;

			boost::any	iter_;

		public:
			ConnectionImpl(void * dataflow, disconnect_fun_type fun, boost::any const & iter)
				: dataflow_(dataflow)
				, disconnect_fun_(fun)
				, iter_(iter)
			{
			}

			void	disconnect()
			{
				disconnect_fun_(dataflow_, iter_);
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
			std::shared_ptr<detail::ConnectionImpl> p = impl_.lock();
			if (p)
			{
				p->disconnect();
			}
		}

		bool	connected()
		{
			return bool(impl_.lock());
		}

	private:

		template<template<class> class, class, class> friend class DataFlow;

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

		ScopedConnection & operator = (Connection const & rhs)
		{
			disconnect();
			Connection::operator = (rhs);

			return *this;
		}

		~ScopedConnection()
		{
			disconnect();
		}
	};

} }

#endif