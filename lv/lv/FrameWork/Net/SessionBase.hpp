// *********************************************************************
//  SessionBase   version:  1.0   ¡¤  date: 01/02/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SESSIONBASE_HPP
#define LV_NET_SESSIONBASE_HPP

#include <lv/lvlib2.hpp>
#include <lv/FrameWork/Net/Fwd.hpp>
#include <lv/Buffer.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/signals2.hpp>
#include <boost/system/system_error.hpp>


namespace lv { namespace net {

	enum ErrorType
	{
		ErrorConnect,
		ErrorHandshake,
		ErrorRead,
		ErrorWrite
	};

	char const *	error_string(ErrorType type);

	class SessionBase : public boost::enable_shared_from_this<SessionBase>
	{
	public:

		typedef boost::signals2::signal<void(ErrorType, boost::system::error_code const &)>	ErrorEvent;
		typedef boost::signals2::signal<void()>				ConnectEvent;
		typedef boost::signals2::signal<void(BufferPtr)>	ReceiveEvent;
		typedef boost::signals2::signal<void(BufferPtr)>	WriteEvent;

	private:

		ErrorEvent		error_event_;
		ConnectEvent	connect_event_;
		ReceiveEvent	receive_event_;
		WriteEvent		write_event_;

		SocketHolderPtr	socket_;

	protected:

		ContextPtr	context_;

	public:

		SessionBase(ContextPtr context, SocketHolderPtr socket);

		virtual	~SessionBase();

		std::string		remote_ip();

		SocketHolderPtr	socket();

		ErrorEvent &	error_event();
		ConnectEvent &	connect_event();
		ReceiveEvent &	receive_event();
		WriteEvent &	write_event();

		virtual	void	shutdown();

		virtual	void	close();

		/// @exception runtime_error
		virtual	void	connect(std::string const & ip, std::string const & port, std::string const & to_bind = std::string());

		virtual void	start_read();

		virtual	void	start_write(BufferPtr buf);

		// For internal use. You should use on_connected instead.
		virtual	void	server_side_start();

	protected:

		virtual	void	on_error(ErrorType type, boost::system::error_code const & error);

		virtual	void	on_connected_internal();

		virtual	void	on_connected();
		virtual	void	on_receive(BufferPtr buf);
		virtual	void	on_write(BufferPtr buf);

		virtual	void	handle_read(BufferPtr buf, size_t bytes_transferred, boost::system::error_code const & error);

		virtual	void	handle_write(BufferPtr buf, boost::system::error_code const & error);

		virtual	void	handle_connect(boost::system::error_code const & error);
	};

} }

#endif