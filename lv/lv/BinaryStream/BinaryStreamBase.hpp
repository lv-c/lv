// *********************************************************************
//  BinaryStreamBase   version:  1.0   ¡¤  date: 2012/03/09
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2012 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once


namespace lv::bstream
{
	class BinaryStreamBase
	{
		bool	switch_endian_;

	public:

		BinaryStreamBase()
			: switch_endian_(false)
		{
		}

		bool	switch_endian() const
		{
			return switch_endian_;
		}

		void	switch_endian(bool s)
		{
			switch_endian_ = s;
		}
	};
}
