// *********************************************************************
//  Socks5ClientSession   version:  1.0   ¡¤  date: 10/14/2010
//  --------------------------------------------------------------------
//  see rfc1928, rfc1929
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SOCKS5CLIENTSESSION_HPP
#define LV_NET_SOCKS5CLIENTSESSION_HPP

#include <lv/FrameWork/AutoLink.hpp>
#include <lv/FrameWork/Net/TcpSession.hpp>
#include <lv/FrameWork/Net/PacketProxy.hpp>
#include <lv/BinaryStream/Fwd.hpp>

namespace lv { namespace net {

	class Socks5ClientSession : public TcpSession
	{
		typedef public TcpSession	base_type;

		bool	use_proxy_;

		enum Status
		{
			None,
			MethodSelect,
			Auth,
			Request,
			Established
		};

		Status	status_;

		BufferPtr	cache_;

		std::string	ip_;

		std::string	port_;

		volatile bool	first_error_;

	public:
		
		explicit Socks5ClientSession(ContextPtr context);

		/// @exception runtime_error
		virtual	void	connect(std::string const & ip, std::string const & port,
			boost::asio::ip::address const & to_bind = boost::asio::ip::address());

	protected:

		virtual	void	on_connected_internal();

		virtual	void	handle_connect(boost::system::error_code const & error);

		virtual	void	handle_read(BufferPtr buf, size_t bytes_transferred, boost::system::error_code const & error);

		virtual	void	handle_write(BufferPtr buf, boost::system::error_code const & error);

		virtual	void	gen_request_error(uint8 rep, ErrorType & err_type, boost::system::error_code & error);

	private:

		PacketProxy		socks_send();

		void	send_request();

		void	on_error_internal(ErrorType type, boost::system::error_code const & error);


		/// @exception std::ios_base::failure

		void	handle_method_select_response(BinaryIStream & bis);
		void	handle_auth_response(BinaryIStream & bis);
		void	handle_request_response(BinaryIStream & bis);

	};

} }

#endif