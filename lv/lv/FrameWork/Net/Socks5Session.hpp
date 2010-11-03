// *********************************************************************
//  Socks5Session   version:  1.0   ¡¤  date: 10/14/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SOCKS5SESSION_HPP
#define LV_NET_SOCKS5SESSION_HPP

#include <lv/FrameWork/AutoLink.hpp>
#include <lv/FrameWork/Net/TcpSession.hpp>
#include <lv/FrameWork/Net/PacketProxy.hpp>

namespace lv { namespace net {

	class Socks5Session : public TcpSession<ClientSide>
	{
		typedef public TcpSession<ClientSide>	base_type;

		static uint8 const	version_ = 0x05;

		bool	use_proxy_;

		enum Status
		{
			None,
			MethodSelect,
			Auth,
			Request,
			Established
		};

		enum Method : uint8
		{
			NoAuth			=	0,
			UserPassword	=	2
		};

		enum AddrType : uint8
		{
			IPV4		=	1,
			DomainName	=	3,
			IPV6		=	4
		};

		Status	status_;

		BufferPtr	cache_;

		boost::asio::ip::tcp::endpoint	endpoint_;

		bool	first_error_;

	public:
		
		explicit Socks5Session(ContextPtr context);

		/// @exception runtime_error
		virtual	void	start(std::string const & ip, std::string const & port);

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
	};

} }

#endif