// *********************************************************************
//  SessionBase   version:  1.0   ¡¤  date: 01/02/2010
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

#include <deque>
#include <mutex>
#include <array>
#include <atomic>


namespace lv::net
{
	char const *	error_string(ErrorType type);

	class SessionBase : public std::enable_shared_from_this<SessionBase>
	{
		ErrorEvent		error_event_;
		ConnectEvent	connect_event_;
		ReceiveEvent	receive_event_;
		WriteEvent		write_event_;
		CloseEvent		close_event_;		// the session is just closed, either on error, or by the user

		SocketHolderPtr	socket_;

		std::atomic<bool>	closed_;

		//
		std::array<Buffer, 2>	write_buffers_;

		size_t		write_index_;

		bool		writing_;


		using lock_guard = std::lock_guard<std::mutex>;

		std::mutex	write_mutex_;

		static size_t const	max_buffer_size_ = 100 * 1024;

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
		CloseEvent &	close_event();

		virtual	void	shutdown();

		virtual	void	close();

		// whether @close has been called
		bool			closed();

		/// @exception runtime_error
		virtual	void	connect(std::string const & ip, std::string const & port, std::string const & to_bind = std::string());

		// thread-safe
		virtual	void	start_write(ConstBufferRef buf);

		// For internal use. You should use on_connected instead.
		virtual	void	server_side_start();

	protected:

		// could be null
		virtual void	start_read(BufferPtr buf);

		virtual	void	on_error(ErrorType type, boost::system::error_code const & error);

		virtual	void	on_connected_internal();
		virtual	void	on_receive_internal(Buffer const & buf);
		virtual	void	on_error_internal(ErrorType type, boost::system::error_code const & error);

		virtual	void	on_connected();
		virtual	void	on_receive(Buffer const & buf);
		virtual	void	on_write(size_t size);
		virtual	void	on_closed();

		virtual	void	handle_read(BufferPtr buf, size_t bytes_transferred, boost::system::error_code const & error);

		virtual	void	handle_write(size_t buf_index, boost::system::error_code const & error);

		virtual	void	handle_connect(boost::system::error_code const & error);

	private:

		void			check_write();

		std::string		get_ip(bool remote);

	};

}