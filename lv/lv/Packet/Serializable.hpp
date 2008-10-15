// *********************************************************************
//  Serializable   version:  1.0   ¡¤  date: 10/02/2008
//  --------------------------------------------------------------------
//  Provides three common interfaces, one for all loadable objects, one 
//	for all savable objects and the last for all serializable objects (
//	both loadable and savable)
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2008 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_SERIALIZABLE_HPP
#define LV_SERIALIZABLE_HPP

#include <boost/type_traits/add_reference.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/static_assert.hpp>

namespace lv
{

	namespace detail
	{

		template<class T>
		struct IsSharedPtr : boost::mpl::false_ {};

		template<class T>
		struct IsSharedPtr<boost::shared_ptr<T> > : boost::mpl::true_ {};

		//////////////////////////////////////////////////////////////////////////
		// saver

		template<class T, class OArchive, class Enable = void>
		struct Saver
		{
			static void	save(typename boost::add_reference<T>::type t, OArchive & oa)
			{
				oa << t;
			}
		};


		template<class T, class class OArchive>
		struct Saver<T, OArchive, typename boost::enable_if<IsSharedPtr<T> >::type>
		{
			static void save(T t, OArchive & oa)
			{
				oa << *t;
			}
		};

		//////////////////////////////////////////////////////////////////////////
		// loader

		template<class T, class IArchive, class Enable = void>
		struct Loader
		{
			BOOST_STATIC_ASSERT(boost::is_reference<T>::value);

			static void load(T t, IArchive & ia)
			{
				ia >> t;
			}
		};

		template<class T, class IArchive>
		struct Loader<T, IArchive, typename boost::enable_if<IsSharedPtr<T> >::type>
		{
			static void load(T t, IArchive & ia)
			{
				ia >> (*t);
			}
		};


	}


	/**
	 * @param Tag used to distinguish different kinds of serializable objects
	 */
	template<class OArchive, class Tag = void>
	class Savable
	{
	public:
		virtual void save(OArchive & oa, unsigned int const version) = 0;
	};

	/**
	 * @param T the type of object to be saved. supports boost::shared_ptr<> type
	 *	(and reference type)
	 */
	template<typename T, class OArchive, class Tag = void>
	class SavableImpl : public Savable<OArchive, Tag>
	{
		T	t_;
	public:

		SavableImpl(T t) : t_(t) {}
	
		virtual void save(OArchive & oa, unsigned int const version)
		{
			detail::Saver::<T, OArchive>::save(t_, oa);
		}
	};

	/// saves nothing
	template<class OArchive, class Tag = void>
	class NullSavable : public Savable<OArchive, Tag>
	{
	public:
		virtual void save(OArchive &, unsigned int const) {}
	};

	//////////////////////////////////////////////////////////////////////////
	
	template<class IArchive, class Tag = void>
	class Loadable
	{
	public:
		virtual	void load(IArchive & ia, unsigned int const version) = 0;
	};

	template<typename T, class IArchive, class Tag = void>
	class LoadableImpl : public Loadable<IArchive, Tag>
	{
		T t_;

		BOOST_STATIC_ASSERT(boost::is_reference<T>::value || IsSharedPtr<T>::value);

	public:
		LoadableImpl(T t) : t_(t) {}

		virtual	void load(IArchive & ia, unsigned int const version)
		{
			detail::Loader<T, IArchive>::load(t_, ia);
		}
	};
	
	// loads nothing
	template<class IArchive, class Tag = void>
	class NullLoadable : public Loadable<IArchive, Tag>
	{
	public:
		virtual	void load(IArchive &, unsigned int const) {}

	};


}

#endif // LV_SERIALIZABLE_HPP