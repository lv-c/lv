// *********************************************************************
//  SessionBase   version:  1.0   ��  date: 01/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/AutoLink.hpp>
#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/FrameWork/Net/Event.hpp>

#include <list>
#include <mutex>


namespace lv::net
{
	char const *	error_string(ErrorType type);

	class SessionBase : public std::enable_shared_from_this<SessionBase>
	{
		ErrorEvent		error_event_;
		ConnectEvent	connect_event_;
		ReceiveEvent	receive_event_;
		WriteEvent		write_event_;

		SocketHolderPtr	socket_;

		volatile bool	closed_;

		//

		std::list<BufferPtr>	write_queue_;

		bool	writing_;

		using lock_guard = std::lock_guard<std::mutex>;

		std::mutex	write_mutex_;

	protected:

		ContextPtr	context_;

	public:

		SessionBase(ContextPtr context, SocketHolderPtr socket);

		virtual	~SessionBase();

		std::string		remote_ip();
		std::string		local_ip();

		SocketHolderPtr	socket();

		ErrorEvent &	error_event();
		ConnectEvent &	connect_event();
		ReceiveEvent &	receive_event();
		WriteEvent &	write_event();

		virtual	void	shutdown();

		virtual	void	close();

		// whether @close has been called
		bool			closed();

		/// @exception runtime_error
		virtual	void	connect(std::string const & ip, std::string const & port, std::string const & to_bind = std::string());

		// thread-safe
		virtual	void	start_write(BufferPtr buf);

		// For internal use. You should use on_connected instead.
		virtual	void	server_side_start();

	protected:

		virtual	void	write_impl(BufferPtr buf);

		virtual void	start_read();

		virtual	void	on_error(ErrorType type, boost::system::error_code const & error);

		virtual	void	on_connected_internal();
		virtual	void	on_receive_internal(BufferPtr buf);
		virtual	void	on_error_internal(ErrorType type, boost::system::error_code const & error);

		virtual	void	on_connected();
		virtual	void	on_receive(BufferPtr buf);
		virtual	void	on_write(BufferPtr buf);

		virtual	void	handle_read(BufferPtr buf, size_t bytes_transferred, boost::system::error_code const & error);

		virtual	void	handle_write(BufferPtr buf, boost::system::error_code const & error);

		virtual	void	handle_connect(boost::system::error_code const & error);

	private:

		std::string		get_ip(bool remote);

	};

}